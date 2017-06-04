#!/usr/bin/env python
#! -*- coding: utf-8 -*-

import matplotlib.pyplot as plt
import numpy as np

def __main():
    args = sys.argv

    if len(args) != 3:
        print('usage: python graph.py [readFilePath] [saveFilePath]')
        sys.exit()

    filePath = args[1]
    savePath = args[2]

    # open window and make 3graph
    fig, (ax1, ax2, ax3) = plt.subplots(3)

    # declear line class ,initialize graph and line
    ax1Line, = ax1.plot(ax1X, ax1Y)
    ax2Line, = ax2.plot(ax2X, ax2Y)
    ax3Line, = ax3.plot(ax3X, ax3Y)



if __name__ =='__main__':
    __main()
