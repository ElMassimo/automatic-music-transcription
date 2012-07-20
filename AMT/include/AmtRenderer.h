
#ifndef __AMT_RENDERER__
#define __AMT_RENDERER__

#include "AMT.h"
#include "SineWave.h"
#include "FileWvOut.h"
#include "Plucked.h"

using namespace stk;

namespace AMT
{
	class AmtRenderer
	{
	private:
		static StkFloat* noteFrequency;
		StkFrames* frames;

	public: 
		AmtRenderer(unsigned int samples);
		~AmtRenderer();

		static void Initialize();
		static void CleanUp();

	    void AddNote(Note &note, int start);

	    void AddNotes(Note** notes, int amount, int start = 0);

		void SaveFile(std::string fileName);
	};
}
#endif