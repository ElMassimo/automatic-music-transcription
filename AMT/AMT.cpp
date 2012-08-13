#include <windows.h>
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

void ExecuteGA(MusicGA& ga, int executionNumber, string resultsDirectory)
{
	// Set the scores file name
	stringstream evoTxt;
	evoTxt << resultsDirectory << "\\Evolution " << executionNumber << ".txt";
	ga.set(gaNscoreFilename, evoTxt.str().c_str());

	// Evolve the genetic algorithm then dump out the results of the run.
	time_t evolutionStart, evolutionEnd;
	time (&evolutionStart);
	ga.evolve();
	time (&evolutionEnd);
	double runningTime = difftime (evolutionEnd, evolutionStart);

	// Save the best individual
	NotesGenome& best = (NotesGenome&) ga.statistics().bestIndividual();
	stringstream fileName;
	fileName << resultsDirectory << "\\Audio " << executionNumber;
	AmtUtils::SaveAudio(best, fileName.str());
	fileName << ".txt";
	best.SaveToFile(fileName.str());
	
	// Save stats
	ofstream stats(resultsDirectory + "\\Results.txt", ios_base::app);
	stats << ga.statistics().current(GAStatistics::Minimum) << "\t" << ga.statistics().current(GAStatistics::Mean) << "\t";
	stats << ga.statistics().current(GAStatistics::Deviation) << "\t" << runningTime << endl;
	stats.close();

	// Check whether it is elitist
	if (best.score() > ga.statistics().current(GAStatistics::Minimum))
	{
		ofstream notElitist(resultsDirectory + "\\NotElistist.txt", ios_base::app);
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

	// Check how many times we want to execute the algorithm
	int numberOfExecutions = 1;
	if(argc > 2 && strcmp(argv[1], "nexec") == 0)
		numberOfExecutions = strtol(argv[2], NULL, 10);

	string resultsDirectory = "Results";
	if(argc > 4 && strcmp(argv[3], "rdir") == 0)
		resultsDirectory = argv[4];

	// Check the command line in case we need to replace some parameters
	ga.parameters(argc, argv);
	
	// Prepare the results file
	CreateDirectoryA(resultsDirectory.c_str(), NULL);
	ofstream results(resultsDirectory + "\\Results.txt", ios_base::trunc);
	results.close();

	// Execute the algorithm and save the results of each execution
	for (int i = 0; i < numberOfExecutions; i++)
	{
		ExecuteGA(ga, i, resultsDirectory);	
	}

	// Produce average results
	ifstream execResults(resultsDirectory + "\\Results.txt");
	string line;
	double min = 0, mean = 0, deviation = 0, runningTime = 0; 
	for (int i = 0; i < numberOfExecutions; i++)
	{
		getline(execResults, line);
		istringstream lineStream(line);
		string token;
		lineStream >> token;
		min += strtod(token.c_str(), NULL);
		lineStream >> token;
		mean += strtod(token.c_str(), NULL);
		lineStream >> token;
		deviation += strtod(token.c_str(), NULL);
		lineStream >> token;
		runningTime += strtod(token.c_str(), NULL);
	}
	execResults.close();

	min /= numberOfExecutions;
	mean /= numberOfExecutions;
	deviation /= numberOfExecutions;
	runningTime /= numberOfExecutions;

	ofstream sumResults(resultsDirectory + "\\Aggregated Results.txt");
	sumResults << min << "\t" << mean << "\t" << deviation << "\t" << runningTime << endl;
	sumResults.close();

	return 0;
}
