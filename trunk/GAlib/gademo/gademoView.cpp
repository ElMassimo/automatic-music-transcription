// gademoView.cpp : implementation of the CGademoView class
//

#include "stdafx.h"
#include "gademo.h"

#include "gademoDoc.h"
#include "gademoView.h"

#include <math.h>
#include <ga/GARealGenome.C>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGademoView

IMPLEMENT_DYNCREATE(CGademoView, CView)

BEGIN_MESSAGE_MAP(CGademoView, CView)
	//{{AFX_MSG_MAP(CGademoView)
	ON_COMMAND(GA_CNTRL_EVOLVE, OnEvolve)
	ON_COMMAND(GA_CNTRL_REWIND, OnReset)
	ON_COMMAND(GA_CNTRL_STEP, OnStep)
	ON_COMMAND(GA_CNTRL_STOP, OnStop)
	ON_COMMAND(GA_CNTRL_SOME, OnEvolveSome)
	ON_COMMAND(GA_PARAMETERS, OnParameters)
	ON_COMMAND(GA_VIEW_GRID, OnSelectViewGrid)
	ON_UPDATE_COMMAND_UI(GA_VIEW_GRID, OnUpdateViewGrid)
	ON_COMMAND(GA_MALG_CROWDING, OnSelectGACrowding)
	ON_UPDATE_COMMAND_UI(GA_MALG_CROWDING, OnUpdateGACrowding)
	ON_COMMAND(GA_MALG_SIMPLE, OnSelectGASimple)
	ON_UPDATE_COMMAND_UI(GA_MALG_SIMPLE, OnUpdateGASimple)
	ON_COMMAND(GA_MALG_SS, OnSelectGASteadyState)
	ON_UPDATE_COMMAND_UI(GA_MALG_SS, OnUpdateGASteadyState)
	ON_COMMAND(GA_MALG_SS_SHARING, OnSelectGASSSharing)
	ON_UPDATE_COMMAND_UI(GA_MALG_SS_SHARING, OnUpdateGASSSharing)
	ON_COMMAND(GA_MALG_DEME, OnSelectGADeme)
	ON_UPDATE_COMMAND_UI(GA_MALG_DEME, OnUpdateGADeme)
	ON_COMMAND(GA_MALG_INCREMENTAL, OnSelectGAIncremental)
	ON_UPDATE_COMMAND_UI(GA_MALG_INCREMENTAL, OnUpdateGAIncremental)
	ON_COMMAND(GA_MREP_REAL, OnSelectRepReal)
	ON_UPDATE_COMMAND_UI(GA_MREP_REAL, OnUpdateRepReal)
	ON_COMMAND(GA_MREP_BIN, OnSelectRepBinary)
	ON_UPDATE_COMMAND_UI(GA_MREP_BIN, OnUpdateRepBinary)
	ON_COMMAND(GA_MREP_BIN8, OnSelectRepBinary8Bit)
	ON_UPDATE_COMMAND_UI(GA_MREP_BIN8, OnUpdateRepBinary8Bit)
	ON_COMMAND(GA_MFUNC_RIPPLES, OnSelectFuncRipples)
	ON_UPDATE_COMMAND_UI(GA_MFUNC_RIPPLES, OnUpdateFuncRipples)
	ON_COMMAND(GA_MFUNC_LOAF, OnSelectFuncLoaf)
	ON_UPDATE_COMMAND_UI(GA_MFUNC_LOAF, OnUpdateFuncLoaf)
	ON_COMMAND(GA_MFUNC_FOXHOLES, OnSelectFuncFoxholes)
	ON_UPDATE_COMMAND_UI(GA_MFUNC_FOXHOLES, OnUpdateFuncFoxholes)
	ON_COMMAND(GA_MFUNC_SCHWEFEL, OnSelectFuncSchwefel)
	ON_UPDATE_COMMAND_UI(GA_MFUNC_SCHWEFEL, OnUpdateFuncSchwefel)
	ON_COMMAND(GA_MFUNC_RIPPLE_SHIFT, OnSelectFuncRipplesShifted)
	ON_UPDATE_COMMAND_UI(GA_MFUNC_RIPPLE_SHIFT, OnUpdateFuncRipplesShifted)
	ON_COMMAND(GA_MREP_OPERATORS, OnOperators)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGademoView construction/destruction

CGademoView::CGademoView(){
	_theGenome = 0;
	_theGA = 0;
	_params = 0;
	_ops = 0;
	
	_pmut = (float)0.01;
	_pcross = (float)0.9;
	_ngen = 150;
	_popsize = 200;
	_npop = 1;

	_enabled = 1;
	_running = 0;
	_showGrid = 0;

	_whichGA = SIMPLE;
	_whichFunction = RIPPLE;
	_whichRep = REAL;
	_whichCross = SINGLE_POINT;
	_whichMut = GAUSSIAN;

	configure();
}

CGademoView::~CGademoView(){
	delete _params;
	delete _ops;
	delete _theGA;
	delete _theGenome;
}

BOOL CGademoView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CGademoView drawing

void CGademoView::OnDraw(CDC* pDC){
	CGademoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	draw(pDC);
}

/////////////////////////////////////////////////////////////////////////////
// CGademoView diagnostics

#ifdef _DEBUG
void CGademoView::AssertValid() const
{
	CView::AssertValid();
}

void CGademoView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGademoDoc* CGademoView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGademoDoc)));
	return (CGademoDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGademoView message handlers


void CGademoView::OnSelectViewGrid() {	
	_showGrid = (_showGrid ? 0 : 1);
	InvalidateRect(NULL);
}

void CGademoView::OnUpdateViewGrid(CCmdUI* pCmdUI) {
	if(_showGrid) pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);	
}




void CGademoView::OnEvolve() {
	if(!_running) {
		_enabled = -1;
		AfxBeginThread(Evolve, this, THREAD_PRIORITY_NORMAL);
	}
}

void CGademoView::OnReset() {
	_enabled = 0;
	while(_running) { ; } // wait for thread to stop
	if(_theGA) _theGA->initialize();
	InvalidateRect(NULL);
}

void CGademoView::OnStep() {
	if(!_running) {
		if(_theGA) _enabled = _theGA->generation()+1;
		AfxBeginThread(Evolve, this, THREAD_PRIORITY_NORMAL);
	}
}

void CGademoView::OnStop() {
	_enabled = 0;
}

void CGademoView::OnEvolveSome() {
	if(!_running) {
		if(_theGA) _enabled = _theGA->generation()+10;
		AfxBeginThread(Evolve, this, THREAD_PRIORITY_NORMAL);
	}
}




UINT CGademoView::Evolve(LPVOID param) {
	CGademoView* ptr = reinterpret_cast<CGademoView*>(param);

	if(ptr->_running) return 0;

	if(! ptr->_theGA) return 0;

	int n = ptr->_enabled;
	while(ptr->_enabled != 0) {
		ptr->_running = 1;
		if((n < 0 && ptr->_theGA->done() == gaFalse) || ptr->_theGA->generation() < n){
			ptr->_theGA->step();
			ptr->InvalidateRect(NULL);
		}
		else {
			ptr->_enabled = 0;
		}
	}
	ptr->_running = 0;
	return 0;
}







void CGademoView::OnSelectFuncRipples() {
	_whichFunction = RIPPLE;
	configure();	
	InvalidateRect(NULL);	
}

void CGademoView::OnSelectFuncRipplesShifted() {
	_whichFunction = RIPPLE_SHIFTED;
	configure();	
	InvalidateRect(NULL);		
}

void CGademoView::OnSelectFuncLoaf() {
	_whichFunction = LOAF;
	configure();	
	InvalidateRect(NULL);
}

void CGademoView::OnSelectFuncFoxholes() {
	_whichFunction = FOXHOLES;
	configure();	
	InvalidateRect(NULL);
}

void CGademoView::OnSelectFuncSchwefel() {
	_whichFunction = SCHWEFEL;
	configure();	
	InvalidateRect(NULL);
}

void CGademoView::OnUpdateFuncFoxholes(CCmdUI* pCmdUI) {
	if(_whichFunction == FOXHOLES) pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
}

void CGademoView::OnUpdateFuncLoaf(CCmdUI* pCmdUI) {
	if(_whichFunction == LOAF) pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
}

void CGademoView::OnUpdateFuncRipples(CCmdUI* pCmdUI) {
	if(_whichFunction == RIPPLE) pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
}

void CGademoView::OnUpdateFuncRipplesShifted(CCmdUI* pCmdUI) {
	if(_whichFunction == RIPPLE_SHIFTED) pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);	
}

void CGademoView::OnUpdateFuncSchwefel(CCmdUI* pCmdUI) {
	if(_whichFunction == SCHWEFEL) pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);	
}






void CGademoView::OnSelectGASimple() {
	_whichGA = SIMPLE;
	configure();	
	InvalidateRect(NULL);
}

void CGademoView::OnSelectGASteadyState() {
	_whichGA = STEADY_STATE;
	configure();	
	InvalidateRect(NULL);
}

void CGademoView::OnSelectGASSSharing() {
	_whichGA = STEADY_STATE_SHARING;
	configure();	
	InvalidateRect(NULL);
}

void CGademoView::OnSelectGAIncremental() {
	_whichGA = INCREMENTAL;
	configure();	
	InvalidateRect(NULL);
}

void CGademoView::OnSelectGACrowding() {
	_whichGA = CROWDING;
	configure();	
	InvalidateRect(NULL);
}

void CGademoView::OnSelectGADeme() {
	_whichGA = DEME;
	configure();	
	InvalidateRect(NULL);
}

void CGademoView::OnUpdateGASimple(CCmdUI* pCmdUI) {
	if(_whichGA == SIMPLE) pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
}

void CGademoView::OnUpdateGASteadyState(CCmdUI* pCmdUI) {
	if(_whichGA == STEADY_STATE) pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
}

void CGademoView::OnUpdateGASSSharing(CCmdUI* pCmdUI) {
	if(_whichGA == STEADY_STATE_SHARING) pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
}

void CGademoView::OnUpdateGAIncremental(CCmdUI* pCmdUI) {
	if(_whichGA == INCREMENTAL) pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
}

void CGademoView::OnUpdateGACrowding(CCmdUI* pCmdUI) {
	if(_whichGA == CROWDING) pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
}

void CGademoView::OnUpdateGADeme(CCmdUI* pCmdUI) {
	if(_whichGA == DEME) pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
}





void CGademoView::OnUpdateRepBinary(CCmdUI* pCmdUI) {
	if(_whichRep == BINARY_4BIT) pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
}

void CGademoView::OnUpdateRepBinary8Bit(CCmdUI* pCmdUI) {
	if(_whichRep == BINARY_8BIT) pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);	
}

void CGademoView::OnUpdateRepReal(CCmdUI* pCmdUI) {	
	if(_whichRep == REAL) pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
}

void CGademoView::OnSelectRepBinary() {	
	_whichRep = BINARY_4BIT;
	configure();	
	InvalidateRect(NULL);
}

void CGademoView::OnSelectRepBinary8Bit() {		
	_whichRep = BINARY_8BIT;
	configure();	
	InvalidateRect(NULL);
}

void CGademoView::OnSelectRepReal() {
	_whichRep = REAL;
	configure();	
	InvalidateRect(NULL);	
}






void CGademoView::OnParameters() {
	if(! _params) {
		_params = new CGAParametersDialog(this);
		_params->m_pcross = pCrossover();
		_params->m_pmut = pMutation();
		_params->m_ngen = nGenerations();
		_params->m_popsize = populationSize();
		if(_params->Create() != TRUE) {
			delete _params;
			_params = 0;
		}
	}
	else {
		_params->ShowWindow(SW_RESTORE);
		_params->EnableWindow();
		_params->UpdateWindow();
//		_params->SetActiveWindow();
	}

	if(_params) {
		_params->GetDlgItem(IDC_APPLY)->EnableWindow(FALSE);
		_params->GetDlgItem(IDC_REVERT)->EnableWindow(FALSE);
	}
}

void CGademoView::OnOperators() {
	if(! _ops) {
		_ops = new CGAOperatorsDialog(this);
		if(_ops->Create() != TRUE) {
			delete _ops;
			_ops = 0;
		}
	}
	else {
		_ops->ShowWindow(SW_RESTORE);
		_ops->EnableWindow();
		_ops->UpdateWindow();
		_ops->SetActiveWindow();
	}

	if(_ops) {
		_ops->GetDlgItem(IDC_OP_APPLY)->EnableWindow(FALSE);
		_ops->GetDlgItem(IDC_OP_REVERT)->EnableWindow(FALSE);
		_ops->configure();
	}
}







float CGademoView::pCrossover(float n) {
	if(0.0<=n && n<=1.0) {
		_pcross = _theGA->pCrossover(n);
	}
	return _pcross;
}
float CGademoView::pMutation(float n) {
	if(0.0<=n && n<=1.0) {
		_pmut = _theGA->pMutation(n);
	}
	return _pmut;
}
int CGademoView::populationSize(int n) {
	return _popsize = _theGA->populationSize(n);
}
int CGademoView::nGenerations(int n) {
	return _ngen = _theGA->nGenerations(n);
}
CGademoView::Mutation CGademoView::mutation(Mutation m) {
	return _whichMut = m;
}
CGademoView::Crossover CGademoView::crossover(Crossover m) {
	return _whichCross = m;
}













// Default values for the user-selectable parameters.
CGademoView::Function CGademoView::_whichFunction = RIPPLE;
CGademoView::Algorithm CGademoView::_whichGA = SIMPLE;
CGademoView::Representation CGademoView::_whichRep = REAL;
CGademoView::Crossover CGademoView::_whichCross = AVERAGING;
CGademoView::Mutation CGademoView::_whichMut = GAUSSIAN;



// This is where we contruct our genome, configure it, construct
// a genetic algorithm, configure that, and set everything up for
// an evolution.
void 
CGademoView::configure() {
	delete _theGenome;
	delete _theGA;

	int nbits = 4;
	if(_whichRep == BINARY_8BIT) nbits = 8;

	// construct a genome.  the function matters since that 
	// determines what the bounding box will be for the plot.
	// we also care about which representation has been selected.

	switch(_whichFunction) {
	case LOAF:
		if(_whichRep == BINARY_4BIT || _whichRep == BINARY_8BIT) {
			GABin2DecPhenotype map;
			map.add(nbits, -6, 6);
			map.add(nbits, -6, 6);
			_theGenome = new GABin2DecGenome(map, Loaf);
		}
		else _theGenome = new GARealGenome(2, GARealAlleleSet(-6,6), Loaf);
		break;
	case FOXHOLES:
		if(_whichRep == BINARY_4BIT || _whichRep == BINARY_8BIT) {
			GABin2DecPhenotype map;
			map.add(nbits, -60, 60);
			map.add(nbits, -60, 60);
			_theGenome = new GABin2DecGenome(map, Foxholes);
		}
		else _theGenome = new GARealGenome(2, GARealAlleleSet(-60,60), Foxholes);
		break;
	case SCHWEFEL:
		if(_whichRep == BINARY_4BIT || _whichRep == BINARY_8BIT) {
			GABin2DecPhenotype map;
			map.add(nbits, -500, 500);
			map.add(nbits, -500, 500);
			_theGenome = new GABin2DecGenome(map, Schwefel);
		}
		else _theGenome = new GARealGenome(2, GARealAlleleSet(-500,500), Schwefel);
		break;
	case RIPPLE_SHIFTED:
		if(_whichRep == BINARY_4BIT || _whichRep == BINARY_8BIT) {
			GABin2DecPhenotype map;
			map.add(nbits, -10, 10);
			map.add(nbits, -10, 10);
			_theGenome = new GABin2DecGenome(map, RipplesShifted);
		}
		else _theGenome = new GARealGenome(2, GARealAlleleSet(-10,10), RipplesShifted);
		break;
	case RIPPLE:
	default:
		if(_whichRep == BINARY_4BIT || _whichRep == BINARY_8BIT) {
			GABin2DecPhenotype map;
			map.add(nbits, -10, 10);
			map.add(nbits, -10, 10);
			_theGenome = new GABin2DecGenome(map, Ripples);
		}
		else _theGenome = new GARealGenome(2, GARealAlleleSet(-10,10), Ripples);
		break;
	}


	// set the operators on the genome, depending on what kind
	// of genome we have and what operators are desired.  note that
	// we could just use the defaults that are in GAlib, but by
	// doing it here we know exactly what operators we will be 
	// using.  (defaults are listed in the GAlib documentation)

	_theGenome->crossover(GenericCrossover);
	_theGenome->mutator(GenericMutation);

	if(_whichRep == BINARY_4BIT || _whichRep == BINARY_8BIT) {
		_theGenome->initializer(GA1DBinaryStringGenome::UniformInitializer);
//		_theGenome->comparator();

		if(_whichMut != FLIP) _whichMut = FLIP;
		if(_whichCross != UNIFORM && _whichCross != SINGLE_POINT &&
			_whichCross != TWO_POINT) _whichCross = UNIFORM;
	}
	else {
		_theGenome->initializer(GARealGenome::UniformInitializer);
		_theGenome->comparator(RealComparator);

		if(_whichMut == FLIP) _whichMut = GAUSSIAN;
		// any crossover is ok for real represenation...
	}

	if(_ops) _ops->configure();

	// now construct the genetic algorithm using the genome
	// that we have configured.

	switch(_whichGA) {
	case STEADY_STATE:
		_theGA = new GASteadyStateGA(*_theGenome);
		break;
	case STEADY_STATE_SHARING: {
		_theGA = new GASteadyStateGA(*_theGenome);
		GASharing share;
		_theGA->scaling(share);
							   }
		break;
	case INCREMENTAL:
		_theGA = new GAIncrementalGA(*_theGenome);
		break;
	case DEME:
		_theGA = new GADemeGA(*_theGenome);
		break;
	case CROWDING:
		_theGA = new GADCrowdingGA(*_theGenome);
		break;
	case SIMPLE:
	default:
		_theGA = new GASimpleGA(*_theGenome);
		break;
	}

	// set up the parameters on the genetic algorithm

	_theGA->pMutation(_pmut);
	_theGA->pCrossover(_pcross);
	_theGA->nGenerations(_ngen);
	_theGA->populationSize(_popsize);
	_theGA->initialize();
}

void 
CGademoView::draw(CDC* pDC) {
	if(! _theGA || ! _theGenome) return;

	RECT rect;
	GetClientRect(&rect);

	int width = rect.right;
	int height = rect.bottom;

	TEXTMETRIC  tm;
	pDC->GetTextMetrics( &tm );
	int charht = tm.tmHeight + tm.tmExternalLeading;

	const int BUF=30;

	unsigned int w = width - 2 * BUF;
	unsigned int h = height - 2 * BUF - charht;
	unsigned int d = (w < h ? w : h);
	w -= d;
	h -= d;
	unsigned int originx = BUF + w/2;
	unsigned int originy = BUF + h/2;

	float maxx = 0;
	float minx = 0;

	if(_whichRep == BINARY_4BIT || _whichRep == BINARY_8BIT) {
		const GABin2DecPhenotype& map = ((GABin2DecGenome*)_theGenome)->phenotypes();
		maxx = map.max(0);
		minx = map.min(0);
	}
	else {
		maxx = ((GARealGenome*)_theGenome)->alleleset().upper();
		minx = ((GARealGenome*)_theGenome)->alleleset().lower();
	}

	float factor = (float)d;
	factor /= maxx - minx;

 	int txtx = width/2;
	int txty = height - charht -5;

    pDC->SetTextColor(RGB(0,0,255));

	CString txt;
	txt.Format("%d", _theGA->generation());
	pDC->TextOut(txtx, txty, txt);

	static int NCOL=5;
	static int rpval[] = {100,  0,  0,  0,100};
	static int gpval[] = {  0,100,  0,100,100};
	static int bpval[] = {  0,  0,100,100,  0};
	static int rval[]  = {255,  0,  0,  0,255};
	static int gval[]  = {  0,255,  0,255,255};
	static int bval[]  = {  0,  0,255,255,  0};

	if(_showGrid) {
		CPen gpen( PS_SOLID, 2, RGB(240, 240, 240) );
		CPen* oldpen = pDC->SelectObject( &gpen );
		for(float i=minx; i<=maxx; i+=1.0){
			int x = (int)(originx + d/2 + factor*i);
			pDC->MoveTo(x,(int)(originy + d/2 + factor*maxx));
			pDC->LineTo(x,(int)(originy + d/2 - factor*maxx));
			int y = (int)(originy + d/2 - factor*i);
			pDC->MoveTo((int)(originx + d/2 - factor*maxx),y);
			pDC->LineTo((int)(originx + d/2 + factor*maxx),y);
		}
		pDC->SelectObject( oldpen );
	}

	if(_whichGA == DEME) {
		GADemeGA& ga = (GADemeGA&)(*_theGA);
		for(int i=0; i<ga.nPopulations(); i++) {
			CPen poppen( PS_SOLID, 2, RGB(rpval[i%NCOL], gpval[i%NCOL], bpval[i%NCOL]) );
			CPen bestpen( PS_SOLID, 2, RGB(rval[i%NCOL], gval[i%NCOL], bval[i%NCOL]) );
			drawPopulation(pDC, ga.population(i), originx, originy, d, factor,
				&poppen, &bestpen);
		}
	}
	else {
		CPen greypen( PS_SOLID, 2, RGB(150,150,150) );
		CPen redpen( PS_SOLID, 2, RGB(255,0,0) );
		drawPopulation(pDC, _theGA->population(), originx, originy, d, factor,
			&greypen, &redpen);
	}
}

// in windows, upper left is 0,0 and lower right is +,+
void 
CGademoView::drawPopulation(CDC* pDC, const GAPopulation& pop, 
								 int originx, int originy, int d, float factor,
								 CPen* miscpen, CPen* bestpen) {
	CPen* oldpen = pDC->SelectObject( miscpen );
	int xbest=0;
	int ybest=0;

	if(_whichRep == BINARY_4BIT || _whichRep == BINARY_8BIT) {
		for(int i=0; i<pop.size(); i++) {
			GABin2DecGenome& g = (GABin2DecGenome&)(pop.individual(i));
			int x = (int)(originx + d/2 + factor * g.phenotype(0));
			int y = (int)(originy + d/2 - factor * g.phenotype(1));
			pDC->MoveTo(x-1,y);
			pDC->LineTo(x+1,y);
			pDC->MoveTo(x,y-1);
			pDC->LineTo(x,y+1);
		}

		GABin2DecGenome& b = (GABin2DecGenome&)(pop.best());
		xbest = (int)(originx + d/2 + factor * b.phenotype(0));
		ybest = (int)(originy + d/2 - factor * b.phenotype(1));
	}
	else {
		for(int i=0; i<pop.size(); i++) {
			GARealGenome& g = (GARealGenome&)(pop.individual(i));
			int x = (int)(originx + d/2 + factor * g.gene(0));
			int y = (int)(originy + d/2 - factor * g.gene(1));
			pDC->MoveTo(x-1,y);
			pDC->LineTo(x+1,y);
			pDC->MoveTo(x,y-1);
			pDC->LineTo(x,y+1);
		}
		GARealGenome& b = (GARealGenome&)(pop.best());
		xbest = (int)(originx + d/2 + factor * b.gene(0));
		ybest = (int)(originy + d/2 - factor * b.gene(1));
	}

	pDC->SelectObject( bestpen );

	pDC->SelectStockObject(GRAY_BRUSH);
	pDC->MoveTo(xbest-2,ybest);
	pDC->LineTo(xbest+2,ybest);
	pDC->MoveTo(xbest,ybest-2);
	pDC->LineTo(xbest,ybest+2);

	pDC->SelectObject( oldpen );
}





// Mutation is define generically here so that we can change 
// mutation behavior during the course of an evolution without
// restarting the evolution.
int
CGademoView::GenericMutation(GAGenome& g, float pmut) {
	int n = 0;
	if(_whichRep == BINARY_4BIT || _whichRep == BINARY_8BIT) {
		switch(_whichMut) {
		case FLIP:
		default:
			n = GA1DBinaryStringGenome::FlipMutator(g, pmut);
			break;
		}
	}
	else {
		switch(_whichMut) {
		case BOUNDARY:
			n = RealBoundaryMutation(g, pmut);
			break;
		case UNIFORM_MUT:
			n = RealUniformMutation(g, pmut);
			break;
		case GAUSSIAN:
		default:
			n = RealGaussianMutation(g, pmut);
			break;
		}
	}
	return n;
}

// This generic crossover operator lets us change crossover
// during the course of an evolution.
int
CGademoView::GenericCrossover(const GAGenome& p1, const GAGenome& p2, GAGenome* c1, GAGenome* c2) {
	int n = 0;
	if(_whichRep == BINARY_4BIT || _whichRep == BINARY_8BIT) {
		switch(_whichCross) {
		case SINGLE_POINT:
			n = GA1DBinaryStringGenome::OnePointCrossover(p1,p2,c1,c2);
			break;
		case TWO_POINT:
			n = GA1DBinaryStringGenome::TwoPointCrossover(p1,p2,c1,c2);
			break;
		case UNIFORM:
		default:
			n = GA1DBinaryStringGenome::UniformCrossover(p1,p2,c1,c2);
			break;
		}

	}
	else {
		switch(_whichCross) {
		case UNIFORM:
			n = GARealGenome::UniformCrossover(p1,p2,c1,c2);
			break;
		case BLX:
			n = RealBLXCrossover(p1,p2,c1,c2);
			break;
		case SINGLE_POINT:
			n = GARealGenome::OnePointCrossover(p1,p2,c1,c2);
			break;
		case TWO_POINT:
			n = GARealGenome::TwoPointCrossover(p1,p2,c1,c2);
			break;
		case AVERAGING:
		default:
			n = RealAveragingCrossover(p1,p2,c1,c2);
			break;
		}
	}

	return n;
}







// calculate the distance between two two-dimensional genomes.
// used only in the deterministic crowding ga
float
CGademoView::RealComparator(const GAGenome& g1, const GAGenome& g2) {
  GARealGenome &a = (GARealGenome &)g1;
  GARealGenome &b = (GARealGenome &)g2;

  float dist = 0;

  float x = b.gene(0) - a.gene(0);
  float y = b.gene(1) - a.gene(1);
  dist = (float)sqrt(x*x + y*y);

  return dist;
}

// warning! this gaussian implementation does not consider enumerated and
// discretized allele sets!!
int
CGademoView::RealGaussianMutation(GAGenome& g, float pmut) {
  GARealGenome &genome = (GARealGenome &)g;

  int n=0;
  for(int i=0; i<genome.length(); i++) {
	  if(GAFlipCoin(pmut)) {
		  float v;
		  do {
			  v = GAGaussianFloat(genome.gene(i));
		  } while(v < genome.alleleset(i).lower() || v > genome.alleleset(i).upper()); 
		  genome.gene(i, v);
		  n++;
	  }
  }
  return n;
}

int
CGademoView::RealUniformMutation(GAGenome& g, float pmut) {
  GARealGenome &genome = (GARealGenome &)g;

  int n=0;
  for(int i=0; i<genome.length(); i++) {
	  if(GAFlipCoin(pmut)) {
		  genome.gene(i, genome.alleleset(i).allele());
		  n++;
	  }
  }
  return n;
}

int
CGademoView::RealBoundaryMutation(GAGenome& g, float pmut) {
  GARealGenome &genome = (GARealGenome &)g;

  int n=0;
  for(int i=0; i<genome.length(); i++) {
	  if(GAFlipCoin(pmut)) {
		  genome.gene(i, (GARandomBit() ? 
							genome.alleleset(i).lower() :
							genome.alleleset(i).upper()));
		  n++;
	  }
  }
  return n;
}


// warning! these crossovers assume that mom and dad and children are all
// the same length!
int
CGademoView::RealBLXCrossover(const GAGenome& p1, const GAGenome& p2, GAGenome* c1, GAGenome* c2) {
  const GARealGenome &mom = (const GARealGenome &)p1;
  const GARealGenome &dad = (const GARealGenome &)p2;

  int n=0;

  if(c1) {
	  GARealGenome& sis = (GARealGenome&)*c1;
	  for(int i=0; i<mom.length(); i++) {
		  float lo = GAMin(mom.gene(i), dad.gene(i));
		  float hi = GAMax(mom.gene(i), dad.gene(i));
		  float dist = (float)0.5 * (hi - lo);
		  lo -= dist;
		  hi += dist;
		  lo = GAMax(lo, sis.alleleset(i).lower());
		  hi = GAMin(hi, sis.alleleset(i).upper());
		  sis.gene(i, GARandomFloat(lo, hi));
	  }
	  n++;
  }
  if(c2) {
	  GARealGenome& bro = (GARealGenome&)*c2;
	  for(int i=0; i<mom.length(); i++) {
		  float lo = GAMin(mom.gene(i), dad.gene(i));
		  float hi = GAMax(mom.gene(i), dad.gene(i));
		  float dist = (float)0.5 * (hi - lo);
		  lo -= dist;
		  hi += dist;
		  lo = GAMax(lo, bro.alleleset(i).lower());
		  hi = GAMin(hi, bro.alleleset(i).upper());
		  bro.gene(i, GARandomFloat(lo, hi));
	  }
	  n++;
  }

  return n;
}

int
CGademoView::RealAveragingCrossover(const GAGenome& p1, const GAGenome& p2, GAGenome* c1, GAGenome* c2) {
  const GARealGenome &mom = (const GARealGenome &)p1;
  const GARealGenome &dad = (const GARealGenome &)p2;

  int n=0;
  if(c1) {
	  GARealGenome& sis = (GARealGenome&)*c1;
	  for(int i=0; i<mom.length(); i++) {
		  float v = (float)0.5 * (mom.gene(i) + dad.gene(i));
		  v = GAMax(v, sis.alleleset(i).lower());
		  v = GAMin(v, sis.alleleset(i).upper());
		  sis.gene(i, v);
	  }
	  n++;
  }
  if(c2) {
	  GARealGenome& bro = (GARealGenome&)*c2;
	  for(int i=0; i<mom.length(); i++) {
		  float v = (float)0.5 * (mom.gene(i) + dad.gene(i));
		  v = GAMax(v, bro.alleleset(i).lower());
		  v = GAMin(v, bro.alleleset(i).upper());
		  bro.gene(i, v);
	  }
	  n++;
  }

  return n;
}








/*****************************************************************************/
/* Type:        2D FUNCTION                                                  */
/* Name:        Objective2D_1                                                */
/* Description: 2D tooth                                                     */
/* Boundaries:  -6 < x < 6                                                   */
/*              -6 < y < 6                                                   */
/* Source:      modified Himmelblau's function from Deb, K.                  */
/*              'GA in multimodal function optimazation' Masters thesis      */
/*		TCGA Rep. 89002 / U. of Alabama                              */
/*****************************************************************************/
float CGademoView::Loaf(GAGenome& genome) {
	float x = 0;
	float y = 0;

	if(_whichRep == BINARY_4BIT || _whichRep == BINARY_8BIT) {
		GABin2DecGenome& g = (GABin2DecGenome&)genome;
		x = g.phenotype(0);
		y = g.phenotype(1);
	}
	else {
		GARealGenome& g = (GARealGenome&)genome;
		x = g.gene(0);
		y = g.gene(1);
	}
	float z = -((x*x+y-11)*(x*x+y-11)+(x+y*y-7)*(x+y*y-7))/200 + 10;
	return z;
}


/*****************************************************************************/
/* Type:        2D FUNCTION                                                  */
/* Name:        Objective2D_2                                                */
/* Description: Foxholes (25)                                                */
/* Boundaries:  -60 < x < 60                                                 */
/*              -60 < y < 60                                                 */
/* Source:      Shekel's Foxholes problem from De Jong's Diss.(1975)         */
/*              'GA in multimodal function optimazation' Masters thesis      */
/*		TCGA Rep. 89002 / U. of Alabama                              */
/*****************************************************************************/
float CGademoView::Foxholes(GAGenome& genome) {
	static float ai[25],bi[25];
	static int inited = 0;
	if(!inited) {
		for (int j=0; j<25; j++) {
			ai[j] = (float)(16 * ((j % 5) -2));
			bi[j] = (float)(16 * ((j / 5) -2));
		}
		inited = 1;
	}

	float x =0;
	float y =0;
	if(_whichRep == BINARY_4BIT || _whichRep == BINARY_8BIT) {
		GABin2DecGenome& g = (GABin2DecGenome&)genome;
		x = g.phenotype(0);
		y = g.phenotype(1);
	}
	else {
		GARealGenome& g = (GARealGenome&)genome;
		x = g.gene(0);
		y = g.gene(1);
	}

	int i;
	double sum = 0.0;

	for (i=0; i<25; i++) {
		sum += (1.0 / (1.0 + i + pow((x-ai[i]),6.0) + pow((y-bi[i]),6.0)));
	}
	double z = 500.0 - (1 / (0.002 + sum));
	return (float)z;
}


/*****************************************************************************/
/* Type:        2D FUNCTION                                                  */
/* Name:        Objective2D_3                                                */
/* Description: Schwefel's nasty (4 glob. Max bei (+-420.96/+-420.96)        */
/* Boundaries:  -500 < x < 500                                               */
/*              -500 < y < 500                                               */
/* Source:      Schwefel's function in Schoeneburg                           */
/*****************************************************************************/
float CGademoView::Schwefel(GAGenome& genome) {
	float x =0;
	float y =0;
	if(_whichRep == BINARY_4BIT || _whichRep == BINARY_8BIT) {
		GABin2DecGenome& g = (GABin2DecGenome&)genome;
		x = g.phenotype(0);
		y = g.phenotype(1);
	}
	else {
		GARealGenome& g = (GARealGenome&)genome;
		x = g.gene(0);
		y = g.gene(1);
	}

	double z = fabs(x) * sin(sqrt(fabs(x))) + fabs(y) * sin(sqrt(fabs(y)));
	return (float)(500.0 + z);
}


/*****************************************************************************/
/* Type:        2D FUNCTION                                                  */
/* Name:        Objective2D_4                                                */
/* Description: Mexican Hat                                                  */
/* Boundaries:  -10 < x < 10                                                 */
/*              -10 < y < 10                                                 */
/* Source:                                                                   */
/*****************************************************************************/
float CGademoView::Ripples(GAGenome& genome) {
	float x =0;
	float y =0;
	if(_whichRep == BINARY_4BIT || _whichRep == BINARY_8BIT) {
		GABin2DecGenome& g = (GABin2DecGenome&)genome;
		x = g.phenotype(0);
		y = g.phenotype(1);
	}
	else {
		GARealGenome& g = (GARealGenome&)genome;
		x = g.gene(0);
		y = g.gene(1);
	}

	double z = sin(sqrt(x*x + y*y))*sin(sqrt(x*x + y*y)) - 0.5;
	z /= ((1.0 + 0.001*(x*x + y*y))*(1.0 + 0.001*(x*x + y*y)));
	z = (0.5 - z);
	return (float)z;
}



/*****************************************************************************/
/* Type:        2D FUNCTION                                                  */
/* Name:        Objective2D_5                                                */
/* Description: Mexican Hat                                                  */
/* Boundaries:  -10 < x < 10                                                 */
/*              -10 < y < 10                                                 */
/* Source:      same as ripples but with center shifted by 2 in x            */
/*****************************************************************************/
float CGademoView::RipplesShifted(GAGenome& genome) {
	float x =0;
	float y =0;
	if(_whichRep == BINARY_4BIT || _whichRep == BINARY_8BIT) {
		GABin2DecGenome& g = (GABin2DecGenome&)genome;
		x = g.phenotype(0);
		y = g.phenotype(1);
	}
	else {
		GARealGenome& g = (GARealGenome&)genome;
		x = g.gene(0);
		y = g.gene(1);
	}

	double z = sin(sqrt((x-2)*(x-2) + y*y))*sin(sqrt((x-2)*(x-2) + y*y)) - 0.5;
	z /= ((1.0 + 0.001*((x-2)*(x-2) + y*y))*(1.0 + 0.001*((x-2)*(x-2) + y*y)));
	z = (0.5 - z);
	return (float)z;
}
