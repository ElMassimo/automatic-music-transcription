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

void GA(int argc, char** argv)
{
	GARandomSeed(217);

	// Initialize the music evaluator
	MusicEvaluator musicEvaluator;
	musicEvaluator.LoadAudioFile("Test.wav");

	// Create the genome and the genetic algorithm instance
	NotesGenome notesGenome(musicEvaluator);
	MusicGA ga(notesGenome);

	// Set the default parameters we want to use, then check the command line for
	// other arguments that might modify these.
	ga.set(gaNpopulationSize, 50);	// population size
	ga.set(gaNnReplacement, 20); // number of replacement
	ga.set(gaNpCrossover, 0.6);		// probability of crossover
	ga.set(gaNpMutation, 0.1);		// probability of mutation
	ga.set(gaNnGenerations, 10);	// number of generations
	ga.set(gaNscoreFrequency, 1);	// how often to record scores
	ga.set(gaNflushFrequency, 10);	// how often to dump scores to file
	ga.set(gaNselectScores,		// which scores should we track?
		GAStatistics::Maximum|GAStatistics::Minimum|GAStatistics::Mean|GAStatistics::Deviation);
	ga.set(gaNscoreFilename, "evolution.txt");
	ga.parameters(argc, argv);

	// Calculate the time of evolution
	time_t evolutionStart, evolutionEnd;
	time (&evolutionStart);

	// Evolve the genetic algorithm then dump out the results of the run.
	ga.evolve();

	time (&evolutionEnd);

	NotesGenome& best = (NotesGenome&) ga.statistics().bestIndividual();
	best.SaveToFile("best.txt");
	AmtUtils::SaveAudio(best,"Best");

	cout << "\n" << ga.statistics() << "\n";
	cout << "\nFitness of the best individual: " << best.evaluate();
	cout << "\n\nRunning time: " << difftime (evolutionEnd, evolutionStart) << "s";
	ofstream stats("Resultados.txt", ios_base::_Noreplace);
	stats << "Something\n";
	stats.close();
}

int	main(int argc, char** argv) {
	AmtUtils::RunTests();
	GA(argc, argv);	
	return 0;
}
