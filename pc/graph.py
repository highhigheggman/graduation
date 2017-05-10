#!/usr/bin/env python
#! -*- coding: utf-8 -*-

import matplotlib.pyplot as plt
import numpy as np

def __main():

    # open window and make 3graph
    fig, (ax1, ax2, ax3) = plt.subplots(3)

    # declear point
    ax1X, ax1Y = 0, 0
    ax2X, ax2Y = 0, 0
    ax3X, ax3Y = 0, 0

    # declear line class ,initialize graph and line
    ax1Line, = ax1.plot(ax1X, ax1Y)
    ax2Line, = ax2.plot(ax2X, ax2Y)
    ax3Line, = ax3.plot(ax3X, ax3Y)

    # realtime plot
    while True:
        # update points
        ax1X += 0.1
        ax1Y = 1
        ax2X += 0.1
        ax2Y = 1
        ax3X += 0.1
        ax3Y = 1

        # update line use poins
        ax1Line.set_data(ax1X, ax1Y)
        ax2Line.set_data(ax2X, ax2Y)
        ax3Line.set_data(ax3X, ax3Y)

        plt.pause(.01)


if __name__ =='__main__':
    __main()
