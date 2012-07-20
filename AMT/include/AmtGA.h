#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <ga/ga.h>
#include <ga/std_stream.h>

namespace AMT
{
	class AmtGA : public GASteadyStateGA  {

	public:
		GADefineIdentity("AmtGA", 1590);
		AmtGA(const GAGenome&);
		AmtGA(const GAPopulation&);
		AmtGA(const AmtGA&);
		AmtGA& operator=(const AmtGA&);
	};
}