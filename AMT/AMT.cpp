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
	stats << ga.statistics().current(GAStatistics::Deviation) << "\t" << runningTime << "\t" << ga.statistics().generationsToFindTheBest << endl;
	stats.close();

	// Check whether it is elitist
	if (best.score() > ga.statistics().current(GAStatistics::Minimum))
	{
		ofstream notElitist(resultsDirectory + "\\NotElistist.txt", ios_base::app);
		notElitist << executionNumber << endl;
		notElitist.close();
	}
}

int ParseOption(string option, int argc, char** argv)
{
	for (int i = 0; i < argc; i++)
		if(option == argv[i])
			return i + 1;
	return -1;
}

int	main(int argc, char** argv)
{
	// Parse command line options
	int optionIndex;

	// How many times we want to execute the algorithm
	int numberOfExecutions = 1;
	if((optionIndex = ParseOption("nexec", argc, argv)) > 0)
		numberOfExecutions = strtol(argv[optionIndex], NULL, 10);

	// Where should we store the execution results
	string resultsDirectory = "Results";
	if((optionIndex = ParseOption("rdir", argc, argv)) > 0)
		resultsDirectory = argv[optionIndex];

	// Which audio file to translate
	string audioFileName = "Test.wav";
	if((optionIndex = ParseOption("afile", argc, argv)) > 0)
		audioFileName = argv[optionIndex];
	else
	{	// Check which sample file we should use
		int sample = 2;
		if((optionIndex = ParseOption("sample", argc, argv)) > 0)
			sample = strtol(argv[optionIndex], NULL, 10);

		// Check if we should use a sample file
		int length = 8;
		if((optionIndex = ParseOption("slength", argc, argv)) > 0)
			length = strtol(argv[optionIndex], NULL, 10);

		// Run some routine tests and create the sample file
		AmtUtils::CreateSampleFile(sample, length);
	}
	
	// Initialize the music evaluator
	MusicEvaluator musicEvaluator;
	musicEvaluator.LoadAudioFile(audioFileName);

	// Create the genome and the genetic algorithm instance
	NotesGenome notesGenome(musicEvaluator);
	MusicGA ga(notesGenome);

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
	int genToBest = 0;
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
		lineStream >> token;
		genToBest += strtol(token.c_str(), NULL, 10);
	}
	execResults.close();

	min /= numberOfExecutions;
	mean /= numberOfExecutions;
	deviation /= numberOfExecutions;
	runningTime /= numberOfExecutions;
	genToBest /= numberOfExecutions;

	ofstream sumResults(resultsDirectory + "\\Aggregated Results.txt");
	sumResults << min << "\t" << mean << "\t" << deviation << "\t" << runningTime << "\t" << genToBest << endl;
	sumResults.close();

	sumResults.open("Aggregated Results.txt", ios_base::app);
	sumResults << min << "\t" << mean << "\t" << deviation << "\t" << runningTime << "\t" << genToBest << endl;
	sumResults.close();
	return 0;
}
