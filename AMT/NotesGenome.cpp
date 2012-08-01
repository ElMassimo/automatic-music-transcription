#include <ga/garandom.h>
#include "NotesGenome.h"

using namespace AMT;

MusicEvaluator* NotesGenome::musicEvaluator = NULL;

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
		Note* note = new Note(GARandomInt(0, 127), GARandomInt(0, 44100));
		int addIndex = GARandomInt(0, genome.size() - 1);
		genome.AddNote(addIndex, *note);
		genome._evaluated = gaFalse;
		return 1;
	}

	if(GAFlipCoin(pmut) && genome.size() > 0){
		int splitIndex = GARandomInt(0, genome.size() - 1);
		double when = GARandomDouble(0,1);
		double silenceDuration = GARandomDouble(0,1-when);
		genome.SplitNote(splitIndex, when, silenceDuration);
		genome._evaluated = gaFalse;
		return 1;
	}

	if(GAFlipCoin(pmut) && genome.size() > 0){
		int removeIndex = GARandomInt(0, genome.size() - 1);
		genome.EraseNote(removeIndex);
		genome._evaluated = gaFalse;
		return 1;
	}

	return 0;
}

float NotesGenome::Compare(const GAGenome& g1, const GAGenome& g2){	
	NotesGenome &genome1 = (NotesGenome&) g1;
	NotesGenome &genome2 = (NotesGenome&) g2;
	return abs(genome1.totalDuration - genome2.totalDuration);
}

int NotesGenome::Cross(const GAGenome& parent1, const GAGenome& parent2, GAGenome* child1, GAGenome* child2){
	NotesGenome& p1 = (NotesGenome&)parent1;
	NotesGenome& p2 = (NotesGenome&)parent2;

	int n = 0;
	int minDuration = min<int>(p1.totalDuration, p2.totalDuration);
	int cutPoint = GARandomInt(0, minDuration);
	
	if(child1){
		NotesGenome& c1 = (NotesGenome &) *child1;
		c1.CombineNotes(p1, p2, cutPoint);
		n++;
	}
	if(child2){
		NotesGenome &c2 = (NotesGenome&) *child2;
		c2.CombineNotes(p2, p1, cutPoint);
		n++;
	}

	return n;
}

float NotesGenome::Evaluate(GAGenome& genome)
{	
	return musicEvaluator->NoteFitnessEvaluator(genome);
}

NotesGenome::NotesGenome(MusicEvaluator& mEval) : GAGenome(Init, Mutate, Compare)
{
	musicEvaluator = &mEval;
	crossover(Cross); 
	evaluator(Evaluate);
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

void NotesGenome::SaveToFile(char* fileName) const
{
	std::ofstream notesFile(fileName);
	if(notesFile)
		this->write(notesFile);
}