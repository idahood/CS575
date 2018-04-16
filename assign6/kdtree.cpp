#include <iostream> 
#include "mat.h"
#include "rand.h"

void build_kdtree(Matrix &original, int start, int end, int col) {
    int pivot = (end + start) / 2;
    original.sortRowsByCol(col, start, end);

    if (end - start <= 2) {
        return;
    }
    else {
        col = (col % (original.numCols() - 1)) + 1;
        build_kdtree(original, start, pivot - 1, col);
        build_kdtree(original, pivot + 1, end, col);
    }
}

int nearest(const Matrix &tree, int start, int end, Matrix &item) {
    int pivot = (end + start) / 2;
    std::cout << "RANGE: " << start << "  to  " << end << std::endl;
    //single row (leaf)
    if (end - start == 0) {
        int idx = start;
        Matrix leaf = tree.subMatrix(idx, 0, 1, 0);
        std::cout << "BESTLEAF: " << sqrt(item.dist2(leaf)) << " " << idx << std::endl;
        return idx;
    }

    //parent cases
    //parent with 1 child
    else if (end - start == 1) {
        Matrix parent = tree.subMatrix(start, 0, 1, 0);
        Matrix leaf = tree.subMatrix(end, 0, 1, 0);
        if (sqrt(item.dist2(leaf)) < sqrt(item.dist2(parent))) {
            std::cout << "BESTLEAF: " << sqrt(item.dist2(leaf)) 
                << " " << end << std::endl;
            return end;
        }
        else {
            std::cout << "BESTPARENT: " << sqrt(item.dist2(parent))
                << " " << start << std::endl;
            return start;
        }
    }
    //parent with 2 children
    else if (end - start == 2) {
        Matrix parent = tree.subMatrix(pivot, 0, 1, 0);
        Matrix leaf1 = tree.subMatrix(start, 0, 1, 0);
        Matrix leaf2 = tree.subMatrix(end, 0, 1, 0);
        double best_dist = sqrt(item.dist2(leaf1));
        int best = start;
        if (sqrt(item.dist2(pivot)) < best_dist) {
            best_dist = sqrt(item.dist2(pivot));
            best = pivot;
        }
        if (sqrt(item.dist2(end)) < best_dist) {
            best_dist = sqrt(item.dist2(end));
            best = end;
        }
        if (best == start) {
            std::cout << "BESTLEAF: " << best_dist << " " << best << std::endl;
            return best;
        }
        else if (best == pivot) {
            std::cout << "BESTPARENT: " << best_dist << " " << best << std::endl;
            return best;
        }
        else {
            std::cout << "BESTLEAF: " << best_dist << " " << best << std::endl;
            return best;
        }
    }

    //recurse more
    else {
        if (sqrt(item.dist2(tree.subMatrix(start, 0, 1, 0))) 
                < sqrt(item.dist2(tree.subMatrix(pivot, 0, 1, 0)))) {
            return nearest(tree, start, pivot - 1, item);
        }
        else {
           return nearest(tree, pivot + 1, end, item);
        }
    }
}

int main(int argc, char *argv[]) {
    //Parse input file
    Matrix x("kd tree");
    char **labels;
    labels = x.readLabeledRow();

    Matrix eval("eval");
    eval.read();

    //Recursively build a K-D Tree
    build_kdtree(x, 0, x.numRows() - 1, 1);

    x.printLabeledRow(labels);

    Matrix unlabeled = x.subMatrix(0, 1, 0, 0);

    for (int r = 0; r < eval.numRows(); r++) {
        std::cout << "SOLVE:";
        eval.writeLine(r);
        std::cout<< std::endl;
        Matrix row("row");
        row = eval.subMatrix(r, 0, 1, 0);
        int idx = nearest(unlabeled, 0, unlabeled.numRows() - 1, row);
        std::cout << "Ans:";
        unlabeled.writeLine(idx);
        std::cout << idx << " " << labels[idx] << std::endl << std::endl;
    }
}
