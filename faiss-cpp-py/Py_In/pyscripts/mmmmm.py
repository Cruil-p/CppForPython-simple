#coding=utf-8
#import sys
#import os
import time
import random
import numpy as np
from numpy import linalg as LA
import pickle
from sklearn.cluster import MiniBatchKMeans
from sklearn.preprocessing import normalize
def getObjKmeansWriteFile(n_clus, dim, xt):
    file_model = open("kmeans_model", 'wb')
    # train the index
    print("training index \n")
    xt = np.array(xt)
    print(xt[0])
    print(xt.shape)
    print("dim is:", dim)
    xt = xt.reshape(-1, dim)

    xt_double = np.array(xt, dtype = np.double)
    obj_kmeans = MiniBatchKMeans(n_clusters = n_clus, random_state = 0).fit(xt_double)
    print("training finish \n")
    pickle.dump(obj_kmeans, file_model)
    file_model.close()
    return True

def getClusCentersFromFile():
    file_model = open("kmeans_model", 'rb')
    obj_kmeans = pickle.load(file_model)

    print(obj_kmeans.cluster_center_[0])
    file_model.close()
    return obj_kmeans.cluster_centers_


def getObjKmeans(n_clus, dim, xt):
    #train the index
    print("training index \n")
    xt = np.array(xt)
    print(xt[0])
    print(xt.shape)
    print("dim is:", dim)
    xt = xt.reshape(-1, dim)

    xt_double = np.array(xt, dtype = np.float32)
    obj_kmeans = MiniBatchKMeans(n_clusters = n_clus, random_state = 0, batch_size = 100, verbose = 1,max_iter = 1, n_init = 1).fit(xt_double)
    print("training finish \n")
    return obj_kmeans

def getClusCenters(obj_kmeans):

    print(obj_kmeans.cluster_centers_[0])
    return obj_kmeans.cluster_centers_
    #array_a = np.random.randn(2,4)
    #print(array_a)
    #return array_a

def getClusIndex(obj_kmeans, xb, dim):
    print("adding data......")
    #xb = np.array(xb, dtype=np.float32）
    xb = xb.astype(np.float32)
    print("dim: ", dim)
    xb = xb.reshape(-1, dim)
    print(xb[0])

    idx_part = obj_kmeans.predict(xb)
    print(idx_part[0], idx_part[1])
    idx_total = []
    idx_total.extend(idx_part)
    idx_total = np.array(idx_total)
    print(idx_total[0], idx_total[1])
    return idx_total

#n = [81, 6]#8192 64
#data = []
#for i in range(n[0] * n[1]):
#    data.append(random.random())

#obj = getObjKmeans(n[0], n[1], data)
#getClusCenters(obj)
#getClusIndex(obj, np.array(data), 6)
