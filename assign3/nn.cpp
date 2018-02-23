#include <iostream> 
#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include "mat.h"
#include "rand.h"

std::vector<std::string> split(std::string str){
    std::istringstream buf(str);
    std::istream_iterator<std::string> beg(buf), end;
    std::vector<std::string> tokens(beg, end);
    return tokens;
}

int main () {
    std::string line = "";
    int inputs = 0;
    int training_rows = 0;
    int training_cols = 0;
    
    int testing_rows = 0;
    int testing_cols = 0;

    std::vector<std::string> token_line;

    //Parse number of inputs
    getline(std::cin, line);
    inputs = std::stoi(line);

    //Parse training rows, cols
    getline(std::cin, line);
    token_line = split(line);
    training_rows = stoi(token_line[0]);
    training_cols = stoi(token_line[1]);

    //Parse training matrix
    Matrix training(training_rows, training_cols, "training");
    training.constant(0); //Needed to set defined: true
    for (int r = 0; r < training_rows; r++) {
        getline(std::cin, line);
        token_line = split(line);
        for ( int c = 0; c < training_cols; c++ ) {
            training.set(r, c, std::stod(token_line[c]));
        }
    }

    training.print();
    
    //Parse testing rows, cols
    getline(std::cin, line);
    token_line = split(line);
    testing_rows = stoi(token_line[0]);
    testing_cols = stoi(token_line[1]);

    //Parse testing matrix
    Matrix testing(testing_rows, testing_cols, "testing");
    testing.constant(0); //Needed to set defined: true
    for (int r = 0; r < testing_rows; r++) {
        getline(std::cin, line);
        token_line = split(line);
        for ( int c = 0; c < testing_cols; c++ ) {
            testing.set(r, c, std::stod(token_line[c]));
        }
    }

    testing.print();
}
