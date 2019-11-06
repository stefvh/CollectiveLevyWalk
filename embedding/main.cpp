/*
 * AUTHOR: Stef Van Havermaet <stefvanhavermaet@gmail.com>
 *
 * Initiate one experiment for multiple runs.
 *
 */

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <string>
#include <cstdlib>

#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/loop_functions.h>

#include "../loop_functions/rw_loop_functions/rw_loop_functions.h"

/****************************************/
/****************************************/

int main(int argc, char** argv) {
    /*
     * Passing arguments
     */
    std::string strExperimentFileName;
    std::string strSeedsFileName;

    int opt; 
    while((opt = getopt(argc, argv, ":f:s:")) != -1)  
    { 
        switch(opt)  
        {  
            case 'f':
                strExperimentFileName = optarg;
                break;
            case 's':
                strSeedsFileName = optarg;
                break;
            case ':':  
                printf("Option requires a value: %c\n", opt);  
                break;  
            case '?':  
                printf("Unknown option: %c\n", optopt); 
                break;  
        }  
    }

    if (strExperimentFileName.empty()) {
        printf("Experiment filename should be specified!\n Run: ./main -f <experimentFilename> -s <seedsFilename>\n");
        return -1;
    }
    if (strSeedsFileName.empty()) {
        printf("Seeds filename should be specified!\n Run: ./main -f <experimentFilename> -s <seedsFilename>\n");
        return -1;
    }

    std::vector<UInt32> vSeeds;
    std::ifstream cSeedsIFS(strSeedsFileName.c_str());
    UInt32 iSeed;
    while (cSeedsIFS >> iSeed) { vSeeds.push_back(iSeed); }

    /*
     * Initialize ARGoS
     */
    static argos::CSimulator& cSimulator = argos::CSimulator::GetInstance();
    cSimulator.SetExperimentFileName(strExperimentFileName);
    cSimulator.LoadExperiment();

    static CRandomWalkLoopFunctions& cLoopFunctions = dynamic_cast<CRandomWalkLoopFunctions&>(cSimulator.GetLoopFunctions());
    
    /*
     * Run the experiment Ns (number of seeds) times
     * Save data in corresponding folder and files.
     */
    system(("mkdir " + strExperimentFileName).c_str());
    for (UInt32 i = 0; i < vSeeds.size(); ++i) {
        argos::LOG << vSeeds[i] << std::endl;
        //cSimulator.Reset(vSeeds[i]);
        //cSimulator.Execute();
        //TODO: get data from loop functions for i-th run
        //TODO: get output filename 
        //TODO: write to files the data for i-th run (adding "_" + i)
    }

    cSimulator.Destroy();

    return 0;
}