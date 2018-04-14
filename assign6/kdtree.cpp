#include <iostream> 
#include "mat.h"
#include "rand.h"

void build_kdtree(Matrix &original, int start, int end, int col) {
    original.sortRowsByCol(col, start, end);
    int split = (start + end) / 2;
    std::cout << split << std::endl;
    if (col == original.numCols() - 1) {
        return;
    }
    else {
        build_kdtree(original, start, split, col + 1);
        build_kdtree(original, split, end, col + 1);
    }
}

int main(int argc, char *argv[]) {
    //Parse input file
    Matrix x("x");
    char **labels;
    labels = x.readLabeledRow();

    build_kdtree(x, 0, x.numRows() - 1, 1);
    x.print();
}
