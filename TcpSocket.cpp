#include "StdAfx.h"
#include "TcpSocket.h"
#include "GlobalFunction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int SocketTimeout = 6000;

BOOL CTcpSocket::Create( LPCTSTR lpszSocketAddress /*= NULL*/, UINT nSocketPort /*= 0*/ )
{
	m_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_socket == INVALID_SOCKET)
	{
		return FALSE;
	}

	if (lpszSocketAddress != NULL)
	{
		SOCKADDR_IN InternetAddr;
		INT nPortId = nSocketPort;
		InternetAddr.sin_family = AF_INET;
		InternetAddr.sin_addr.s_addr = inet_addr(CGlobalFunction::ConverCStringToStdString(CString(lpszSocketAddress)).c_str());
		InternetAddr.sin_port = htons(nPortId);

		if(0 != bind(m_socket, (sockaddr *)&InternetAddr, sizeof(InternetAddr)))
		{
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CTcpSocket::Listen()
{
	if(0 != listen(m_socket, 5))
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CTcpSocket::Accept( CTcpSocket & rConnectedSocket, SOCKADDR* lpSockAddr /*= NULL */, int* lpSockAddrLen /*= NULL */ )
{
	rConnectedSocket.Close();

	sockaddr_in sockClient;
	memset(&sockClient, 0, sizeof(sockaddr_in));
	int iLength = sizeof(sockaddr_in);

	rConnectedSocket.m_socket = accept(m_socket, (sockaddr*)&sockClient, &iLength);
	if (rConnectedSocket.m_socket == INVALID_SOCKET)
	{
		int nErr = WSAGetLastError();
		return FALSE;
	}

	int nSendTimeOut = SocketTimeout; //设置发送超时
	if(::setsockopt(rConnectedSocket.m_socket, SOL_SOCKET, SO_SNDTIMEO, (char *)&nSendTimeOut, sizeof(nSendTimeOut)) == SOCKET_ERROR)
	{
		int nErr = WSAGetLastError();
		return FALSE;
	}

	int nRecvTimeOut = SocketTimeout; //设置接收超时
	if(::setsockopt(rConnectedSocket.m_socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&nRecvTimeOut, sizeof(nRecvTimeOut)) == SOCKET_ERROR)
	{
		int nErr = WSAGetLastError();
		return FALSE;
	}
	
	return TRUE;
}

BOOL CTcpSocket::Connect( LPCTSTR lpszSocketAddress /*= NULL*/, UINT nSocketPort /*= 0*/ )
{
	SOCKADDR_IN InternetAddr;
	INT nPortId = nSocketPort;
	InternetAddr.sin_family = AF_INET;
	struct hostent * server;
	server = gethostbyname(CGlobalFunction::ConverCStringToStdString(CString(lpszSocketAddress)).c_str());
	if (!server)
	{
		return FALSE;
	}
	memcpy((char *)&InternetAddr.sin_addr.s_addr,(char*)server->h_addr, server->h_length);
	//InternetAddr.sin_addr.s_addr = inet_addr((char*)server->h_addr);
	//InternetAddr.sin_addr.s_addr = inet_addr(CGlobalFunction::ConverCStringToStdString(CString(lpszSocketAddress)).c_str());
	InternetAddr.sin_port = htons(nPortId);

	if(0 != connect(m_socket, (sockaddr *)&InternetAddr, sizeof(InternetAddr)))
	{
		return FALSE;
	}

	int nSendTimeOut = SocketTimeout; //设置发送超时
	if(::setsockopt(m_socket, SOL_SOCKET, SO_SNDTIMEO, (char *)&nSendTimeOut, sizeof(nSendTimeOut)) == SOCKET_ERROR)
	{
		int nErr = WSAGetLastError();
		return FALSE;
	}

	int nRecvTimeOut = SocketTimeout; //设置接收超时
	if(::setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&nRecvTimeOut, sizeof(nRecvTimeOut)) == SOCKET_ERROR)
	{
		int nErr = WSAGetLastError();
		return FALSE;
	}

	return TRUE;
}

BOOL CTcpSocket::Close()
{
	BOOL bRet = TRUE;
	if (m_socket != INVALID_SOCKET)
	{
		bRet = closesocket(m_socket) == 0 ? TRUE : FALSE;
		m_socket = INVALID_SOCKET;
	}
	return bRet;
}

int CTcpSocket::Send( const char* lpBuf, int nBufLen, int nFlags /*= 0*/ )
{
	return send(m_socket, lpBuf, nBufLen, nFlags);
}

int CTcpSocket::Recv( char* lpBuf, int nBufLen, int nFlags /*= 0*/ )
{
	return recv(m_socket, lpBuf, nBufLen, nFlags);
}

CTcpSocket::CTcpSocket()
	: m_socket(INVALID_SOCKET)
{
}

CTcpSocket::~CTcpSocket()
{
}

BOOL CTcpSocket::CheckValid()
{
	return m_socket != INVALID_SOCKET;
}
