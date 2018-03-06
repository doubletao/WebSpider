////////////////////////////////// HttpClient.cpp
#include "stdafx.h"
#include "HttpClient.h"

#define BUFFER_SIZE	 1024

#define NORMAL_CONNECT			 INTERNET_FLAG_KEEP_CONNECTION
#define SECURE_CONNECT				NORMAL_CONNECT | INTERNET_FLAG_SECURE
#define NORMAL_REQUEST			 INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE 
#define SECURE_REQUEST			 NORMAL_REQUEST | INTERNET_FLAG_SECURE | INTERNET_FLAG_IGNORE_CERT_CN_INVALID

CHttpClient::CHttpClient(LPCTSTR strAgent)
{
	m_pSession = new CInternetSession(strAgent);
	m_pConnection = NULL;
	m_pFile = NULL;
}

	
CHttpClient::~CHttpClient(void)
{
	Clear();
	if (NULL != m_pSession)
	{
		m_pSession->Close();
		delete m_pSession;
		m_pSession = NULL;
	}
}

void CHttpClient::Clear()
{
	if (NULL != m_pFile)
	{
		m_pFile->Close();
		delete m_pFile;
		m_pFile = NULL;
	}

	if (NULL != m_pConnection)
	{
		m_pConnection->Close();
		delete m_pConnection;
		m_pConnection = NULL;
	}
}

int CHttpClient::ExecuteRequest(int strMethod, LPCTSTR strUrl, LPCTSTR strPostData, std::string &cstrResponse)
{
	int result =FAILURE ;
	//WCHAR* wPostData = strPostData.GetBuffer();
	CString strServer;
	CString strObject;
	DWORD dwServiceType;
	INTERNET_PORT nPort;

	AfxParseURL(strUrl, dwServiceType, strServer, strObject, nPort);
	if (AFX_INET_SERVICE_HTTP != dwServiceType && AFX_INET_SERVICE_HTTPS != dwServiceType)
	{
		return FAILURE;
	}

	try
	{
		m_pConnection = m_pSession->GetHttpConnection(strServer,
			dwServiceType == AFX_INET_SERVICE_HTTP ? NORMAL_CONNECT : SECURE_CONNECT,
			nPort);
		m_pFile = m_pConnection->OpenRequest(strMethod, strObject,
			NULL, 1, NULL, NULL,
			(dwServiceType == AFX_INET_SERVICE_HTTP ? NORMAL_REQUEST : SECURE_REQUEST));

		/*设置请求相关参数*/
		m_pFile->AddRequestHeaders(L"Accept: */*,application/json");//accept请求报头域，表示客户端接受哪些类型的信息
		m_pFile->AddRequestHeaders(L"Accept-Charset:UTF8");
		m_pFile->AddRequestHeaders(L"Accept-Language: zh-cn;q=0.8,en;q=0.6,ja;q=0.4");
		m_pFile->AddRequestHeaders(L"Content-Type:application/json");//content为实体报头域，格式及编码

		//m_pFile->SendRequest(NULL, 0, (LPVOID)(LPCTSTR)strPostData, strPostData == NULL ? 0 : _tcslen(strPostData));

		/*请求body内容先转为UTF-8编码，与服务端保持一致,cword为要发送内容*/
		char*cword; //ANSI指针
		if (strPostData != NULL){
			DWORD num = WideCharToMultiByte(CP_UTF8, 0, strPostData, -1, NULL, 0, NULL, NULL);//計算這個UNICODE实际由几个UTF-8字組成
			cword = (char*)calloc(num, sizeof(char)); //申请空间
			if (cword == NULL)						//是否申请
			{
				free(cword);
			}
			memset(cword, 0, num*sizeof(char));	 //初始化
			WideCharToMultiByte(CP_UTF8, 0, strPostData, -1, cword, num, NULL, NULL);
			printf("content长度为%d\n", strlen(cword));
			m_pFile->SendRequest(NULL, 0, cword, strlen(cword));//发送请求
		}
		else{
			m_pFile->SendRequest(NULL, 0, NULL, 0);//发送请求
		}
		

		DWORD dwRet;
		m_pFile->QueryInfoStatusCode(dwRet);//查询执行状态
		printf("HTTP_STATUS_code:%d\n", dwRet);
		if (dwRet == HTTP_STATUS_OK){//http请求执行失败
			result = SUCCESS;
		}

		/*保存http响应*/
		char szChars[BUFFER_SIZE + 1] = { 0 };
		string strRawResponse = "";
		UINT nReaded = 0;
		while ((nReaded = m_pFile->Read((void*)szChars, BUFFER_SIZE)) > 0)
		{
			szChars[nReaded] = '\0';
			strRawResponse += szChars;
			memset(szChars, 0, BUFFER_SIZE + 1);
		}

		cstrResponse = strRawResponse;//最终响应结果
		Clear();
	}
	catch (CInternetException* e)
	{
		Clear();
		DWORD dwErrorCode = e->m_dwError;
		e->Delete();

		DWORD dwError = GetLastError();

		printf("dwError = %d", dwError, 0);

		cstrResponse = "CInternetException\n";

		if (ERROR_INTERNET_TIMEOUT == dwErrorCode)
		{
			return OUTTIME;
		}
		else
		{
			return FAILURE;
		}
	}
	return result;
}

int CHttpClient::HttpGet(LPCTSTR strUrl, LPCTSTR strPostData, std::string &cstrResponse)
{
	return ExecuteRequest(CHttpConnection::HTTP_VERB_GET, strUrl, NULL, cstrResponse);
}

int CHttpClient::HttpPost(LPCTSTR strUrl, LPCTSTR strPostData, std::string &cstrResponse)
{
	return ExecuteRequest(CHttpConnection::HTTP_VERB_POST, strUrl, strPostData, cstrResponse);
}
int CHttpClient::HttpPut(LPCTSTR strUrl, LPCTSTR strPostData, std::string &cstrResponse)
{
	return ExecuteRequest(CHttpConnection::HTTP_VERB_PUT, strUrl, strPostData, cstrResponse);
}

void CHttpClient::SaveAllImg(std::string cstrHtml, CString strFilePath)
{
	static int nHtmlIndex = 0;
	nHtmlIndex++;
	htmlcxx::HTML::ParserDom parser;
	tree<htmlcxx::HTML::Node> dom = parser.parseTree(cstrHtml);
	int nFileIndex = 0;
	for(tree<htmlcxx::HTML::Node>::iterator it = dom.begin(); it != dom.end(); it++)
	{
		it->parseAttributes();
		std::map<std::string, std::string> mapAttr = it->attributes();
		std::string strTagName = it->tagName();
		std::string strText = it->text();
		if (strTagName == "img")
		{
			if (mapAttr.count("src") > 0)
			{
				try
				{
					std::string cstrUrl = mapAttr["src"];
					CString strUrl = CGlobalFunction::ConvertStdStringToCString(cstrUrl, CP_ACP);
					CInternetSession session;
					CHttpFile *httpFile = (CHttpFile *)session.OpenURL(strUrl);
					CStdioFile imgFile;
					char buff[1024];// 缓存
					CString strExt = CGlobalFunction::GetFileNameExt(strUrl);
					CString strFileName;
					strFileName.Format(_T("%d_%d%s"), nHtmlIndex, nFileIndex++, strExt);
					if (imgFile.Open(strFilePath + strFileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
					{
						DWORD dwStatusCode;
						httpFile->QueryInfoStatusCode(dwStatusCode);
						if(dwStatusCode == HTTP_STATUS_OK)
						{
							int size=0;
							do
							{
								size = httpFile->Read(buff,1024);    // 读取图片
								imgFile.Write(buff,size);
							}while(size > 0);
						}
						imgFile.Close();
					}
					httpFile->Close();
					session.Close();
				}
				catch (CMemoryException* e)
				{
				}
				catch (CFileException* e)
				{
				}
				catch (CException* e)
				{
				}
				catch (...)
				{
				}
			}
		}
	}
}
