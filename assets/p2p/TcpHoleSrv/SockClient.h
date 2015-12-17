#if !defined(AFX_SOCKCLIENT_H__10D2699B_6694_416E_9219_9133014CDA2D__INCLUDED_)
#define AFX_SOCKCLIENT_H__10D2699B_6694_416E_9219_9133014CDA2D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SockClient.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSockClient command target

class CSockClient : public CSocket
{
// Attributes
public:

// Operations
public:
	CSockClient();
	virtual ~CSockClient();

// Overrides
public:
	BOOL SendNewUserLoginNotify ( LPCTSTR lpszClientIP, UINT nClientPort, DWORD dwID );
	BOOL ThreadProc_SockClient();
	CString m_csPeerAddress;
	UINT m_nPeerPort;

	BOOL EstablishConnect(SOCKET hSock, SOCKADDR_IN &sockAddr, BOOL bMainConn );
	DWORD m_dwID;
	BOOL m_bMainConn;
	HANDLE m_hEvtWaitClientBHole;	// 等待客户端B打洞完成
	t_SrvReqDirectConnectPkt m_SrvReqDirectConnectPkt;
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSockClient)
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CSockClient)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
private:
	BOOL SendWelcomeInfo ();
	BOOL Handle_ReqConnClientPkt ( t_ReqConnClientPkt *pReqConnClientPkt );
	BOOL HandleDataReceived(char *data, IN OUT int size);
	HANDLE m_hThread;
	DWORD m_dwThreadID;
	HANDLE m_hEvtEndModule;
};

CSockClient* GetNewSockClient ();
void DeleteNullSocketClient ();

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SOCKCLIENT_H__10D2699B_6694_416E_9219_9133014CDA2D__INCLUDED_)
