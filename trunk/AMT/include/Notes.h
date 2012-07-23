#pragma once
#include <list>
#include "NoteStruct.h"

using namespace std;

typedef list<class Note>::iterator NotesIterator;
typedef list<class Note>::const_iterator NotesConstIterator;

class Notes : public list<class Note>
{
public:
	int totalDuration;

	Notes();
	Notes(const Notes &notes);
	~Notes();

	NotesIterator Erase(int index);
	void ReplaceNotes(const Notes& otherNotes);
};
