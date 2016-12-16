#!/usr/bin/env python3

import pandas
import sklearn.tree
import numpy as np

def load_data():
    return pandas.read_csv('titanic.csv')

def main():
    data = load_data()

    ## Replace string parameters with numerical once
    data['Sex'] = data['Sex'].map({'female': -1, 'male': 1})

    ## Remove points with unknown parameters values
    data = data[np.isnan(data['Pclass']) != True]
    data = data[np.isnan(data['Fare']) != True]
    data = data[np.isnan(data['Age']) != True]
    data = data[np.isnan(data['Sex']) != True]

    ## Create training set
    data = data[np.isnan(data['Survived']) != True]
    xval = data[['Pclass', 'Fare', 'Age', 'Sex']]
    yval = data['Survived']
    print(xval)

    ## Create decision tree
    tree = sklearn.tree.DecisionTreeClassifier(random_state=241)
    tree.fit(xval, yval)

    ## Format output
    name = list(xval.columns.values)
    imps = tree.feature_importances_
    rval = [list(x) for x in zip(imps, name)]
    rval = sorted(rval, key = lambda x: x[0], reverse=True)
    print(rval)

if __name__ == "__main__":
    main()


