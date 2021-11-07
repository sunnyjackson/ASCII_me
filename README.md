# ASCII_me
Webcam &#8594; ASCII image command-line application, written in C++ with [OpenCV](https://opencv.org/)

Includes the ability to accept user-inputs at runtime to scale the ASCII image in the terminal, tune the exposure setting, and cycle through 4 available filters.

# Details
How do ASCII images work? Basically, we pull an image frame from our webcam, reduce it to a manageable size, convert to grayscale, and then map to a particular ASCII character based on grayscale brightness (thanks to [Paul Borke](http://paulbourke.net/dataformats/asciiart/) for the maps).

### Filters
1) **Small ASCII Map**: Map grayscale pixel intensity &#8594; " .:-=+*#%@" (arranged from dimmest to brightest)
2) **Large ASCII Map**: Map grayscale pixel intensity &#8594; " .'`^,:;Il!i><~+_-?][}{1)(|\\/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$"
3) **Edge-Detection**: Map [sobel pixel intensity](https://en.wikipedia.org/wiki/Sobel_operator) &#8594; " .:-=+*#%@"
3) **Colorized**: Map grayscale pixel intensity &#8594; " .:-=+*#%@", and assign RGB color to that character using the "\e\[38;2;\<r\>;\<g\>;\<b\>m" [ANSI escape code](https://en.wikipedia.org/wiki/ANSI_escape_code)

Note that the rescaling step sqaushes the y-dimension of the image frame. We do this because ASCII characters are taller than they are wide, and therefore the ASCII character will stretch the y-dimension of the image frame.
  
# Demo
![Alt Text](https://github.com/sunnyjackson/ASCII_me/blob/master/ASCII_me_demo.gif)

# System prerequisites
Webcam, Linux terminal, and OpenCV

# Notes
- /examples subdirectory contains simple OpenCV examples for verifying proper installation of the OpenCV tool
- /filter subdirectory contains a C application that applies filters to .bmp files, written as a problem set for Harvard's CS50 course. I originally intended to do this project in C and use the grayscale and sobel edge-detection code from /filter, but I chose OpenCV for grabbing webcam frames and OpenCV already provides grayscale and sobel functions so this /filter directory is actually unnecessary. I'm keeping \filter in case I come back later to rewrite this project without the OpenCV dependency
