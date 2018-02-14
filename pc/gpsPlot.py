#!/usr/bin/env python
#! -*- coding: utf-8 -*-

import sys
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from mpl_toolkits.basemap import Basemap
from pyproj import Geod
from PIL import Image

def calcDistance(lon1, lat1, lon2, lat2):
    g = Geod(ellps='WGS84')
    azimuth, back_azimuth, distance = g.inv(lon1, lat1, lon2, lat2)
    return distance


def calcLocation(baseLon, baseLat, az, dist):
    g = Geod(ellps='WGS84')
    lon, lat, back_azimuth = g.fwd(baseLon, baseLat, az, dist)
    return lon, lat

def calcCircle(baseLon, baseLat, dist):
    lonC = []
    latC = []

    for x in range(3601):
        lon, lat = calcLocation(baseLon, baseLat, x*0.1, dist)
        lonC.append(lon)
        latC.append(lat)

    return lonC, latC

def __main():
    args = sys.argv

    if len(args) != 2:
        print('usage: python gpsPlot.py inputPath]')
        sys.exit()

    filePath = args[1]

    df = pd.read_csv(filePath, header=None, parse_dates=[3])
    df.columns = ['lat', 'lon', 'label', 'date']

    # base location
    gatewayLon = 140.715
    gatewayLat = 41.772222


    print(df.head())
    #print(df.dtypes)

    #print(df[df['label']==1]['lat'].max())
    #print(df[df['label']==1]['lon'].max())

    # distance circle
    lon500, lat500 = calcCircle(gatewayLon, gatewayLat, 500)
    lon1000, lat1000 = calcCircle(gatewayLon, gatewayLat, 1000)
    lon1500, lat1500 = calcCircle(gatewayLon, gatewayLat, 1500)
    lon2000, lat2000 = calcCircle(gatewayLon, gatewayLat, 2000)


    # calc the distance to the Gateway
    suc250 = fail250 = 0
    suc500 = fail500 = 0
    suc750 = fail750 = 0
    suc1000 = fail1000 = 0
    suc1250 = fail1250 = 0
    suc1500 = fail1500 = 0
    suc1750 = fail1750 = 0
    suc2000 = fail2000 = 0

    for (sucLon, sucLat) in zip (df[df['label']==1]['lon'], df[df['label']==1]['lat']):
        dist = calcDistance(gatewayLon, gatewayLat, sucLon, sucLat)
        #print((str)(sucLat) + (',') + (str)(sucLon))
        #print(dist)
        if dist <= 250:
            suc250 += 1
        elif dist <= 500:
            suc500 += 1
        elif dist <= 750:
            suc750 += 1
        elif dist <= 1000:
            suc1000 += 1
        elif dist <= 1250:
            suc1250 += 1
        elif dist <= 1500:
            suc1500 += 1
        elif dist <= 1750:
            suc1750 += 1
        elif dist <= 2000:
            suc2000 += 1

    for (failLon, failLat) in zip (df[df['label']==0]['lon'], df[df['label']==0]['lat']):
        dist = calcDistance(gatewayLon, gatewayLat, failLon, failLat)
        if dist <= 250:
            fail250 += 1
        elif dist <= 500:
            fail500 += 1
        elif dist <= 750:
            fail750 += 1
        elif dist <= 1000:
            fail1000 += 1
        elif dist <= 1250:
            fail1250 += 1
        elif dist <= 1500:
            fail1500 += 1
        elif dist <= 1750:
            fail1750 += 1
        elif dist <= 2000:
            fail2000 += 1

    print('250m:' + str(suc250) + '/'+ str(suc250 + fail250))
    print('500m:' + str(suc500) + '/'+ str(suc500 + fail500))
    print('750m:' + str(suc750) + '/'+ str(suc750 + fail750))
    print('1000m:' + str(suc1000) + '/'+ str(suc1000 + fail1000))
    print('1250m:' + str(suc1250) + '/'+ str(suc1250 + fail1250))
    print('1500m:' + str(suc1500) + '/'+ str(suc1500 + fail1500))
    print('1750m:' + str(suc1750) + '/'+ str(suc1750 + fail1750))
    print('2000m:' + str(suc2000) + '/'+ str(suc2000 + fail2000))


    # draw graph
    fig = plt.figure(figsize=(20,10))
    maxLon = round(df['lon'].max() + 0.01, 3)
    minLon = round(df['lon'].min() - 0.01, 3)
    maxLat = round(df['lat'].max() + 0.01, 3)
    minLat = round(df['lat'].min() - 0.01, 3)

    print(maxLon)
    print(maxLat)
    print(minLon)
    print(minLat)

    m = Basemap(projection='merc',
                llcrnrlat=minLat,
                urcrnrlat=maxLat,
                llcrnrlon=minLon,
                urcrnrlon=maxLon,
                lat_ts=0,
                resolution='l'
                )

    file_name = '/Users/yoshiki-t/Desktop/map.png'
    bg_img = Image.open(file_name)
    m.imshow(bg_img, origin='upper')

    '''
    m.drawcoastlines(color='lightgray')
    m.drawcountries(color='lightgray')
    m.fillcontinents(color='white', lake_color='#eeeeee');
    m.drawmapboundary(fill_color='#eeeeee')

    '''
    x1, y1 = m(np.array(df[df['label']==1]['lon']),np.array( df[df['label']==1]['lat']))
    x2, y2 = m(np.array(df[df['label']==0]['lon']),np.array( df[df['label']==0]['lat']))
    x3, y3 = m(gatewayLon, gatewayLat)
    x4, y4 = m(lon500, lat500)
    x5, y5 = m(lon1000, lat1000)
    x6, y6 = m(lon1500, lat1500)
    x7, y7 = m(lon2000, lat2000)
    m.plot(x1, y1, color='yellow', marker ='.', linestyle='None')
    m.plot(x2, y2, color='black', marker ='.', linestyle='None')
    m.plot(x3, y3, color='red', marker ='.', markersize=18)
    m.plot(x4, y4, color='black')
    m.plot(x5, y5, color='black')
    m.plot(x6, y6, color='black')
    m.plot(x7, y7, color='black')
    plt.show()

if __name__ =='__main__':
    __main()
