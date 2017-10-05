Project Name: [UAS](http://www.ubcuas.com/) IMage Processing Unit (IMP)
============================================
![Alt txt](https://github.com/Rainerino/UAS_IMP/blob/master/Team%20picture.JPG "Optional Title")
## Introduction

> This Program is a prototype for the image processing unit of University of British Columbia Unmanned Aircraft System design team ([UAS](http://www.ubcuas.com/))'s Ground Control Station [GCOM private](https://github.com/ubcuas/GCOM) to face the object recongization chanllenge of [2017 Unmanned Systems Canada(USC) student Competition](https://www.unmannedsystems.ca/home/students/student-competition-details/)

### Overview of GCOM

> This is a repository for UBC Unmanned Aircraft Systems' ground control station software. The software will act as a form of master server and has the following features:

> * Control the ground antenna tracker
> * Download and tag images off of the drone
> * Send/receive data and commands to and from the drone
> * Stitch incoming images into orthphotos
> * Perform detection algorithms on images off of drone
> * Perform analytics on retrieved images
> * Keep a reliable/recoverable communication with the drone


> To keep the program as sreamlined and optimized as possible, the software uses a modular design, with a solid and well-documented framework to hold everything together. This allows for the deployment of numerous versions of GCOM, one for each of our drones, where each version will have different modules enabled.

## Design and Implementation

### Overview

IMP used [OpenCV](http://opencv.org/) as the machine learning core to process images such as below, and return the number of gooses on the images, with its location relative to the image(the absolute coordinate of the gooses on the image). You can have some fun by looking for goooses on this image. There are 6 of them, 3 different types.
![Alt txt](https://github.com/Rainerino/UAS_IMP/blob/master/Testing%20images.jpg "Optional Title")

### Process flow

1. Fetch the images
2. Crop the image to bits of (504*378) sized images
3. Filter with the most optimal filter, chosen out of(medianBlur, equalizeHist, GaussianBlur).

`GaussianBlur(src, src_gray, Size(7, 7), 0, 0);`

4. We assume that the noise filter out small features such as grass, leaves. Now run a color estimation algorithm.

`approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );`

5. Fetch all the contours found, and filter the ones that are smaller than our hypothetical size of the goose. We made this estimation based on the height we are going to fly, the size of the actual goose and the FOV of camera. 
6. Run pattern recognition with [Haar Cascade](http://docs.opencv.org/trunk/d7/d8b/tutorial_py_face_detection.html) library in OpenCV, which works well with objects with simple but distinct features(We are trying to find the shape of Goose, insteaad of focusing on feather pattern, because the aerial photo will not be good enough to include enough features for the pattern of feathers.) We trained the cascade with our training image
![Alt txt](https://github.com/Rainerino/UAS_IMP/blob/master/trained%20image/geese_0_5050.jpg "Optional Title")
7. Post process the data. 


### Implementation and Testing

 We were able to achieve about 70-80% recognition rate with our testing images. Here are some of our testing samples and result :

1. 

![Alt txt](https://github.com/Rainerino/UAS_IMP/blob/master/Close_up_with_Snow.jpg "Optional Title")

2/3 found and recognized

---

2. 

![Alt txt](https://github.com/Rainerino/UAS_IMP/blob/master/Uniform_background.jpg "Optional Title")

6/6 foudn and recognized

---


3.

![Alt txt](https://github.com/Rainerino/UAS_IMP/blob/master/Crazy_difficulty.jpg "Optional Title")

1/3 found, but also hundreds more found!

---

## Conclusion

The algorithm was integrated into GCOM with a GUI and location calculation algorithm(which gets the GPS coordinate and attitude of the images and calcuate, return the GPS coordinaate of gooses). 

Here is one of the aerial images we took that has gooses on it:

![Alt txt](https://github.com/Rainerino/UAS_IMP/blob/master/Aerial_Goose.JPG "Optional Title")



