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

    # connect db
    config ={
        'user': 'web',
        'password': '176520-Kz',
        'host': 'localhost',
        'database' : 'acc_DB',
    }

    cnx = mysql.connector.connect(**config)
    cursor = cnx.cursor(prepared=True)

    stmt = "SELECT maxAcc, minAcc, DATE_FORMAT(sensorTime, '%H:%i:%S')
            FROM acc_DB.sensorVal
            WHERE deviceID = 10001
           ORDER BY sensorTime DESC LIMIT 100;"

    cursor.excute(stmt)

    # get data useing unique date name list
    maxList = []
    minList = []
    accList = []

    '''
    # Nearest Neighbors
    testAccList = accList[250:350]
    testMaxList = maxList[250:350]
    testMinList = minList[250:350]

    neigh = NearestNeighbors(n_neighbors=5)
    neigh.fit(testAccList)
    distances, indices = neigh.kneighbors([[2.0, 1.0]])
    print(distances)
    print(neigh.radius_neighbors([[2.0, 1.0]], 0.25))
    '''

if __name__ =='__main__':
    __main()
