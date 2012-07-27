#pragma once
#include "Notes.h"
#include "SineWave.h"
#include "FileWvOut.h"

using namespace stk;

namespace AMT
{
	class AmtRenderer
	{
	private:
		static StkFloat* noteFrequency;

	public: 
		StkFrames* frames;

		AmtRenderer(unsigned int samples);
		~AmtRenderer();

		static void Initialize();
		static void CleanUp();

	    void AddNote(Note &note, int start);

	    void AddNotes(Notes &notes, int start = 0);
		
		void SaveFile(std::string fileName);
	};
}
