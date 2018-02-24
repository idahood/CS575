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

double f(double x){
    if (x > 0.5) {
        return 1.0;
    }
    else {
        return 0.0;
    }
}

int main () {
    const int MAX_ITER = 5000;
    const float LEARN_RATE = 0.01;
    initRand();

    std::string line = "";
    std::vector<std::string> token_line;    
    int inputs = 0;
    int training_rows = 0;
    int training_cols = 0;
    int testing_rows = 0;
    int testing_cols = 0;

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

    //TODO: Should I normalize x and t?

    Matrix x;
    x = training.extract(0, 0, training_rows, inputs);
    x.setName("x");
    //Normalize x
    double max = x.max();
    x.scalarMult(1.0/max);

    Matrix t;
    t = training.extract(0, inputs, 0, 0);
    t.setName("t");

    Matrix w(inputs, training_cols - inputs, "w");
    w = w.rand(0.0, 1.0);

    for (int i = 0; i < MAX_ITER; i++) {
        t = training.extract(0, inputs, 0, 0);
        Matrix y("y");
        y = x.dot(w);
        y.setName("y");
        //y.map(f);

        Matrix error;
        error = t.sub(y);
        error.setName("error");
        //std::cout << "ITER: " << i << std::endl;
        //error.print();

        Matrix x_t("x tranpose");
        x_t = x.transpose();
        w = w.add(x_t.dot(error).scalarMult(LEARN_RATE));
    }

    Matrix result;
    result = testing.dot(w);
    result.map(f);
    result.setName("result");
    testing.print();
    w.print();
    result.print();
}
