/*
 * AUTHOR: Stef Van Havermaet <stefvanhavermaet@gmail.com>
 *
 * Create random seeds as input to the random walk experiment simulations.
 *
 */

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <string>
#include <random>

#include <argos3/core/utility/datatypes/datatypes.h>

/****************************************/
/****************************************/

int main(int argc, char** argv) {
    /*
     * Passing arguments
     */
    std::string sOutputFileName = "seeds.txt";
    int size = 30;
    
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
    const argos::UInt32 range_from  = 1;
    const argos::UInt32 range_to    = 5000;
    std::random_device                  rand_dev;
    std::mt19937                        generator(rand_dev());
    std::uniform_int_distribution<argos::UInt32>  distr(range_from, range_to);

    /*
     * Create and save seeds in output file
     */
    std::ofstream cSeedsOFS(sOutputFileName.c_str());
    for (size_t i = 0; i < size; ++i) {
        cSeedsOFS << distr(generator) << std::endl;
    }

    return 0;
}