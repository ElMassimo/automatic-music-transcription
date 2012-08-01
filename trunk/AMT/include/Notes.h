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

	void AddNote(int noteIndex, Note& newNote);
	void FlipSilence(int noteIndex);
	NotesIterator EraseNote(int noteIndex);
	void ReplaceNotes(const Notes& otherNotes);
	bool SplitNote(int noteIndex, double when, double silenceDuration);

	// Crop the notes of series at a point in time
	void CropAt(int when);

private:
	NotesIterator& GetNoteAt(int &when);
};
