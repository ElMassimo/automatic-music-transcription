#pragma once
#include <ga/GASStateGA.h>
#include "NotesGenome.h"

namespace AMT {
	class MusicGA :	public GASteadyStateGA
	{
	private:
		static GABoolean TerminateUponGeneration(GAGeneticAlgorithm &);

	public:
		MusicGA(NotesGenome& genome);
		~MusicGA();
	};
}