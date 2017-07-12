#!/usr/bin/env python
#! -*- coding: utf-8 -*-

import sys
import os
import glob
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

def returnFileInt(path):
    base = os.path.basename(path)
    root, ext = os.path.splitext(base)

    if(root.isdigit()):
        return int(root)
    else:
        return root

def __main():
    args = sys.argv

    if len(args) != 2:
        print('usage: python readData.py inputPath]')
        sys.exit()

    # make empty dataFrame
    df = pd.DataFrame(columns=['date', 'x', 'y', 'z'])


    # if inputPath is directory Path
    if os.path.isdir(args[1]):
        filePathList = [i for i in glob.glob(os.path.join(args[1], '*')) if not os.path.isdir(i)]
        filePathList.sort(key = returnFileInt)

        print(filePathList)

        # read csv
        for filePathList in filePathList:
            tempDf = pd.read_csv(filePathList, header=None, parse_dates=[0], na_values = 'na', dtype={1:'float64', 2:'float64', 3:'float64'})
            tempDf.columns = ['date', 'x', 'y', 'z']
            print(tempDf.head())
            print(tempDf.dtypes)
            df = df.append(tempDf, ignore_index=True)

    # if inputPath is file Path
    elif os.path.isfile(args[1]):
        filePath = args[1]
        # read csv
        tempDf = pd.read_csv(filePath, header=None, parse_dates=[0], na_values = 'na', dtype={1:'float64', 2:'float64', 3:'float64'})
        tempDf.columns = ['date', 'x', 'y', 'z']
        df = df.append(tempDf, ignore_index=True)


    print(df.head())
    print(df.dtypes)

    # get unique date name list
    uDateList = df['date'].sort_values().unique()

    # get data useing unique date name list
    sumD = 0
    uDateSize = uDateList.size

    for uDateList in uDateList:
        sumD += len(df[df['date'] == uDateList])

    print(sumD)
    print(uDateSize)
    print(sumD/uDateSize)

if __name__ =='__main__':
    __main()
