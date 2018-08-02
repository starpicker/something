#!/usr/bin/python

import numpy as np
import matplotlib.pyplot as plt
from sklearn import linear_model
from sklearn import decomposition
from sklearn import cross_validation

## read data
features = np.loadtxt('data/features_ALL.txt', delimiter=',')
ratings = np.loadtxt('data/labels.txt', delimiter=',')

features_test = np.loadtxt('features.txt', delimiter=',')

pca = decomposition.PCA(n_components=20)
pca.fit(features)
features = pca.transform(features)

regr = linear_model.LinearRegression()
regr.fit(features, ratings)

features_test = pca.transform(features_test)
myrating = regr.predict(features_test)

#print myrating

np.savetxt('predictions.txt', myrating, delimiter=',', fmt = '%.04f')

# predictions = np.zeros(ratings.size);

# for i in range(0, 500):
	# features_train = np.delete(features, i, 0)
	# features_test = features[i, :]
	# ratings_train = np.delete(ratings, i, 0)
	# ratings_test = ratings[i]
	# pca = decomposition.PCA(n_components=20)
	# pca.fit(features_train)
	# features_train = pca.transform(features_train)
	# features_test = pca.transform(features_test)
	# regr = linear_model.LinearRegression()
	# regr.fit(features_train, ratings_train)
	# predictions[i] = regr.predict(features_test)
	# print i

# np.savetxt('cross_valid_predictions.txt', predictions, delimiter=',', fmt = '%.04f')

# corr = np.corrcoef(predictions, ratings)[0, 1]
# print corr

