import os 
n = 1
for filename in os.listdir('.'):
    if filename.endswith(".JPG"):
	os.rename(filename, "%d.jpg" %(n))
	n += 1
