#include <iostream> 
#include "mat.h"
#include "rand.h"

Matrix center(Matrix &original) {
    Matrix result = original;
    // TODO: Should I transpose here? Rows vs Columns in 1.1 of handout
    // After testing the resulting images look better w/out the double transpose

    for (int c = 0; c < result.numCols(); c++) {
        double mean_x = result.meanCol(c);
        double stddev_x = result.stddevCol(c);
        for (int r = 0; r < result.numRows(); r++) {
            double val = result.get(r, c);
            result.set(r, c, (val - mean_x)/stddev_x);
        }
    }

    return result;
}

Matrix uncompress(const Matrix &compressed, Matrix &original) {
    Matrix result = compressed;

    for (int c = 0; c < result.numCols(); c++) {
        double mean_x = original.meanCol(c);
        double stddev_x = original.stddevCol(c);
        for (int r = 0; r < result.numRows(); r++) {
            double val = result.get(r, c);
            result.set(r, c, (val * stddev_x) + mean_x);
        }
    }

    return result;
}

Matrix parser(std::string filename) {
    Matrix result;

    if (filename.compare(filename.size() - 3, 3, "ppm") == 0) {
        result.readImagePpm(filename, "image");
    }
    else if (filename.compare(filename.size() - 3, 3, "pgm") == 0) {
        result.readImagePgm(filename, "image");
    }
    else {
        std::cerr << filename << " unsupported filetype" << std::endl;
        exit(1);
    }

    return result;
}

void putter(std::string filename, Matrix &output) {
    if (filename.compare(filename.size() - 3, 3, "ppm") == 0) {
        output.writeImagePpm("z-after.ppm", "Reconstituted Image");
    }
    else if (filename.compare(filename.size() - 3, 3, "pgm") == 0) {
        output.writeImagePgm("z-after.pgm", "Reconstituted Image");
    }
    else {
        std::cerr << filename << " unsupported filetype" << std::endl;
        exit(1);
    }
}

int main(int argc, char *argv[]) {
    //std::string filename = argv[1];
    //int k = atoi(argv[2]);

    //Parse input file
    int k;
    scanf("%i", &k);

    Matrix image("Pic");
    image.readImagePpm("", "Pic");
    image.printSize();

    // Center the data
    Matrix X_prime = center(image);
    Matrix X_prime_1 = X_prime;
    Matrix X_prime_2 = X_prime;

    // Compute covariance matrix
    Matrix M;
    M = X_prime_1.transpose().scalarMult(1.0 / image.numRows()).dot(X_prime_2);

    // Compute eigenvalues(W) and eigenvectors(V) of M
    Matrix V = M;
    Matrix W("EigenValues");
    W = V.eigenSystem();
    W.printSize();

    // Normalize the eigenvectors
    Matrix V_prime = V;
    V_prime.scalarMult(1.0 / (V.max() - V.min()));

    // Sort eigenvectors by eigenvalue
    // eigenSystem() call returns pairs sorted in decreasing magnitude
    Matrix V_hat = V.extract(0, 0, k, 0);

    // Translate the normalized data
    Matrix X_dbl_prime = X_prime.dotT(V_hat);
    X_dbl_prime.setName("Encoded");
    X_dbl_prime.printSize();

    // Recovering data from compressed data
    Matrix X_star = X_dbl_prime.dot(V_hat);
    X_star = uncompress(X_star, image);

    // Calculate difference between original and reconstituted image
    // TODO: May need to divide dist2 return value by number of elements in
    // X_star?
    std::cout << "DIST: " << image.dist2(X_star) / (X_star.numRows() * X_star.numCols()) << std::endl;

    // Save reconstitued image
    //putter(filename, X_star);
    X_star.writeImagePpm("z-after.ppm", "Reconstituted Image");
}
