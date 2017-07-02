#!/usr/bin/env python
#! -*- coding: utf-8 -*-

import sys
import os
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

def __main():
    args = sys.argv

    if len(args) != 3:
        print('usage: python graph.py [readFilePath] [saveFilePath]')
        sys.exit()

    filePath = args[1]
    savePath = args[2]

    # read csv
    df = pd.read_csv(filePath, header=None, parse_dates=[0])

    # change colums name
    df.columns = ['time', 'x', 'y', 'z']

    # print(df[df['time'].str.contains('11:17:49')])

    print(df.head())

    print(df.dtypes)

"""
    # open window and make 3graph
    fig, (ax1, ax2, ax3) = plt.subplots(3)

    # declear line class ,initialize graph and line
    ax1Line, = ax1.plot(ax1X, ax1Y)
    ax2Line, = ax2.plot(ax2X, ax2Y)
    ax3Line, = ax3.plot(ax3X, ax3Y)
"""


if __name__ =='__main__':
    __main()
