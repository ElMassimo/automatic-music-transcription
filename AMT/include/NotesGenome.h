#pragma once
#include <ga/GAGenome.h>
#include "Notes.h"
#include "MusicEvaluator.h"

#define DURATION_UPPER 2.0
#define DURATION_LOWER 0.5
#define P_CHANGE_DURATION 5
#define P_CHANGE_PITCH 5
#define P_FLIP_SILENCE 1
#define P_SPLIT_NOTE 2
#define P_MERGE_NOTES 2
#define P_RANDOMIZE_NOTE 1

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

	void SaveToFile(string fileName) const;
};

}