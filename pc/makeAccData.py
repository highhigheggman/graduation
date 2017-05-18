#!/usr/bin/env python
#! -*- coding: utf-8 -*-

import sys
import numpy as np

def __main():
    args = sys.argv

    if len(args) != 2:
        print('usage: python makeAccData.py [saveFilePath]')
        sys.exit()

    savePath = args[1]

    # make random accData
    accList = np.random.randint(0, 256, (600, 3))
    print(accList)

    # save file
    wf = open(savePath, 'w')
    wf.write(accList)
    wf.close()


if __name__ =='__main__':
    __main()
