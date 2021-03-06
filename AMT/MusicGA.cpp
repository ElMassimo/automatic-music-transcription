#include "MusicGA.h"
using namespace AMT;

GABoolean MusicGA::TerminateUponGenerationOrConvergence(GAGeneticAlgorithm & ga)
{
	if(TerminateUponGeneration(ga))
		return gaTrue;
	
	return TerminateUponConvergence(ga);
}

MusicGA::MusicGA(NotesGenome& genome, int selectionType, int tournamentSize) : GASteadyStateGA(genome)
{
	// Set the default parameters we want to use, then check the command line for
	// other arguments that might modify these.
	this->set(gaNpopulationSize, 50);
	this->set(gaNnReplacement, 50);		
	this->set(gaNpCrossover, 0.6);		
	this->set(gaNpMutation, 0.1);		
	this->set(gaNnGenerations, 10);		
	this->set(gaNscoreFrequency, 1);	// how often to record scores
	this->set(gaNflushFrequency, 10);	// how often to dump scores to file
	this->set(gaNselectScores, GAStatistics::Minimum|GAStatistics::Mean);
	this->set(gaNpConvergence, 0.995);
	this->minimize();

	switch(selectionType)
	{
	case SELECTION_ROULETTE_WHEEL:
		this->selector(GARouletteWheelSelector());
		break;

	case SELECTION_RANK :
		this->selector(GARankSelector());
		break;
		
	case SELECTION_TOURNAMENT:
		this->selector(GATournamentSelector());
		break;

	case SELECTION_REAL_TOURNAMENT:
	default:
		this->selector(GARTSelector(tournamentSize));
		break;
	}
}

MusicGA::~MusicGA()
{
}
