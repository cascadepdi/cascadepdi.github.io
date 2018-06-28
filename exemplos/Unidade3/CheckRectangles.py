#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon May 28 14:18:31 2018

@author: robotica
"""
import cv2

file = open("info3/info.dat","r")
for x in range(1,10):
    line = file.readline()
    vector = line.split(" ")
    img = cv2.imread("info3/"+vector[0])
    pos = 2
    vector[-1] = vector[-1].replace("\n", "")
    vector[1:] = list(map(int, vector[1:]))
    
    for i in range (0,vector[1]):
        cv2.rectangle(img,(vector[pos],vector[pos+1]),(vector[pos]+vector[pos+2],vector[pos+1]+vector[pos+3]),(255,0,0),2)
        pos = pos + 4
    
    cv2.imwrite("CheckPositives/"+vector[0], img)
    