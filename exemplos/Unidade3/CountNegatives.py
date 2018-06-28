import os
import cv2 

f = open("bg.txt","w")
for x in os.listdir("Negatives"):
    gray = cv2.imread("Negatives/"+x,0)
    gray = cv2.resize(gray,(500,160))
    cv2.imwrite("Negatives/"+x,gray)
    f.write("Negatives/"+x+"\n")
f.close()
