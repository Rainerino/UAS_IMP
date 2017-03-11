import urllib
import cv2
import numpy as np
import os

def store_raw_image():
    neg_images_link = 'http://image-net.org/api/text/imagenet.synset.geturls?wnid=n00523513'
    #the python 2 is diff from 3. just urllib.urlopen()
    neg_image_urls = urllib.urlopen(neg_images_link).read().decode()

    #this makes directory in the syste
    if not os.path.exists('neg'):
        os.makedirs('neg')

    pic_num = 1

    for i in neg_image_urls.split('\n'):
        try:
            print(i)
            urllib.urlretrieve(i, "neg/"+str(pic_num)+'.jpg')
            img = cv2.imread("neg/"+str(pic_num)+'.jpg', cv2.IMREAD_GRAYSCALE)
            resized_image = cv2.resize(img,(100,100))
            cv2.imwrite("neg/"+str(pic_num)+'.jpg', resized_image)
            pic_num += 1

        except Exception as e:
            print(str(e))

    #neg_image_link = 'http://image-net.org/api/text/imagenet.synset.geturls?wnid=n00523513'
    #neg_image_urls = urllib.urlopen(neg_images_link).read().decode()

def create_pos_n_neg():
    for file_type in ['neg']:
        for img in os.listdir(file_type):

            if file_type == 'pos':
                line = file_type+'/'+img+' 1 0 0 50 50\n'
                with open('info.dat','a') as f:
                    f.write(line)
            elif file_type == 'neg':
                line = file_type+'/'+img+'\n'
                with open('bg.txt','a') as f:
                    f.write(line)
                    
def find_white():
	for file_type in ['neg']:
		for img in os.listdir(file_type):
			for ugly in os.listdir('uglies'):
				try:
					current = str(file_type) + '/' + str(img)
					ugly = cv2.imread('uglies/' + str(ugly) )
					question = cv2.imread(current)
					if ugly.shape == question.shape and not(np.bitwise_xor(ugly, question).any()) :
						print("find one!")
						os.remove(current)
				except Exception as e:
					print(str(e))
#store_raw_image()
#create_pos_n_neg()
