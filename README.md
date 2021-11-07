# ASCII_me
Webcam->ASCII image command-line application, written in C++ with the OpenCV library

# Details
Includes ability to accept user-inputs at runtime to scale the ASCII image in the terminal, cycle through 4 filters (small ASCII map, large ASCII map, edge-detection, colorized) and scale exposure setting

# Demo
![Alt Text](https://github.com/sunnyjackson/ASCII_me/blob/master/ASCII_me_demo.gif)

# System prerequisites
Webcam, Linux terminal, and OpenCV

# Notes
- /examples subdirectory contains simple OpenCV examples for verifying proper installation of the OpenCV tool
- /filter subdirectory contains a C application that applies filters to .bmp files, written as a problem set for Harvard's CS50 course. I originally intended to do this project in C and use the grayscale and sobel edge-detection code from /filter, but I chose OpenCV for grabbing webcam frames and OpenCV already provides grayscale and sobel functions so this /filter directory is actually unnecessary. Keeping it in though, in case I come back later to rewrite this project without the OpenCV dependency
