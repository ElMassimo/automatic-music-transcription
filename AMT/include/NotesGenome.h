#pragma once
#include <ga/GAGenome.h>
#include "Notes.h"
#include "MusicEvaluator.h"

using namespace std;

namespace AMT {

class NotesGenome : public GAGenome, public Notes
{	
public:
	static MusicEvaluator* musicEvaluator;

	GADefineIdentity("NotesGenome", 201);
	NotesGenome(MusicEvaluator& eval);	
	NotesGenome(Evaluator eval);
	NotesGenome(const NotesGenome& orig);
	~NotesGenome();

	static void DefaultMusicInitializer(GAGenome&);
	static int DefaultMusicMutator(GAGenome&, float);
	static float Compare(const GAGenome&, const GAGenome&);
	static int SPXCrossover(const GAGenome&, const GAGenome&, GAGenome*, GAGenome*);
	static float DefaultMusicEvaluator(GAGenome&);

	NotesGenome& operator=(const GAGenome& orig);
	
    virtual int equal(const GAGenome&) const;
	virtual int write(ostream&) const;
	virtual GAGenome* clone(CloneMethod) const;
	virtual void copy(const GAGenome& orig);

	void SaveToFile(char* fileName) const;
};

}