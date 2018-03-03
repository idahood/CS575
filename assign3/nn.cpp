#include <iostream> 
#include "mat.h"
#include "rand.h"

double f(double x){
    if (x > 0.5) {
        return 1.0;
    }
    else {
        return 0.0;
    }
}

int main () {
    const int MAX_ITER = 100;
    const float LEARN_RATE = 0.1;
    initRand();

    int inputs = 0;
    Matrix training("training");
    Matrix testing("testing");

    //Parse input file
    scanf("%i", &inputs);
    training.read();
    testing.read();

    //Extract sub-matrices from training data
    Matrix x;
    x = training.extract(0, 0, training.numRows(), inputs + 1);
    x.setName("x");

    Matrix t;
    t = training.extract(0, inputs, 0, 0);
    t.setName("t");

    //Normalize testing
    double test_max = testing.max();
    Matrix temp_testing = testing;
    temp_testing.setName("temp_testing");
    temp_testing.scalarMult(1.0/test_max);
    Matrix norm_testing(testing.numRows(), testing.numCols() + 1, "norm_testing");
    norm_testing.constant(-1.0);
    norm_testing.insert(temp_testing, 0, 0);

    //Normalize x
    double x_max = x.max();
    x.scalarMult(1.0/x_max);

    //Append bias column to x
    Matrix bias(training.numRows(), 1, "bias");
    bias.constant(-1.0);
    x.insert(bias, 0, inputs);

    //Initialize weights
    Matrix w(inputs + 1, training.numCols() - inputs, "w");
    w = w.rand(0.0, 1.0);

    //perceptron loop
    for (int i = 0; i < MAX_ITER; i++) {
        t = training.extract(0, inputs, 0, 0); 
        Matrix y;
        y = x.dot(w).map(f);
        y.setName("y");

        Matrix error;
        error = t.sub(y);
        error.setName("error");

        w.add(x.Tdot(error).scalarMult(LEARN_RATE));
    }

    Matrix result;
    result = norm_testing.dot(w);
    result.map(f);
    result.setName("result");

    std::cout << "BEGIN TESTING" << std::endl;
    for (int i = 0; i < testing.numRows(); i++) {
        testing.writeLine(i);
        result.writeLine(i);
        std::cout << std::endl;
    }
}
