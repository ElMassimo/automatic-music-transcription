#include "Notes.h"

Notes::Notes()
{
	totalDuration = 0;
}

Notes::Notes(const Notes &notes)
{
	ReplaceNotes(notes);
}

Notes::~Notes()
{
}

NotesIterator Notes::Erase(int index)
{
	NotesIterator it = this->begin();
	advance(it, index);
	return this->erase(it);
}

void Notes::ReplaceNotes(const Notes& otherNotes)
{
	this->clear();
	this->insert(this->begin(), otherNotes.begin(), otherNotes.end());
	totalDuration = otherNotes.totalDuration;	
}