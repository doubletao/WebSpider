#pragma once
class CTcpSocket
{
private:
	SOCKET m_socket;
public:
	CTcpSocket();
	virtual ~CTcpSocket();
	BOOL Create(LPCTSTR lpszSocketAddress = NULL, UINT nSocketPort = 0);
	BOOL Listen();
	BOOL Accept(CTcpSocket & rConnectedSocket, SOCKADDR* lpSockAddr = NULL , int* lpSockAddrLen = NULL );
	BOOL Connect(LPCTSTR lpszSocketAddress = NULL, UINT nSocketPort = 0);
	BOOL Close();
	int Send(const char* lpBuf, int nBufLen, int nFlags = 0);
	int Recv(char* lpBuf, int nBufLen, int nFlags = 0);
	BOOL CheckValid();
};
