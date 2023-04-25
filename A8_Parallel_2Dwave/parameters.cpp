// parameters.cpp
#include "parameters.h"

#include <fstream>
#include <iostream>
#include <filesystem>

// See parameters.h for how to use the function in this module

int readParametersFromFile(const std::string& filename, Parameters& param)
{
    if (not std::filesystem::exists(filename)) {
        std::cerr << "Error: parameter file '" << filename << "' not found.\n";
        return 2;
    }    

    std::ifstream infile(filename);
    // The following line causes 'infile' to return exceptions for errors.
    // (instead of the default behavior of setting an internal flag and having the program continue.)
    infile.exceptions(std::ifstream::failbit|std::ifstream::badbit);  

    try {
        infile >> param.c;
        infile >> param.tau;
        infile >> param.x1;
        infile >> param.x2;
        infile >> param.runtime;
        infile >> param.dx;
        infile >> param.outtime;
        infile >> param.outfilename;
        infile.close();
    }
    catch (std::ifstream::failure& e) {
        std::cerr << "Error while reading file '" << filename << "'.\n";
        return 3;
    }
    // Check input sanity, quit if there are errors
    bool correctinput = false ; // assume the worst first.
    if (param.c <= 0.0) {
        std::cerr << "wave speed c must be postive.\n";
    } else if (param.tau <= 0.0) {
        std::cerr << "damping time tau must be positive or zero\n";
    } else if (param.x1 >= param.x2) {
        std::cerr << "x1 must be less that x2.\n";
    } else if (param.dx < 0) {
        std::cerr << "dx must be postive.\n";
    } else if (param.dx > param.x2 - param.x1) {
        std::cerr << "dx too large for domain.\n";
    } else if (param.runtime < 0.0) {
        std::cerr << "runtime must be positive.\n";
    } else if (param.outtime < 0.0) {
        std::cerr << "outtime must be positive.\n";
    } else if (param.outfilename.size() == 0) {
        std::cerr << "no output filename given.\n";
    } else {
        correctinput = true;
    }
    if (not correctinput) {
        std::cerr << "Parameter value error in file '" << filename << "'\n";
        return 4;
    }
    
    return 0;
}
