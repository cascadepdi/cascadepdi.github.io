import cv2
gray = cv2.imread("coke-resize.jpg",0)
gray = cv2.resize(gray,(125,40))
cv2.imwrite("coke-gray.jpg",gray)
