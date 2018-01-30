#!/usr/bin/env python3
# Author: Dr. Robert Heckendorn, Computer Science Department, University of Idaho, 2016
#
# ID3 decision tree algorithm
#
import sys
from math import *

## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## 
#
# IO support for reading from stdin and writing to stdout
#

# read in a classifier problem
def readProblem() :
    global Features, FeatureList, FeatureValues, Data

    FeatureList = []     # list of all the features including Ans
    FeatureValues = {}   # potential values of all the features, even ones not in examples
    Data = []            # example classification data

    # read number of features
    numFeatures = int(sys.stdin.readline())

    # read in features and answer which must be called: Ans
    for i in range(0, numFeatures+1) :
        line = sys.stdin.readline().strip()
        fields = line.split()
        FeatureList.append(fields[0])
        FeatureValues[fields[0]] = fields[2:] # dictionary value is a list

    # read number of samples
    numSamples = int(sys.stdin.readline())
    
    # read in example classifications
    for line in sys.stdin.readlines() :
        fields = line.split()
        sample = {}
        for i in range(0, len(FeatureList)) :
            sample[FeatureList[i]] = fields[i]
        Data.append(sample)



# write out indented classifier tree
amountIndent = 3*" "

def printDTree(tree):
    printDTreeAux("", tree)
    
def printDTreeAux(indent, tree) :
    name = tree[0]
    d = tree[1]
    if type(d) is dict :
        for v in FeatureValues[name] :
            print(indent + name + "=" + v)
            printDTreeAux(indent + amountIndent, d[v])
    else :
        print(indent + d)


## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## 
#
# Decision Tree:  list either [Feature, Value] or [Feature, dictionary of value:tree]
# Author: Robert B. Heckendorn, University of Idaho
# 
# The OUTPUT:
#
# ["Ans", "Yes"]
# or 
# ["Price", {"$" : tree1, "$$" : tree2, "$$$" : tree1}]
# 
# 
# DATA: a list of dictionary for each training case
# 
# [{"Ans" : "Yes", "Alt" : "Yes", "Price" : "$$" ... },
#  ... ]
# 
# FEATUREVALUES: dictionary of Feature:[list of values]
# 
# {"Ans" : ["Yes", "No"], "Alt" : ["Yes", "No"], "Price" : ["$", "$$", "$$$"] ... }
# 
# 

# 
# select(data, feature, value) -> subset of data as list of dictionaries that have feature=value
# count(data, feature, value) -> number of cases in data in which feature has value 
# entropy(data, feature) -> [0, 1], feature is generally "Ans" for ID3


# list of the items in data that have feature equal to value
def select(data, feature, value) :
    return [ item for item in data if item[feature]==value ]


# count how many items in the data have feature equal to value
def count(data, feature, value) :
    num = 0
    for d in data :
        if d[feature]==value : num+=1
    return num


# what is the entropy of a question about feature?
# sum the entropy over the possible values of the feature.
def entropy(data, feature) :
    result = []
    for d in data:
        num = 0
        for f in feature:
            if d[f] != 0:
                num += -d[f] * log2(d[f])

# current entropy - expected entropy after getting info about feature 
# entropy(data, "Ans") - sum_{v=featurevalues} p_v * entropy(select(data, feature, v), "Ans")
def gain(data, feature) :
    ???


# If there one and only one value for the given feature in given data 
# If not return None
def isOneLabel(data, feature) :
    ???

# select the most popular Ans value left in the data for the constraints
# up to now.
def maxAns(data) :
    AnsDict = {}
    for d in data:
        if d['Ans'] in AnsDict:
            AnsDict[d['Ans']] += 1
        else:
            AnsDict[d['Ans']] = 1

    return max(AnsDict.keys(), key=(lambda k: AnsDict[k]))

# this is the ID3 algorithm
def ID3BuildTree(data, availableFeatures) :
    # if data is empty
    if len(data) == 0:
        return None

    # only one label for the Ans feature at this point?
    ???

    # ran out of discriminating features
    if len(availableFeatures) == 0:
        return maxAns(data)

    # pick maximum information gain
    else :
        bestFeature = None
        bestGain = None
        for feature in availableFeatures :
            g = gain(data, feature)
            print("GAIN: ", feature, ":", round(g, 4));
            if bestGain == None or g>bestGain :
                bestGain = g
                bestFeature = feature
                bestList = [feature]
            elif g==bestGain :
                bestList.append(feature)
        print("BEST:", round(bestGain, 4), bestList);
        print()
            
        # recursively construct tree on return
        treeLeaves = {}   # start with empty dictionary
        availableFeatures = availableFeatures[:]
        availableFeatures.remove(bestFeature)
        for v in FeatureValues[bestFeature] :
            treeLeaves[v] = ID3BuildTree(select(data, bestFeature, v), availableFeatures)
        return [bestFeature, treeLeaves]    # list of best feature and dictionary of trees


# do the work
def main() :
    readProblem()
    FeatureList.remove("Ans")
    tree = ID3BuildTree(Data, FeatureList)
    printDTree(tree)

main()
