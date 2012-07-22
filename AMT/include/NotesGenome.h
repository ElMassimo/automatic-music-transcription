#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <ga/GAListGenome.h>
#include <fftw++.h>
#include "Note.h"

using namespace std;
using namespace AMT;

typedef list<class Note> Notes;
typedef Notes::iterator NotesIterator;

class NotesGenome : public GAGenome
{
public:
	GADefineIdentity("NotesGenome", 201);
	NotesGenome();	
	NotesGenome(const NotesGenome& orig);
	~NotesGenome();

	static void Init(GAGenome&);
	static int Mutate(GAGenome&, float);
	static float Compare(const GAGenome&, const GAGenome&);
	static float Evaluate(GAGenome&);
	static int Cross(const GAGenome&, const GAGenome&, GAGenome*, GAGenome*);

	NotesGenome& operator=(const GAGenome& orig);
	
    virtual int equal(const GAGenome&) const;
	virtual int write(ostream&) const;
	virtual GAGenome* clone(CloneMethod) const;
	virtual void copy(const GAGenome& orig);

	int size() const;

	Notes notes;
	int totalDuration;

	static Complex* originalFFTs;
	static int totalSampleDuration;

};

