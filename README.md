Project Name: UAS IMage Processing Unit (IMP)
============================================
![Alt txt](https://github.com/Rainerino/UAS_IMP/blob/master/Team%20picture.JPG "Optional Title")
## Introduction

This Program is a prototype for the image processing unit of University of British Columbia Unmanned Aircraft System design team's Ground Control Station [GCOM private](https://github.com/ubcuas/GCOM) to face the object recongization chanllenge of [2017 Unmanned Systems Canada(USC) student Competition](https://www.unmannedsystems.ca/home/students/student-competition-details/)

### Overview of GCOM

This is a repository for UBC Unmanned Aircraft Systems' ground control station software. The software will act as a form of master server and has the following features:

* Control the ground antenna tracker
* Download and tag images off of the drone
* Send/receive data and commands to and from the drone
* Stitch incoming images into orthphotos
* Perform detection algorithms on images off of drone
* Perform analytics on retrieved images
* Keep a reliable/recoverable communication with the drone


To keep the program as sreamlined and optimized as possible, the software uses a modular design, with a solid and well-documented framework to hold everything together. This allows for the deployment of numerous versions of GCOM, one for each of our drones, where each version will have different modules enabled.

## Design

### Overview

IMP used [OpenCV](http://opencv.org/) as the machine learning core to process images such as below, and return the number of gooses on the images, with its location relative to the image(the absolute coordinate of the gooses on the image). You can have some fun by looking for goooses on this image. There are 6 of them, 3 different types.
![Alt txt](https://github.com/Rainerino/UAS_IMP/blob/master/Testing%20images.jpg "Optional Title")

### Process flow

