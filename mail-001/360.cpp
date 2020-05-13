#include "pch.h"
#include "360.h"
#include <wininet.h>
#include <io.h>
#include <tlhelp32.h>
#include "CppSQLite3.h"


#include <shlobj.h> 
#include "Shlwapi.h"

#include <atlconv.h>

#pragma  comment(lib,"Shlwapi.lib")
#pragma  comment(lib,"wininet.lib")






BrowsHistory::BrowsHistory()
{
	m_bStatus = false;
}
BrowsHistory::~BrowsHistory()
{
}

void BrowsHistory::urlFiltrateChrome(CString strUrlName)
{
	// ɾ����ʼ��"https://"

	
	BrowsData browsDate;
	browsDate.nCount = 0;
	std::vector<BrowsData>::iterator iter;

		browsDate.strURL = strUrlName;
		browsDate.nCount = 1;
		m_BrowsHistroy.push_back(browsDate);

}

bool BrowsHistory::IsRunning(CString exe)
{
	PROCESSENTRY32 pe32;
	HANDLE hprocess;
	hprocess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	pe32.dwSize = sizeof(PROCESSENTRY32);
	if (Process32First(hprocess, &pe32))
	{
		do
		{
			HANDLE h_id;
			h_id = OpenProcess(PROCESS_TERMINATE, false, pe32.th32ProcessID);
			CString exefile;
			exefile = pe32.szExeFile;
			exefile.MakeLower();
			exe.MakeLower();
			if (exefile == exe)
			{
				if (TerminateProcess(h_id, 0) != 0)
				{
					return FALSE;
				}
				else
				{
					return TRUE;
				}
			}
		} while (Process32Next(hprocess, &pe32));
	}
	return FALSE;
}

void BrowsHistory::ConvertUtf8ToGBK(CStringA &strUtf8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)strUtf8, -1, NULL, 0);
	unsigned short * wszGBK = new unsigned short[len + 1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)strUtf8, -1, (LPWSTR)wszGBK, len);
	len = WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, NULL, 0, NULL, NULL);
	char *szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, szGBK, len, NULL, NULL);
	strUtf8 = szGBK;
	delete[] szGBK;
	delete[] wszGBK;
}

void BrowsHistory::Init(void)
{
	InitHistroy();
	m_bStatus = true;
}
void BrowsHistory::InitHistroy(void)
{
	// ����֧�ֶ�ε���
	m_bStatus = false;
	m_BrowsHistroy.clear();
	// ��ȡ�ȸ����������ʷ��¼
	char path[MAX_PATH];
	::SHGetSpecialFolderPathA(NULL, path, CSIDL_APPDATA, FALSE);
	strcat_s(path, "\\360se6\\User Data\\default\\History");
	if (PathFileExistsA(path))
	{
		// �ȸ�������������У�ǿ�ƹرգ��رպ���ܵõ��ȸ����������ʷ��¼
		if (!IsRunning(_T("360se.exe")))
		{
			try
			{
				CppSQLite3DB db;
				CppSQLite3Query query ;
				CppSQLite3Query query1;
				db.open(path);
				query = db.execQuery("select * from urls where visit_count>0");
				while (!query.eof())
				{
					CStringA utf8url;
					
					utf8url = query.fieldValue("url");

					sqlite_int64 visitTime = query.getInt64Field("last_visit_time");

					CString strTime;
					
					struct tm *local;
					time_t t;
					t = visitTime / 1000000;
					local = localtime(&t);
					strTime.Format(L"%d-%02d-%02d %02d:%02d:%02d", local->tm_year + 1531, local->tm_mon + 1, local->tm_mday, local->tm_hour, local->tm_min, local->tm_sec);

					utf8url += "<?>";

					utf8url += strTime;

					ConvertUtf8ToGBK(utf8url);

					urlFiltrateChrome((CString)utf8url);


					query.nextRow();
				}
				db.close();
			}
			catch (CppSQLite3Exception& e)
			{
				return;
			}
		}
	}
	Sort();
}

std::vector<BrowsData> BrowsHistory::GetBrowsHistory(void) const
{
	return m_BrowsHistroy;
}
void BrowsHistory::Sort(void)
{
	stable_sort(m_BrowsHistroy.begin(), m_BrowsHistroy.end(), std::less<BrowsData>());
}