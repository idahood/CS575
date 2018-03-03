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

int argmax(Matrix m, int r) {
    int idx = 0;
    double max = m.get(r, idx);
    for (int c = 0; c < m.numCols(); c++) {
        if (m.get(r, c) > max) {
            max = m.get(r, c);
            idx = c;
        }
    }
    return idx;
}

int main () {
    const int MAX_ITER = 10000;
    const float LEARN_RATE = 0.1;
    initRand();

    int inputs = 0;
    int H_SIZE = 0;
    Matrix training("training");
    Matrix testing("testing");

    //Parse input file
    scanf("%i", &inputs);
    scanf("%i", &H_SIZE);
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

    for (int i = 0; i < MAX_ITER; i++) {
        //Forward
        //h
        Matrix h(training.numRows(), H_SIZE, "h");
        h = x.dot(v).map(f);
        Matrix h_plus(training.numRows(), H_SIZE + 1, "h_plus");
        h_plus.constant(-1.0);
        h_plus.insert(h, 0, 0);
        Matrix h_plus1 = h_plus;
        Matrix h_plus2 = h_plus;
        Matrix h_plus3 = h_plus;

        //y
        Matrix y(t.numRows(), training.numCols() - inputs, "y");
        y = h_plus.dot(w).map(f);
    
        //Mission Accomplished!
        if (y.equal(t)) {
            std::cout << "Training completed early on iter: " << i << std::endl;
            break;
        }

        //Backprop starts
        //w_delta
        Matrix w_delta(training.numRows(), training.numCols() - inputs, "w_delta");
        Matrix y1 = y;
        y1.setName("y1");
        Matrix y2 = y;
        y2.setName("y2");
        Matrix y3 = y;
        y3.setName("y3");
        w_delta = y1.sub(t).mult(y2).mult(y3.scalarPreSub(1.0));

        //h_delta
        Matrix h_delta = h_plus1;
        h_delta.mult(h_plus2.scalarPreSub(1.0));
        h_delta.mult(w_delta.dotT(w));

        //update w
        w.sub(h_plus3.Tdot(w_delta).scalarMult(LEARN_RATE));

        //update v
        Matrix h_delta_minus(training.numRows(), H_SIZE, "h_delta_minus");
        h_delta_minus = h_delta.extract(0, 0, training.numRows(), H_SIZE);
        v.sub(x.Tdot(h_delta_minus).scalarMult(LEARN_RATE));
    }

    Matrix temp_h(testing.numRows(), H_SIZE + 1, "temp_h");
    temp_h.constant(-1.0);
    temp_h.insert(norm_testing.dot(v).map(f), 0, 0);
    temp_h.setName("temp_h");

    Matrix result("result");
    result = temp_h.dot(w).map(f);
    result.map(step);
    result.setName("result");

    std::cout << "BEGIN TESTING" << std::endl;
    for (int i = 0; i < testing.numRows(); i++) {
        testing.writeLine(i);
        std::cout << argmax(result, i);
        std::cout << std::endl;
    }
}
