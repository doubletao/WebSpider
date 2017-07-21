/*
 * ��Ҫ��һЩ����ͨ���ԵĹ��ߺ�����
 * 2015.09.07 �Ž���
 * �й�����
*/
#pragma once
#include <string>
#include <set>
#include <vector>
#include <queue>
#include <afxmt.h>
using std::string;
using std::queue;

class CGlobalFunction
{
public:
	static CString GetNewGUID();//�����µ�GUID
	static HRESULT GuidToString(const GUID& guid, CString& str);//ת��GUID���ַ���
	static void GetPathAndName(const CString &strSrcFilePath, CString &strPath, CString &strName);//�����ļ�ȫ����ȡ�ļ�·�����ļ���
	static CString GetUnDuplicateFileName(const CString &strSrcFilePath);//����ָ���ļ�����ȡһ�����������ļ���(ͨ�����(n)����ʽ����������)
	static BOOL IsFileExists(const CString &strSrcFilePath);//�ж��ļ��Ƿ����
	static string ConverCStringToStdString(CString & cstrValue);//CString��string֮���ת��
	static string ConverCStringToStdString(CString & cstrValue, DWORD dwFormatW);//CString��string֮���ת��
	static CString ConvertStdStringToCString(const string & strValue);//CString��string֮���ת��
	static CString ConvertStdStringToCString(const string & strValue, DWORD dwFormatW);//CString��string֮���ת��
	static CString GetFilePath(CString strFullFileName);//��ȡ�ļ�·��
	static CString GetFileName(CString strFullFileName);//��ȡ������չ�����ļ���
	static CString GetFileNameWithExt(CString strFullFileName);//��ȡ����չ�����ļ���
	static CString GetFileNameExt(const CString strSrcFileName);//��ȡ�ļ���չ��.
	static CString FMT(LPCTSTR lpszFormat, ...);//�Ѳ������Ĳ�����ʽ��
	static BOOL MakeSureDirectoryExists(CString strFilePath, CString & strErrInfo);//ȷ���ļ�·�����ڣ�����������򴴽�������true��ʾ·�����ڻ��ؽ��ɹ�
	static void LoadBmpFileToCbitmap(const CString & strFilePath, CBitmap & bm);//��ָ��BMP�ļ�װ�ص�ָ��Cbitmap
	static CString GetSmartFileSize(unsigned long long llFileSize);//��ȡ�ļ���С���ı�����
	static unsigned long long GetFileSize(const CString & strFulePath);//��ȡָ���ļ���С
	static void SetBootStart(BOOL bStart, CString strVariable);//�趨��ǰ���򿪻���������������д��ע���ı�����
	static CString GetCurPath();//��ȡ��ǰexe·��
	static CString GetNameOfComputer();//��ȡ��������
	static CString ConvertNumToCString(DWORD dwValue);//ת��DWORD��csing
	static CString ConvertNumToCString(int nValue);//ת��������csing
	static CString ConvertNumToCString(float fValue);//ת����������csing
	static CString ConvertNumToCString(double dValue);//ת��˫����С����csing
	static int ConvertCStringToInt(CString strValue);//ת��cstring������
	static float ConvertCStringToFloat(CString strValue);//ת��cstring��������
	static double ConvertCStringToDouble(CString strValue);//ת��cstring��˫����С��
	static int ConvertStdSringToInt(string & strValue);//ת��string������
	static DWORD ConvertStdSringToDWORD(string & strValue);//ת��string���޷�������
	static float ConvertStdStringToFloat(string & strValue);//ת��string��������
	static double ConvertStdStringToDouble(string & strValue);//ת��string��˫����С��
	static std::vector<string> SplitStdString(string & strValue, std::set<char> & setMark, BOOL bIncludeMark);//��strValue��setMark�ָ�Ϊstring���飬bIncludeMarkָ���ָ����Ƿ�����ָ�������
	static std::vector<CString> SplitCString(CString & strValue, std::set<TCHAR> & setMark, BOOL bIncludeMark);//��strValue��setMark�ָ�Ϊstring���飬bIncludeMarkָ���ָ����Ƿ�����ָ�������
	static CString ConvertNumToChinese(DWORD nNum);//ת�����ֵ����ģ�Ŀǰ��֧�������ҽ�֧�ֵ�ǧ����
	static BOOL CanConvertToNum(CString strNum);//�ж�һ�����Ƿ���Ա�ת��Ϊ���֣����ж����ַ��Ƿ������֣�
	static std::vector<CString> ScanFilePath(CString strFilePath);//����ָ��·������ȡ·���µ��ļ����б����ݹ��ļ��У�
	static BOOL IsDirectory(CString strPath);// �Ƿ����ļ���
	static BOOL DeleteFileOrPath(CString strFilePath);//ɾ���ļ����У�
	static BOOL RenameFileOrPath(CString strDesPath, CString strSrcPath);//�����ļ����У�
	static BOOL CopyFileOrPath(CString strDesPath, CString strSrcPath);//�����ļ����У�
	static BOOL ValidFileName(CString strFileName);//���һ���ļ����Ƿ�Ϸ�(ͨ���ܷ����������ļ��������ж�)
};

class CLogProcessor
{
private:
	queue<CString> m_queStrLogs;
	CRITICAL_SECTION m_lockLog;

	CWinThread * m_pThLog;
	CEvent m_hStopEvent;

	CLogProcessor(void);
public:
	~CLogProcessor(void);
	static CLogProcessor & Instance();

	UINT static LogThread(LPVOID pParam);

	void AddLogInfo(CString strLog);
	void WriteOneLog(CString strLog);
};

//ʱ��������ĺ꿪��
#define TIME_DEBUG_RECORD_OPEN

//ʱ��������ı�ݷ�ʽ�����Զ���¼������
#ifndef TIME_DEBUG_RECORD
#define TIME_DEBUG_RECORD \
	TimerRecorder tm(__FUNCTION__);
#endif

#ifndef TIME_DEBUG_RECORD_FILTER
#define TIME_DEBUG_RECORD_FILTER(strFilter) \
	TimerRecorder tm(__FUNCTION__, (strFilter));
#endif

//ʱ��������࣬�ڹ���ʱ���µ�ǰcpuʱ�ӣ�������ʱ�������������ʱ�����������к�ʱ����
class TimerRecorder
{
	friend class CLogProcessor;
private:
	static BOOL g_bShutDown;//�ܿ���
	static CString g_strFilter;//������������Ϊ��ʱ���������������ͬ����Ϣ
	static BOOL g_bFileMode;//trueָ��ͨ���ļ���ʽ�����falseָ��ͨ��trace��ʽ���
	static CString g_strFileName;//������ļ�ʱ���ļ���
	int m_nBorn;
	CString m_strInfo;
	CString m_strFilter;
private:
	void WriteToFile(CString strMsg);
	void WriteToOutput(CString strMsg);
public:
	TimerRecorder(CString strInfo, LPCTSTR lpFilter = NULL);
	TimerRecorder(const char * pStrInfo, const char * pFilter = NULL);
	~TimerRecorder();
	static void ReadConfig(CString strConfigPath = _T(""));//�������ļ��ж�ȡ����
	void Record(BOOL bResetTime = TRUE, LPCTSTR lpStr = NULL);
};
