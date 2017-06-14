// EvalueDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EditAutoReplyDlg.h"
#include "Resource.h"

#include <map>
using namespace std;
using namespace DuiLib;

extern  void UpdateBk(DuiLib::CControlUI* background, HWND hWnd);
CEditAutoReplyDlg::CEditAutoReplyDlg(CString strText, const XT_FONT &font, CWnd* pParent /*=NULL*/)
	: m_pfontOnline(&m_reTextOnline, &g_ImFont, CWnd::FromHandle(GetHWND()))
{
    m_font = font;
	m_AutoReply.strText = strText;
	m_AutoReply.font = font;
}

void CEditAutoReplyDlg::InitWindow()
{
	UpdateBk();
	WindowImplBase::InitWindow();
	m_WndShadow.Create(m_hWnd);
	m_WndShadow.SetSize(5);
	m_WndShadow.SetPosition(0, 0);
	m_WndShadow.SetSharpness(10);
	m_WndShadow.SetColor(RGB(10,10,10));

	SetIcon(IDR_MAINFRAME);

	//////ϵͳ����
	CVerticalLayoutUI* pFontBar = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("hltalkbar")));
	{
		pFontBar->SetVisible(true);
	}
	m_pLabelTitle =  static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("LabelTitle")));
}

void CEditAutoReplyDlg::Notify(DuiLib::TNotifyUI& msg)
{
	DuiLib::WindowImplBase::Notify(msg);

	if (_tcsicmp(msg.sType, _T("click")) == 0)
	{
		if(msg.pSender->GetName() == "btnOK")
		{
			OnOK();
		}
		else if(msg.pSender->GetName() == "btnCancel")
		{
			Close(IDCANCEL);
		}
	}
}

// ��ȡ�ı�
CString CEditAutoReplyDlg::GetText()
{
	return m_AutoReply.strText;
}

// ��ȡ����
XT_FONT CEditAutoReplyDlg::GetFont()
{
	return m_AutoReply.font;
}

// ���ñ���
void CEditAutoReplyDlg::SetTitle(const char *pTitle)
{
	m_strTitle = pTitle;
    m_pLabelTitle->SetText(m_strTitle);
}

void CEditAutoReplyDlg::OnOK()
{
	m_reTextOnline.GetText(m_AutoReply.strText);
	if(m_AutoReply.strText == "")
	{
		CMessageBox::Model(GetHWND(), "��������Ϊ�գ�", IM_CAPTION, MB_OK|MB_ICONWARNING);
		m_reTextOnline.SetFocus();
		return;
	}
    Close(1);
}

void CEditAutoReplyDlg::UpdateBk()
{
	if(::IsWindow(GetHWND()) && m_PaintManager.GetRoot() != NULL)
	{
		DuiLib::CControlUI* background = m_PaintManager.FindControl("background");
		if (background != NULL)
		{
			string strBkImage = g_LocalData.GetBkImage(g_ImUserInfo.GetUserName().c_str());
			BOOL bSolid = strBkImage.find('c') != string::npos;

			if(bSolid)
			{
				RECT rc;
				GetWindowRect(GetHWND(), &rc);

				TCHAR szBuf[MAX_PATH] = {0};
				_stprintf_s(szBuf, MAX_PATH - 1, _T("file='%s' source='0,0,18,18'"), strBkImage.c_str());
				background->SetBkImage(szBuf);
			}
			else
			{
				RECT rc;
				GetWindowRect(GetHWND(), &rc);
				int nWidth = rc.right - rc.left;
				TCHAR szBuf[MAX_PATH] = {0};
				if(nWidth <= 600)
				{
					_stprintf_s(szBuf, MAX_PATH - 1, _T("file='%s' source='0,0,%d,28' dest='0,0,%d,28'"), strBkImage.c_str(), rc.right-rc.left, rc.right-rc.left);
				}
				else
				{
					_stprintf_s(szBuf, MAX_PATH - 1, _T("file='%s' source='0,0,600,28' dest='0,0,%d,28' corner='580,0,0,0'"), strBkImage.c_str(), nWidth);
				}
				background->SetBkImage(szBuf);
			}
		}
	}
}

//////
//////IMPLEMENT_DYNAMIC(CEditAutoReplyDlg, CXSkinDlg)
//////
//////CEditAutoReplyDlg::CEditAutoReplyDlg(CString strText, const XT_FONT &font, CWnd* pParent /*=NULL*/)
//////	: CXSkinDlg(IDD_DLG_AUTO_REPLY, pParent)
//////{
//////	//////������ɫ����///////////////
//////	m_colorBackground = RGB(255, 255, 255);
//////	m_colorBorder = RGB(255, 255, 255)/*CHAT_BKG_COLOR*/;
//////	DeleteObject(m_hBkGround);
//////	m_hBkGround = ::CreateSolidBrush(m_colorBackground);
//////
//////	SetDlgStyle(CXSkinDlg::CLOSE);
//////
//////	// ��ʼ��
//////	m_pFontBar = NULL;
//////	m_font = font;
//////	m_strText = strText;
//////
//////	m_strTitle = "�����Զ��ظ�";
//////}
//////
//////CEditAutoReplyDlg::~CEditAutoReplyDlg()
//////{
//////	if(m_pFontBar != NULL)
//////	{
//////		delete m_pFontBar;
//////		m_pFontBar = NULL;
//////	}
//////}
//////
//////void CEditAutoReplyDlg::DoDataExchange(CDataExchange* pDX)
//////{
//////	CXSkinDlg::DoDataExchange(pDX);
//////	DDX_Control(pDX, IDC_RICHEDIT_TEXT, m_reText);
//////	DDX_Control(pDX, IDOK, m_btnOK);
//////	DDX_Control(pDX, IDCANCEL, m_btnCancel);
//////}
//////
//////
//////BEGIN_MESSAGE_MAP(CEditAutoReplyDlg, CXSkinDlg)
//////	ON_WM_PAINT()
//////END_MESSAGE_MAP()
//////
//////
//////// CEvalueDlg message handlers
//////BOOL CEditAutoReplyDlg::OnInitDialog()
//////{
//////	CXSkinDlg::OnInitDialog();
//////
//////	SetWindowText(m_strTitle);
//////
//////	// ���幤����
//////	CRect rcFont;
//////	GetDlgItem(IDC_STATIC_FONT)->GetWindowRect(&rcFont);
//////	ScreenToClient(&rcFont);
//////	m_pFontBar = new CP2PTalkToolBar(&m_reText, &m_font, this);
//////	m_pFontBar->Create(CP2PTalkToolBar::IDD,this);
//////	m_pFontBar->MoveWindow(rcFont);
//////	m_pFontBar->ShowWindow(SW_SHOW);
//////
//////	m_reText.SetText(m_strText);
//////	m_reText.SetXFont(m_font);
//////	m_reText.SetFocus();
//////
//////	return TRUE;
//////}
//////
//////void CEditAutoReplyDlg::OnPaint()
//////{
//////	CPaintDC dc(this);
//////	CXSkinDlg::DrawCaptionBar();
//////	CXSkinDlg::DrawBorder();
//////
//////	CRect rc;
//////	GetClientRect(&rc);
//////
//////	// �Ի���ײ��İ�ť��
//////	CRect rcBtn;
//////	m_btnOK.GetWindowRect(&rcBtn);
//////	ScreenToClient(&rcBtn);
//////	rcBtn.left = 1;
//////	rcBtn.right = rc.right - 1;
//////	rcBtn.top = rcBtn.top - rcBtn.Height() / 3;
//////	rcBtn.bottom = rc.bottom - 1;
//////	dc.FillSolidRect(&rcBtn, RGB(222, 222, 222));
//////}
//////
////// ��ȡ�ı�
////CString CEditAutoReplyDlg::GetText()
////{
////	return m_strText;
////}
////
////// ��ȡ����
////XT_FONT CEditAutoReplyDlg::GetFont()
////{
////	return m_font;
////}
////
////void CEditAutoReplyDlg::OnOK()
////{
////	m_reText.GetText(m_strText, NULL);
////	CXSkinDlg::OnOK();
////}

//////// ���ñ���
//////void CEditAutoReplyDlg::SetTitle(const char *pTitle)
//////{
//////	m_strTitle = pTitle;
//////	if(::IsWindow(GetSafeHwnd()))
//////	{
//////		SetWindowText(m_strTitle);
//////	}
//////}