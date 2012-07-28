#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <ga/GAListGenome.h>
#include <fftw++.h>
#include "Notes.h"

using namespace std;

class NotesGenome : public GAGenome, public Notes
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

	void SaveToFile(char* fileName) const;
};

