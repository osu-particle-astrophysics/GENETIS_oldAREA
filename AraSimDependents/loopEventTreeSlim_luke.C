#include "TFile.h"
#include "TTree.h"
#include "TMath.h"
#include <math.h>
#include <iostream>
#include <vector>
using namespace std;

void loopEventTreeSlim(vector <vector <double> > &ray_data){
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
        nu_antenna_theta = acos( nu_ant_z / R) * 180./M_PI;
        
        //write to console
        //std::cout << "Event: " << iEvent << " x loc " << nu_ant_x << " y loc " << nu_ant_y << " z loc " << nu_ant_z << std::endl;
        //std::cout << "Event: " << iEvent << " weight " << weight << " R " << R << " theta " << nu_antenna_theta << std::endl;
                

        ray_data[iEvent][0] = iEvent;
        ray_data[iEvent][1] = weight;
        ray_data[iEvent][2] = R;
        ray_data[iEvent][3] = nu_antenna_theta;
    }

}

int dumbAS_fitness(vector <vector <double> > &ray_arry, vector<double> &chrom, double &fitness, double &thresh){
    
    fitness = 0;
    for(int i=0; i <  100000; i++){

    //get gain at an angle
    double gainAtAngle;
    double th;
    th = ray_arry[i][3];
    gainAtAngle = chrom[0]*(1/2.0)*(1/sqrt(M_PI)) +
                  chrom[1]*(1/2.0)*sqrt(3/M_PI)*cos(th*M_PI/180.0f) +
                  chrom[2]*(1/4.0)*sqrt(5/M_PI)*(3*pow(cos(th*M_PI/180.0f), 2)- 1) +
                  chrom[3]*(1/4.0)*sqrt(7/M_PI)*(5*pow(cos(th*M_PI/180.0f),3)- 3*cos(th*M_PI/180.0f)) +
                  chrom[4]*(3/16.0)*sqrt(1/M_PI)*(35*pow(cos(th*M_PI/180.0f),4) - 30*pow(cos(th*M_PI/180.0f),2)+3) +
                  chrom[5]*(1/16.0)*sqrt(11/M_PI)*(15*cos(th*M_PI/180.0f) - 70*pow(cos(th*M_PI/180.0f),3)+63*pow(cos(th*M_PI/180.0f),5)) +
                  chrom[6]*(1/32.0)*sqrt(13/M_PI)*(-5 + 105*pow(cos(th*M_PI/180.0f),2)-315*pow(cos(th*M_PI/180.0f),4) + 231*pow(cos(th*M_PI/180.0f),6)) +
                  chrom[7]*(1/32.0)*sqrt(15/M_PI)*(-35*cos(th*M_PI/180.0f)+ 315*pow(cos(th*M_PI/180.0f),3) -693*pow(cos(th*M_PI/180.0f),5) + 429*pow(cos(th*M_PI/180.0f),7)) +
                  chrom[8]*(1/256.0)*sqrt(17/M_PI)*(35 - 1260*pow(cos(th*M_PI/180.0f),2) + 6930*pow(cos(th*M_PI/180.0f),4) - 12012*pow(cos(th*M_PI/180.0f),6) + 6435*pow((cos(th*M_PI/180.0f)),8)) +
                  chrom[9]*(1/256.0)*sqrt(19/M_PI)*(315*cos(th*M_PI/180.0f)- 4620*pow(cos(th*M_PI/180.0f),3) + 18018*pow(cos(th*M_PI/180.0f),5) - 25740*pow(cos(th*M_PI/180.0f),7) + 12155*pow((cos(th*M_PI/180.0f)),9)) +
                  chrom[10]*(1/512.0)*sqrt(21/M_PI)*(-63 +3465*pow(cos(th*M_PI/180.0f),2) - 30030*pow(cos(th*M_PI/180.0f),4) + 90090*pow(cos(th*M_PI/180.0f),6) -109395*pow((cos(th*M_PI/180.0f)),8)+46189*pow(cos(th*M_PI/180.0f),10)) +  
                  chrom[11]*(1/512.0)*sqrt(23/M_PI)*(-693*pow(cos(th*M_PI/180.0f),1) +15015*pow(cos(th*M_PI/180.0f),3) - 90090*pow(cos(th*M_PI/180.0f),5) +218790*pow((cos(th*M_PI/180.0f)),7)-230945*pow(cos(th*M_PI/180.0f),9)+88179*pow(cos(th*M_PI/180.0f),11)) +
                  chrom[12]*(1/2048.0)*sqrt(25/M_PI)*(231 -18018*pow(cos(th*M_PI/180.0f),2) +225225*pow(cos(th*M_PI/180.0f),4) - 1021020*pow(cos(th*M_PI/180.0f),6) +2078505*pow((cos(th*M_PI/180.0f)),8)-1939938*pow(cos(th*M_PI/180.0f),10)+676039*pow(cos(th*M_PI/180.0f),12));

    // cout << gainAtAngle << endl;
    //get fitness

        if(gainAtAngle / pow(ray_arry[i][2],2) > thresh){
            fitness = fitness + ray_arry[i][1];
        }
    }

return 0;
}

int main(int argc, char* argv[] ){
    double fitness = 0.0;
    double thrshold =atof(argv[1]);
    vector <vector <double> > ray( 100000 , vector<double> (4, 0) );
    vector <double> omni = {3.5449, 0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
    loopEventTreeSlim(ray);
    dumbAS_fitness(ray, omni, fitness, thrshold);
    cout << fitness << endl;
    return 0;
}
