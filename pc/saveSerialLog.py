#!/usr/bin/env python
#! -*- coding: utf-8 -*-

import serial
import serial.tools.list_ports
import time
import subprocess
import sys
import re
from operator import attrgetter

def __main():

    args = sys.argv

    if len(args) != 2:
        print('usage: python devPath')
        sys.exit()

    # wait arduino bootup
    time.sleep(3.0)

    # open Serial
    ser = serial.Serial(
        port = args[1],
        baudrate = 9600,
        bytesize = serial.EIGHTBITS,
        parity = serial.PARITY_EVEN,
        stopbits = serial.STOPBITS_ONE,
        timeout = 1
    )


    while True:

        # wait data
        while(ser.inWaiting() < 1):
            time.sleep(0.1)

        #line = ser.readline().strip().decode('utf-8')
        line = ser.readline()
        print(line)

        subprocess.call('date', shell=True)

    ser.close()


if __name__ =='__main__':
    __main()

