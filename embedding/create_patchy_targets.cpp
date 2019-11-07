/*
 * AUTHOR: Stef Van Havermaet <stefvanhavermaet@gmail.com>
 *
 * Create random patchy target positions as input to the random walk experiment simulations.
 *
 */

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <string>
#include <random>

#include <argos3/core/utility/datatypes/datatypes.h>
#include <argos3/core/utility/math/general.h>
#include <argos3/core/utility/math/angles.h>

/****************************************/
/****************************************/

bool IsDuplicateTarget(
    argos::Real fX,
    argos::Real fY,
    std::vector<argos::Real> vX,
    std::vector<argos::Real> vY,
    int iVectorSize
) {
    if (iVectorSize < 1) return false;
    
    bool bDuplicate = false;
    argos::Real radSumSq = 0.1156;
    for (int i = 0; i < iVectorSize && !bDuplicate; ++i) {
        argos::Real distSq = (fX - vX[i]) * (fX - vX[i]) + (fY - vY[i]) * (fY - vY[i]);
        bDuplicate = distSq < radSumSq;
    }
    return bDuplicate;
}

/****************************************/
/****************************************/

int main(int argc, char** argv) {
    /*
     * Passing arguments
     */
    std::string sOutputFileName = "targets_patchy.txt";
    int patches_size = 10;
    int patch_size = 100;
    
    int opt; 
    while((opt = getopt(argc, argv, ":f:n:p:")) != -1)  
    { 
        switch(opt)  
        {
            case 'f':
                sOutputFileName = optarg;
                break;
            case 'n':
                patches_size = atoi(optarg);
                break;
            case 'p':
                patch_size = atoi(optarg);
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
    std::random_device                          rand_dev;
    std::mt19937                                generator(rand_dev());
    std::uniform_real_distribution<argos::Real> distr(0.0, 1.0);
    /*
     * Use pre-determined patch location (To align with thesis results)
     */
    std::vector<argos::Real> v_patch_x_ {
        0.45, 68.0, -26.5, -56.7, 47.2, -66.1, -11.3, -54.2, 49.2, 58.6 
    };
    std::vector<argos::Real> v_patch_y_ {
        73.7, 69.9, 54.1, 32.11, 1.1, -18.9, -51.1, -73.7, -49.1, -75.3
    };
    /*
     * Add some noise to the pre-determined patch locations
     */
    std::vector<argos::Real> v_patch_x;
    std::vector<argos::Real> v_patch_y;
    for (size_t i = 0; i < patches_size; ++i) {
        std::uniform_real_distribution<argos::Real> distrX(v_patch_x_[i] - 0.25, v_patch_x_[i] + 0.25);
        v_patch_x.push_back(distrX(generator));
        std::uniform_real_distribution<argos::Real> distrY(v_patch_y_[i] - 0.25, v_patch_y_[i] + 0.25);
        v_patch_y.push_back(distrY(generator));
    }
    /*
     * Create and save target positions for each patch in output file
     */
    std::ofstream cPatchyTargetsOFS(sOutputFileName.c_str());
    std::string strDelimiter = "\t";
    cPatchyTargetsOFS << "#PATCHY" << strDelimiter << patches_size << strDelimiter << patch_size << std::endl;
    cPatchyTargetsOFS << "#X" << strDelimiter << "Y" << std::endl;

    std::vector<argos::Real> vX;
    std::vector<argos::Real> vY;
    for (size_t i = 0; i < patches_size; ++i) {
        for (size_t j = 0; j < patch_size; ++j) {
            argos::Real fA;
            argos::Real fR;
            argos::Real fX;
            argos::Real fY;
            do {
                fA = distr(generator) * 2.0 * M_PI;
                fR = 2.2 * Sqrt(distr(generator));
                fX = v_patch_x[i] + fR * argos::Cos(argos::CRadians(fA));
                fY = v_patch_y[i] + fR * argos::Sin(argos::CRadians(fA));
            } while (IsDuplicateTarget(fX, fY, vX, vY, i*j + j)); 
            vX.push_back(fX);
            vY.push_back(fY);
            cPatchyTargetsOFS << fX << strDelimiter << fY << std::endl;
        }
    }

    return 0;
}