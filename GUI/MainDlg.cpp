#include "stdafx.h"
#include "GUI.h"
#include "MainDlg.h"
#include "../Diff_capacitance/Core.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
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


// MainDlg dialog
MainDlg::MainDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(MainDlg::IDD, pParent)
	, dispersion(data.dispersion)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void MainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	// semiconductor props
	DDX_Text(pDX, IDC_EFF_MASS_EL, data.eff_mass_el);
	DDV_MinMaxDouble(pDX, data.eff_mass_el, 1e-4, 100);
	DDX_Text(pDX, IDC_EFF_MASS_HOLE, data.eff_mass_hole);
	DDV_MinMaxDouble(pDX, data.eff_mass_hole, 1e-4, 100);
	DDX_Text(pDX, IDC_PERMITTIVITY, data.permittivity);
	DDV_MinMaxDouble(pDX, data.permittivity, 1, 100);
	DDX_Text(pDX, IDC_BAND_GAP, data.band_gap);
	DDV_MinMaxDouble(pDX, data.band_gap, 1e-4, 10);
	DDX_Text(pDX, IDC_BAND_THICKNESS, data.thikness);
	DDV_MinMaxDouble(pDX, data.thikness, 1e-10, 1e-3);
	DDX_Text(pDX, IDC_IMP_CONC, data.imp_conc);
	DDV_MinMaxDouble(pDX, data.imp_conc, -1e24, 1e24);

	// problem props
	DDX_Text(pDX, IDC_VS_MIN, data.v_s_min);
	DDV_MinMaxDouble(pDX, data.v_s_min, -10, 10);
	DDX_Text(pDX, IDC_VS_MAX, data.v_s_max);
	DDV_MinMaxDouble(pDX, data.v_s_max, -10, 10);
	DDX_Text(pDX, IDC_NUM_POINTS, data.point_number);
	DDV_MinMaxInt(pDX, data.point_number, 10, 4000);

	dispersion = data.dispersion;
	DDX_Radio(pDX, IDC_KEIN, dispersion);
	data.dispersion = static_cast<Params::Dispersion>(dispersion);
	DDX_Control(pDX, IDC_RESULT, resultWnd);
	DDX_Control(pDX, IDC_PROGRESS, progressCtrl);
}

BEGIN_MESSAGE_MAP(MainDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CALCULATE, &MainDlg::OnBnClickedCalculate)
END_MESSAGE_MAP()


// MainDlg message handlers
BOOL MainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetupProgress();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void MainDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void MainDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR MainDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void MainDlg::OnBnClickedCalculate()
{
	if (UpdateData() && CheckData())
	{
		WriteLine(_T("Calculation started. Please wait...\r\n"));
		StartProgress();
		setProgress(2);		// just for debugging
		Core(&data, this);
		WriteLine(_T("\r\nCalculation finished."));
		setProgress(100);
	}
	else
	{
		AfxMessageBox(CString("Some data can be incorrect. Please verify!"));
	}
}

bool MainDlg::CheckData()
{
	return true;
}

// add string to the output window
void MainDlg::WriteLine(LPCTSTR text)
{
	CString fullText;
	resultWnd.GetWindowText(fullText);
	fullText += "\r\n";
	fullText += text;
	resultWnd.SetWindowText(fullText);
	UpdateData(FALSE);
	spinMessageLoop();
}

void MainDlg::spinMessageLoop()
{
	// Windows mantra
	MSG msg;
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage (&msg);
		DispatchMessage (&msg);
	}
	if (msg.message == WM_QUIT)
	{
		::PostQuitMessage(msg.wParam);
	}
	AfxGetApp()->OnIdle(0);
}

void MainDlg::SetupProgress()
{
	progressCtrl.SetRange32(0, 100);	// range for progress value (assumed to be percent of completion)
}

void MainDlg::StartProgress()
{
	progressCtrl.SetPos(0);
}

int MainDlg::getProgress() const
{
	return progressCtrl.GetPos();
}

int MainDlg::setProgress(int progressValue)
{
#ifdef _DEBUG
	assert(0 <= progressValue && progressValue <= 100 && "wrong progress value. Should be in range 0..100");
#endif // _DEBUG

	if (progressValue < 0)
		progressValue = 0;
	if (progressValue > 100)
		progressValue = 100;

	int wasProgressValue = progressCtrl.GetPos();
	progressCtrl.SetPos(progressValue);
	
	// force visual updating
	spinMessageLoop();

	return wasProgressValue;
}
