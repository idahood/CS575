#include <iostream> 
#include "mat.h"
#include "rand.h"

Matrix center(Matrix &original) {
    Matrix result = original.transpose();
    //TODO: Should I transpose here? Rows vs Columns in 1.1 of handout
    
    for (int c = 0; c < result.numCols(); c++) {
        double mean_x = original.meanCol(c);
        double stddev_x = original.stddevCol(c);
        for (int r = 0; r < result.numRows(); r++) {
            double val = result.get(r, c);
            result.set(r, c, (val - mean_x)/stddev_x);
        }
    }

    return result.transpose();
}

int main(int argc, char *argv[]) {
    char* filename = argv[1];
    int k = atoi(argv[2]);

    Matrix image;
    image.readImagePgm(filename, "image");

    // Center the data
    Matrix X_prime = center(image);
    Matrix X_prime_1 = X_prime;
    Matrix X_prime_2 = X_prime;
    std::cout << "X_prime ";
    X_prime.printSize();

    // Compute covariance matrix
    Matrix M;
    M = X_prime_1.transpose().scalarMult(1.0 / image.numRows()).dot(X_prime_2);
    std::cout << "M ";
    M.printSize();

    // Compute eigenvalues(W) and eigenvectors(V) of M
    Matrix V = M;
    Matrix W;
    W = V.eigenSystem();
    std::cout << "W ";
    W.printSize();
    std::cout << "V ";
    V.printSize();

    // Normalize the eigenvectors
    Matrix V_prime = V;
    V_prime.scalarMult(1.0 / (V.max() - V.min()));
    std::cout << "V_prime ";
    V_prime.printSize();

    // Sort eigenvectors by eigenvalue
    // eigenSystem() call returns pairs sorted in decreasing magnitude
    Matrix V_hat = V.extract(0, 0, k, 0);
    std::cout << "V_hat ";
    V_hat.printSize();

    // Translate the normalized data
    Matrix X_dbl_prime = X_prime.dotT(V_hat);
    std::cout << "X_dbl_prime ";
    X_dbl_prime.printSize();

}
