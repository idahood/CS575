#!/usr/bin/env python

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

def f(x):
    return np.where(x < 0.4, 0.0, x)

def display_train_digit(idx, data, label, x_dim, y_dim):
    print('Index: ' + str(idx))
    image = data[idx].reshape([x_dim, y_dim])
    plt.title('Example: %d  Label: %d' % (idx, label[idx]))
    plt.imshow(image, cmap=plt.get_cmap('gray_r'))
    plt.show()

def display_test_digit(idx, data, x_dim, y_dim):
    print('Index: ' + str(idx))
    image = data[idx].reshape([x_dim, y_dim])
    image = f(image)    #REDUCE NOISE DUE TO "ERASED" LAYER
    plt.title('Example: %d' % (idx))
    plt.imshow(image, cmap=plt.get_cmap('gray_r'))
    plt.axvline(x = 0, color='r')
    plt.axvline(x = 24, color='r')
    plt.axvline(x = 48, color='r')
    plt.axvline(x = 72, color='r')
    plt.axvline(x = 96, color='r')
    plt.axvline(x = 120, color='r')
    plt.show()

def main():
    #train_data = pd.read_csv('train.csv', index_col=0,
    #                         header=0).as_matrix().astype(dtype=np.float32)
    #train_labels = pd.read_csv('train_labels.csv', index_col=0,
    #                          header=0).as_matrix()

    test_data = pd.read_csv('test.csv', index_col=0,
                            header=0).as_matrix().astype(dtype=np.float32)

    for i in range (0, 4):
        display_test_digit(i, test_data, 24, 120)

if __name__ == '__main__':
    main()
