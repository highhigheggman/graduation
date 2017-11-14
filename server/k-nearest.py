#!/usr/bin/env python
#! -*- coding: utf-8 -*-

import sys
import os
import glob
import numpy as np
import mysql.connector
from sklearn.metrics import accuracy_score
from sklearn.neighbors import NearestNeighbors

def __main():
    args = sys.argv

    if len(args) != 3:
        print('usage: python k-nearest.py [maxAcc] [minAcc]')
        sys.exit()

    nowAcc = [args[1], args[2]]

    # connect db
    config ={
        'user': 'web',
        'password': '176520-Kz',
        'host': 'localhost',
        'database' : 'acc_DB',
    }

    cnx = mysql.connector.connect(**config)

    # check connect
    if(cnx.is_connected() == False):
        exit()

    # select data from DB
    cursor = cnx.cursor()
    stmt = "SELECT maxAcc, minAcc FROM acc_DB.sensorVal WHERE deviceID = 10001 ORDER BY sensorTime DESC LIMIT 100;"
    cursor.execute(stmt)
    accList = cursor.fetchall()

    # Nearest Neighbors
    neigh = NearestNeighbors(n_neighbors=5)
    neigh.fit(accList)
    #distances, indices = neigh.kneighbors([nowAcc])
    #print(distances)
    print(len(neigh.radius_neighbors([nowAcc], 0.25)[0][0]))
    #print(neigh.radius_neighbors([nowAcc], 0.25))

if __name__ =='__main__':
    __main()
