#pragma once


#include <iostream>
#include <algorithm>
#include <functional>
#include <vector>
#include "afxwin.h"
using namespace std;
struct BrowsData
{
public:
	// ��ַ
	CString strURL;

	// ��Ӧ��ַ���ʴ���
	unsigned int nCount;

	// ����<������
	bool operator < (const BrowsData &m)const
	{
		return nCount > m.nCount;
	}
};
class BrowsHistory
{
private:

	// �����õ���ַ�ͷ��ʴ���
	std::vector<BrowsData> m_BrowsHistroy;
private:
	// IE��ַ���ˣ���ֻȡ��ַcomǰ�ߵ�
	void urlFiltrateIE(LPWSTR lpszSourceUrlName);

	// Chrome��ַ���ˣ���ֻȡ��ַcomǰ�ߵ�
	void urlFiltrateChrome(CString strUrlName);
	// Firefox��ַ���ˣ���ֻȥ��ַcomǰ�ߵ�
	void urlFiltrateFirefox(CString strUrlName, int nCount);
	// ��ѯ�����Ƿ��Ѵ���, ����true��ʾ���ڣ��Զ����������
	bool IsRunning(CString exe);
	// ����ת��
	void ConvertUtf8ToGBK(CStringA &strUtf8);
	// ��ȡ�������ʷ��¼
	void InitHistroy(void);
	// ���̺߳���
	static void ThreadPro(LPVOID * ptr);
	// �Ի�õ���ַ��������
	void Sort(void);
public:
	BrowsHistory();
	~BrowsHistory();
	// ��ȡ��ַ�Ľ��̣��Ƿ�ִ���ꣻִ����ʱΪtrue��
	bool m_bStatus;
	// ��ʼ��
	void Init(void);
	// ��ȡ�������ʷ��¼
	std::vector<BrowsData> GetBrowsHistory(void) const;
};