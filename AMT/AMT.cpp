#include <list>
#include <sstream>
#include <stdio.h>
#include <ga/std_stream.h>
#include <ga/GASStateGA.h>
#include "AmtRenderer.h"
#include "NotesGenome.h"

#define cout STD_COUT
#define endl STD_ENDL

using namespace AMT;

float Objective(GAGenome &);

int	main(int argc, char** argv) {
	
// 	NotesGenome test;
// 	NotesGenome::Init(test);
// 	cout << "Before:\n" << test;
// 	Note* newNote = new Note(55, 50000);
// 	test.AddNote(0, *newNote);
// 	newNote = new Note(0, 500000, true);
// 	test.AddNote(1, *newNote);
// 	cout << "\nAfter new notes:\n" << test;
// 	test.EraseNote(1);
// 	cout << "\nAfter erase silence:\n" << test;
// 	test.SplitNote(0,0.5,0.01);
// 	cout << "\nAfter splitting note:\n" << test;

	// GARandomSeed();

	// Create the initial genome for the genetic algorithm to use.
	// It's necessary to set the original audio FFT's
	NotesGenome notesGenome;
	notesGenome.evaluator(Objective);

	// Now that we have a genome, we use it to create our GA.
	GASteadyStateGA ga(notesGenome);
	//ga.populationSize(30);
	//ga.pReplacement(1);

	// Set the default parameters we want to use, then check the command line for
	// other arguments that might modify these.
	ga.set(gaNpopulationSize, 200);	// population size
	ga.set(gaNpCrossover, 0.6);		// probability of crossover
	ga.set(gaNpMutation, 0.1);		// probability of mutation
	ga.set(gaNnGenerations, 1);	// number of generations
	ga.set(gaNscoreFrequency, 1);	// how often to record scores
	ga.set(gaNflushFrequency, 10);	// how often to dump scores to file
	ga.set(gaNselectScores,		// which scores should we track?
		GAStatistics::Maximum|GAStatistics::Minimum|GAStatistics::Mean);
	ga.set(gaNscoreFilename, "bog.dat");
	ga.parameters(argc, argv);
			
	// Evolve the genetic algorithm then dump out the results of the run.
	ga.evolve();

	NotesGenome& best = (NotesGenome&) ga.statistics().bestIndividual();
	
	cout << "the ga generated the list:\n" << best << "\n";
	cout << "the list contains " << best.size() << " nodes\n";
	cout << "the ga used the parameters:\n" << ga.parameters() << "\n";	
	cout << "\n" << ga.statistics() << "\n";

	AmtRenderer::Initialize();	
	AmtRenderer renderer(best.totalDuration);
	renderer.AddNotes(best);
	renderer.SaveFile("Best");

// 	for(int i = 0; i < ga.populationSize(); i++)
// 	{
// 		NotesGenome& individual = (NotesGenome&) ga.population().individual(i);
// 		AmtRenderer rendererIndividual(individual.totalDuration);
// 		rendererIndividual.AddNotes(individual);
// 		stringstream fileName;
// 		fileName << "Individual " << i;
// 		rendererIndividual.SaveFile(fileName.str());
// 	}
		
	Notes notes;
	notes.push_front(*(new Note(57,44100)));
	AmtRenderer renderer3(44100);
	renderer3.AddNotes(notes);
	renderer3.SaveFile("Test");
	AmtRenderer::CleanUp();

	return 0;
}

/* ----------------------------------------------------------------------------
Objective function
---------------------------------------------------------------------------- */
float Objective(GAGenome & c)
{
	NotesGenome& genome = (NotesGenome&)c;
	return 100 / (genome.size() + 1);
}
