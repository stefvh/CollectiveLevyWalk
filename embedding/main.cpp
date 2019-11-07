/*
 * AUTHOR: Stef Van Havermaet <stefvanhavermaet@gmail.com>
 *
 * Initiate one experiment configuration for multiple runs.
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
    for (UInt32 i = 0; i < vSeeds.size(); ++i) {
        // Reset loop functions and controllers
        cSimulator.Reset(vSeeds[i]);
        // Run i-th iteration of experiment
        cSimulator.Execute();
        // Gather data
        // Write step lengths
        if(!cLoopFunctions.GetData()->WalkDistances.empty()) {
            std::ostringstream cStepLengthsOSS;
            cStepLengthsOSS << cLoopFunctions.GetOutputParams()->OutputStepLengthsFolderName 
                            << cLoopFunctions.GetOutputParams()->OutputFileName
                            << "_" << i << ".txt";
            std::ofstream cStepLengthsOFS(cStepLengthsOSS.str().c_str());
            cStepLengthsOFS << "#StepLength(double)" << std::endl;
            for(size_t j = 0; j < cLoopFunctions.GetData()->WalkDistances.size(); ++j) {
                cStepLengthsOFS << cLoopFunctions.GetData()->WalkDistances[j] << std::endl;
            }
        }
        // Write state counters
        if(!cLoopFunctions.GetData()->Robots.empty()) {
            std::ostringstream cStateCountersOSS;
            cStateCountersOSS << cLoopFunctions.GetOutputParams()->OutputStateCountersFolderName 
                            << cLoopFunctions.GetOutputParams()->OutputFileName
                            << "_" << i << ".txt";
            std::ofstream cStateCounterOFS(cStateCountersOSS.str().c_str());
            cStateCounterOFS << "#WalkTicks(UInt32)\tRotateTicks(UInt32)\tCollisionAvoidanceTicks(UInt32)" << std::endl;
            for(size_t j = 0; j < cLoopFunctions.GetData()->Robots.size(); ++j) {
                cStateCounterOFS << cLoopFunctions.GetData()->Robots[j].TicksInWalkState << "\t"
                                << cLoopFunctions.GetData()->Robots[j].TicksInRotateState << "\t"
                                << cLoopFunctions.GetData()->Robots[j].TicksInCollisionAvoidanceState
                                << std::endl;
            }
        }
        // Write target findings
        if(!cLoopFunctions.GetData()->TargetFindings.empty()) {
            std::ostringstream cTargetFindingsOSS;
            cTargetFindingsOSS << cLoopFunctions.GetOutputParams()->OutputTargetFindingsFolderName 
                            << cLoopFunctions.GetOutputParams()->OutputFileName
                            << "_" << i << ".txt";
            std::ofstream cTargetFindingsOFS(cTargetFindingsOSS.str().c_str());
            cTargetFindingsOFS << "#Tick(UInt32)\tRobotId(UInt32)\tTargetId(UInt32)" << std::endl;
            for(size_t j = 0; j < cLoopFunctions.GetData()->TargetFindings.size(); ++j) {
                cTargetFindingsOFS << cLoopFunctions.GetData()->TargetFindings[j].SimulationTick << "\t"
                                << cLoopFunctions.GetData()->TargetFindings[j].RobotId << "\t"
                                << cLoopFunctions.GetData()->TargetFindings[j].TargetId
                                << std::endl;
            }
        }
    }

    cSimulator.Destroy();

    return 0;
}