#ifndef MOMELOCITY
#define MOMELOCITY

#include <vector>
#include <fstream>

#include "Hit.hh"

using namespace std;

class TrackerHit;

class MomentumVelocity
{
public:
	MomentumVelocity();
	~MomentumVelocity();
	vector <TrackerHit *> & GetHitsArray();
	void WriteMomelocity( string filename);
private:
	vector <TrackerHit *> hits_array;


};
#endif
