
// CodeCounterDlg.h : ͷ�ļ�
//

#pragma once

#include "afxwin.h"
#include "CounterDef.h"
#include "FileCounter.h"
#include <map>
#include "afxcmn.h"
#include "Thread/spinLock.h"
#include "Thread/ThreadManage.h"

#define MAX_FIND_FILE_NUM		9999

// CCodeCounterDlg �Ի���
class CCodeCounterDlg : public CDialogEx
{
	enum EM_TIMER_INDEX
	{
		EM_TIMER_SHOW_LIST,
	};
typedef map<CString, stCounterConfig>	MAP_CONFIG;
// ����
public:
	CCodeCounterDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CODECOUNTER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnScan();
	afx_msg void OnBnClickedRadioFolder();
	afx_msg void OnBnClickedRadioFile();
	afx_msg void OnBnClickedOk();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButton1();

public:
	CButton m_radioCounterType;
	CString m_cstrPath;
	DWORD m_nNullCount;
	DWORD m_nCodeCount;
	DWORD m_nNoteCount;
	DWORD m_nNoteCodeCount;
	DWORD m_nAllCount;
	CListCtrl m_List;
	CString m_cstrTest;
	CListBox m_lstBoxType;
	CString m_cstrAddType;
	CString m_cstrAddNote;
	CString m_cstrAddBeginNote;
	CString m_cstrAddEndNote;

private:	
	// ����õ�һ���ļ���Ŀ¼·��
	CString		GetDirectory(void);
	// ���ļ����µõ������ļ��б�
	void		FindFile(char *lpPath, vector<CString> &vctFilePath, size_t nMaxNum = MAX_FIND_FILE_NUM);
	// ��ini�ļ��ж�ȡ����
	void		ReadIniFile();
	// �������õ�ini�ļ���
	void		WriteIniFile();
	// ��ʾ�ļ��б���ϸ����
	void		ShowListData(CString cstrFileName, const stFileMsg stData);
	// �������
	void		Clear();

private:
	EM_COUTER_AREA		m_nCounterType;		// ͳ������
	vector<CString>		m_vctFileList;		// �ҵ����ļ��б�
	CStringArray		m_strArray;			// �ļ������б�
	MAP_CONFIG			m_mapTypeConfig;	// ͳ�������б�
	CFileCounter		m_CFileCounter;		// Դ��ͳ����
	int					m_nCountNum;		// ͳ�Ƶ��ļ���
};
