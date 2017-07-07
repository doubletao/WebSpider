
// WebSpiderDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WebSpider.h"
#include "WebSpiderDlg.h"
#include "afxdialogex.h"
#include <sstream>
#include "GlobalFunction.h"
#include "TcpSocket.h"
#include "HttpClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CWebSpiderDlg �Ի���




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


// CWebSpiderDlg ��Ϣ�������

BOOL CWebSpiderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	InitialLayout();
	m_strRetPath = CGlobalFunction::GetCurPath() + _T("\\result");

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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
	if (
		pStcWebUrlList
		&& pStcKeyWordList 
		&& pEditWebUrlList 
		&& pEditKeyWordList
		&& pBtnClearRet
		&& pBtnOpenRet
		&& pBtnStart
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
		pBtnClearRet->MoveWindow(nWholeWidth - nBtnWidth * 3 - nGap * 3, nWholeHeight - nGap - nBtnHeight, nBtnWidth, nBtnHeight);
		pBtnOpenRet->MoveWindow(nWholeWidth - nBtnWidth * 2 - nGap * 2, nWholeHeight - nGap - nBtnHeight, nBtnWidth, nBtnHeight);
		pBtnStart->MoveWindow(nWholeWidth - nBtnWidth * 1 - nGap * 1, nWholeHeight - nGap - nBtnHeight, nBtnWidth, nBtnHeight);
	}
	Invalidate(FALSE);
}

void CWebSpiderDlg::DealOneURL(CString strURL, std::set<CString> & setKeyWord, CString strPath)
{
	//�ȴӷ����������������
	CString strRet;
	CHttpClient client(IE_AGENT);
	if (SUCCESS != client.HttpGet(strURL, NULL, strRet))
	{
		//��ȡʧ�ܣ�ֱ�ӷ���
		return;
	}
	if (!strRet.IsEmpty())
	{
		//��server���������������ʹ��
		CString strServer;
		CString strObject;
		DWORD dwServiceType;
		INTERNET_PORT nPort;
		AfxParseURL(strURL, dwServiceType, strServer, strObject, nPort);
		//���Ը��ݹؼ��ֽ���http���
		std::set<CString> setURL = FindKeyWordURL(strRet, setKeyWord);
		//����������ȡ����URL�����Ա���
		for (std::set<CString>::iterator it = setURL.begin(); it != setURL.end(); it++)
		{
			CString strSubURL = *it;
			CString strSubServer;
			//���ж����URL�Ƿ�����
			AfxParseURL(strSubURL, dwServiceType, strSubServer, strObject, nPort);
			//�Ҳ���ͷ�������Ž�server����Ȼ������һ��
			if (AFX_INET_SERVICE_HTTP != dwServiceType && AFX_INET_SERVICE_HTTPS != dwServiceType)
			{
				strSubURL = _T("http://") + strServer + strSubURL;
				AfxParseURL(strSubURL, dwServiceType, strSubServer, strObject, nPort);
				//�����Ҳ���ͷ�����������
				if (AFX_INET_SERVICE_HTTP != dwServiceType && AFX_INET_SERVICE_HTTPS != dwServiceType)
				{
					continue;
				}
			}
			//�ҵ��˾ͼ���
			CString strRet;
			CHttpClient client(IE_AGENT);
			if (SUCCESS == client.HttpGet(strURL, NULL, strRet))
			{
				CString strGUID = CGlobalFunction::GetNewGUID();
				CFile file;
				if(file.Open(strPath + _T("\\") + strGUID + _T(".html"), CFile::modeCreate | CFile::modeWrite))
				{
					std::string strTmp = CGlobalFunction::ConverCStringToStdString(strRet);
					file.Write(strTmp.c_str(), strTmp.size() * sizeof(char));
					file.Close();
				}
			}
		}
	}
}

std::set<CString> CWebSpiderDlg::FindKeyWordURL(CString & strHtml, std::set<CString> & setKeyWord)
{
	std::set<CString> setRet;
	setRet.insert(_T("/yc7369/article/details/38065435"));
	return setRet;
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CWebSpiderDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
	//��������ļ���
	CString strErr;
	CGlobalFunction::MakeSureDirectoryExists(m_strRetPath + _T("\\"), strErr);
	//���ȴӿؼ��õ��ؼ����б���ַ�б�
	std::set<CString> setStrURL;
	setStrURL.insert(_T("http://blog.csdn.net/yc7369/article/details/38065435"));
	std::set<CString> setStrKeyWord;
	//����URL�б�
	for (std::set<CString>::iterator it = setStrURL.begin(); it != setStrURL.end(); it++)
	{
		CString strURL = *it;
		//����һ����ַ
		DealOneURL(strURL, setStrKeyWord, m_strRetPath);
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
//���ֹ��ıȽ�ԭʼ��д�������ű������http
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
