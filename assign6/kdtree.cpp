#include <iostream> 
#include <iomanip>
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

int nearest(const Matrix &tree, int start, int end, int col, Matrix &item, float best) {
    int pivot = (end + start) / 2;
    int bestex;
    float candidate_best;

    std::cout << "RANGE: " << start << "  to  " << end << std::endl;

    //single row (leaf)
    if (end == start) {
        int idx = start;
        Matrix leaf = tree.subMatrix(idx, 0, 1, 0);
        candidate_best = sqrt(item.dist2(leaf));
        std::cout << candidate_best << " " << best << std::endl;
        if (candidate_best < best) {
            best = candidate_best;
            bestex = idx;
            std::cout << "BESTLEAF: " << std::setprecision(3) << 
                std::fixed << best << " " << bestex << std::endl;
        }
        return bestex;
    }

    //parent cases
    else {
        int parent_idx = pivot;
        int child1_idx = (start + pivot - 1) / 2;
        int child2_idx = (pivot + 1 + end) / 2;
        Matrix parent = tree.subMatrix(parent_idx, 0, 1, 0);
        Matrix child1 = tree.subMatrix(child1_idx, 0, 1, 0);
        Matrix child2 = tree.subMatrix(child2_idx, 0, 1, 0);

        //Try lower half
        if (item.get(0, col) <= parent.get(0, col)) {
            std::cout << "LOWER" << std::endl;
            //try first child if it is there
            if (pivot - start > 0) {
                std::cout << "FIRST CHILD" << std::endl;
                bestex = nearest(tree, start, pivot - 1,
                        (col + 1) % tree.numCols(), item, best);
                if (abs(item.get(0, col) - parent.get(0, col)) > best) {
                    std::cout << "PRUNE" << std::endl;
                    return bestex;
                }
            }

            //try parent
            candidate_best = sqrt(item.dist2(parent));
            std::cout << candidate_best << " " << best << std::endl;
            if (candidate_best < best) {
                best = candidate_best;
                bestex = parent_idx;
                std::cout << "BESTPARENT1: " << std::setprecision(3) 
                    << std::fixed << best << " " 
                    << bestex << std::endl;
                if (best == 0) {
                    return bestex;
                }
            }

            //try second child if it is there
            if (end - pivot > 0) {
                std::cout << "SECOND CHILD" << std::endl;
                bestex = nearest(tree, pivot + 1, end, 
                        (col + 1) % tree.numCols(), item, best);
            }
        }

        //Try upper half
        else {
            std::cout << "UPPER" << std::endl;
            //try second child if it is there
            if (end - pivot > 0) {
                std::cout << "SECOND CHILD" << std::endl;
                bestex = nearest(tree, pivot + 1, end, 
                        (col + 1) % tree.numCols(), item, best);
                if (abs(item.get(0, col) - parent.get(0, col)) > best) {
                    std::cout << "PRUNE" << std::endl;
                    return bestex;
                }
            }

            //try parent
            candidate_best = sqrt(item.dist2(parent));
            if (candidate_best < best) {
                best = candidate_best;
                bestex = parent_idx;
                std::cout << "BESTPARENT2: " << std::setprecision(3) 
                    << std::fixed << best << " " 
                    << bestex << std::endl;
                if (best == 0) {
                    return bestex;
                }
            }

            //try first child if it is there
            if (pivot - start > 0) {
                std::cout << "FIRST CHILD" << std::endl;
                bestex = nearest(tree, start, pivot - 1, 
                        (col + 1) % tree.numCols(), item, best);
            }
        }
        return bestex;
    }
    std::cout << "ERROR: Should not hit here. Thanos wins." << std::endl;
    exit(1);
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

    std::cout << "KDTree version of matrix";
    x.printLabeledRow(labels);

    Matrix unlabeled = x.subMatrix(0, 1, 0, 0);

    for (int r = 0; r < eval.numRows(); r++) {
        std::cout << "SOLVE:";
        eval.writeLine(r);
        std::cout<< std::endl;
        Matrix row("row");
        row = eval.subMatrix(r, 0, 1, 0);
        int idx = nearest(unlabeled, 0, unlabeled.numRows() - 1, 1, row, 2147483647);
        std::cout << "Ans:";
        unlabeled.writeLine(idx);
        std::cout << idx << " " << labels[idx] << std::endl << std::endl;
    }
}
