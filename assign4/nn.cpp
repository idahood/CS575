#include <iostream> 
#include "mat.h"
#include "rand.h"

double f(double x){
    return 1.0/(1.0 + exp(-4.0 * x));
}

double step(double x){
    if (x > 0.5) {
        return 1.0;
    }
    else {
        return 0.0;
    }
}

int main () {
    const int MAX_ITER = 10000;
    const float LEARN_RATE = 0.1;
    const int H_SIZE = 2;
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

    //Build weight (w, v) matrices
    Matrix v(inputs + 1, H_SIZE, "v");
    v = v.rand(0.0, 1.0);

    Matrix w(H_SIZE + 1, training.numCols() - inputs, "w");
    w = w.rand(0.0, 1.0);

    //Backprop loop
    for (int i = 0; i < MAX_ITER; i++) {
        Matrix h(training.numRows(), H_SIZE, "h");
        h = x.dot(v).map(f);
        Matrix h_plus(training.numRows(), H_SIZE + 1, "h_plus");
        h_plus.constant(-1.0);
        h_plus.insert(h, 0, 0);

        Matrix y(t.numRows(), training.numCols() - inputs, "y");
        y = h_plus.dot(w).map(f);
    
        //Watch your operators here
        Matrix w_delta(training.numRows(), training.numCols() - inputs, "w_delta");
        Matrix y_temp = y;
        w_delta = y.sub(t).mult(y_temp).mult(y_temp.scalarPreSub(1));
        y_temp.print();
        y.print();

        Matrix h_delta(training.numRows(), H_SIZE + 1, "h_delta");
        h_delta = h_plus.mult(h_plus.scalarPreSub(1)).mult(w_delta.dotT(w));

        //update w
        Matrix w_temp = h_plus.Tdot(w_delta);
        w_temp.scalarMult(LEARN_RATE);
        w = w.sub(w_temp);
        //w.sub(h_plus.Tdot(w_delta).scalarMult(LEARN_RATE));

        //update v
        Matrix h_delta_minus(training.numRows(), H_SIZE, "h_delta_minus");
        h_delta_minus = h_delta.extract(0, 0, training.numRows(), H_SIZE);
        v.sub(x.Tdot(h_delta_minus).scalarMult(LEARN_RATE));

        //DEBUG
        std::cout << "ITER: " << i << std::endl;
        w.print();
        v.print();
    }

    Matrix temp_h(testing.numRows(), H_SIZE + 1, "temp_h");
    temp_h.constant(-1.0);
    temp_h.insert(norm_testing.dot(v), 0, 0);
    temp_h.setName("temp_h");

    Matrix result("result");
    result = temp_h.dot(w).map(f).map(step);
    result.setName("result");

    std::cout << "BEGIN TESTING" << std::endl;
    for (int i = 0; i < testing.numRows(); i++) {
        testing.writeLine(i);
        result.writeLine(i);
        std::cout << std::endl;
    }
}
