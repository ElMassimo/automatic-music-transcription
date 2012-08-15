#pragma once
#include <ga/GASStateGA.h>
#include "NotesGenome.h"

#define SELECTION_REAL_TOURNAMENT 0
#define SELECTION_RANK 1
#define SELECTION_ROULETTE_WHEEL 2
#define SELECTION_TOURNAMENT 3
#define DEFAULT_TOURNAMENT_SIZE 2

namespace AMT {
	class MusicGA :	public GASteadyStateGA
	{
	private:
		static GABoolean TerminateUponGenerationOrConvergence(GAGeneticAlgorithm &);

	public:
		MusicGA(NotesGenome& genome, int selectionType = SELECTION_REAL_TOURNAMENT, int tournamentSize = DEFAULT_TOURNAMENT_SIZE);
		~MusicGA();
	};
}