#!/usr/bin/env python

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

def f(x):
    return np.where(x < 0.4, 0.0, x)

def display_train_digit(idx, data, label, x_dim, y_dim):
    '''
    Train data is labeled, and 24 x 24
    '''
    print('Index: ' + str(idx))
    image = data[idx].reshape([x_dim, y_dim])
    plt.title('Example: %d  Label: %d' % (idx, label[idx]))
    plt.imshow(image, cmap=plt.get_cmap('gray_r'))
    plt.show()

def display_test_digit(idx, data, x_dim, y_dim):
    '''
    Test data is unlabeled, and 24 x 120
    There is some "noise" appears to be cleared by flooring elements < 0.4
    '''
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

def test_splitter(line, NUM_SPLITS):
    '''
    Takes line from test data, splits into 5 consituent characters
    Returns a list of ndarrays
    '''

    line = line.reshape([24, 120])

    result = []
    STEP = 24
    for i in range (0, NUM_SPLITS):
        result.append(line[0:24, i*STEP:(i+1)*STEP])
    return result

def main():
    #train_data = pd.read_csv('train.csv', index_col=0,
    #                         header=0).as_matrix().astype(dtype=np.float32)
    #train_labels = pd.read_csv('train_labels.csv', index_col=0,
    #                          header=0).as_matrix()

    test_data = pd.read_csv('test.csv', index_col=0,
                            header=0).as_matrix().astype(dtype=np.float32)

    '''
    split_images = test_splitter(test_data[19999], 5)
    for image in split_images:
        image = image.reshape([24, 24])
        image = f(image)    #REDUCE NOISE DUE TO "ERASED" LAYER
        plt.imshow(image, cmap=plt.get_cmap('gray_r'))
        plt.show()
    '''
    massaged_test_data = []
    #massaged_test_data = np.empty((0,576), dtype=np.float32) #HOW DO I INIT THIS TO BE 1x576!!
    for (counter, line) in enumerate(test_data):
        line = line.reshape([24, 120])
        STEP = 24
        for i in range (0, 5):
            temp = line[0:24, i*STEP:(i+1)*STEP].reshape(1,576)
            massaged_test_data.append(temp)
    test = np.array(massaged_test_data)
    print(test.shape)

if __name__ == '__main__':
    main()
