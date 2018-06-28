#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed May 30 21:09:00 2018

@author: robotica
"""

import cv2
import matplotlib.pyplot as plt

coke = cv2.CascadeClassifier("cascade.xml")
cont = 0
img = cv2.imread("test2.jpg")
gray = cv2.imread("test2.jpg",0)
cokes = coke.detectMultiScale(gray, 1.05, 3)
for (x,y,w,h) in cokes:
    cv2.rectangle(img,(x,y),(x+w,y+h),(255,0,0),2)
plt.imshow(img)
cv2.imwrite("demo3.jpg",img)

#for x in os.listdir("negtest"):
#    #gray = cv2.imread("test.jpg",0)
#    gray = cv2.imread("negtest/"+x,0)
#    cokes = coke.detectMultiScale(gray, 1.05, 3)
#    for (x,y,w,h) in cokes:
#        cont = cont+1
#        cv2.rectangle(gray,(x,y),(x+w,y+h),(255,0,0),2)
#        plt.imshow(gray)
#        plt.pause(1)
    
    #cv2.imwrite("teste.jpg",img)
    #plt.imshow(gray)
    
    
