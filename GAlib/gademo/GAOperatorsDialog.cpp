// GAOperatorsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "gademo.h"
#include "GAOperatorsDialog.h"
#include "gademoDoc.h"
#include "gademoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGAOperatorsDialog dialog


CGAOperatorsDialog::CGAOperatorsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CGAOperatorsDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGAOperatorsDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pParent = pParent;
//	SetDefID(IDC_APPLY);
}

BOOL CGAOperatorsDialog::Create(){
	return CDialog::Create(CGAOperatorsDialog::IDD, m_pParent);
}


void CGAOperatorsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGAOperatorsDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGAOperatorsDialog, CDialog)
	//{{AFX_MSG_MAP(CGAOperatorsDialog)
	ON_BN_CLICKED(IDC_OP_APPLY, OnApply)
	ON_BN_CLICKED(IDC_OP_CLOSE, OnClose)
	ON_BN_CLICKED(IDC_OP_REVERT, OnRevert)
	ON_BN_CLICKED(IDC_CROSS_AVERAGE, OnSelectCrossAverage)
	ON_BN_CLICKED(IDC_CROSS_BLX, OnSelectCrossBLX)
	ON_BN_CLICKED(IDC_CROSS_ONEPT, OnSelectCrossOnePT)
	ON_BN_CLICKED(IDC_CROSS_TWOPT, OnSelectCrossTwoPT)
	ON_BN_CLICKED(IDC_CROSS_UNIFORM, OnSelectCrossUniform)
	ON_BN_CLICKED(IDC_MUT_BOUNDARY, OnSelectMutBoundary)
	ON_BN_CLICKED(IDC_MUT_FLIP, OnSelectMutFlip)
	ON_BN_CLICKED(IDC_MUT_GAUSSIAN, OnSelectMutGaussian)
	ON_BN_CLICKED(IDC_MUT_UNIFORM, OnSelectMutUniform)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGAOperatorsDialog message handlers

void CGAOperatorsDialog::configure() {
	if(CGademoView::_whichRep == CGademoView::BINARY_4BIT || CGademoView::_whichRep == CGademoView::BINARY_8BIT) {
		GetDlgItem(IDC_MUT_FLIP)->EnableWindow(TRUE);
	
		GetDlgItem(IDC_CROSS_BLX)->EnableWindow(FALSE);
		GetDlgItem(IDC_CROSS_AVERAGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_MUT_GAUSSIAN)->EnableWindow(FALSE);
		GetDlgItem(IDC_MUT_BOUNDARY)->EnableWindow(FALSE);
		GetDlgItem(IDC_MUT_UNIFORM)->EnableWindow(FALSE);
	}
	else {
		GetDlgItem(IDC_MUT_FLIP)->EnableWindow(FALSE);

		GetDlgItem(IDC_CROSS_BLX)->EnableWindow(TRUE);
		GetDlgItem(IDC_CROSS_AVERAGE)->EnableWindow(TRUE);
		GetDlgItem(IDC_MUT_GAUSSIAN)->EnableWindow(TRUE);
		GetDlgItem(IDC_MUT_BOUNDARY)->EnableWindow(TRUE);
		GetDlgItem(IDC_MUT_UNIFORM)->EnableWindow(TRUE);
	}

	((CButton*)GetDlgItem(IDC_CROSS_ONEPT))->SetCheck((CGademoView::_whichCross == CGademoView::SINGLE_POINT) ? 1 : 0);
	((CButton*)GetDlgItem(IDC_CROSS_TWOPT))->SetCheck((CGademoView::_whichCross == CGademoView::TWO_POINT) ? 1 : 0);
	((CButton*)GetDlgItem(IDC_CROSS_BLX))->SetCheck((CGademoView::_whichCross == CGademoView::BLX) ? 1 : 0);
	((CButton*)GetDlgItem(IDC_CROSS_UNIFORM))->SetCheck((CGademoView::_whichCross == CGademoView::UNIFORM) ? 1 : 0);
	((CButton*)GetDlgItem(IDC_CROSS_AVERAGE))->SetCheck((CGademoView::_whichCross == CGademoView::AVERAGING) ? 1 : 0);

	((CButton*)GetDlgItem(IDC_MUT_GAUSSIAN))->SetCheck((CGademoView::_whichMut == CGademoView::GAUSSIAN) ? 1 : 0);
	((CButton*)GetDlgItem(IDC_MUT_BOUNDARY))->SetCheck((CGademoView::_whichMut == CGademoView::BOUNDARY) ? 1 : 0);
	((CButton*)GetDlgItem(IDC_MUT_UNIFORM))->SetCheck((CGademoView::_whichMut == CGademoView::UNIFORM_MUT) ? 1 : 0);
	((CButton*)GetDlgItem(IDC_MUT_FLIP))->SetCheck((CGademoView::_whichMut == CGademoView::FLIP) ? 1 : 0);
}

void CGAOperatorsDialog::OnApply() {
	CGademoView& parent = ((CGademoView&)(*m_pParent));
	UpdateData(TRUE);
	GetDlgItem(IDC_OP_REVERT)->EnableWindow(FALSE);
	GetDlgItem(IDC_OP_APPLY)->EnableWindow(FALSE);
	CGademoView::_whichCross = (CGademoView::Crossover)m_cross;
	CGademoView::_whichMut = (CGademoView::Mutation)m_mut;	
	configure();
}

void CGAOperatorsDialog::OnClose() {
	EndDialog(TRUE);
	m_pParent->SetFocus();	
}

void CGAOperatorsDialog::OnRevert() {
	CGademoView& parent = ((CGademoView&)(*m_pParent));
	UpdateData(FALSE);
	GetDlgItem(IDC_OP_REVERT)->EnableWindow(FALSE);
	GetDlgItem(IDC_OP_APPLY)->EnableWindow(FALSE);
	m_cross = CGademoView::_whichCross;
	m_mut = CGademoView::_whichMut;	
	configure();
}

void CGAOperatorsDialog::OnSelectCrossAverage() {
	GetDlgItem(IDC_OP_REVERT)->EnableWindow(TRUE);
	GetDlgItem(IDC_OP_APPLY)->EnableWindow(TRUE);
	m_cross = CGademoView::AVERAGING;	
}

void CGAOperatorsDialog::OnSelectCrossBLX() {
	GetDlgItem(IDC_OP_REVERT)->EnableWindow(TRUE);
	GetDlgItem(IDC_OP_APPLY)->EnableWindow(TRUE);
	m_cross = CGademoView::BLX;	
}

void CGAOperatorsDialog::OnSelectCrossOnePT() {
	GetDlgItem(IDC_OP_REVERT)->EnableWindow(TRUE);
	GetDlgItem(IDC_OP_APPLY)->EnableWindow(TRUE);
	m_cross = CGademoView::SINGLE_POINT;	
}

void CGAOperatorsDialog::OnSelectCrossTwoPT() {
	GetDlgItem(IDC_OP_REVERT)->EnableWindow(TRUE);
	GetDlgItem(IDC_OP_APPLY)->EnableWindow(TRUE);
	m_cross = CGademoView::TWO_POINT;	
}

void CGAOperatorsDialog::OnSelectCrossUniform() {
	GetDlgItem(IDC_OP_REVERT)->EnableWindow(TRUE);
	GetDlgItem(IDC_OP_APPLY)->EnableWindow(TRUE);
	m_cross = CGademoView::UNIFORM;	
}

void CGAOperatorsDialog::OnSelectMutBoundary() {
	GetDlgItem(IDC_OP_REVERT)->EnableWindow(TRUE);
	GetDlgItem(IDC_OP_APPLY)->EnableWindow(TRUE);
	m_mut = CGademoView::BOUNDARY;	
}

void CGAOperatorsDialog::OnSelectMutFlip() {
	GetDlgItem(IDC_OP_REVERT)->EnableWindow(TRUE);
	GetDlgItem(IDC_OP_APPLY)->EnableWindow(TRUE);
	m_mut = CGademoView::FLIP;	
}

void CGAOperatorsDialog::OnSelectMutGaussian() {
	GetDlgItem(IDC_OP_REVERT)->EnableWindow(TRUE);
	GetDlgItem(IDC_OP_APPLY)->EnableWindow(TRUE);
	m_mut = CGademoView::GAUSSIAN;
}

void CGAOperatorsDialog::OnSelectMutUniform() {
	GetDlgItem(IDC_OP_REVERT)->EnableWindow(TRUE);
	GetDlgItem(IDC_OP_APPLY)->EnableWindow(TRUE);	
	m_mut = CGademoView::UNIFORM_MUT;
}
