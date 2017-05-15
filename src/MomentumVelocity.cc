#include "MomentumVelocity.hh"

MomentumVelocity::MomentumVelocity()
{
  
}

MomentumVelocity::~MomentumVelocity()
{

}

vector <TrackerHit *> & MomentumVelocity::GetHitsArray()
{
	return hits_array;
}

void MomentumVelocity::WriteMomelocity( string filename)
{
	ofstream file(filename.data());

  file << "velocity,momentum,edep,position,chamber_number,track_id" << "\n";

	for (int i = 0; i < hits_array.size(); i++)
	{
		file  << hits_array[i]->GetVelocity() << ","
          << hits_array[i]->GetMomentum() << ","
          << hits_array[i]->GetEdep() << ","
          << hits_array[i]->GetPos() << ","
          << hits_array[i]->GetChamberNb() << ","
          << hits_array[i]->GetTrackID()
          << "\n";
	}
	file.close();
}
