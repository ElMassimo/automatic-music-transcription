#include <stdio.h>
#include <ga/GA1DBinStrGenome.h>
#include <ga/GAListGenome.h>
#include <ga/GADemeGA.h>
#include <ga/std_stream.h>
#include <MusicNoteLib.h>
#include "AmtRenderer.h"
#include "AmtGA.h"
#include "NotesGenome.h"
#include <ga/GASStateGA.h>

#define cout STD_COUT
#define endl STD_ENDL

using namespace AMT;

float Objective(GAGenome &);
// Objective function and initializer declarations.
float objective(GAGenome &);
void ListInitializer(GAGenome &);

int	main(int argc, char** argv) {

	AmtRenderer::Initialize();

	AmtRenderer renderer(44100 * 10);

	Note* notes[20];
	for(int i = 0; i < 20; i++)
		notes[i] = new Note(40 + i, 22050);

	renderer.AddNotes(notes, 20);

	for(int i = 0; i < 20; i++)
		delete notes[i];

	renderer.SaveFile("Test");

	AmtRenderer::CleanUp();


	cout << "Example 8\n\n";
	cout << "This program runs a steady-state GA whose objective function\n";
	cout << "tries to maximize the size of the list and tries to make lists\n";
	cout << "that contain the number 101 in the nodes.  The lists contain\n";
	cout << "ints in the nodes.\n\n";
	cout.flush();

	// See if we've been given a seed to use (for testing purposes).  When you
	// specify a random seed, the evolution will be exactly the same each time
	// you use that seed number.

	for(int i=1; i<argc; i++) {
		if(strcmp(argv[i++],"seed") == 0)
			GARandomSeed((unsigned int)atoi(argv[i]));
	}

	// Create the initial genome for the genetic algorithm to use.  Set the
	// initializer and mutation before we make the genetic algorithm.

	GAListGenome<int> genome(objective);
	genome.initializer(ListInitializer);
	//  genome.mutator(GAListGenome<int>::SwapMutator);
	genome.mutator(GAListGenome<int>::DestructiveMutator);

	// Now that we have a genome, we use it to create our GA.  After creating the
	// GA we set the parameters and tell the GA to use sigma truncation scaling
	// rather than the default (linear scaling).  Set the crossover to single
	// point crossover.  The genetic algorithm handles crossover since genomes
	// don't know about other genomes.  We could set the crossover on the genome
	// if we wanted - either way will work.

	GASteadyStateGA ga(genome);
	GASigmaTruncationScaling scale;
	ga.scaling(scale);
	ga.crossover(GAListGenome<int>::OnePointCrossover);

	// Set the default parameters we want to use, then check the command line for
	// other arguments that might modify these.

	ga.set(gaNpopulationSize, 40);	// population size
	ga.set(gaNpCrossover, 0.6);		// probability of crossover
	ga.set(gaNpMutation, 0.05);		// probability of mutation
	ga.set(gaNnGenerations, 50);		// number of generations
	ga.set(gaNscoreFrequency, 1);		// how often to record scores
	ga.set(gaNflushFrequency, 10);	// how often to dump scores to file
	ga.set(gaNselectScores,		// which scores should we track?
		GAStatistics::Maximum|GAStatistics::Minimum|GAStatistics::Mean);
	ga.set(gaNscoreFilename, "bog.dat");
	ga.parameters(argc, argv);

	// Evolve the genetic algorithm then dump out the results of the run.

	ga.evolve();

	genome = ga.statistics().bestIndividual();
	//  cout << "the ga generated the list:\n" << genome << "\n";
	cout << "the list contains " << genome.size() << " nodes\n";
	cout << "the ga used the parameters:\n" << ga.parameters() << "\n";

	cout << "This example uses a genetic algorithm with multiple populations.\n";
	cout << endl;

	// See if we've been given a seed to use (for testing purposes).  When you
	// specify a random seed, the evolution will be exactly the same each time
	// you use that seed number.

	unsigned int seed = 0;
	for(int ii=1; ii<argc; ii++) {
		if(strcmp(argv[ii++],"seed") == 0) {
			seed = atoi(argv[ii]);
		}
	}

	NotesGenome genome2();
	GASteadyStateGA ga2(genome2);
	ga2.populationSize(30);
	ga2.pReplacement(1);
	cout << "initializing..."; cout.flush();
	ga2.initialize(seed);
	cout << "evolving..."; cout.flush();
	while(!ga.done()) {
		ga2.step();
		cout << "."; cout.flush();
	}
	cout << endl;

	cout << "best individual is: " << ga2.statistics().bestIndividual() << "\n";
	cout << "\n" << ga2.statistics() << "\n";  

	return 0;
}

// This is the 1-max objective function - try to maximize the number of 1s in 
// a bit string of arbitrary length.
float
	Objective(GAGenome& g) {
		GA1DBinaryStringGenome & genome = (GA1DBinaryStringGenome &)g;
		float score=0.0;
		for(int i=0; i<genome.length(); i++)
			score += genome.gene(i);
		score /= genome.length();
		return score;
}


/* ----------------------------------------------------------------------------
Objective function
  There is no limit to the size of a list (only the memory you have on your
computer).  This objective function tries to build a list that contains the
number 101 in all of its nodes.  If we don't put some control on this objective
then the list will grow without bound.  So we dampen it a bit with a penalty 
for large size.  However, this will make the score go negative, so we must use
a scaling object that allows negative objective scores.
  We could get lists with no contents, so we have to check for that.  Just make
sure that head has contents.  If it does, then the rest of the genome will.
  When we access the node contents we have to dereference the member functions.
For example, we do *chrom.head() not chrom.head().  This is because the member
functions return a pointer to the node's contents, not the actual contents.
---------------------------------------------------------------------------- */
float
objective(GAGenome & c)
{
  GAListGenome<int> & genome = (GAListGenome<int> &)c;
  int count=0;
  if(!genome.head()) return 0;
  count = (*genome.head() == 101) ? 1 : 0; // move to head of the list
  for(int i=1; i<genome.size(); i++)
    count += (*genome.next() == 101) ? 1 : 0; // check each element of the list
  return 5*count - genome.size();
}

/* ----------------------------------------------------------------------------
Here is the initializer for our genomes.  It builds a list of n items of type
int.  Notice that we first destroy any list that is already in the genome 
before we do our initialization.  This is so that the genomes can be re-used.
When you re-run a GA, it does not destroy the individuals in the population - 
it reuses them.  Thus, the initializer must make sure that the genome is 
cleaned up before it tries to initialize it.
---------------------------------------------------------------------------- */
void
ListInitializer(GAGenome & c)
{
  GAListGenome<int> &child=(GAListGenome<int> &)c;

  while(child.head()) child.destroy(); // destroy any pre-existing list

  int n=75;
  child.insert(100,GAListBASE::HEAD);
  for(int i=0; i<n; i++)
    child.insert(i+100);
  for(int j=0; j<n; j++)
    child.swap(GARandomInt(0,n-1), GARandomInt(0,n-1));
}
