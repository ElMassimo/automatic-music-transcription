#include "NotesGenome.h"

void NotesGenome::Init(GAGenome& g){
	NotesGenome &genome = (NotesGenome&) g;
	int size = GARandomInt(1,10);		
	genome.totalDuration = 0;
	for(int i=0; i < size; i++)
	{		
		int noteNumber = 0;
		bool isRest = GARandomBit();
		if(!isRest)
			noteNumber = GARandomInt(0,127);
		int duration = GARandomInt(1,44100);
		Note* note = new Note(noteNumber,duration,isRest); 

		genome.push_back(*note);
		genome.totalDuration += duration;
	}
}

int NotesGenome::Mutate(GAGenome& g, float pmut)
{
	NotesGenome &genome = (NotesGenome&) g;

	if(pmut <= 0.0)
		return 0;

	if(GAFlipCoin(pmut) && genome.size() > 0){
		int removeIndex = GARandomInt(0, genome.size() - 1);
		genome.EraseNote(removeIndex);
		return 1;
	}

	return 0;
}

float NotesGenome::Compare(const GAGenome& g1, const GAGenome& g2){	
	NotesGenome &genome1 = (NotesGenome&) g1;
	NotesGenome &genome2 = (NotesGenome&) g2;
	return abs(genome1.totalDuration - genome2.totalDuration);
}

float NotesGenome::Evaluate(GAGenome&){
  // your evaluation here
	return 0;
}

int NotesGenome::Cross(const GAGenome& mom, const GAGenome& dad, GAGenome* sis, GAGenome* bro){
  // your crossover here
	//GA1DBinaryStringGenome &mom=(GA1DBinaryStringGenome &)p1;
 // GA1DBinaryStringGenome &dad=(GA1DBinaryStringGenome &)p2;

 // int n=0;
 // unsigned int site = GARandomInt(0, mom.length());
 // unsigned int len = mom.length() - site;

 // if(c1){
 //   GA1DBinaryStringGenome &sis=(GA1DBinaryStringGenome &)*c1;
 //   sis.copy(mom, 0, 0, site);
 //   sis.copy(dad, site, site, len);
 //   n++;
 // }
 // if(c2){
 //   GA1DBinaryStringGenome &bro=(GA1DBinaryStringGenome &)*c2;
 //   bro.copy(dad, 0, 0, site);
 //   bro.copy(mom, site, site, len);
 //   n++;
 // }

 // return n;
	return 0;
}

NotesGenome::NotesGenome(void) : GAGenome(Init, Mutate, Compare)
{
    evaluator(Evaluate); 
    crossover(Cross); 
}

NotesGenome::NotesGenome(const NotesGenome& orig)
{ 
	copy(orig);
}

NotesGenome::~NotesGenome(void)
{
}

NotesGenome& NotesGenome::operator=(const GAGenome& orig){
	if(&orig != this)
		copy(orig);
	return *this;
}

int NotesGenome::equal(const GAGenome& c) const
{
	if(this == &c) return 1;
	const NotesGenome& b = DYN_CAST(const NotesGenome&, c);
	return (*this == b);
}

int NotesGenome::write(STD_OSTREAM & os) const 
{
	if(this->empty())
		os << "The genome has no notes." << "\n";
	else
		for (NotesConstIterator it = this->begin() ; it != this->end(); it++)
			os << *it;
	return 0;
}

void NotesGenome::copy(const GAGenome& orig)
{
	if(&orig == this)
		return;

	const NotesGenome& genome = (const NotesGenome&) orig;
	GAGenome::copy(genome);
	this->ReplaceNotes(genome);
}

GAGenome* NotesGenome::clone(CloneMethod method) const 
{
	return new NotesGenome(*this);
}