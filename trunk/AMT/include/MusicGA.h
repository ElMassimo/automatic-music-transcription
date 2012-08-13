#pragma once
#include <ga/GASStateGA.h>
#include "NotesGenome.h"

namespace AMT {
	class MusicGA :	public GASteadyStateGA
	{
	private:
		static GABoolean TerminateUponGenerationOrConvergence(GAGeneticAlgorithm &);

	public:
		MusicGA(NotesGenome& genome);
		~MusicGA();
	};
}