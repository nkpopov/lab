#!/usr/bin/env python3

import sys
import pandas as pd
import numpy as np
from sklearn import metrics
from sklearn.cross_validation import KFold
from sklearn.cross_validation import cross_val_score
from sklearn.neighbors import KNeighborsRegressor
from sklearn.preprocessing import scale
from sklearn.datasets import load_boston

def main():
    data = load_boston()
    yval = data.target
    xval = scale(data.data)
    rng  = np.linspace(1, 10, 200)
    scr  = list()

    for i in rng:
        knn = KNeighborsRegressor(n_neighbors=5, weights='distance', p=i)
        kf  = KFold(n=len(xval), n_folds=5, shuffle=True, random_state=42)
        val = cross_val_score(knn, xval, y=yval, scoring='mean_squared_error', cv=kf)
        scr.append(np.mean(val))

    v = max(scr)
    p = scr.index(v)
    print(scr)
    print("i, p, v = {0}, {1}, {2}".format(p, rng[p], v))

if __name__ == "__main__":
    main()

