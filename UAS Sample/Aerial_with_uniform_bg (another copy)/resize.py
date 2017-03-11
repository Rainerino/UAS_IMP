
import os
import cv2
path = '/home/yiyi/Documents/CLionProjects/UAS/UAS Sample/Aerial_with_uniform_bg (another copy)'
for file_type in[path]:
	for img in os.listdir(file_type):
            if img.endswith('.jpg'):
                image = cv2.imread(img)
                resize = cv2.resize(image, (1000, 1000))
                cv2.imwrite(img, resize)
