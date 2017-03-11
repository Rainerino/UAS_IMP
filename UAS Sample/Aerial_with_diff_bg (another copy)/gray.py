import cv2
import os
filename = ''
img = cv2.imread(filename, cv2.IMREAD_GRAYSCALE)
resize = cv2.resize(img, (50, 50))
cv2.imwrite('Geese_sample_5050.jpg', resize)
