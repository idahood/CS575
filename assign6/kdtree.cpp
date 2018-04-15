#include <iostream> 
#include "mat.h"
#include "rand.h"

void build_kdtree(Matrix &original, int start, int end, int col, char **labels) {
    int pivot = (end + start) / 2;
    original.sortRowsByCol(col, start, end);

    if (end - start == 1) {
        return;
    }
    else if (end - start == 2) {
        original.sortRowsByCol(col, start, end);
        return;
    }
    else {
        col = (col % (original.numCols() - 1)) + 1;
        //What if pivot - 1 is out of bounds?
        build_kdtree(original, start, pivot - 1, col, labels);
        build_kdtree(original, pivot + 1, end, col, labels);
    }
}

int main(int argc, char *argv[]) {
    //Parse input file
    Matrix x("kd tree");
    char **labels;
    labels = x.readLabeledRow();

    //Recursively build a KD-Tree
    build_kdtree(x, 0, x.numRows() - 1, 1, labels);

    x.printLabeledRow(labels);
}
