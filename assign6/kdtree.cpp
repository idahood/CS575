#include <iostream> 
#include <iomanip>
#include "mat.h"
#include "rand.h"

float best = 2147483647;
int bestex = -1;

void build_kdtree(Matrix &original, int start, int end, int col) {
    int split = (end + start) / 2;
    original.sortRowsByCol(col, start, end);

    if (end - start <= 2) {
        return;
    }
    else {
        col = (col % (original.numCols() - 1)) + 1;
        build_kdtree(original, start, split - 1, col);
        build_kdtree(original, split + 1, end, col);
    }
}

void nearest(const Matrix &tree, int start, int end, int col, Matrix &item) {
    int split = (end + start) / 2;
    int parent_idx = split;
    int child1_idx = (start + split - 1) / 2;
    int child2_idx = (split + 1 + end) / 2;
    Matrix parent = tree.subMatrix(parent_idx, 0, 1, 0);
    Matrix child1 = tree.subMatrix(child1_idx, 0, 1, 0);
    Matrix child2 = tree.subMatrix(child2_idx, 0, 1, 0);
    float candidate_best;


    std::cout << "RANGE: " << start << "  to  " << end << std::endl;

    //single row (leaf)
    if (end == start) {
//        std::cout << "LEAF" << std::endl;
        candidate_best = sqrt(item.dist2(parent));
        if (candidate_best < best) {
            best = candidate_best;
            bestex = split;
            std::cout << "BESTLEAF: " << std::setprecision(3) << 
                std::fixed << best << " " << bestex << std::endl;
        }
        return;
    }

    //parent cases
    else {
        //Try lower half
//        std::cout << "LOWER" << std::endl;
        if (item.get(0, col) <= parent.get(0, col)) {
            //try first child if it is there
            if (split - start > 0) {
//                std::cout << "CHILD1" << std::endl;
                nearest(tree, start, split - 1,
                        (col + 1) % tree.numCols(), item);
                if (abs(item.get(0, col) - parent.get(0, col)) > best) {
//                    std::cout << "SHORTCUT" << std::endl;
                    return;
                }
            }

            //try parent
            candidate_best = sqrt(item.dist2(parent));
            if (candidate_best < best) {
                best = candidate_best;
                bestex = parent_idx;
                std::cout << "BESTPARENT1(" << col << "): " << std::setprecision(3) 
                    << std::fixed << best << " " 
                    << bestex << std::endl;
                if (best == 0) {
                    return;
                }
            }

            //try second child if it is there
            if (end - split > 0) {
//                std::cout << "CHILD2" << std::endl;
                nearest(tree, split + 1, end, 
                        (col + 1) % tree.numCols(), item);
                candidate_best = sqrt(item.dist2(child2));
                if (candidate_best < best) {
                    best = candidate_best;
                    bestex = child1_idx;
                }
            }
        }

        //Try upper half
        else {
        //try second child if it is there
//        std::cout << "UPPER" << std::endl;
            if (end - split > 0) {
//                std::cout << "CHILD2" << std::endl;
                nearest(tree, split + 1, end, 
                        (col + 1) % tree.numCols(), item);
                if (abs(item.get(0, col) - parent.get(0, col)) > best) {
//                    std::cout << "SHORTCUT" << std::endl;
                    return;
                }
            }

            //try parent
            candidate_best = sqrt(item.dist2(parent));
            if (candidate_best < best) {
                best = candidate_best;
                bestex = parent_idx;
                std::cout << "BESTPARENT2(" << col << "): " << std::setprecision(3) 
                    << std::fixed << best << " " 
                    << bestex << std::endl;
                if (best == 0) {
                    return;
                }
            }

            //try first child if it is there
            if (split - start > 0) {
//                std::cout << "CHILD1" << std::endl;
                nearest(tree, start, split - 1, 
                        (col + 1) % tree.numCols(), item);
                candidate_best = sqrt(item.dist2(child1));
                if (candidate_best < best) {
                    best = candidate_best;
                    bestex = child1_idx;
                }
            }
        }
        return;
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
    std::cout << std::endl << std::endl;

    Matrix unlabeled = x.subMatrix(0, 1, 0, 0);

    for (int r = 0; r < eval.numRows(); r++) {
        std::cout << "SOLVE:";
        eval.writeLine(r);
        std::cout << std::endl;
        Matrix row("row");
        row = eval.subMatrix(r, 0, 1, 0);
        best = 2147483647;
        bestex = -1;
        nearest(unlabeled, 0, unlabeled.numRows() - 1, 0, row);
        std::cout << "Ans:";
        unlabeled.writeLine(bestex);
        int orig_idx = x.get(bestex, 0);
        std::cout << orig_idx << " " << labels[orig_idx] 
            << std::endl << std::endl;
    }
}
