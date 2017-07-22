
// WebSpiderDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WebSpider.h"
#include "WebSpiderDlg.h"
#include "afxdialogex.h"
#include <sstream>
#include "TcpSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CWebSpiderDlg 对话框




CWebSpiderDlg::CWebSpiderDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWebSpiderDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWebSpiderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CWebSpiderDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_START, &CWebSpiderDlg::OnBnClickedBtnStart)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_CLEAR_RET_PATH, &CWebSpiderDlg::OnBnClickedBtnClearRetPath)
	ON_BN_CLICKED(IDC_BTN_OPEN_RET_PATH, &CWebSpiderDlg::OnBnClickedBtnOpenRetPath)
END_MESSAGE_MAP()


// CWebSpiderDlg 消息处理程序

BOOL CWebSpiderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	InitialLayout();
	m_strRetPath = CGlobalFunction::GetCurPath() + _T("\\result");
	CWnd * pWnd = GetDlgSafeItem(IDC_EDIT_WEBURL_LIST);
	if (pWnd)
	{
		//pWnd->SetWindowText(_T("http://www.ip138.com/ip2city.asp"));//一个获取本机ip的网站，留存
		pWnd->SetWindowText(_T("http://www.miui.com/forum-587-1.html"));
	}

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

CWnd * CWebSpiderDlg::GetDlgSafeItem(int nID)
{
	CWnd * pWnd = GetDlgItem(nID);
	if (pWnd && ::IsWindow(pWnd->GetSafeHwnd()))
	{
		return pWnd;
	}
	return NULL;
}

void CWebSpiderDlg::InitialLayout()
{
	CWnd * pStcWebUrlList = GetDlgSafeItem(IDC_STATIC_1);
	CWnd * pStcKeyWordList = GetDlgSafeItem(IDC_STATIC_2);
	CWnd * pEditWebUrlList = GetDlgSafeItem(IDC_EDIT_WEBURL_LIST);
	CWnd * pEditKeyWordList = GetDlgSafeItem(IDC_EDIT_KEYWORD_LIST);
	CWnd * pBtnClearRet = GetDlgSafeItem(IDC_BTN_CLEAR_RET_PATH);
	CWnd * pBtnOpenRet = GetDlgSafeItem(IDC_BTN_OPEN_RET_PATH);
	CWnd * pBtnStart = GetDlgSafeItem(IDC_BTN_START);
	CWnd * pCheckLoop = GetDlgSafeItem(IDC_CHECK_LOOP);
	if (
		pStcWebUrlList
		&& pStcKeyWordList 
		&& pEditWebUrlList 
		&& pEditKeyWordList
		&& pBtnClearRet
		&& pBtnOpenRet
		&& pBtnStart
		&& pCheckLoop
		)
	{
		CRect rtClient;
		GetClientRect(rtClient);
		int nGap = 3;
		int nBtnWidth = 80;
		int nBtnHeight = 22;
		int nWholeWidth = rtClient.Width();
		int nWholeHeight = rtClient.Height();
		int nHalfPos = nWholeWidth / 2;
		pStcWebUrlList->MoveWindow(nGap, nGap, nHalfPos / 2, nBtnHeight);
		pStcKeyWordList->MoveWindow(nHalfPos, nGap, nHalfPos / 2, nBtnHeight);
		pEditWebUrlList->MoveWindow(nGap, nGap + nBtnHeight, nHalfPos - nGap * 3, nWholeHeight - nGap * 3 - nBtnHeight * 2);
		pEditKeyWordList->MoveWindow(nHalfPos, nGap + nBtnHeight, nHalfPos - nGap * 3, nWholeHeight - nGap * 3 - nBtnHeight * 2);
		pCheckLoop->MoveWindow(nGap, nWholeHeight - nGap - nBtnHeight, nBtnWidth, nBtnHeight);
		pBtnClearRet->MoveWindow(nWholeWidth - nBtnWidth * 3 - nGap * 3, nWholeHeight - nGap - nBtnHeight, nBtnWidth, nBtnHeight);
		pBtnOpenRet->MoveWindow(nWholeWidth - nBtnWidth * 2 - nGap * 2, nWholeHeight - nGap - nBtnHeight, nBtnWidth, nBtnHeight);
		pBtnStart->MoveWindow(nWholeWidth - nBtnWidth * 1 - nGap * 1, nWholeHeight - nGap - nBtnHeight, nBtnWidth, nBtnHeight);
	}
	Invalidate(FALSE);
}

void CWebSpiderDlg::DealOneURL(CString strURL, std::set<CString> & setKeyWord, CString strPath)
{
	//先从服务器将结果下下来
	std::string cstrRet;
	CHttpClient client(IE_AGENT);
	if (SUCCESS != client.HttpGet(strURL, NULL, cstrRet))
	{
		//读取失败，直接返回
		return;
	}
	if (cstrRet.size() > 0)
	{
		//将server解析出来，供后边使用
		CString strServer;
		CString strObject;
		DWORD dwServiceType;
		INTERNET_PORT nPort;
		AfxParseURL(strURL, dwServiceType, strServer, strObject, nPort);
		//尝试根据关键字解析http结果
		std::map<CString, CString> mapURL = FindKeyWordURL(cstrRet, setKeyWord);
		//遍历解析提取到的URL并尝试保存
		for (std::map<CString, CString>::iterator it = mapURL.begin(); it != mapURL.end(); it++)
		{
			CString strUrlName = it->second;
			CString strSubURL = it->first;
			CString strSubServer;
			//先判定这个URL是否完整
			AfxParseURL(strSubURL, dwServiceType, strSubServer, strObject, nPort);
			//找不到头，则试着将server加上然后再试一次
			if (AFX_INET_SERVICE_HTTP != dwServiceType && AFX_INET_SERVICE_HTTPS != dwServiceType)
			{
				strSubURL = _T("http://") + strServer + _T("/") + strSubURL;
				AfxParseURL(strSubURL, dwServiceType, strSubServer, strObject, nPort);
				//还是找不到头，则放弃本条
				if (AFX_INET_SERVICE_HTTP != dwServiceType && AFX_INET_SERVICE_HTTPS != dwServiceType)
				{
					continue;
				}
			}
			//找到了就继续
			//判定是否已经加载过这个网址
			if (m_setStrLoadedUrl.count(strSubURL) < 1)
			{
				m_setStrLoadedUrl.insert(strSubURL);
				std::string cstrRet;
				CHttpClient client(IE_AGENT);
				if (SUCCESS == client.HttpGet(strSubURL, NULL, cstrRet))
				{

					CString strFileName = strUrlName;
					//对帖名做一个简要处理，把问号等符号干掉
					strFileName.Replace(_T("/"), _T(""));
					strFileName.Replace(_T("\\"), _T(""));
					strFileName.Replace(_T(":"), _T(""));
					strFileName.Replace(_T("*"), _T(""));
					strFileName.Replace(_T("\""), _T(""));
					strFileName.Replace(_T("?"), _T(""));
					strFileName.Replace(_T("<"), _T(""));
					strFileName.Replace(_T(">"), _T(""));
					strFileName.Replace(_T("|"), _T(""));
					strFileName.Replace(_T("="), _T(""));
					strFileName.Replace(_T("+"), _T(""));
					if (!CGlobalFunction::ValidFileName(strFileName))
					{
						strFileName = _T("帖名非法");
					}
					strFileName = strPath + _T("\\") + strFileName + _T(".html");
					strFileName = CGlobalFunction::GetUnDuplicateFileName(strFileName);
					CFile file;
					if(file.Open(strFileName, CFile::modeCreate | CFile::modeWrite))
					{
						file.Write(cstrRet.c_str(), cstrRet.size() * sizeof(char));
						file.Close();
					}
					//然后将图片存出来
					{
						CHttpClient client(IE_AGENT);
						CString strPath = CGlobalFunction::GetCurPath() + _T("\\img\\");
						CString strErr;
						CGlobalFunction::MakeSureDirectoryExists(strPath, strErr);
						client.SaveAllImg(cstrRet, strPath);
					}
					//get之后等待一个大于半秒(500ms)小于10秒(10000ms)的随机时长，用了对付爬虫识别
					srand(clock());
					int nTm = rand() % 9500 + 500;
					Sleep(nTm);
				}
			}
		}
	}
}

std::map<CString, CString> CWebSpiderDlg::FindKeyWordURL(std::string & cstrHtml, std::set<CString> & setKeyWord)
{
	//转换关键字到cstr
	std::set<std::string> csetKeyWord;
	for (std::set<CString>::iterator it = setKeyWord.begin(); it != setKeyWord.end(); it++)
	{
		CString strKey = *it;
		csetKeyWord.insert(CGlobalFunction::ConverCStringToStdString(strKey, CP_ACP));
	}

	std::map<CString, CString> mapRet;

	//解析分两轮，第一轮找有没有utf8的标记，如果有，转换后再继续
	{
		htmlcxx::HTML::ParserDom parser;
		tree<htmlcxx::HTML::Node> dom = parser.parseTree(cstrHtml);
		for(tree<htmlcxx::HTML::Node>::iterator it = dom.begin(); it != dom.end(); it++)
		{
			it->parseAttributes();
			std::map<std::string, std::string> mapAttr = it->attributes();
			std::string strTagName = it->tagName();
			std::string strText = it->text();
			for (std::map<std::string, std::string>::iterator itEle = mapAttr.begin(); itEle != mapAttr.end(); itEle++)
			{
				if (itEle->second.find("utf-8") != std::string::npos
					|| itEle->second.find("UTF-8") != std::string::npos)
				{
					//如果是utf8编码，则需要转码
					CString strGBK = CGlobalFunction::ConvertStdStringToCString(cstrHtml);
					std::string cstrGBK = CGlobalFunction::ConverCStringToStdString(strGBK, CP_ACP);
					cstrHtml = cstrGBK;
					break;
				}
			}
		}
	}

	//第二轮才真正开始查找关键字
	{
		htmlcxx::HTML::ParserDom parser;
		tree<htmlcxx::HTML::Node> dom = parser.parseTree(cstrHtml);
		//输出所有的文本节点
		for(tree<htmlcxx::HTML::Node>::iterator it = dom.begin(); it != dom.end(); it++)
		{
			BOOL bSaved = FALSE;
			if (it.node)
			{
				if (it.node->first_child)
				{
					tree_node_<htmlcxx::HTML::Node> * pNode = it.node->first_child;
					while (pNode)
					{
						std::string cstrChild = pNode->data.text();
						if (cstrChild.size() > 0)
						{
							if (csetKeyWord.size() > 0)
							{
								//如果关键字列表不为空，子节点中发现有关键字，则查找本节点是否有链接，如果有，就加入下一步的保存列表
								for (std::set<std::string>::iterator itKey = csetKeyWord.begin(); itKey != csetKeyWord.end(); itKey++)
								{
									std::string cstrKey = *itKey;
									if (cstrChild.find(cstrKey) != std::string::npos)
									{
										htmlcxx::HTML::Node & itNode = *it;
										itNode.parseAttributes();
										std::map<std::string, std::string> mapAttr = itNode.attributes();
										for (std::map<std::string, std::string>::iterator itEle = mapAttr.begin(); itEle != mapAttr.end(); itEle++)
										{
											if (itEle->first.find("href") != std::string::npos)
											{
												mapRet.insert(std::make_pair(CGlobalFunction::ConvertStdStringToCString(itEle->second), CGlobalFunction::ConvertStdStringToCString(cstrChild, CP_ACP)));
												bSaved = TRUE;
												break;
											}
										}
										if (bSaved)
										{
											break;
										}
									}
								}
								if (bSaved)
								{
									break;
								}
							}
							else
							{
								//如果没有给关键字，则无差别保存
								htmlcxx::HTML::Node & itNode = *it;
								itNode.parseAttributes();
								std::map<std::string, std::string> mapAttr = itNode.attributes();
								for (std::map<std::string, std::string>::iterator itEle = mapAttr.begin(); itEle != mapAttr.end(); itEle++)
								{
									if (itEle->first.find("href") != std::string::npos)
									{
										mapRet.insert(std::make_pair(CGlobalFunction::ConvertStdStringToCString(itEle->second), CGlobalFunction::ConvertStdStringToCString(cstrChild, CP_ACP)));
										bSaved = TRUE;
										break;
									}
								}
							}
						}
						pNode = pNode->next_sibling;
					}
				}
			}
		}
	}

	return mapRet;
}

void CWebSpiderDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CWebSpiderDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CWebSpiderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CWebSpiderDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	InitialLayout();
}

void CWebSpiderDlg::OnBnClickedBtnStart()
{
	CWaitCursor wait;
	//创建结果文件夹
	CString strErr;
	CGlobalFunction::MakeSureDirectoryExists(m_strRetPath + _T("\\"), strErr);
	//首先从控件拿到关键字列表及网址列表
	CWnd * pEditWebUrlList = GetDlgSafeItem(IDC_EDIT_WEBURL_LIST);
	CWnd * pEditKeyWordList = GetDlgSafeItem(IDC_EDIT_KEYWORD_LIST);
	if (pEditKeyWordList
		&& pEditWebUrlList)
	{
		std::vector<CString> vecStrURL;
		std::vector<CString> vecStrKeyWord;
		std::set<TCHAR> setMark;
		setMark.insert(_T('\n'));
		CString strAllUrl;
		pEditWebUrlList->GetWindowText(strAllUrl);
		strAllUrl.Replace(_T('\r'), _T(''));
		vecStrURL = CGlobalFunction::SplitCString(strAllUrl, setMark, FALSE);
		CString strAllKeyWord;
		pEditKeyWordList->GetWindowText(strAllKeyWord);
		strAllKeyWord.Replace(_T('\r'), _T(''));
		vecStrKeyWord = CGlobalFunction::SplitCString(strAllKeyWord, setMark, FALSE);
		std::set<CString> setStrURL;
		for (int i = 0; i < vecStrURL.size(); i++)
		{
			setStrURL.insert(vecStrURL[i]);
		}
		std::set<CString> setStrKeyWord;
		for (int i = 0; i < vecStrKeyWord.size(); i++)
		{
			setStrKeyWord.insert(vecStrKeyWord[i]);
		}

		BOOL bLoop = FALSE;
		CButton * pCheckLoop = (CButton *)GetDlgSafeItem(IDC_CHECK_LOOP);
		if (pCheckLoop)
		{
			bLoop = pCheckLoop->GetCheck();
		}

		do
		{
			//遍历URL列表
			for (std::set<CString>::iterator it = setStrURL.begin(); it != setStrURL.end(); it++)
			{
				CString strURL = *it;
				//处理一个网址
				DealOneURL(strURL, setStrKeyWord, m_strRetPath);
			}
		}while(bLoop);
	}
}

void CWebSpiderDlg::OnBnClickedBtnClearRetPath()
{
	CGlobalFunction::DeleteFileOrPath(m_strRetPath);
}


void CWebSpiderDlg::OnBnClickedBtnOpenRetPath()
{
	CString strErr;
	CGlobalFunction::MakeSureDirectoryExists(m_strRetPath + _T("\\"), strErr);
	ShellExecute(NULL, _T("open"), m_strRetPath, NULL, m_strRetPath, SW_SHOWNORMAL);
}

#if 0
//纯手工的比较原始的写法，留着便于理解http
void CWebSpiderDlg::OnBnClickedBtnStart()
{
	CString strHost = _T("blog.csdn.net");
	CString strPath = _T("/haunt_/article/details/50378608");
	CTcpSocket tcpSocket;
	tcpSocket.Create();
	tcpSocket.Connect(strHost, 80);

	std::stringstream stream;
	stream << "GET " << CGlobalFunction::ConverCStringToStdString(strPath);// << "?" << get_content;
	stream << " HTTP/1.1\r\n";
	stream << "Host: " << CGlobalFunction::ConverCStringToStdString(strHost) << "\r\n";
	stream <<"User-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; zh-CN; rv:1.9.2.3) Gecko/20100401 Firefox/3.6.3\r\n";
	stream <<"Connection:close\r\n\r\n";

	std::string strMsg = stream.str();

	tcpSocket.Send(strMsg.c_str(), strMsg.size());

	const int nBufSize = 1024;
	char cBuf[nBufSize];
	memset(cBuf, 0, nBufSize * sizeof(char));

	CFile file;
	if(file.Open(_T("a.txt"), CFile::modeCreate | CFile::modeWrite))
	{
		int nRet = 1;
		while(nRet > 0)
		{
			nRet = tcpSocket.Recv(cBuf, nBufSize - 1);
			if (nRet > 0 && nRet < nBufSize)
			{
				cBuf[nRet] = 0;
				file.Write(cBuf, nRet);
			}
			memset(cBuf, 0, nBufSize * sizeof(char));
		}
		file.Close();
	}

	tcpSocket.Close();
}
#endif
