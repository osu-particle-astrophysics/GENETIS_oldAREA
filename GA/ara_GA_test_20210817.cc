#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <math.h>
#include <random>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <chrono>
#include <thread>
#include <math.h>
#include <iterator>
#include <array>   
#include <cmath> 
#include <algorithm>  

using namespace std;


enum DataType {
    gain, 
    phase,
    poly,
    sphHarm
};

enum InputFlag {
    err = -1,
    parentFlag,
    algoFlag
};

InputFlag mapInputFlag (std::string const& inString) {
    if (inString == "-p") return parentFlag;
    if (inString == "-gp") return algoFlag;
    return err;
}

/*
    Chromosome; an individual member of a generation
*/
class Chromosome {

    public :

        int xSize, ySize = 1;
        double fitness;

        vector<vector<double>> genotype;



        /*
            Constructor for chromosome

            @param xSize1 The x-size of the 2D genotype matrix
            @param ySize1 The y-size of the 2D genotype matrix
        */
        Chromosome(int xSize1, int ySize1)
        {   

            xSize = xSize1;
            ySize = ySize1;

            // 2D genotype matrix that describes the chromosome
            genotype = vector<vector<double>> (xSize, vector<double> (ySize, 0));

        }

        /*
            Member Function; prints the chromosome genotype to the console

            @return Returns 0 on success
        */
        int printGenotype()
        {   
            for(int x = 0; x < xSize; x++)
            {
                for(int y = 0; y < ySize; y++)  
                {   
                    cout << genotype[x][y] << ' ';
                }
                cout << endl;
            }

            return 0;
        }


        /*
            Member Function; assigns random genes to populate the chromosome's genotype
            in from a uniform distribution with range [lowerBound, upperBound]

            @param lowerBound The lower bound allowed for random gene assignment
            @param upperBound The upper bound allowed for random gene assignment 

            @return Returns 0 on success
        */
        int randomGenotype(double lowerBound = -5, double upperBound = 5)
        {
            // Generate a high quality random number from a uniform distrbution with range [0,S]
            std::random_device rd{};
            std::mt19937 generator{rd()};
            std::uniform_real_distribution<> dis(lowerBound, upperBound);

            for(int x = 0; x < xSize; x++)
            {
                for(int y = 0; y < ySize; y++)  
                {   
                    genotype[x][y] = dis(generator);
                }
            }

            return 0;
        }


        /*
            Member Function; assigns chromosome's genotype to the 2D vector passed to the function

            @param assignment 2D vector to assign as chromosome's genotype

            @return Returns 0 on success
        */
        int assignGenotype(vector<vector<double>> assignment)
        {

            xSize = assignment.size();
            ySize = assignment[0].size();

            genotype = vector<vector<double>> (xSize, vector<double> (ySize, 0));

            for(int x = 0; x < xSize; x++)
            {
                for(int y = 0; y < ySize; y++)  
                {   
                    genotype[x][y] = assignment[x][y];
                }
            }

            return 0;
        }

};



/*
    Generation; an single generation containing many chromosomes
*/
class Generation
{
public:

    int genSize;
    double maxFitness, avgFitness;
    bool fitnessAssigned;
    vector<Chromosome> children;


    /*
        Constructor for generation

        @param genSize1 The size of the generation (number of chromosomes)
    */
    Generation(int genSize1){
        genSize = genSize1;
        maxFitness = 0;
        avgFitness = 0;
        fitnessAssigned = false;
        children = vector <Chromosome> ( genSize, Chromosome(1,1) );
    }

    int resize(int newSize)
    {
        children.erase(children.begin(), children.end());
        genSize = newSize;
        children = vector <Chromosome> ( genSize, Chromosome(1,1) );

        return 0;
    }

    /*
        Member function; sorts the generation child vector by fitness score

        @return Returns 0 on success
    */
    int sort()
    {   

        auto& tmp = children;
        std::sort(children.begin(), children.end(), 
            [&tmp](const Chromosome &l, const Chromosome &r){return l.fitness < r.fitness;});

        return 0;
    }

    /*
        Member function; sorts the generation child vector by fitness score

        @return Returns 0 on success
    */
    int survive(double upperBound, double lowerBound)
    {   
        if(fitnessAssigned == true)
        {

            children.erase(children.begin() + lowerBound*genSize, children.begin() + upperBound*genSize);
            genSize = children.size();
            return 0;

        }

        return 1;
    }


    /*
        Member function; prints generation by each chromosome, printing
        the child fitness score and genotype

        @return Returns 0 on success
    */
    int printGeneration(){
        for (int chromo = 0; chromo < genSize; chromo++)
        {
            cout << "Chromosome " << chromo << " fit is : " << children[chromo].fitness << ", genotype is :\n";
            children[chromo].printGenotype();
        }

        return 0;
    }


    /*
        Member function; evaluates the maximum and average fitness score of the generation

        @return Returns 0 on success, returns 1 if generation has not been scored (failure)
    */
    int fitMetric()
    {
        if(fitnessAssigned == true)
        {
            double sum = 0;
            for (int chromo = 0; chromo < genSize; chromo++)
            {
                if(children[chromo].fitness > maxFitness){

                    maxFitness = children[chromo].fitness ;

                }

                sum += children[chromo].fitness;
            }

            avgFitness = sum/genSize;
            return 0;

        } else {

            return 1;

        }
    }

};

int readCoeff(string filePath, DataType dataType, vector<vector<double>> &dataVector ); 
int readVeff(string filePath, Chromosome &parent);
int parseArgs(int argc, char const *argv[], Generation &inputGen, int &roul_x_no, int &roul_mut_no, int &tour_x_no, int &tour_mut_no);
int writeCoeff(ofstream &outFile, DataType dataType, Chromosome &outputChromo);
int writeInfo(ofstream &outFile, Chromosome &outputChromo, int runNum, int iterNum);
int writeRadPattern(ofstream &outFile, DataType dataType, Chromosome &outputChromo);
int necWrite(DataType dataType, Generation outputGen);
int writeHist(ofstream &histFile, Generation &gen);
float evalGain(vector<float> &gainVector);
int constraints(Chromosome &chromo);
int tournament(Generation &gen, Chromosome &newChromo, bool mut);
int roullette(Generation &gen, Chromosome &newChromo, bool mut);
int mutation(Chromosome &c1, Chromosome &newChromo);
int crossover(Chromosome &c1, Chromosome &c2, Chromosome &newChromo);
int scoreGen(Generation &gen, vector<double> goal, stringstream &maxBuffer, stringstream &avgBuffer);

const float PI = 3.14159265359;

int main(int argc, char const *argv[])
{
    //Read file into array
    /*
    buffer = holds lines as they are brought in
    line = holds each line as it is read in
    word = holds each word as it is read in
    val = holds each word as a float
    child_no = holds integer number of children to be created
    parent_no = hold integer number of parents brought in
    column_no = index for theta gain and phi gain values
    row_no = index for frequency, theta, and phi values
    gain_input = holds gain values that change between generations
    phase_input = holds phase values that change between generations
    max_val = maximum value of either theta gain or phi gain of parent arrays
    min_val = minimum value of either theta gain or phi gain of parent arrays
    gain_output = holds gain_output phi gain and theta gain values
    */
    
    int rc = 0;
    int roul_xover = 30;
    int roul_mut = 20;
    int tour_xover = 5;
    int tour_mut = 20;

    cout << "Starting GA..." << endl << endl;

    // Read in input arguments and parse in data from files
    Generation inputGen(0);
    rc = parseArgs(argc, argv, inputGen, roul_xover, roul_mut, tour_xover, tour_mut);

    // Check for error
    if ( rc == 1 ){
        cout << "ERROR PARSING INPUT ARGUMENTS, PROGRAM CANNOT EXECUTE...";
        this_thread::sleep_for(chrono::milliseconds(500));
        cout << "\n";
        this_thread::sleep_for(chrono::milliseconds(500));
        cout << "PROGRAM EXITING\n";

        exit(EXIT_FAILURE);
    }

    cout << "Input gen size is " << inputGen.genSize << endl;
    inputGen.printGeneration();
    // Determine number of children from given GA inputs
    int roul_no=roul_xover+roul_mut; 
    int tour_no=tour_xover+tour_mut;
    int child_no=tour_no+roul_no;

    // Write Veff to history files
    if(inputGen.genSize > 1 | child_no == 0) {

        ofstream fitnessFile;
        fitnessFile.open("fitnessFile.txt", ios::out | ios::app);
        writeHist(fitnessFile, inputGen);

        if(child_no == 0)
        {
            cout << "\nGA execution complete... Exiting\n" << endl;
            exit(EXIT_SUCCESS); 
        }
    }

    Generation outputGen(child_no);

     // If parent_no is 0, produce random children with no parents 
    if(inputGen.genSize == 0){
   		
        for(int ind = 0; ind < outputGen.genSize; ind++)
        {
        
        Chromosome chromo(120, 13);
        chromo.randomGenotype(-5, 5);
        constraints(chromo);
        outputGen.children[ind] = chromo;

        }       

   	} else {

        inputGen.sort();

        // Fittest of generation survive
        inputGen.survive(0.5, 0.20);
        
        // Roulette Crossover Selection
        for(int ind = 0; ind < roul_xover; ind++){
            
            Chromosome chromo(120, 13);
            roullette(inputGen, chromo, false);
            constraints(chromo);
            outputGen.children[ind] = chromo;

        }

        // Roulette Mutation Selection
        for(int ind = roul_xover; ind < (roul_mut+roul_xover); ind++){
            
            Chromosome chromo(120, 13);
            roullette(inputGen, chromo, true);
            constraints(chromo);
            outputGen.children[ind] = chromo;

        }

        // Tournament Crossover Selection
        for(int ind = (roul_mut+roul_xover); ind < (roul_mut+roul_xover+tour_xover); ind++){
            
            Chromosome chromo(120, 13);
            tournament(inputGen, chromo, false);
            constraints(chromo);
            outputGen.children[ind] = chromo;

        }

        // Tournament Mutation Selection
        for(int ind = (roul_mut+roul_xover+tour_xover); ind < (roul_mut+roul_xover+tour_xover+tour_mut); ind++){
            
            Chromosome chromo(120, 13);
            tournament(inputGen, chromo, true);
            constraints(chromo);
            outputGen.children[ind] = chromo;

        }

    }

    /** WRITE VALUES INTO OUTPUT FILES **/
    necWrite(DataType::sphHarm, outputGen);
    cout << "\nGA execution complete... Exiting\n" << endl;
    exit(EXIT_SUCCESS);

    return 0;
    
 }

float evalGain(vector<float> &gainVector) 
{
	int theta[37];
	float gain[37];

    for (int th = 0; th < 37; th++)
    {
        theta[th] = (double) th*5.0f;
    }

    int th = 0;
    for(int th2 = 0; th2 < 37; th2++)
    {
    	gain[th2] = gainVector[0]*(1/2.0)*(1/sqrt(PI)) +
                   gainVector[1]*(1/2.0)*sqrt(3/PI)*cos(theta[th]*PI/180.0f) +
                   gainVector[2]*(1/4.0)*sqrt(5/PI)*(3*pow(cos(theta[th]*PI/180.0f), 2)- 1) +
                   gainVector[3]*(1/4.0)*sqrt(7/PI)*(5*pow(cos(theta[th]*PI/180.0f),3)- 3*cos(theta[th]*PI/180.0f)) +
                   gainVector[4]*(3/16.0)*sqrt(1/PI)*(35*pow(cos(theta[th]*PI/180.0f),4) - 30*pow(cos(theta[th]*PI/180.0f),2)+3) +
                   gainVector[5]*(1/16.0)*sqrt(11/PI)*(15*cos(theta[th]*PI/180.0f) - 70*pow(cos(theta[th]*PI/180.0f),3)+63*pow(cos(theta[th]*PI/180.0f),5)) +
                   gainVector[6]*(1/32.0)*sqrt(13/PI)*(-5 + 105*pow(cos(theta[th]*PI/180.0f),2)-315*pow(cos(theta[th]*PI/180.0f),4) + 231*pow(cos(theta[th]*PI/180.0f),6)) +
                   gainVector[7]*(1/32.0)*sqrt(15/PI)*(-35*cos(theta[th]*PI/180.0f)+ 315*pow(cos(theta[th]*PI/180.0f),3) -693*pow(cos(theta[th]*PI/180.0f),5) + 429*pow(cos(theta[th]*PI/180.0f),7)) +
                   gainVector[8]*(1/256.0)*sqrt(17/PI)*(35 - 1260*pow(cos(theta[th]*PI/180.0f),2) + 6930*pow(cos(theta[th]*PI/180.0f),4) - 12012*pow(cos(theta[th]*PI/180.0f),6) + 6435*pow((cos(theta[th]*PI/180.0f)),8)) +
                   gainVector[9]*(1/256.0)*sqrt(19/PI)*(315*cos(theta[th]*PI/180.0f)- 4620*pow(cos(theta[th]*PI/180.0f),3) + 18018*pow(cos(theta[th]*PI/180.0f),5) - 25740*pow(cos(theta[th]*PI/180.0f),7) + 12155*pow((cos(theta[th]*PI/180.0f)),9)) +
                   gainVector[10]*(1/512.0)*sqrt(21/PI)*(-63 +3465*pow(cos(theta[th]*PI/180.0f),2) - 30030*pow(cos(theta[th]*PI/180.0f),4) + 90090*pow(cos(theta[th]*PI/180.0f),6) -109395*pow((cos(theta[th]*PI/180.0f)),8)+46189*pow(cos(theta[th]*PI/180.0f),10)) +  
                   gainVector[11]*(1/512.0)*sqrt(23/PI)*(-693*pow(cos(theta[th]*PI/180.0f),1) +15015*pow(cos(theta[th]*PI/180.0f),3) - 90090*pow(cos(theta[th]*PI/180.0f),5) +218790*pow((cos(theta[th]*PI/180.0f)),7)-230945*pow(cos(theta[th]*PI/180.0f),9)+88179*pow(cos(theta[th]*PI/180.0f),11)) +
                   gainVector[12]*(1/2048.0)*sqrt(25/PI)*(231 -18018*pow(cos(theta[th]*PI/180.0f),2) +225225*pow(cos(theta[th]*PI/180.0f),4) - 1021020*pow(cos(theta[th]*PI/180.0f),6) +2078505*pow((cos(theta[th]*PI/180.0f)),8)-1939938*pow(cos(theta[th]*PI/180.0f),10)+676039*pow(cos(theta[th]*PI/180.0f),12));
    }


    float minGain = gain[0];
    for(int th = 1; th < 37; th++)
    {
    	if(gain[th] < minGain)
    	{
    		minGain = gain[th];
    	}
    }

    return minGain;

}

int parseArgs(int argc, char const *argv[], Generation &inputGen, int &roul_x_no, int &roul_mut_no, int &tour_x_no, int &tour_mut_no)
{
    
    // Parse input arguments from the command-line
    // returns 0 on success
    // returns 1 if an error or unexpected input occurs

    /*
        INPUT ARGUMENTS : 

        argc = number of arguments given on the command-line
        argv[] = array of arguments passed on the command line
        gainVector = vector read gain coefficient values will be stored to
        phaseVector = vector read phase coefficent values will be stored to 
        Veff = vector read Veff (fitness score) values will be written to 
        parent_no = variable that number of parents read-in will be written to 
        roul_x_no = variable that number of roullete crossbreeds will be written to
        roul_mut_no = variable that number of roulette mutations will be written to
        tour_x_no = variable that number of tournament crossbreeds will be written to
        tour_mut_no = variable that number of tournament mutations will be written to
    */



    vector<int> flagIndices(1, 0);
    int numFlags = 0;

    // Find location of input flags (if any) passed on the command line
    for (int i = 1; i < argc; i++)
    {
        // Use '-' as a flag indicator
        if (argv[i][0] == '-')
        {
            //Store found location in a vector for future reference
            flagIndices[numFlags] = i;
            flagIndices.push_back(0);
            numFlags++;
        }
    }

    // Begin parsing in inputs by looking at provided flags
    int numParents;
    for (int i = 0; i < flagIndices.size(); i++)
    {
        
        if(flagIndices[i] != 0)
        {

            // For current flag, switch through possible flags
            int argIndex = flagIndices[i];
            switch ( mapInputFlag(argv[argIndex]) )
            {   
                // Parent flag is found
                case parentFlag:

                    cout << "Parent Flag Read" << endl;

                    // Next argv is the number of parents being input
                    numParents = stoi(argv[argIndex + 1]);
                    inputGen.resize(numParents);


                    if (numParents != 0) 
                    {

                        // // Push back input vectors to adapt to the number of parents being used
                        // for (int j = 0; j < numParents-1; j++)
                        // {   
                        //     gainVector.push_back(vector<vector<float>> (60,vector <float>(13, 0)));
                        //     phaseVector.push_back(vector<vector<float>> (60,vector <float>(13, 0)));
                        //     Veff.push_back(0.0f);
                        // }

                        // cout << endl;



                        // Read coefficients and Veff of each parent
                        for (int chromo = 0; chromo < numParents; chromo++)
                        {   
                            cout << "Reading parent " << chromo+1 << "/" << numParents << endl;

                            vector<vector<double>> inputGain(60, vector<double>(13, 0));
                            vector<vector<double>> inputPhase(60, vector<double>(13, 0));
                            vector<vector<double>> coeff;
                            coeff.reserve(inputGain.size() + inputPhase.size());

                            // Read gain coefficients of parent
                            cout << "Reading gain coeff... " << endl;
                            readCoeff(argv[argIndex + chromo + 2], DataType::gain, inputGain);

                            // Read phase coefficients of parent
                            cout << "Reading phase coeff... " << endl;
                            readCoeff(argv[argIndex + chromo + 2], DataType::phase, inputPhase);

                            coeff.insert( coeff.end(), inputGain.begin(), inputGain.end() );
                            coeff.insert( coeff.end(), inputPhase.begin(), inputPhase.end() );
                            inputGen.children[chromo].assignGenotype(coeff);

                            // Read Veff of parent 
                            cout << "Reading Veff..." << endl;
                            readVeff(argv[argIndex + chromo + 2], inputGen.children[chromo]);
                            cout << "Finished parent \n" << endl;
                        }
                        cout << endl;

                	   }

                    break;


                // Algorithm parameter flag found
                case algoFlag:

                    cout << "Algorithm Flag Read" << endl;
                    
                    // Parse algorithm parameters from argv array
                    roul_x_no = atoi(argv[argIndex + 1]);
                    roul_mut_no = atoi(argv[argIndex + +2]);
                    tour_x_no = atoi(argv[argIndex + 3]);
                    tour_mut_no = atoi(argv[argIndex + 4]);

                    cout << "\nRoulette Crossbreeds: " << roul_x_no << "\nRoulette Mutations: " << roul_mut_no << endl;
                    cout << "\nTournament Crossbreeds: " << tour_x_no << "\nTournament Mutations: " << tour_mut_no << endl;
                    cout << "\nTotal Children: " << roul_x_no + roul_mut_no + tour_mut_no + tour_x_no << "\n\n" << endl;
                    break;

                // A bad flag was saved to the array
                default:
                    cout << "Input Flag Error, IGNORING FLAG : " << argv[flagIndices[i]] << endl;
                    this_thread::sleep_for(chrono::milliseconds(1000));
                    break;
                }
            }

        }

        cout << "Finished parsing inputs..." << endl;
        return 0;
}


int readCoeff(string filePath, DataType dataType, vector<vector<double>> &dataVector ) 
{

    // Read and parse coefficients from nec+ file at filePath
    // returns 0 on success
    // returns 1 if an error occurs

    /*
        INPUT ARGUMENTS : 
    
        filePath = pathway to file being read
        dataType =  type of cofficient data being read
        dataVector = vector to store coefficent data

    */

    // NEC+ file parameters
    const int NUM_COlUMNS = 13;
    const int NUM_ROWS = 60;
    int START_LINE = 160021;

    // Starting point for gain coefficents
    if(dataType == gain){

        START_LINE = 160021;

    // Starting point for phase coefficents
    } else if (dataType == phase){
        
        START_LINE = 160085;

    // No data type given, return error
    }else{

        return 1;

    }


    int lineNum, rowCount, columnCount;
    string line, word;

    lineNum = 0;
    rowCount = 0;

    ifstream parentFile;

    // Open file and begin reading 
    parentFile.open(filePath);
    if ( parentFile.is_open() ){

        // Read until end of file
        while(!(parentFile.eof())){

            // Get the current line
            string line;
            getline(parentFile, line);

            // If lineNum is at START_LINE and less than 60 rows have been read, save input data
            if( (lineNum >= START_LINE) && (rowCount < 60) ){

                if( (lineNum > (START_LINE + 1)) && (lineNum < (START_LINE + NUM_ROWS + 2)) ){

                    columnCount = 0;
                    istringstream iss(line);
                    string element;

                    // Delimit data by ','
                    while(getline(iss, element, ',')){

                        string :: size_type sz;

                        // Store a data element in the dataVector
                        dataVector[rowCount][columnCount] = stof(element, &sz);

                        // Increment column index
                        columnCount++;
                        
                    }

                    // Increment row index
                    rowCount++;
                }

            }

            // Increment line number
            lineNum++;
        }

        // Close file when done
        parentFile.close();

    // If the file is not open, return an error
    } else {

        cout << "Problem opening file at : " << filePath << endl;
        return 1;

    }
 

    return 0;

}



int readVeff(string filePath, Chromosome &parent)
{   

    // Read and parse Veff from NEC+ file found at filePath
    // returns 0 on success
    // returns 1 if an error occurs

    /*
        INPUT ARGUMENTS : 
    
        filePath = pathway to file being read
        Veff =  variable to save the Veff score to

    */

    // Line number for Veff in a NEC+ file
    const int VEFF_LINE = 160149;

    int lineNum;
    string line, word;

    lineNum = 1;

    // Open file and begin reading
    ifstream parentFile;
    parentFile.open(filePath);

    if ( parentFile.is_open() ){

        while(!(parentFile.eof())){

            string line;
            getline(parentFile, line);

            // lineNum is the Veff data line
            if( lineNum == VEFF_LINE ){

                istringstream iss(line);
                string element;

                int count = 0;
                while( getline(iss, element, ':') ) {
                        
                    string :: size_type sz;
                    element.erase(0, 1);

                    // Veff score follows a ':'
                    if (count == 1) {
                        parent.fitness = stof(element, &sz)/1.0e9;
			cout << "Fit is " << parent.fitness << endl;
                    }
                        
                    count++;

                }
            }
            lineNum++;
        }

        parentFile.close();
        
    // If the file is not open, return an error
    } else {

        cout << "Problem opening file at : " << filePath << endl;
        return 1;

    }

    return 0;
}


int writeCoeff(ofstream &outFile, DataType dataType, Chromosome &outputChromo)
{   

    // Write coefficients found in gainVector and phaseVector of data-type dataType to ofstream object outFile
    // returns 0 on success
    // returns 1 if an error occurs

    /*

        INPUT ARGUMENTS : 
    
        outFile = ofstream object of output file being written to
        dataType = either poly or sphHarm, for the dataType being used 
        gainVector = gain coefficients to write to file
        phaseVector = phase coefficients to write to file

    */


	stringstream output;

    // Write gain coefficent header
    output << "Gain Coefficients" << endl;
    output << "Fit Type : ";

    // Write fit type
    if(dataType == DataType::poly){

        output << "Poly" << endl;

    } else if(dataType == DataType::sphHarm) {

        output << "Sph Harm" << endl;

    }

    // Begin writing coefficients, comma and line delimitted
    for (int i = 0; i < 60; i++)
    {
        for (int j = 0; j < 13; j++)
        {

            if(j == 12){
                output << outputChromo.genotype[i][j] << "\n";
            }else{
                output << outputChromo.genotype[i][j] << ",";
            }

        }
    }

    // Write phase coefficent header
    output << "\n\nPhase Coefficients" << endl;
    output << "Fit Type : ";

    // Write fit type
    if(dataType == DataType::poly){

        output << "Poly" << endl;

    } else if(dataType == DataType::sphHarm) {

        output << "Sph Harm" << endl;

    }

    // Begin writing coefficents, comma and line delimitted
    for (int i = 60; i < 120; i++)
    {
        for (int j = 0; j < 13; j++)
        {

            if(j == 12){
                output << outputChromo.genotype[i][j] << "\n";
            }else{
                output << outputChromo.genotype[i][j] << ",";
            }

        }
    }

    outFile << output.str().c_str() << endl;;

    return 0;

}

int writeInfo(ofstream &outFile, Chromosome &outputChromo, int runNum, int iterNum)
{
    // Write extra info to ofstream object outFile
    // returns 0 on success
    // returns 1 if an error occurs

    /*

        INPUT ARGUMENTS : 
    
        outFile = ofstream object of output file being written to
        Veff = effective volume associated with data
        runNum = generation of GA to write to NEC+
        iterNum = iteration of GA to write to NEC+

    */

    outFile << "test Veff : " << outputChromo.fitness << endl << endl;
    outFile << "Iter : " << iterNum << endl << endl;
    outFile << "Run : " << runNum << endl << endl;

    return 0;
}

int writeRadPattern(ofstream &outFile, DataType dataType, Chromosome &outputChromo)
{

    // Compute and write the radiation pattern defined by gainVector and phaseVector to ofstream object outFile
    // returns 0 on success
    // returns 1 if an error occurs

    /*

        INPUT ARGUMENTS : 
    
        outFile = ofstream object of output file being written to
        dataType = data-type (poly or sph. harm.) that will be used to reconstruct radiation pattern
        gainVector = gain coefficients to use for reconstruction
        phaseVector = phase coefficients to use for reconstruction

    */

	stringstream output;

    // Populate theta and phi arrays
    double theta[37];
    double phi[72];
    int freq[60];

    for (int th = 0; th < 37; th++)
    {
        theta[th] = (double) th*5.0f;
    }

    for (int p = 0; p < 72; p++)
    {
        phi[p] = (double) p*5.0f;
    }

    // Create an populate gain and phase arrays that define an antenna radiation pattern
    double gain[60][72][37] = {{{0}}};
    double gainDB[60][72][37] = {{{0}}};
    double phase[60][72][37] = {{{0}}};
    for (int f = 0; f < 60; f++)
    {
        output << "freq : " << 83.33 + 16.67*f << " MHz" << endl;
        //SWR will not be accurate so give it an impossible value to avoid confusion
        output << " MHz\rSWR : 0.0\n"; 
        output << " Theta \t Phi \t Gain(dB)   \t   Gain   \t   Phase(deg)\n";


        for (int p = 0; p < 72; p++)
        {
            for (int th = 0; th < 37; th++)
            {   

                // Populate the gain and phase vectors for a polynomial fit
                if(dataType == DataType::poly) {


                    gain[f][p][th] = outputChromo.genotype[f][0]*pow(theta[th]*PI/180.0, 0) + 
                                     outputChromo.genotype[f][1]*pow(theta[th]*PI/180.0, 1) + 
                                     outputChromo.genotype[f][2]*pow(theta[th]*PI/180.0, 2) + 
                                     outputChromo.genotype[f][3]*pow(theta[th]*PI/180.0, 3) + 
                                     outputChromo.genotype[f][4]*pow(theta[th]*PI/180.0, 4) + 
                                     outputChromo.genotype[f][5]*pow(theta[th]*PI/180.0, 5) + 
                                     outputChromo.genotype[f][6]*pow(theta[th]*PI/180.0, 6) + 
                                     outputChromo.genotype[f][7]*pow(theta[th]*PI/180.0, 7) + 
                                     outputChromo.genotype[f][8]*pow(theta[th]*PI/180.0, 8) + 
                                     outputChromo.genotype[f][9]*pow(theta[th]*PI/180.0, 9) + 
                                     outputChromo.genotype[f][10]*pow(theta[th]*PI/180.0, 10) +  
                                     outputChromo.genotype[f][11]*pow(theta[th]*PI/180.0, 11) + 
                                     outputChromo.genotype[f][12]*pow(theta[th]*PI/180.0, 12);

                    if (gain[f][p][th] > 0) {
                        
                        gainDB[f][p][th] = 10*log10(gain[f][p][th]); 

                    } else {

                        gain[f][p][th] = 0;
                        gainDB[f][p][th] = -80.000;

                    }
                     

                    phase[f][p][th] = fmod((outputChromo.genotype[f+60][0]*pow(theta[th]*PI/180.0f, 0) + 
                                  outputChromo.genotype[f+60][1]*pow(theta[th]*PI/180.0f, 1) + 
                                  outputChromo.genotype[f+60][2]*pow(theta[th]*PI/180.0f, 2) + 
                                  outputChromo.genotype[f+60][3]*pow(theta[th]*PI/180.0f, 3) + 
                                  outputChromo.genotype[f+60][4]*pow(theta[th]*PI/180.0f, 4) + 
                                  outputChromo.genotype[f+60][5]*pow(theta[th]*PI/180.0f, 5) + 
                                  outputChromo.genotype[f+60][6]*pow(theta[th]*PI/180.0f, 6) + 
                                  outputChromo.genotype[f+60][7]*pow(theta[th]*PI/180.0f, 7) + 
                                  outputChromo.genotype[f+60][8]*pow(theta[th]*PI/180.0f, 8) + 
                                  outputChromo.genotype[f+60][9]*pow(theta[th]*PI/180.0f, 9) + 
                                  outputChromo.genotype[f+60][10]*pow(theta[th]*PI/180.0f, 10) + 
                                  outputChromo.genotype[f+60][11]*pow(theta[th]*PI/180.0f, 11) + 
                                  outputChromo.genotype[f+60][12]*pow(theta[th]*PI/180.0f, 12))*180/PI, 180);

                    output  << theta[th] << " \t " << phi[p] << " \t " << gainDB[f][p][th] << "     \t   " << gain[f][p][th] << "     \t    " << phase[f][p][th] << endl; 

                }

                // Populate the gain and phase vectors for a spherical harmonic fit
                else if(dataType == DataType::sphHarm) {

                    gain[f][p][th] = outputChromo.genotype[f][0]*(1/2.0)*(1/sqrt(PI)) +
                                     outputChromo.genotype[f][1]*(1/2.0)*sqrt(3/PI)*cos(theta[th]*PI/180.0f) +
                                     outputChromo.genotype[f][2]*(1/4.0)*sqrt(5/PI)*(3*pow(cos(theta[th]*PI/180.0f), 2)- 1) +
                                     outputChromo.genotype[f][3]*(1/4.0)*sqrt(7/PI)*(5*pow(cos(theta[th]*PI/180.0f),3)- 3*cos(theta[th]*PI/180.0f)) +
                                     outputChromo.genotype[f][4]*(3/16.0)*sqrt(1/PI)*(35*pow(cos(theta[th]*PI/180.0f),4) - 30*pow(cos(theta[th]*PI/180.0f),2)+3) +
                                     outputChromo.genotype[f][5]*(1/16.0)*sqrt(11/PI)*(15*cos(theta[th]*PI/180.0f) - 70*pow(cos(theta[th]*PI/180.0f),3)+63*pow(cos(theta[th]*PI/180.0f),5)) +
                                     outputChromo.genotype[f][6]*(1/32.0)*sqrt(13/PI)*(-5 + 105*pow(cos(theta[th]*PI/180.0f),2)-315*pow(cos(theta[th]*PI/180.0f),4) + 231*pow(cos(theta[th]*PI/180.0f),6)) +
                                     outputChromo.genotype[f][7]*(1/32.0)*sqrt(15/PI)*(-35*cos(theta[th]*PI/180.0f)+ 315*pow(cos(theta[th]*PI/180.0f),3) -693*pow(cos(theta[th]*PI/180.0f),5) + 429*pow(cos(theta[th]*PI/180.0f),7)) +
                                     outputChromo.genotype[f][8]*(1/256.0)*sqrt(17/PI)*(35 - 1260*pow(cos(theta[th]*PI/180.0f),2) + 6930*pow(cos(theta[th]*PI/180.0f),4) - 12012*pow(cos(theta[th]*PI/180.0f),6) + 6435*pow((cos(theta[th]*PI/180.0f)),8)) +
                                     outputChromo.genotype[f][9]*(1/256.0)*sqrt(19/PI)*(315*cos(theta[th]*PI/180.0f)- 4620*pow(cos(theta[th]*PI/180.0f),3) + 18018*pow(cos(theta[th]*PI/180.0f),5) - 25740*pow(cos(theta[th]*PI/180.0f),7) + 12155*pow((cos(theta[th]*PI/180.0f)),9)) +
                                     outputChromo.genotype[f][10]*(1/512.0)*sqrt(21/PI)*(-63 +3465*pow(cos(theta[th]*PI/180.0f),2) - 30030*pow(cos(theta[th]*PI/180.0f),4) + 90090*pow(cos(theta[th]*PI/180.0f),6) -109395*pow((cos(theta[th]*PI/180.0f)),8)+46189*pow(cos(theta[th]*PI/180.0f),10)) +  
                                     outputChromo.genotype[f][11]*(1/512.0)*sqrt(23/PI)*(-693*pow(cos(theta[th]*PI/180.0f),1) +15015*pow(cos(theta[th]*PI/180.0f),3) - 90090*pow(cos(theta[th]*PI/180.0f),5) +218790*pow((cos(theta[th]*PI/180.0f)),7)-230945*pow(cos(theta[th]*PI/180.0f),9)+88179*pow(cos(theta[th]*PI/180.0f),11)) +
                                     outputChromo.genotype[f][12]*(1/2048.0)*sqrt(25/PI)*(231 -18018*pow(cos(theta[th]*PI/180.0f),2) +225225*pow(cos(theta[th]*PI/180.0f),4) - 1021020*pow(cos(theta[th]*PI/180.0f),6) +2078505*pow((cos(theta[th]*PI/180.0f)),8)-1939938*pow(cos(theta[th]*PI/180.0f),10)+676039*pow(cos(theta[th]*PI/180.0f),12));

                    if (gain[f][p][th] > 0) {
                        
                        gainDB[f][p][th] = 10*log10(gain[f][p][th]); 

                    } else {

                        gainDB[f][p][th] = -80.000;
                        
                    }


                    phase[f][p][th] = fmod(outputChromo.genotype[f+60][0]*(1/2.0)*(1/sqrt(PI)) +
                                     outputChromo.genotype[f+60][1]*(1/2.0)*sqrt(3/PI)*cos(theta[th]*PI/180.0f) +
                                     outputChromo.genotype[f+60][2]*(1/4.0)*sqrt(5/PI)*(3*pow(cos(theta[th]*PI/180.0f), 2) - 1) +
                                     outputChromo.genotype[f+60][3]*(1/4.0)*sqrt(7/PI)*(5*pow(cos(theta[th]*PI/180.0f),3)- 3*cos(theta[th]*PI/180.0f)) +
                                     outputChromo.genotype[f+60][4]*(3/16.0)*sqrt(1/PI)*(35*pow(cos(theta[th]*PI/180.0f),4) - 30*pow(cos(theta[th]*PI/180.0f),2)+3) +
                                     outputChromo.genotype[f+60][5]*(1/16.0)*sqrt(11/PI)*(15*cos(theta[th]*PI/180.0f) - 70*pow(cos(theta[th]*PI/180.0f),3)+63*pow(cos(theta[th]*PI/180.0f),5)) +
                                     outputChromo.genotype[f+60][6]*(1/32.0)*sqrt(13/PI)*(-5 + 105*pow(cos(theta[th]*PI/180.0f),2)-315*pow(cos(theta[th]*PI/180.0f),4) + 231*pow(cos(theta[th]*PI/180.0f),6)) +
                                     outputChromo.genotype[f+60][7]*(1/32.0)*sqrt(15/PI)*(-35*cos(theta[th]*PI/180.0f)+ 315*pow(cos(theta[th]*PI/180.0f),3) -693*pow(cos(theta[th]*PI/180.0f),5) + 429*pow(cos(theta[th]*PI/180.0f),7)) +
                                     outputChromo.genotype[f+60][8]*(1/256.0)*sqrt(17/PI)*(35 - 1260*pow(cos(theta[th]*PI/180.0f),2) + 6930*pow(cos(theta[th]*PI/180.0f),4) - 12012*pow(cos(theta[th]*PI/180.0f),6) + 6435*pow((cos(theta[th]*PI/180.0f)),8)) +
                                     outputChromo.genotype[f+60][9]*(1/256.0)*sqrt(19/PI)*(315*cos(theta[th]*PI/180.0f)- 4620*pow(cos(theta[th]*PI/180.0f),3) + 18018*pow(cos(theta[th]*PI/180.0f),5) - 25740*pow(cos(theta[th]*PI/180.0f),7) + 12155*pow((cos(theta[th]*PI/180.0f)),9)) +
                                     outputChromo.genotype[f+60][10]*(1/512.0)*sqrt(21/PI)*(-63 +3465*pow(cos(theta[th]*PI/180.0f),2) - 30030*pow(cos(theta[th]*PI/180.0f),4) + 90090*pow(cos(theta[th]*PI/180.0f),6) -109395*pow((cos(theta[th]*PI/180.0f)),8)+46189*pow(cos(theta[th]*PI/180.0f),10)) +  
                                     outputChromo.genotype[f+60][11]*(1/512.0)*sqrt(23/PI)*(-693*pow(cos(theta[th]*PI/180.0f),1) +15015*pow(cos(theta[th]*PI/180.0f),3) - 90090*pow(cos(theta[th]*PI/180.0f),5) +218790*pow((cos(theta[th]*PI/180.0f)),7)-230945*pow(cos(theta[th]*PI/180.0f),9)+88179*pow(cos(theta[th]*PI/180.0f),11)) +
                                     outputChromo.genotype[f+60][12]*(1/2048.0)*sqrt(25/PI)*(231 -18018*pow(cos(theta[th]*PI/180.0f),2) +225225*pow(cos(theta[th]*PI/180.0f),4) - 1021020*pow(cos(theta[th]*PI/180.0f),6) +2078505*pow((cos(theta[th]*PI/180.0f)),8)-1939938*pow(cos(theta[th]*PI/180.0f),10)+676039*pow(cos(theta[th]*PI/180.0f),12))*180/PI,180);

                    output  << theta[th] << " \t " << phi[p] << " \t " << gainDB[f][p][th] << "     \t   " << gain[f][p][th] << "     \t    " << phase[f][p][th] << endl; 

                } else {

                    return 1;

                }

            }
        }

        // float max = 0.0;
        // for(int i = 0; i < 37; i++){
        //     if(gain[f][0][i] > max){
        //         max = gain[f][0][i];
        //     } 

        // }

        // double sum = 0.0;
        // for(int i = 0; i < 37; i++){
        //     sum = sum + (gain[f][0][i]*sin(theta[i]*PI/180.0f)/max);
        //     if(gain[f][0][i] > max){
        //         max = gain[f][0][i];
        //     } 

        // }

        // float checkHPBW = 2*PI*sum*(5*PI/180)*max;
        // cout << "Max : " << max << "\tHPBW Check : "<< checkHPBW / (4*PI) << endl;

    }

    outFile << output.str().c_str() << endl;

    return 0;
}

int necWrite(DataType dataType, Generation outputGen)
{   
    // Write all data to a unique chile NEC+ file
    // returns 0 on success
    // returns 1 if an error occurs

    /*

        INPUT ARGUMENTS : 
    
        dataType = data-type (poly or sph. harm.) that will be used to reconstruct radiation pattern 
        gainVector = gain coefficients to write
        phaseVector = phase coefficients to write
        Veff = effective volume to write

    */

    string txt = ".txt";
    string base;

    cout << "\n\nFinished GA, writing to output files\n\n";

    for(int i = 0; i < outputGen.genSize; i++) {

        base = "child_";

        ofstream outFile;
        base.append(to_string(i));
        base.append(txt);
        outFile.open(base);

        writeRadPattern(outFile, dataType, outputGen.children[i]);
        writeCoeff(outFile, dataType, outputGen.children[i]);
        writeInfo(outFile, outputGen.children[i], 0, i);

        outFile.close();
        cout << "Child " << i+1 << "/" << outputGen.genSize << " complete" << endl;
    }

    return 0;
}

int writeHist(ofstream &histFile, Generation &gen)
{
    for(int i = 0; i < gen.genSize; i++){
	if(i < gen.genSize-1){
            histFile << gen.children[i].fitness << ",";
        } else {
	    histFile << gen.children[i].fitness;
        }
    }


    histFile << endl;

    return 0;

}



/*
    Assigns a genotype to a new child chromosome by crossover method

    @param c1 Parent chromosome 1
    @param c2 Parent chromosome 2
    @param newChromo Child chromosome to assign genotype to

    @return Returns 0 on success
*/
int crossover(Chromosome &c1, Chromosome &c2, Chromosome &newChromo)
{   

    // Make c1 the better performing fitness score
    if (c1.fitness < c2.fitness)
    {
        Chromosome temp = c2;
        c2 = c1;
        c1 = temp;
    }

    // The var. 'a' dictates how much the crossover favors the better chromosome
    double a = 0.8;
    for(int x = 0; x < c1.xSize; x++)
    {
        for (int y = 0; y < c1.ySize; y++)
        {
            // New gene is average of parent genes, biased by 'a'
            newChromo.genotype[x][y] = a*c1.genotype[x][y] + (1-a)*c2.genotype[x][y];
        }

    }

    return 0;
}

/*
    Assigns a genotype to a new child chromosome by mutation method

    @param c1 Parent chromosome 1
    @param c2 Parent chromosome 2
    @param newChromo Child chromosome to assign genotype to

    @return Returns 0 on success
*/
int mutation(Chromosome &c1, Chromosome &newChromo)
{       
    // Initialize mean and variance of normal distribution
    double mean;
    double var = 1.0;

    std::random_device rd{};
    std::mt19937 gen{rd()};

    vector<int> indices = {0,1,0};

    for(int x = 0; x < c1.xSize; x++)
    {
        for (int y = 0; y < c1.ySize; y++)
        {

            shuffle(indices.begin(), indices.end(), gen);

            if(indices[0])
            {
                // Mean will be the parent's current gene value
                mean = c1.genotype[x][y];

                // Generate a high qaulity random number from the normal distrbution
                std::normal_distribution<> d{mean,var};

                // Assign the random number as the gene
                newChromo.genotype[x][y] = d(gen);

            } else {
                newChromo.genotype[x][y] = c1.genotype[x][y];   
            }
        }

    }

    return 0;
}

/*
    Select two parents for reproduction using the roullette selection method

    @param gen Generation to select parents from
    @param newChromo Child chromosome that will be assigned a genotype
    @param mut Boolean flag. False = mutation assignment. True = crossover assignment

    @return Returns 0 on success, returns 1 on failure
*/
int roullette(Generation &gen, Chromosome &newChromo, bool mut = false)
{   


    // Create two empty chromosomes for parent assignment
    Chromosome p1(gen.children[0].xSize, gen.children[0].ySize); 
    Chromosome p2(gen.children[0].xSize, gen.children[0].ySize);

    // Find the sum of the fitness scores
    double S = 0.0;
    for (int chromo = 0; chromo < gen.genSize; chromo++)
    {
        S += gen.children[chromo].fitness;
    }

    // Generate a high qaulity random number from a uniform distrbution with range [0,S]
    std::random_device rd{};
    std::mt19937 generator{rd()};
    std::uniform_real_distribution<> dis(0, S);

    // Generate two random thresholds in the range [0,S], and sort from low->high
    vector<double> thresh = {dis(generator), dis(generator)};
    sort(thresh.begin(), thresh.end());

    // Spin the roulette wheel
    S = 0.0;
    bool flag = false;
    for (int chromo = 0; chromo < gen.genSize; chromo++)
    {
        S += gen.children[chromo].fitness;

        // The first threshold has been reached
        if(S >= thresh[0] && flag==false)
        {

            // Mark that the first threshold has been passed
            flag = true;

            // Assign parent to chromosome that passed the threshold
            p1 = gen.children[chromo];

            // If mutation, mutate with this parent and stop
            if(mut == true){
                mutation(p1, newChromo);
                return 0;
            }
        }

        // The second threshold has been reached
        if(S >= thresh[1])
        {
            // Assign parent to chromosome that passed the threshold
            p2 = gen.children[chromo];

            // Perform the crossover with both parents
            crossover(p1, p2, newChromo);

            return 0;
        }
    }

    // Return 1 on failure
    return 1;
}

/*
    Select two parents for reproduction using the tournament selection method

    @param gen Generation to select parents from
    @param newChromo Child chromosome that will be assigned a genotype
    @param mut Boolean flag. False = mutation assignment. True = crossover assignment

    @return Returns 0 on success, returns 1 on failure
*/
int tournament(Generation &gen, Chromosome &newChromo, bool mut = false)
{

    // Create two empty chromosomes for parent assignment
    Chromosome p1(gen.children[0].xSize, gen.children[0].ySize); 
    Chromosome p2(gen.children[0].xSize, gen.children[0].ySize);

    // Generate a high quality random integer from a uniform distrbution with range [0,k] 
    std::random_device rd{};
    std::mt19937 generator{rd()};
    std::uniform_real_distribution<> dis(0, gen.genSize);

    // k is the tournament bracket size
    int k = gen.genSize/3;

    // Vecetor for storing chromosome indices
    vector<int> indices(gen.genSize, 0);

    // Create a k-size vector of Chromosomes that will be selected 
    vector<Chromosome> selected(k, Chromosome(1,1));

    // Populate the indices vector 
    for (int i = 0; i < gen.genSize; i++)
    {
        indices[i] = i;
    }

    // Shuffle the indices vector
    shuffle(indices.begin(), indices.end(), generator);

    // Read out the shuffles indices vector k-times, assigning chromosomes at these 
    // indices to the selected bracket
    for (int i = 0; i < k; i++)
    {
        selected[i] = gen.children[indices[i]];
    }

    // Sort the selected chromsomes by fitness score
    auto& tmp1 = selected;
    std::sort(selected.begin(), selected.end(), 
    [&tmp1](const Chromosome &l, const Chromosome &r){return l.fitness < r.fitness;});

    // Select the highest performing chromosome as a parent
    p1 = selected[k-1];

    // If mut==True, stop selection, perfrom mutation, and end
    if (mut == true)
    {
        mutation(p1, newChromo);
        return 0;
    }


    // Repeat the tournament process to select another parent

    // Populate the indices vector 
    for (int i = 0; i < gen.genSize; i++)
    {
        indices[i] = i;
    }

    // Shuffle the indices vector
    shuffle(indices.begin(), indices.end(), generator);

    // Read out the shuffles indices vector k-times, assigning chromosomes at these 
    // indices to the selected bracket
    for (int i = 0; i < k; i++)
    {
        selected[i] = gen.children[indices[i]];
    }

    // Sort the selected chromsomes by fitness score
    auto& tmp2 = selected;
    std::sort(selected.begin(), selected.end(), 
    [&tmp2](const Chromosome &l, const Chromosome &r){return l.fitness < r.fitness;});

    // Select the highest performing chromosome as a parent
    p2 = selected[k-1];

    // Perform the crossover with both parents
    crossover(p1, p2, newChromo);

    return 0;
}

int constraints(Chromosome &chromo)
{
    
    int numTh = 100;

    // Array of theta values to evaluate the radiation pattern
    vector<double> theta(numTh, 0);
    vector<vector<double>> gain(60, vector<double>(numTh, 0));

    // Populate the theta array from 0-180 deg
    for (int th = 1; th < numTh; th++)
    {
        theta[th] = (double) (th)*(180.0f/(numTh-1));
    }

    for(int f = 0; f < 60; f++)
    {
        // Evaluate the power gain at each theta by exapnding the spherical harmonic coefficents
        chromo.genotype[f][0] = 2*sqrt(M_PI);
        for(int th = 0; th < numTh; th++)
        {   
        
            gain[f][th] = chromo.genotype[f][0]*(1/2.0)*(1/sqrt(M_PI)) +
                      chromo.genotype[f][1]*(1/2.0)*sqrt(3/M_PI)*cos(theta[th]*M_PI/180.0f) +
                      chromo.genotype[f][2]*(1/4.0)*sqrt(5/M_PI)*(3*pow(cos(theta[th]*M_PI/180.0f), 2)- 1) +
                      chromo.genotype[f][3]*(1/4.0)*sqrt(7/M_PI)*(5*pow(cos(theta[th]*M_PI/180.0f),3)- 3*cos(theta[th]*M_PI/180.0f)) +
                      chromo.genotype[f][4]*(3/16.0)*sqrt(1/M_PI)*(35*pow(cos(theta[th]*M_PI/180.0f),4) - 30*pow(cos(theta[th]*M_PI/180.0f),2)+3) +
                      chromo.genotype[f][5]*(1/16.0)*sqrt(11/M_PI)*(15*cos(theta[th]*M_PI/180.0f) - 70*pow(cos(theta[th]*M_PI/180.0f),3)+63*pow(cos(theta[th]*M_PI/180.0f),5)) +
                      chromo.genotype[f][6]*(1/32.0)*sqrt(13/M_PI)*(-5 + 105*pow(cos(theta[th]*M_PI/180.0f),2)-315*pow(cos(theta[th]*M_PI/180.0f),4) + 231*pow(cos(theta[th]*M_PI/180.0f),6)) +
                      chromo.genotype[f][7]*(1/32.0)*sqrt(15/M_PI)*(-35*cos(theta[th]*M_PI/180.0f)+ 315*pow(cos(theta[th]*M_PI/180.0f),3) -693*pow(cos(theta[th]*M_PI/180.0f),5) + 429*pow(cos(theta[th]*M_PI/180.0f),7)) +
                      chromo.genotype[f][8]*(1/256.0)*sqrt(17/M_PI)*(35 - 1260*pow(cos(theta[th]*M_PI/180.0f),2) + 6930*pow(cos(theta[th]*M_PI/180.0f),4) - 12012*pow(cos(theta[th]*M_PI/180.0f),6) + 6435*pow((cos(theta[th]*M_PI/180.0f)),8)) +
                      chromo.genotype[f][9]*(1/256.0)*sqrt(19/M_PI)*(315*cos(theta[th]*M_PI/180.0f)- 4620*pow(cos(theta[th]*M_PI/180.0f),3) + 18018*pow(cos(theta[th]*M_PI/180.0f),5) - 25740*pow(cos(theta[th]*M_PI/180.0f),7) + 12155*pow((cos(theta[th]*M_PI/180.0f)),9)) +
                      chromo.genotype[f][10]*(1/512.0)*sqrt(21/M_PI)*(-63 + 3465*pow(cos(theta[th]*M_PI/180.0f),2) - 30030*pow(cos(theta[th]*M_PI/180.0f),4) + 90090*pow(cos(theta[th]*M_PI/180.0f),6) -109395*pow((cos(theta[th]*M_PI/180.0f)),8)+46189*pow(cos(theta[th]*M_PI/180.0f),10)) +  
                      chromo.genotype[f][11]*(1/512.0)*sqrt(23/M_PI)*(-693*pow(cos(theta[th]*M_PI/180.0f),1) +15015*pow(cos(theta[th]*M_PI/180.0f),3) - 90090*pow(cos(theta[th]*M_PI/180.0f),5) +218790*pow((cos(theta[th]*M_PI/180.0f)),7)-230945*pow(cos(theta[th]*M_PI/180.0f),9)+88179*pow(cos(theta[th]*M_PI/180.0f),11)) +
                      chromo.genotype[f][12]*(1/2048.0)*sqrt(25/M_PI)*(231 -18018*pow(cos(theta[th]*M_PI/180.0f),2) +225225*pow(cos(theta[th]*M_PI/180.0f),4) - 1021020*pow(cos(theta[th]*M_PI/180.0f),6) +2078505*pow((cos(theta[th]*M_PI/180.0f)),8)-1939938*pow(cos(theta[th]*M_PI/180.0f),10)+676039*pow(cos(theta[th]*M_PI/180.0f),12));
                   //chromo.genotype[0][12]*(1/2048.0)*sqrt(27/M_PI)*(3003*pow(cos(theta[th]*M_PI/180.0f),1) -90090*pow(cos(theta[th]*M_PI/180.0f),3)+765765*pow(cos(theta[th]*M_PI/180.0f),5)-2771340*pow((cos(theta[th]*M_PI/180.0f)),7)+4849845*pow(cos(theta[th]*M_PI/180.0f),9)-4056234*pow(cos(theta[th]*M_PI/180.0f),11) + 1300075*pow(cos(theta[th]*M_PI/180.0f),13));
        }


        double minGain = gain[f][0];
        for(int i = 0; i < numTh; i++)
        {
            if(gain[f][i] < minGain)
            {
                minGain = gain[f][i];
            }
        }


        if(minGain < 0)
        {
            for(int j = 1; j < 13; j++)
            {
                chromo.genotype[f][j] = (1/(1-minGain))*chromo.genotype[f][j];
            }
        }

    }




    // float S = 0;
    // for(int ind = 0; ind < chromo.ySize; ind++)
    // {
    //  S = S + pow(chromo.genotype[0][ind], 2);
    // }    

    // S = sqrt(S);

    // for(int ind = 0; ind < chromo.ySize; ind++)
    // {
    //  chromo.genotype[0][ind] = (1/S)*chromo.genotype[0][ind];
    // }    


    return 0;
}
