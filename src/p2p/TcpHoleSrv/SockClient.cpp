// SockClient.cpp : implementation file
//

#include "stdafx.h"
#include "TcpHoleSrv.h"
#include "SockClient.h"
#include <Afxmt.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CPtrArray g_PtrAry_SockClient;
CCriticalSection g_CSFor_PtrAry_SockClient;
extern BOOL g_bDeleteNullSockClient;
DWORD g_nSockClientID = 0;

CSockClient* GetNewSockClient ()
{
	CSockClient *pSockClient = new CSockClient;
	if ( !pSockClient )
	{
		printf ( "New SockClient object failed\n" );
		return NULL;
	}
	g_CSFor_PtrAry_SockClient.Lock();
	g_PtrAry_SockClient.Add ( pSockClient );
	pSockClient->m_dwID = ++g_nSockClientID;
	g_CSFor_PtrAry_SockClient.Unlock();
	printf ( "Current SocketClient array count is %d\n", g_PtrAry_SockClient.GetSize() );

	return pSockClient;
}

//
// 将新客户端登录信息发送给所有已登录的客户端，但不发送给自己
//
BOOL SendNewUserLoginNotifyToAll ( LPCTSTR lpszClientIP, UINT nClientPort, DWORD dwID )
{
	ASSERT ( lpszClientIP && nClientPort > 0 );
	g_CSFor_PtrAry_SockClient.Lock();
	for ( int i=0; i<g_PtrAry_SockClient.GetSize(); i++ )
	{
		CSockClient *pSockClient = (CSockClient*)g_PtrAry_SockClient.GetAt(i);
		if ( pSockClient && pSockClient->m_bMainConn && pSockClient->m_dwID > 0 && pSockClient->m_dwID != dwID )
		{
			if ( !pSockClient->SendNewUserLoginNotify ( lpszClientIP, nClientPort, dwID ) )
			{
				g_CSFor_PtrAry_SockClient.Unlock();
				return FALSE;
			}
		}
	}

	g_CSFor_PtrAry_SockClient.Unlock ();
	return TRUE;
}

CSockClient* FindSocketClient ( DWORD dwID )
{
	g_CSFor_PtrAry_SockClient.Lock ();
	for ( int i=0; i<g_PtrAry_SockClient.GetSize(); i++ )
	{
		CSockClient *pSockClient = (CSockClient*)g_PtrAry_SockClient.GetAt(i);
		if ( pSockClient && pSockClient->m_dwID == dwID )
		{
			g_CSFor_PtrAry_SockClient.Unlock ();
			return pSockClient;
		}
	}
	printf ( "Can't find ID:%u\n", dwID );
	g_CSFor_PtrAry_SockClient.Unlock ();
	return NULL;
}

void DeleteNullSocketClient ()
{
	g_bDeleteNullSockClient = FALSE;
	g_CSFor_PtrAry_SockClient.Lock ();
	for ( int i=0; i<g_PtrAry_SockClient.GetSize(); i++ )
	{
		CSockClient *pSockClient = (CSockClient*)g_PtrAry_SockClient.GetAt(i);
		if ( pSockClient && pSockClient->m_dwID == 0 )
		{
			delete pSockClient;
			g_PtrAry_SockClient.RemoveAt(i);
			i --;
		}
	}
	printf ( "Current SocketClient array count is %d\n", g_PtrAry_SockClient.GetSize() );
	g_CSFor_PtrAry_SockClient.Unlock ();
}

DWORD WINAPI ThreadProc_SockClient(
  LPVOID lpParameter   // thread data
)
{
	CSockClient *pSockClient = reinterpret_cast<CSockClient*>(lpParameter);
	if ( pSockClient )
		return pSockClient->ThreadProc_SockClient ();
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CSockClient

CSockClient::CSockClient()
	: m_dwID ( 0 )
	, m_nPeerPort ( 0 )
	, m_hThread ( NULL )
	, m_hEvtEndModule ( NULL )
	, m_hEvtWaitClientBHole ( NULL )
	, m_dwThreadID ( 0 )
	, m_bMainConn ( FALSE )
{
}

CSockClient::~CSockClient()
{
	this->CancelBlockingCall ();
	if ( HANDLE_IS_VALID(m_hEvtEndModule) )
		::SetEvent ( m_hEvtEndModule );
	WaitForThreadEnd ( &m_hThread );
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CSockClient, CSocket)
	//{{AFX_MSG_MAP(CSockClient)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CSockClient member functions

//
// 和客户端建立连接
//
BOOL CSockClient::EstablishConnect(SOCKET hSock, SOCKADDR_IN &sockAddr, BOOL bMainConn)
{
	ASSERT ( HANDLE_IS_VALID(hSock) );
	m_csPeerAddress = inet_ntoa(sockAddr.sin_addr);
	m_nPeerPort = ntohs(sockAddr.sin_port);
	m_hSocket = hSock;
	m_bMainConn = bMainConn;

	if ( m_bMainConn )
	{
		if ( !SendNewUserLoginNotifyToAll ( m_csPeerAddress, m_nPeerPort, m_dwID ) )
			return FALSE;
		printf ( "[MAIN SOCKET] New connection ( %s:%d:%u ) come in\n", m_csPeerAddress, m_nPeerPort, m_dwID );
	}
	else
	{
		printf ( "<HOLE SOCKET> New connection ( %s:%d:%u ) come in\n", m_csPeerAddress, m_nPeerPort, m_dwID );
	}

	// 创建一个线程来处理该客户端的所有数据通信
	ASSERT ( m_hEvtEndModule == NULL && m_hEvtWaitClientBHole == NULL && m_hThread == NULL );
	m_hEvtEndModule = ::CreateEvent ( NULL, TRUE, FALSE, NULL );
	m_hEvtWaitClientBHole = ::CreateEvent ( NULL, FALSE, FALSE, NULL );
	m_hThread = ::CreateThread ( NULL, 0, ::ThreadProc_SockClient, this, 0, &m_dwThreadID );
	if ( !HANDLE_IS_VALID(m_hThread) || !HANDLE_IS_VALID(m_hEvtEndModule) || !HANDLE_IS_VALID(m_hEvtWaitClientBHole) )
		return FALSE;

	return TRUE;
}

//
// 将其他新登录的客户端信息发送给该客户端
//
BOOL CSockClient::SendNewUserLoginNotify(LPCTSTR lpszClientIP, UINT nClientPort, DWORD dwID)
{
	ASSERT ( lpszClientIP && nClientPort > 0 );
	if ( !m_bMainConn ) return TRUE;
	t_NewUserLoginPkt NewUserLoginPkt;
	STRNCPY_SZ ( NewUserLoginPkt.szClientIP, lpszClientIP );
	NewUserLoginPkt.nClientPort = nClientPort;
	NewUserLoginPkt.dwID = dwID;
	printf ( "Send new user login notify to (%s:%u:%u)\n", m_csPeerAddress, m_nPeerPort, m_dwID );
	return ( SendChunk ( &NewUserLoginPkt, sizeof(t_NewUserLoginPkt), 0 ) == sizeof(t_NewUserLoginPkt) );
}

BOOL CSockClient::ThreadProc_SockClient()
{
	printf ( "Client.%u thread start.\n", m_dwID );
	BOOL bRet = FALSE;
	char szRecvBuffer[NET_BUFFER_SIZE] = {0};
	int nRecvBytes = 0;

	// WinSocket 的句柄是不能跨线程访问的，所以这里重新附加进来
	if ( !Attach ( m_hSocket ) )
		goto finished;

	// 发送欢迎信息，通知客户端连接成功了。
	SendWelcomeInfo ();
	// 循环接收网络数据
	while ( TRUE )
	{
		nRecvBytes = Receive ( szRecvBuffer, sizeof(szRecvBuffer) );
		if ( nRecvBytes > 0 )
		{
			if ( !HandleDataReceived ( szRecvBuffer, nRecvBytes ) )
				goto finished;
		}
		else if ( (nRecvBytes == 0 && GetLastError() != NO_ERROR) || (SOCKET_ERROR == nRecvBytes && GetLastError() == WSAEWOULDBLOCK) )
		{
			SLEEP_BREAK ( 10 );
		}
		// 对方断开连接了
		else
		{
			goto finished;
		}
		SLEEP_BREAK ( 1 );
	}

	bRet = TRUE;
finished:
	Close ();
	printf ( "Client.%u thread end. result : %s\n", m_dwID, bRet?"SCUESS":"FAILED" );
	if ( HANDLE_IS_VALID(m_hEvtEndModule) )
		::SetEvent ( m_hEvtEndModule );
	m_dwID = 0;
	printf ( "ThreadProc_SockClient end\n" );
	g_bDeleteNullSockClient = TRUE;
	return bRet;
}

BOOL CSockClient::HandleDataReceived(char *data, int size)
{
	if ( !data || size < 4 ) return FALSE;

	PACKET_TYPE *pePacketType = (PACKET_TYPE*)data;
	ASSERT ( pePacketType );
	switch ( *pePacketType )
	{
		// 要求与另一个客户端建立直接的TCP连接
	case PACKET_TYPE_REQUEST_CONN_CLIENT:
		{
			ASSERT ( !m_bMainConn );
			ASSERT ( size == sizeof(t_ReqConnClientPkt) );
			t_ReqConnClientPkt *pReqConnClientPkt = (t_ReqConnClientPkt*)data;
			if ( !Handle_ReqConnClientPkt ( pReqConnClientPkt ) )
				return FALSE;
			break;
		}
		// 被动端（客户端B）请求服务器断开连接，这个时候应该将客户端B的外部IP和端口号告诉客户端A，并让客户端A主动
		// 连接客户端B的外部IP和端口号
	case PACKET_TYPE_REQUEST_DISCONNECT:
		{
			ASSERT ( !m_bMainConn );
			ASSERT ( size == sizeof(t_ReqSrvDisconnectPkt) );
			t_ReqSrvDisconnectPkt *pReqSrvDisconnectPkt = (t_ReqSrvDisconnectPkt*)data;
			ASSERT ( pReqSrvDisconnectPkt );
			printf ( "Clinet.%u request disconnect\n", m_dwID );

			CSockClient *pSockClientHole_A = FindSocketClient ( pReqSrvDisconnectPkt->dwInviterHoleID );
			if ( !pSockClientHole_A ) return FALSE;
			pSockClientHole_A->m_SrvReqDirectConnectPkt.dwInvitedID = pReqSrvDisconnectPkt->dwInvitedID;
			STRNCPY_CS ( pSockClientHole_A->m_SrvReqDirectConnectPkt.szInvitedIP, m_csPeerAddress );
			pSockClientHole_A->m_SrvReqDirectConnectPkt.nInvitedPort = m_nPeerPort;

			Close ();

			break;
		}
		// 被动端（客户端B）打洞和侦听均已准备就绪
	case PACKET_TYPE_HOLE_LISTEN_READY:
		{
			ASSERT ( m_bMainConn );
			ASSERT ( size == sizeof(t_HoleListenReadyPkt) );
			t_HoleListenReadyPkt *pHoleListenReadyPkt = (t_HoleListenReadyPkt*)data;
			ASSERT ( pHoleListenReadyPkt );
			printf ( "Client.%u hole and listen ready\n", pHoleListenReadyPkt->dwInvitedID );
			// 通知正在与客户端A通信的服务器线程，以将客户端B的外部IP地址和端口号告诉客户端A
			CSockClient *pSockClientHole_A = FindSocketClient ( pHoleListenReadyPkt->dwInviterHoleID );
			if ( !pSockClientHole_A ) return FALSE;
			if ( HANDLE_IS_VALID(pSockClientHole_A->m_hEvtWaitClientBHole) )
				SetEvent ( pSockClientHole_A->m_hEvtWaitClientBHole );
			break;
		}
	}

	return TRUE;
}

//
// 客户端A请求我（服务器）协助连接客户端B，这个包应该在打洞Socket中收到
//
BOOL CSockClient::Handle_ReqConnClientPkt(t_ReqConnClientPkt *pReqConnClientPkt)
{
	ASSERT ( !m_bMainConn );
	CSockClient *pSockClient_B = FindSocketClient ( pReqConnClientPkt->dwInvitedID );
	if ( !pSockClient_B ) return FALSE;
	printf ( "%s:%u:%u invite %s:%u:%u connection\n", m_csPeerAddress, m_nPeerPort, m_dwID,
		pSockClient_B->m_csPeerAddress, pSockClient_B->m_nPeerPort, pSockClient_B->m_dwID );

	// 客户端A想要和客户端B建立直接的TCP连接，服务器负责将A的外部IP和端口号告诉给B
	t_SrvReqMakeHolePkt SrvReqMakeHolePkt;
	SrvReqMakeHolePkt.dwInviterID = pReqConnClientPkt->dwInviterID;
	SrvReqMakeHolePkt.dwInviterHoleID = m_dwID;
	SrvReqMakeHolePkt.dwInvitedID = pReqConnClientPkt->dwInvitedID;
	STRNCPY_CS ( SrvReqMakeHolePkt.szClientHoleIP, m_csPeerAddress );
	SrvReqMakeHolePkt.nClientHolePort = m_nPeerPort;
	if ( pSockClient_B->SendChunk ( &SrvReqMakeHolePkt, sizeof(t_SrvReqMakeHolePkt), 0 ) != sizeof(t_SrvReqMakeHolePkt) )
		return FALSE;

	// 等待客户端B打洞完成，完成以后通知客户端A直接连接客户端外部IP和端口号
	if ( !HANDLE_IS_VALID(m_hEvtWaitClientBHole) )
		return FALSE;
	if ( WaitForSingleObject ( m_hEvtWaitClientBHole, 6000*1000 ) == WAIT_OBJECT_0 )	//d
	{
		if ( SendChunk ( &m_SrvReqDirectConnectPkt, sizeof(t_SrvReqDirectConnectPkt), 0 ) == sizeof(t_SrvReqDirectConnectPkt) )
			return TRUE;
	}

	return FALSE;
}

BOOL CSockClient::SendWelcomeInfo()
{
	if ( !m_bMainConn ) return TRUE;
	t_WelcomePkt WelcomePkt;
	STRNCPY_CS ( WelcomePkt.szClientIP, m_csPeerAddress );
	WelcomePkt.nClientPort = m_nPeerPort;
	WelcomePkt.dwID = m_dwID;
	_snprintf ( WelcomePkt.szWelcomeInfo, sizeof(WelcomePkt.szWelcomeInfo),
		"Hello, ID.%u, Welcome to login", m_dwID );

	return ( SendChunk ( &WelcomePkt, sizeof(t_WelcomePkt), 0 ) == sizeof(t_WelcomePkt) );
}
