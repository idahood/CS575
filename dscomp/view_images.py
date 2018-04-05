#!/usr/bin/env python

from __future__ import print_function

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from scipy import ndimage
from skimage import filters

def reduce_noise(x):
    return np.where(x < 0.4, 0.0, x)

def enhance(data):
    val = filters.threshold_otsu(data)
    result = data < (val * 1.15)
    return result

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
    #image = reduce_noise(image)    #REDUCE NOISE DUE TO "ERASED" LAYER
    image = enhance(image)    #REDUCE NOISE DUE TO "ERASED" LAYER
    plt.title('Example: %d' % (idx))
    plt.imshow(image, cmap=plt.get_cmap('gray_r'))
    plt.xticks(range(0,120,24))
    plt.axvline(x = 24, color='r')
    plt.axvline(x = 48, color='r')
    plt.axvline(x = 72, color='r')
    plt.axvline(x = 96, color='r')
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

def expression_to_character(data):
    '''
    Takes ndarray, and return resized ndarray
    '''

    result = []
    for line in data:
        expression = line.reshape([24, 120])
        STEP = 24
        for i in range (0, 5):
            character = expression[0:24, i*STEP:(i+1)*STEP].reshape(576,)
            result.append(character)
    return np.array(result)

def main():
    train_data = pd.read_csv('train.csv', index_col=0,
                             header=0).as_matrix().astype(dtype=np.float32)

    test_data = pd.read_csv('test.csv', index_col=0,
                            header=0).as_matrix().astype(dtype=np.float32)

    malformed_indices = []

    '''
    split_images = test_splitter(test_data[3], 5)
    for image in split_images:
        image = image.reshape([24, 24])
        #image = reduce_noise(image)    #REDUCE NOISE DUE TO "ERASED" LAYER
        #image = enhance(image)    #REDUCE NOISE DUE TO "ERASED" LAYER
        plt.imshow(image, cmap=plt.get_cmap('gray_r'))
        plt.show()
    '''

    for idx in malformed_indices:
        display_test_digit(idx, test_data, 24, 120)

if __name__ == '__main__':
    main()
