#!/usr/bin/env python

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

def display_digit(idx, data, label, x_dim, y_dim):
    print(label[idx])
    identifier = label[idx]
    image = data[idx].reshape([x_dim, y_dim])
    plt.title('Example: %d  Label: %d' % (idx, identifier))
    plt.imshow(image, cmap=plt.get_cmap('gray_r'))
    plt.show()

def main():
    train_data = pd.read_csv('train.csv', index_col=0,
                             header=0).as_matrix().astype(dtype=np.float32)
    train_labels = pd.read_csv('train_labels.csv', index_col=0,
                              header=0).as_matrix()

    #test_data = pd.read_csv('test.csv', index_col=0,
    #                        header=0).as_matrix().astype(dtype=np.float32)

    for i in range (0, 4):
        display_digit(i, train_data, train_labels, 24, 24)

main()
