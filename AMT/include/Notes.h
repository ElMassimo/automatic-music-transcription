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

	void AddNote(Note& newNote);
	void InsertNote(int noteIndex, Note& newNote);
	void FlipSilence(int noteIndex, int noteNumber = 0);
	NotesIterator EraseNote(int noteIndex);
	void ReplaceNotes(const Notes& otherNotes);
	void CombineNotes(Notes& n1, Notes& n2, int when );
	bool SplitNote(int noteIndex, double when, double silenceDuration);

	// Crop the notes of series at a point in time
	void CropAt(int when);

private:
	NotesIterator GetNoteAt(int &when);
};
