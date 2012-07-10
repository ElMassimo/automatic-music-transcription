// gademoView.h : interface of the CGademoView class
//
/////////////////////////////////////////////////////////////////////////////

#include <ga/ga.h>
#include <ga/GARealGenome.h>
#include "GAParametersDialog.h"
#include "GAOperatorsDialog.h"

#if !defined(AFX_GADEMOVIEW_H__27ACB97D_9765_11D2_836E_00C04F8F199B__INCLUDED_)
#define AFX_GADEMOVIEW_H__27ACB97D_9765_11D2_836E_00C04F8F199B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CGademoView : public CView
{
protected: // create from serialization only
	CGademoView();
	DECLARE_DYNCREATE(CGademoView)

// Attributes
public:
	CGademoDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGademoView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGademoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CGademoView)
	afx_msg void OnEvolve();
	afx_msg void OnReset();
	afx_msg void OnStep();
	afx_msg void OnStop();
	afx_msg void OnEvolveSome();
	afx_msg void OnParameters();
	afx_msg void OnSelectViewGrid();
	afx_msg void OnUpdateViewGrid(CCmdUI* pCmdUI);
	afx_msg void OnSelectGACrowding();
	afx_msg void OnUpdateGACrowding(CCmdUI* pCmdUI);
	afx_msg void OnSelectGASimple();
	afx_msg void OnUpdateGASimple(CCmdUI* pCmdUI);
	afx_msg void OnSelectGASteadyState();
	afx_msg void OnUpdateGASteadyState(CCmdUI* pCmdUI);
	afx_msg void OnSelectGASSSharing();
	afx_msg void OnUpdateGASSSharing(CCmdUI* pCmdUI);
	afx_msg void OnSelectGADeme();
	afx_msg void OnUpdateGADeme(CCmdUI* pCmdUI);
	afx_msg void OnSelectGAIncremental();
	afx_msg void OnUpdateGAIncremental(CCmdUI* pCmdUI);
	afx_msg void OnSelectRepReal();
	afx_msg void OnUpdateRepReal(CCmdUI* pCmdUI);
	afx_msg void OnSelectRepBinary();
	afx_msg void OnUpdateRepBinary(CCmdUI* pCmdUI);
	afx_msg void OnSelectRepBinary8Bit();
	afx_msg void OnUpdateRepBinary8Bit(CCmdUI* pCmdUI);
	afx_msg void OnSelectFuncRipples();
	afx_msg void OnUpdateFuncRipples(CCmdUI* pCmdUI);
	afx_msg void OnSelectFuncLoaf();
	afx_msg void OnUpdateFuncLoaf(CCmdUI* pCmdUI);
	afx_msg void OnSelectFuncFoxholes();
	afx_msg void OnUpdateFuncFoxholes(CCmdUI* pCmdUI);
	afx_msg void OnSelectFuncSchwefel();
	afx_msg void OnUpdateFuncSchwefel(CCmdUI* pCmdUI);
	afx_msg void OnSelectFuncRipplesShifted();
	afx_msg void OnUpdateFuncRipplesShifted(CCmdUI* pCmdUI);
	afx_msg void OnOperators();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	typedef enum { SIMPLE, STEADY_STATE, STEADY_STATE_SHARING, INCREMENTAL, DEME, CROWDING } Algorithm;
	typedef enum { RIPPLE, RIPPLE_SHIFTED, LOAF, FOXHOLES, SCHWEFEL } Function;
	typedef enum { REAL, BINARY_4BIT, BINARY_8BIT } Representation;
	typedef enum { SINGLE_POINT, TWO_POINT, UNIFORM, AVERAGING, BLX } Crossover;
	typedef enum { FLIP, GAUSSIAN, BOUNDARY, UNIFORM_MUT } Mutation;

	static int GenericCrossover(const GAGenome&, const GAGenome&, GAGenome*, GAGenome*);
	static int GenericMutation(GAGenome&, float);

	static float Ripples(GAGenome&);
	static float RipplesShifted(GAGenome&);
	static float Loaf(GAGenome&);
	static float Foxholes(GAGenome&);
	static float Schwefel(GAGenome&);
	
	static float RealComparator(const GAGenome&, const GAGenome&);

	static int RealAveragingCrossover(const GAGenome&, const GAGenome&, GAGenome*, GAGenome*);
	static int RealBLXCrossover(const GAGenome&, const GAGenome&, GAGenome*, GAGenome*);

	static int RealUniformMutation(GAGenome&, float);
	static int RealBoundaryMutation(GAGenome&, float);
	static int RealGaussianMutation(GAGenome&, float);

	GAGeneticAlgorithm& ga() const { return *_theGA; }
	float pCrossover() const { return _pcross; }
	float pCrossover(float n);
	float pMutation() const { return _pmut; }
	float pMutation(float n);
	int nGenerations() const { return _ngen; }
	int nGenerations(int n);
	int populationSize() const { return _popsize; }
	int populationSize(int n);
	Mutation mutation() const { return _whichMut; }
	Mutation mutation(Mutation);
	Crossover crossover() const { return _whichCross; }
	Crossover crossover(Crossover);

	static Algorithm _whichGA;
	static Function _whichFunction;
	static Representation _whichRep;
	static Crossover _whichCross;
	static Mutation _whichMut;

protected:
	volatile int _enabled;
	volatile int _running;
	volatile int _showGrid;

	GAGenome* _theGenome;
	GAGeneticAlgorithm* _theGA;

	CGAParametersDialog* _params;
	CGAOperatorsDialog* _ops;

	float _pmut;
	float _pcross;
	int _ngen;
	int _popsize;
	int _npop;

	void configure();
	void draw(CDC*);
	void drawPopulation(CDC*, const GAPopulation&, int, int, int, float,CPen*,CPen*);

	static UINT Evolve(LPVOID); 
};

#ifndef _DEBUG  // debug version in gademoView.cpp
inline CGademoDoc* CGademoView::GetDocument()
   { return (CGademoDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GADEMOVIEW_H__27ACB97D_9765_11D2_836E_00C04F8F199B__INCLUDED_)
