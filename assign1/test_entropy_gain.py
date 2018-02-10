#!/usr/bin/env python3
from entropy_gain import *

def test_entropy():
    assert round(entropy(Data, 'Ans'), 4) == 1.6855

def test_calc_entropy():
    assert round(calc_entropy(0.5), 4) == 0.5
    assert round(calc_entropy(0.3), 4) == 0.5211
    assert round(calc_entropy(0.1), 4) == 0.3322

def test_gain():
    assert round(gain(Data, 'Deadline'), 4) == 0.5345
