// TcpHoleSrv.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "TcpHoleSrv.h"
#include "SockClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

HANDLE g_hEvtEndModule = NULL;
// 主连接Socket：管理客户端登录和信息的转发
HANDLE g_hThread_Main = NULL;
CSocket *g_pSock_Main = NULL;
// 打洞连接Socket：客户端需要和其他客户端建立直接的TCP连接时先向该Socket发送打洞申请，由主连接Socket协助建立直接的P2P连接
HANDLE g_hThread_Hole = NULL;
CSocket *g_pSock_Hole = NULL;

BOOL g_bDeleteNullSockClient = FALSE;

DWORD WINAPI ThreadProc_TCPServer(
  LPVOID lpParameter   // thread data
)
{
	BOOL bRet = FALSE;
	UINT nPort = (UINT)lpParameter;
	CSocket Sock;
	// 创建Socket服务器
	try
	{
		if ( !Sock.Socket () )
		{
			printf ( "Create socket failed : %s\n", hwFormatMessage(GetLastError()) );
			return FALSE;
		}
		UINT nOptValue = 1;
		if ( !Sock.SetSockOpt ( SO_REUSEADDR, &nOptValue , sizeof(UINT) ) )
		{
			printf ( "SetSockOpt socket failed : %s\n", hwFormatMessage(GetLastError()) );
			return FALSE;
		}
		if ( !Sock.Bind ( nPort ) )
		{
			printf ( "Bind socket failed : %s\n", hwFormatMessage(GetLastError()) );
			return FALSE;
		}
		if ( !Sock.Listen () )
		{
			printf ( "Socket listen failed ( port : %u )\n", nPort );
			goto finished;
		}
	}
	catch ( CException e )
	{
		char szError[255] = {0};
		e.GetErrorMessage( szError, sizeof(szError) );
		printf ( "Exception occur, %s\n", szError );
		goto finished;
	}

	if ( nPort == SRV_TCP_MAIN_PORT )
	{
		g_pSock_Main = &Sock;
		printf ( "TCP server start ( port : %u ). Waiting for client login ...\n", nPort );
	}
	else
	{
		g_pSock_Hole = &Sock;
		printf ( "TCP server start ( port : %u ). Waiting for client make hole request ...\n", nPort );
	}
	// 等到客户端登录
	while ( TRUE )
	{
		SOCKADDR_IN sockAddr = {0};
		int nAddrLen = sizeof(SOCKADDR);
		CSocket sockClient;
		CSockClient *pSockClient = GetNewSockClient ();
		if ( !pSockClient )
			goto finished;

		// 有连接进来了
		if ( Sock.Accept ( sockClient, (SOCKADDR*)&sockAddr, &nAddrLen ) )
		{
			if ( !pSockClient->EstablishConnect ( sockClient.Detach(), sockAddr, nPort == SRV_TCP_MAIN_PORT ) )
				goto finished;
		}
		else
		{
			goto finished;
		}

		if ( g_bDeleteNullSockClient )
		{
			DeleteNullSocketClient ();
		}
	}
	bRet = TRUE;

finished:
	if ( nPort == SRV_TCP_MAIN_PORT )
		g_pSock_Main = NULL;
	else
		g_pSock_Hole = NULL;
	printf ( "ThreadProc_TCPServer end\n" );

	return bRet;
}

BOOL StartTCPServer ( UINT nPort, HANDLE *phThread )
{
	ASSERT ( phThread );
	DWORD dwThreadID = 0;
	*phThread = ::CreateThread ( NULL, 0, ::ThreadProc_TCPServer, LPVOID(nPort), 0, &dwThreadID );

	return HANDLE_IS_VALID(*phThread);
}

//
// 运行程序
//
int Run ()
{
	if ( !AfxSocketInit() )
		return End( FALSE );

	g_hEvtEndModule = ::CreateEvent ( NULL, TRUE, FALSE, NULL );
	if ( !HANDLE_IS_VALID(g_hEvtEndModule) )
		return End( FALSE );
	
	// 启动TCP主服务，侦听端口为 SRV_TCP_MAIN_PORT
	if ( !StartTCPServer ( SRV_TCP_MAIN_PORT, &g_hThread_Main ) )
		return End( FALSE );
	Sleep ( 500 );
	// 启动TCP协助打动服务，侦听端口为 SRV_TCP_HOLE_PORT
	if ( !StartTCPServer ( SRV_TCP_HOLE_PORT, &g_hThread_Hole ) )
		return End( FALSE );

	printf ( "Press any key to terminate program ...\n" );
	::getchar ();
	return End( TRUE );
}

//
// 结束程序
//
int End ( BOOL bSuccess )
{
	if ( HANDLE_IS_VALID(g_hEvtEndModule) )
		::SetEvent ( g_hEvtEndModule );

	if ( g_pSock_Main ) g_pSock_Main->CancelBlockingCall ();
	if ( g_pSock_Hole ) g_pSock_Hole->CancelBlockingCall ();

	WSACleanup ();

	printf ( "End programe\n" );
	if ( bSuccess ) return 0;

	printf ( "Last error is : %s\n", hwFormatMessage(GetLastError()) );
	return 1;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		cerr << _T("Fatal Error: MFC initialization failed") << endl;
		nRetCode = 1;
		return nRetCode;
	}
	else
	{
		nRetCode = Run ();
	}

	return nRetCode;
}


