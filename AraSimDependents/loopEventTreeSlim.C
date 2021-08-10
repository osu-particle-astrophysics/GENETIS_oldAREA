#include "TFile.h"
#include "TTree.h"
#include "TMath.h"
#include <iostream>
#include <vector>
using namespace std;

void loopEventTreeSlim(){
    TFile f("data/DumbAraSim-106.root");
    
    TTree *eventTreeSlim = (TTree*)f.Get("eventTreeSlim");

    double weight;
    double event_x;
    double event_y;
    double event_z;
    double nu_vector_phi;
    double nu_vector_theta;
    
    double antenna_x = 0;
    double antenna_y = 0;
    double antenna_z = -200;
    
    double R, nu_antenna_theta, nu_ant_x, nu_ant_y, nu_ant_z;

    
    eventTreeSlim->SetBranchAddress("weight", &weight);
    eventTreeSlim->SetBranchAddress("event_x", &event_x);
    eventTreeSlim->SetBranchAddress("event_y", &event_y);
    eventTreeSlim->SetBranchAddress("event_z", &event_z);
    eventTreeSlim->SetBranchAddress("nu_vector_phi", &nu_vector_phi);
    eventTreeSlim->SetBranchAddress("nu_vector_theta", &nu_vector_theta);
    
    for(int iEvent=0; iEvent<eventTreeSlim->GetEntries(); iEvent++){
        eventTreeSlim->GetEntry(iEvent);
        
        nu_ant_x = event_x - antenna_x;
        nu_ant_y = event_y - antenna_y;
        nu_ant_z = event_z - antenna_z;
        
        R = sqrt( pow(nu_ant_x,2) + pow(nu_ant_y,2) + pow(nu_ant_z,2) );
        //take the dot product of the unit vector between the neutrino
        //and the antenna and the vertical
        // which is just the z-component / R
        nu_antenna_theta = acos( nu_ant_z / R) * 180./TMath::Pi();
        std::cout << "Event: " << iEvent << " weight " << weight << " R " << R << " theta " << nu_antenna_theta << std::endl;

        
    }

}

int main(){
    loopEventTreeSlim();
    return 0;
}
