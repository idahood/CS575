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

int nearest(const Matrix &tree, int start, int end, int col, Matrix &item) {
    int pivot = (end + start) / 2;
    std::cout << "RANGE: " << start << "  to  " << end << std::endl;
    //single row (leaf)
    if (end - start == 0) {
        int idx = start;
        Matrix leaf = tree.subMatrix(idx, 0, 1, 0);
        std::cout << "BESTLEAF: " << sqrt(item.dist2(leaf)) << " " << idx << std::endl;
        return idx;
    }

    //out of bounds cases
    if (pivot - 1 < 0) {
        return 0;
    }

    if (pivot + 1 > tree.numRows() - 1) {
        return tree.numRows() - 1;
    }

    //parent cases
    else {
        Matrix parent = tree.subMatrix(pivot, 0, 1, 0);
        Matrix child1 = tree.subMatrix(pivot - 1, 0, 1, 0);
        Matrix child2 = tree.subMatrix(pivot + 1, 0, 1, 0);
        float best = 2147483647;
        int bestex = -1;

        //Try lower half
        if (item.get(0, col) < parent.get(0, col)) {
            //try first child if it is there
            if (end - start > 1) {
                col = (col % (tree.numCols() - 1)) + 1;
                return nearest(tree, start, pivot - 1, col, item);
                if (abs(item.get(0, col) - parent.get(0, col)) > best) {
                    std::cout << "BESTLEAF: " << sqrt(item.dist2(child1))
                        << " " << pivot - 1 << std::endl;
                    return pivot - 1;
                }
            }

            //try parent
            if (sqrt(item.dist2(parent)) < best) {
                best = sqrt(item.dist2(parent));
                if (best == 0) {
                    std::cout << "BESTPARENT: " << sqrt(item.dist2(parent)) 
                        << " " << pivot << std::endl;
                    return pivot;
                }
            }

            //try second child if it is there
            if (end - start > 2) {
                col = (col % (tree.numCols() - 1)) + 1;
                return nearest(tree, start, pivot + 1, col, item);
                if (sqrt(item.dist2(child2)) < best) {
                    best = sqrt(item.dist2(child2));
                    bestex = pivot + 1;
                }
            }
        }

        //Try upper half
        else {
            //try second child if it is there
            if (end - start > 2) {
                col = (col % (tree.numCols() - 1)) + 1;
                return nearest(tree, pivot + 1, end, col, item);
                if (abs(item.get(0, col) - parent.get(0, col)) > best) {
                    std::cout << "BESTLEAF: " << sqrt(item.dist2(child2))
                        << " " << pivot + 1 << std::endl;
                    return pivot + 1;
                }
            }

            //try parent
            if (sqrt(item.dist2(parent)) < best) {
                best = sqrt(item.dist2(parent));
                if (best == 0) {
                    std::cout << "BESTPARENT: " << sqrt(item.dist2(parent))
                        << " " << pivot << std::endl;
                    return pivot;
                }
            }

            //try first child if it is there
            if (end - start > 1) {
                col = (col % (tree.numCols() - 1)) + 1;
                return nearest(tree, start, pivot - 1, col, item);
                if (sqrt(item.dist2(child1)) < best) {
                    best = sqrt(item.dist2(child1));
                    bestex = pivot - 1;
                }
            }
        }
        std::cout << "BESTLEAF: " << sqrt(item.dist2(child1)) 
            << " " << pivot - 1 << std::endl;
        return bestex;
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
        int idx = nearest(unlabeled, 0, unlabeled.numRows() - 1, 1, row);
        std::cout << "Ans:";
        unlabeled.writeLine(idx);
        std::cout << idx << " " << labels[idx] << std::endl << std::endl;
    }
}
