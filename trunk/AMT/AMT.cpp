#include <list>
#include <stdio.h>
#include <ga/std_stream.h>
#include <ga/GASStateGA.h>
#include "AmtRenderer.h"
#include "AmtGA.h"
#include "NotesGenome.h"

#define cout STD_COUT
#define endl STD_ENDL

using namespace AMT;

float Objective(GAGenome &);
// Objective function and initializer declarations.
float objective(GAGenome &);
void ListInitializer(GAGenome &);

int	main(int argc, char** argv) {
	
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
	ga.set(gaNpopulationSize, 40);	// population size
	ga.set(gaNpCrossover, 0.6);		// probability of crossover
	ga.set(gaNpMutation, 0.1);		// probability of mutation
	ga.set(gaNnGenerations, 10);	// number of generations
	ga.set(gaNscoreFrequency, 1);	// how often to record scores
	ga.set(gaNflushFrequency, 10);	// how often to dump scores to file
	ga.set(gaNselectScores,		// which scores should we track?
		GAStatistics::Maximum|GAStatistics::Minimum|GAStatistics::Mean);
	ga.set(gaNscoreFilename, "bog.dat");
	ga.parameters(argc, argv);
			
	// Evolve the genetic algorithm then dump out the results of the run.
	ga.evolve();

	NotesGenome& best = (NotesGenome&) ga.statistics().bestIndividual();	
	
	//  cout << "the ga generated the list:\n" << genome << "\n";
	cout << "the list contains " << best.size() << " nodes\n";
	cout << "the ga used the parameters:\n" << ga.parameters() << "\n";	
	cout << "\n" << ga.statistics() << "\n";
	cout << endl;

	AmtRenderer::Initialize();	
	AmtRenderer renderer(best.totalDuration);
	renderer.AddNotes(best);
	renderer.SaveFile("Best");
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
