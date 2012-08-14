#pragma once
#include <list>
#include "NoteStruct.h"

using namespace std;

typedef list<class Note>::iterator NotesIterator;
typedef list<class Note>::reverse_iterator NotesReverseIterator;
typedef list<class Note>::const_iterator NotesConstIterator;

class Notes : public list<class Note>
{
public:
	int totalDuration;

	Notes();
	Notes(const Notes &notes);
	~Notes();

	// Returns an iterator pointing to the note occurring at the given index.
	NotesIterator GetNote(int noteIndex);

	// Adds a new note at the end
	void AddNote(Note& newNote);

	// Returns any note of the sequence that is near the specified index and is not a silence
	Note* AnyNote(int index);

	// Changes a silence to a note, and vice versa
	void FlipSilence(int noteIndex);

	// Changes the pitch of a note
	void ChangePitch(int noteIndex, bool higher, bool octave);

	// Changes the duration of a note, may or not expand the note after (if there is one)
	void ChangeDuration(int noteIndex, double newDurationPercentage, bool expandNext);

	// Replaces this series of notes copying another one
	void ReplaceNotes(const Notes& otherNotes);

	// Combines two series of notes, copying the first part of one and the second part of the other
	void CombineNotes(Notes& n1, Notes& n2, int when);

	// Splits a note in two and inserts a silence in between
	bool SplitNote(int noteIndex, double when, double silenceDuration);

	// Combines notes that are next to each other and have the same properties
	void MergeRedundantNotes();

	// Crop the notes of series at a point in time
	void CropAt(int when);

	// Returns an iterator pointing to the note occurring at the given time, 
	// and changes the time to represent the offset inside the note to reach it.
	NotesIterator GetNoteAt(int &when);	

	// Calculates the duration, traversing the whole note sequence (for testing purposes only)
	int GetRealDuration();
	
	Notes& operator=(const Notes &notes);

private:
	Note* AnyNote(NotesIterator &it);
};
