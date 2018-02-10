#!/usr/bin/env python3
from math import log2

global FeatureList, FeatureValues, Data
FeatureList = ['Deadline', 'Party', 'Lazy']
FeatureValues = {'Deadline': ['None', 'Near', 'Urgent'], 'Party': ['No', 'Yes'], 'Lazy': ['No', 'Yes'], 'Ans': ['Party', 'Study', 'Pub', 'TV']}
Data = [{'Deadline': 'Urgent', 'Party': 'Yes', 'Lazy': 'Yes', 'Ans': 'Party'}, {'Deadline': 'Urgent', 'Party': 'No', 'Lazy': 'Yes', 'Ans': 'Study'}, {'Deadline': 'Near', 'Party': 'Yes', 'Lazy': 'Yes', 'Ans': 'Party'}, {'Deadline': 'None', 'Party': 'Yes', 'Lazy': 'No', 'Ans': 'Party'}, {'Deadline': 'None', 'Party': 'No', 'Lazy': 'Yes', 'Ans': 'Pub'}, {'Deadline': 'None', 'Party': 'Yes', 'Lazy': 'No', 'Ans': 'Party'}, {'Deadline': 'Near', 'Party': 'No', 'Lazy': 'No', 'Ans': 'Study'}, {'Deadline': 'Near', 'Party': 'No', 'Lazy': 'Yes', 'Ans': 'TV'}, {'Deadline': 'Near', 'Party': 'Yes', 'Lazy': 'Yes', 'Ans': 'Party'}, {'Deadline': 'Urgent', 'Party': 'No', 'Lazy': 'No', 'Ans': 'Study'}]

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
    result = 0
    for f in FeatureValues[feature]:
        p = len(select(data, feature, f)) / len(data)
        result += calc_entropy(p)
    return result

def calc_entropy(p):
    if p != 0:
        return -p * log2(p)
    else:
        return 0

# current entropy - expected entropy after getting info about feature
# entropy(data, "Ans") - sum_{v=featurevalues} p_v * entropy(select(data, feature, v), "Ans")
def gain(data, feature) :
    Entropy_S = entropy(data, 'Ans')

    for val in FeatureValues[feature]:
        p = count(data, feature, val) / len(data)
        subset = select(data, feature, val)
        print(Entropy_S, entropy(subset, 'Ans'))
        Entropy_S -= p * entropy(subset, 'Ans')
    return Entropy_S
