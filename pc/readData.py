#!/usr/bin/env python
#! -*- coding: utf-8 -*-

import sys
import os
import glob
import math
import matplotlib.pyplot as plt
import scipy.fftpack as sf
from scipy import signal
from mpl_toolkits.mplot3d import Axes3D
import numpy as np
import pandas as pd

def returnFileInt(path):
    base = os.path.basename(path)
    root, ext = os.path.splitext(base)

    if(root.isdigit()):
        return int(root)
    else:
        return root

def fft1d(data, frequency, outputPath):
    N = data.size   # num of sample
    dt = 1/frequency  # sampling period

    #data = [i-500 for i in data]

    # remove spike noise using median filter
    medData = signal.medfilt(data, 9)

    # window
    hammingWindow = signal.hamming(N)
    windowedData = hammingWindow * medData

    print(type(data))
   # print(data)
    print('num of sample')
    print(N)
    print('sampling frequency')
    print(frequency)
    print('sampling period')
    print(dt)


    fftRes = sf.fft(medData) / (N/2)
    freq = sf.fftfreq(N, dt)

    #plt.style.use('seaborn-colorblind')
    plt.rcParams['font.family'] ='sans-serif'
    plt.rcParams['xtick.direction'] = 'in'
    plt.rcParams['ytick.direction'] = 'in'
    plt.rcParams['xtick.major.width'] = 1.0
    plt.rcParams['ytick.major.width'] = 1.0
    plt.rcParams['font.size'] = 24
    plt.rcParams['axes.linewidth'] = 1.0

    plt.figure(1, figsize=(20,10))
    plt.plot(freq[1:int(N/2)], np.abs(fftRes)[1:int(N/2)])
    plt.axis('tight')
    plt.xlim(0,15)
    plt.ylim(0,0.06)
    plt.ylabel("amplitude")
    plt.xlabel("frequency[Hz]")

    fig3, ax3 = plt.subplots(figsize=(20,10))
    ax3.set_ylim(0.9, 1.2)
    dtSec = dt * 1000
    xAxis = [i*dtSec for i in range(N)]
    ax3.plot(xAxis, data)
    plt.ylabel("[mv/g]")
    plt.xlabel("time[ms]")

    plt.show()

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

    # noise filter
    df['x'] = signal.medfilt(df['x'], 3)
    df['y'] = signal.medfilt(df['y'], 3)
    df['z'] = signal.medfilt(df['z'], 3)

    # Degital -> Voltage -> Voltage - offset Voltage / (vdd/5) = g
    df['x'] = (df['x'] * 5.0) / 1024 - 2.5
    df['y'] = (df['y'] * 5.0) / 1024 - 2.5
    df['z'] = (df['z'] * 5.0) / 1024 - 2.5

    # Combined acceleration
    df['comAcc'] = np.sqrt(df['x']*df['x'] + df['y']*df['y'] + df['z']*df['z'])


    print(df.head())
    print(df.dtypes)

    # get unique date name list
    uDateList = df['date'].sort_values().unique()

    # get data useing unique date name list
    uDateSize = uDateList.size

    #for uDateList in uDateList:
    #    sumD += len(df[df['date'] == uDateList])

    print(len(df.index))
    print(uDateSize)
    print(len(df.index)/uDateSize)

    # sampling frequency
    f = math.ceil(len(df.index)/uDateSize)

    # make base save file path

    # fft
    fft1d(np.array(df['comAcc'][20000:22048]), f, 'file')
    #fft1d(np.array(df.iloc[[i for i in df.index if i%2==0]]['x'])[0:1024], f/2, 'file')
    #fft1d(np.array(df['y'])[0:2048], f, 'file')
    #fft1d(np.array(df['z']), f, 'file')

if __name__ =='__main__':
    __main()
