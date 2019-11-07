/*
 * AUTHOR: Stef Van Havermaet <stefvanhavermaet@gmail.com>
 *
 * Create random sparse target positions as input to the random walk experiment simulations.
 *
 */

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <string>
#include <random>

#include <argos3/core/utility/datatypes/datatypes.h>
#include <argos3/core/utility/math/general.h>

/****************************************/
/****************************************/

bool IsDuplicateTarget(
    argos::Real fX,
    argos::Real fY,
    std::vector<argos::Real> vX,
    std::vector<argos::Real> vY,
    int iVectorSize
) {
    bool bDuplicate = false;
    argos::Real radSumSq = 0.1156;
    for (int i = 0; i < iVectorSize && !bDuplicate; ++i) {
        argos::Real distSq = (fX - vX[i]) * (fX - vX[i]) + (fY - vY[i]) * (fY - vY[i]);
        bDuplicate = distSq <= radSumSq;
    }
    return bDuplicate;
}

/****************************************/
/****************************************/

int main(int argc, char** argv) {
    /*
     * Passing arguments
     */
    std::string sOutputFileName = "targets_sparse.txt";
    int size = 100;
    argos::Real fNestSideBuffer = 3.06 + 2.0;
    
    int opt; 
    while((opt = getopt(argc, argv, ":f:n:")) != -1)  
    { 
        switch(opt)  
        {
            case 'f':
                sOutputFileName = optarg;
                break;
            case 'n':
                size = atoi(optarg);
                break;
            case ':':  
                printf("Option requires a value: %c\n", opt);  
                break;  
            case '?':  
                printf("Unknown option: %c\n", optopt); 
                break;  
        }
    }

    /*
     * Initialize RNG
     */
    const argos::Real range_from  = -83.0;
    const argos::Real range_to    = 83.0;
    std::random_device                          rand_dev;
    std::mt19937                                generator(rand_dev());
    std::uniform_real_distribution<argos::Real> distr(range_from, range_to);

    /*
     * Create and save sparse target positions in output file
     */
    std::ofstream cSparseTargetsOFS(sOutputFileName.c_str());
    std::string strDelimiter = "\t";
    cSparseTargetsOFS << "#SPARSE" << strDelimiter << size << std::endl;
    cSparseTargetsOFS << "#X" << strDelimiter << "Y" << std::endl;

    std::vector<argos::Real> vX;
    std::vector<argos::Real> vY;
    for (int i = 0; i < size; ++i) {
        argos::Real fX;
        argos::Real fY;
        do {
            fX = distr(generator);
            fY = distr(generator);
        } while ((argos::Abs(fX) < fNestSideBuffer && argos::Abs(fY) < fNestSideBuffer)
            || IsDuplicateTarget(fX, fY, vX, vY, i));
        vX.push_back(fX);
        vY.push_back(fY);
        cSparseTargetsOFS << fX << strDelimiter << fY << std::endl;
    }

    return 0;
}

/****************************************/
/****************************************/