#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed May 30 15:11:05 2018

@author: robotica
"""
import os
import cv2 

f = open("bg.txt","w")
for x in os.listdir("Negatives"):
    f.write("Negatives/"+x+"\n")
f.close()
