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
	const GAStatistics& gaStats = ga.statistics();
	stats << gaStats.current(GAStatistics::Minimum) << "\t"  << runningTime << "\t" << gaStats.generationsToFindTheBest << "\t";
	stats << gaStats.current(GAStatistics::Mean) << "\t" << gaStats.current(GAStatistics::Deviation) << endl;
	stats.close();
}

int ParseOption(string option, int argc, char** argv)
{
	for (int i = 0; i < argc; i++)
		if(option == argv[i])
			return i + 1;
	return -1;
}

void CrunchNumbers(string resultsDir, int nExec)
{
	vector<float> min(nExec), time(nExec), genToBest(nExec);
	float sumMin = 0, sumTime = 0, best = DBL_MAX, bestTime = DBL_MAX;
	int sumGen = 0, bestGen = nExec;

	// Read the stats from every execution
	ifstream execResults(resultsDir + "\\Results.txt");
	for (int i = 0; i < nExec; i++)
	{	
		string line, token;
		getline(execResults, line);
		istringstream lineStream(line);

		// Read fitness of the best, time of the execution, and number of generations to find the best
		lineStream >> token; min[i] = strtod(token.c_str(), NULL);
		lineStream >> token; time[i] = strtod(token.c_str(), NULL);
		lineStream >> token; genToBest[i] = strtol(token.c_str(), NULL, 10);

		// Sum the values to calculate the average
		sumMin += min[i];
		sumTime += time[i];
		sumGen += genToBest[i];

		// Update the best execution stats
		if(min[i] < best)
		{
			best = min[i];
			bestTime = time[i];
			bestGen = genToBest[i];
		}
	}
	execResults.close();

	// Average and standard deviations
	float avgMin = sumMin / nExec;
	float devMin = AmtUtils::StandardDeviation(nExec, avgMin, min);
	
	float avgTime = sumTime / nExec;
	float devTime = AmtUtils::StandardDeviation(nExec, avgTime, time);

	float avgGen = sumGen / nExec;
	float devGen = AmtUtils::StandardDeviation(nExec, avgGen, genToBest);
	
	// Record the results of this bundle of executions
	ofstream sumResults(resultsDir + "\\Aggregated Results.txt");
	sumResults << "Min AVG\t" << "Min DEV\t" << "Time AVG\t" << "Time DEV\t" << "Gen AVG\t" << "Gen DEV\t"; 
	sumResults << "Best\t" << "Time Best\t"<< "Gen Best" << endl;

	stringstream stats;
	stats << avgMin << "\t" << devMin << "\t" << avgTime << "\t" << devTime << "\t" << avgGen<< "\t" << devGen << "\t";
	stats << best << "\t" << bestTime << "\t" << bestGen << endl;

	sumResults << stats.str();
	sumResults.close();

	// Add the summary of this bundle of executions to a general file
	sumResults.open("Aggregated Results.txt", ios_base::app);
	sumResults << stats.str();
	sumResults.close();
}

int	main(int argc, char** argv)
{
	  //***                               //
	 //       COMMAND LINE OPTIONS       //
	//                               ***//    
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
		int sample = SAMPLE_TWONOTES;
		if((optionIndex = ParseOption("sample", argc, argv)) > 0)
			sample = strtol(argv[optionIndex], NULL, 10);

		// Check if we should use a sample file
		int length = 8;
		if((optionIndex = ParseOption("slength", argc, argv)) > 0)
			length = strtol(argv[optionIndex], NULL, 10);

		// Run some routine tests and create the sample file
		AmtUtils::CreateSampleFile(sample, length);
	}
	
	// Check if we should use a different selection
	int selectionType = SELECTION_REAL_TOURNAMENT;
	if((optionIndex = ParseOption("selection", argc, argv)) > 0)
		selectionType = strtol(argv[optionIndex], NULL, 10);

	// Check if the tournament size is specified
	int tournamentSize = DEFAULT_TOURNAMENT_SIZE;
	if((optionIndex = ParseOption("tournament_size", argc, argv)) > 0)
		tournamentSize = strtol(argv[optionIndex], NULL, 10);

	  //***                               //
	 //           GA EXECUTION           //
	//                               ***//  

	// Initialize the music evaluator
	MusicEvaluator musicEvaluator;
	musicEvaluator.LoadAudioFile(audioFileName);

	// Create the genome and the genetic algorithm instance
	NotesGenome notesGenome(musicEvaluator);
	MusicGA ga(notesGenome, selectionType, tournamentSize);
	
	// Check the command line in case we need to replace some parameters
	ga.parameters(argc, argv);
	
	// Save the parameters for future reference
	ofstream parametersFile(resultsDirectory + "\\Parameters.txt");
	ga.parameters().write(parametersFile);
	parametersFile.close();
		
	// Prepare the results file
	AmtUtils::CheckAndCreateDirectory(resultsDirectory);
	ofstream results(resultsDirectory + "\\Results.txt", ios_base::trunc);
	results.close();

	// Execute the algorithm and save the results of each execution
	for (int i = 0; i < numberOfExecutions; i++)
	{
		ExecuteGA(ga, i, resultsDirectory);	
	}

	// Aggregate statistics
	CrunchNumbers(resultsDirectory, numberOfExecutions);

	return 0;
}
