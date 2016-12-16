#!/usr/bin/env python3

import pandas as pd
import numpy as np
from sklearn.cross_validation import KFold
from sklearn.cross_validation import cross_val_score
from sklearn.neighbors import KNeighborsClassifier
from sklearn.preprocessing import scale

def main():
    data = pd.read_csv('wine.data', header=None)
    yval = data.ix[:, 0]
    xval = data.ix[:, 1:]
    xval = scale(xval);
    scr  = list()

    for i in range(1, 51):
        knn = KNeighborsClassifier(n_neighbors=i)
        kf  = KFold(n=len(data), n_folds=5, shuffle=True, random_state=42)
        val = cross_val_score(knn, xval, y=yval, cv=kf)
        scr.append(np.mean(val))

    v = max(scr)
    k = scr.index(v) + 1
    print("k, v = {0}, {1}".format(k, v))
    
if __name__ == "__main__":
    main()


