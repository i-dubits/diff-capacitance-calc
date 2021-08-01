#pragma once

#include "stdafx.h"
#include "../Diff_capacitance/Params.h"
#include "resource.h"

// MainDlg dialog
class MainDlg : public CDialogEx
{
// Construction
public:
	MainDlg(CWnd* pParent = NULL);	// standard constructor
	afx_msg void OnBnClickedCalculate();
	void WriteLine(LPCTSTR text);
	void spinMessageLoop();
	int setProgress(int soFar);
	int getProgress() const;

// Dialog Data
	enum { IDD = IDD_ROOT_DIALOG };
	Params data;
	int dispersion;

private:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	bool CheckData();
	void SetupProgress();
	void StartProgress();

// Implementation
private:
	HICON m_hIcon;
	CEdit resultWnd;
	CProgressCtrl progressCtrl;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	DECLARE_MESSAGE_MAP()
};
