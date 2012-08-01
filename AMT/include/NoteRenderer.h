#pragma once
#include "Notes.h"
#include "SineWave.h"
#include "FileWvOut.h"

using namespace stk;

namespace AMT
{
	class NoteRenderer
	{
	private:
		static StkFloat* noteFrequency;

	public: 
		StkFrames* frames;

		NoteRenderer(unsigned int samples);
		~NoteRenderer();

		static void Initialize();
		static void CleanUp();

	    void AddNote(Note &note, int start);

		// Synthesizes the notes in the available buffer. Doesn't check for buffer overruns.
	    void AddNotes(Notes &notes, int start = 0);
		
		void SaveFile(std::string fileName);
	};
}
