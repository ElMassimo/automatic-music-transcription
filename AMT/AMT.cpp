#include <list>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <time.h>
#include <ga/garandom.h>
#include "MusicEvaluator.h"
#include "NotesGenome.h"
#include "NoteRenderer.h"
#include "MusicGA.h"
#include "MusicGA.h"
#include "AmtUtils.h"

using namespace AMT;
using namespace std;

void RenderPopulation(GAGeneticAlgorithm &ga)
{
	for(int i = 0; i < ga.populationSize(); i++)
	{
		NotesGenome& individual = (NotesGenome&) ga.population().individual(i);
		NoteRenderer rendererIndividual(individual.totalDuration);		rendererIndividual.AddNotes(individual);
		stringstream fileName;
		fileName << "Individual " << i;
		rendererIndividual.SaveFile(fileName.str());
	}
}

void ExecuteGA(MusicGA& ga, int executionNumber)
{
	// Evolve the genetic algorithm then dump out the results of the run.
	time_t evolutionStart, evolutionEnd;
	time (&evolutionStart);
	ga.evolve();
	time (&evolutionEnd);
	double runningTime = difftime (evolutionEnd, evolutionStart);

	// Save the best individual
	NotesGenome& best = (NotesGenome&) ga.statistics().bestIndividual();
	stringstream fileName;
	fileName << "Results\\Audio " << executionNumber;
	AmtUtils::SaveAudio(best, fileName.str());
	fileName << ".txt";
	best.SaveToFile(fileName.str());
	
	// Save stats
	ofstream stats;
	stats.open("Results\\Results.txt", std::ios_base::app);
	stats << ga.statistics().current(GAStatistics::Minimum) << "\t" << ga.statistics().current(GAStatistics::Mean) << "\t";
	stats << ga.statistics().current(GAStatistics::Deviation) << "\t" << runningTime << endl;
	stats.close();

	// It is not elitist!!
	if (best.score() > ga.statistics().current(GAStatistics::Minimum))
	{
		ofstream notElitist;
		notElitist.open("Results\\NotElistist.txt", std::ios_base::app);
		notElitist << executionNumber << endl;
		notElitist.close();
	}
}

int	main(int argc, char** argv) {
	AmtUtils::RunTests();

	// Initialize the music evaluator
	MusicEvaluator musicEvaluator;
	musicEvaluator.LoadAudioFile("Test.wav");

	// Create the genome and the genetic algorithm instance
	NotesGenome notesGenome(musicEvaluator);
	MusicGA ga(notesGenome);

	// Execute the algorithm repeated times
	int numberOfExecutions = 1;
	if(argc > 3 && strcmp(argv[1], "nexec") == 0)
		numberOfExecutions = strtol(argv[2], NULL, 10);

	// Set the default parameters we want to use, then check the command line for
	// other arguments that might modify these.
	ga.set(gaNpopulationSize, 50);	// population size
	ga.set(gaNnReplacement, 50); // number of replacement
	ga.set(gaNpCrossover, 0.6);		// probability of crossover
	ga.set(gaNpMutation, 0.1);		// probability of mutation
	ga.set(gaNnGenerations, 10);	// number of generations
	ga.set(gaNscoreFrequency, 1);	// how often to record scores
	ga.set(gaNflushFrequency, 10);	// how often to dump scores to file
	ga.set(gaNselectScores,	GAStatistics::Minimum|GAStatistics::Mean);
	ga.parameters(argc, argv);
	
	// Prepare the results file
	ofstream results("Results\\Results.txt", ios_base::trunc);
	results.close();

	for (int i = 0; i < numberOfExecutions; i++)
	{
		stringstream evoTxt;
		evoTxt << "Results\\Evolution " << i << ".txt";
		ga.set(gaNscoreFilename, evoTxt.str().c_str());
		ExecuteGA(ga, i);	
	}
	return 0;
}
