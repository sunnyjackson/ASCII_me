#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

/* TO DO
- Upload to github & setup a pages.github.com post for this project
    - Verify that we can pull it to my NUC, and then check performance, shooting for >30 Hz (if still slow, look to beat 30 Hz)

  NEAT IDEAS
- Port to an embedded device with a camera -> extend to Zoom-like function with two devices sharing video over wireless protocol

  NOTES
- Recommending setting terminal preferences to a black background, with font-size as small as possible, and terminal screen as large as possible
- User-inputs are only registered if the "MyVideo" window showing the webcam feed is "active" (i.e. you must click on it for inputs to be registered)
*/

enum Filter{
    Small, Big, EdgeDet, Color
};
char ASCII_map_small[14] = " .,-~:;=!*#$@"; // from dimmest to brightest
char ASCII_map_big[70] = " .'`^,:;Il!i><~+_-?][}{1)(|\\/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$";

void DisplayASCIIImage(Mat image, char* ASCII_map, int mapsize, int exposure);

int main(int argc, char* argv[])
{
    // Establish variables controlling the ASCII image
    Filter f = Small;
    int rows = 45, // number of rows in ASCII_me output
        cols = 150, // number of columns in ASCII_me output
        exposure = 55, // 0-254. Higher values will saturate bright pixels, and provide better contrast in lower-light regions of the output (e.g. around your face)
        mapsize = sizeof(ASCII_map_small)-2;

    // Open the video camera no. 0 (webcam)
    VideoCapture cap(0); 
    if (!cap.isOpened()){
        cout << "Cannot open the video cam" << endl;
        return -1;
    }

    // Declare OpenCV variables
    namedWindow("MyVideo");
    Mat frame, small_frame, grayscale, sobelxy;

    // Prepare terminal
    printf("\e[2J \n"); // clear terminal screen
    printf("\e[38;2;0;255;0m"); // set text-color to green

    while (1)
    {
        // Read a new frame from video
        bool bSuccess = cap.read(frame);
        if (!bSuccess){
             cout << "Cannot read a frame from video stream" << endl;
             break;
        }
        resize(frame, small_frame, Size(cols, rows)); // reduce size
        imshow("MyVideo", small_frame);

        // Generate & Display ASCII Image
        switch (f){
            case Small:
                cvtColor(small_frame, grayscale, COLOR_RGB2GRAY);
                DisplayASCIIImage(grayscale, ASCII_map_small, mapsize, exposure);
                break;

            case Big:
                mapsize = sizeof(ASCII_map_big)-2;
                cvtColor(small_frame, grayscale, COLOR_RGB2GRAY);
                DisplayASCIIImage(grayscale, ASCII_map_big, mapsize, exposure);
                break;

            case EdgeDet:
                mapsize = sizeof(ASCII_map_small)-2;
                cvtColor(small_frame, grayscale, COLOR_RGB2GRAY);
                Sobel(grayscale, sobelxy, -1, 1, 1, 5);
                DisplayASCIIImage(sobelxy, ASCII_map_small, mapsize, exposure);
                break;

            case Color:
                mapsize = sizeof(ASCII_map_small)-2;
                cvtColor(small_frame, grayscale, COLOR_RGB2GRAY);
                for(int i = 0; i < small_frame.rows; i++){
                    for(int j = 0; j < small_frame.cols; j++){
                        int index = grayscale.at<uchar>(i,j)*mapsize/(255-exposure),
                            red = small_frame.at<Vec3b>(i,j)[0],
                            grn = small_frame.at<Vec3b>(i,j)[1],
                            blu = small_frame.at<Vec3b>(i,j)[2];
                            index = (index < mapsize) ? index : mapsize;
                        printf("\e[38;2;%i;%i;%im%c",red,grn,blu,ASCII_map_small[index]);
                    }
                    putchar('\n');
                }
                break;
        }

        // Optional user-controls
        printf("\e[38;2;0;255;0m"); // set color to green
        printf("\r     {-,=} to scale exposure (%i)      {[,]} to scale size (rows: %i, cols: %i)     [f] to cycle filter ()    [q] to quit", exposure, rows, cols);
        switch (waitKeyEx(10)){
            case 'q':
                printf("\e[2J \n"); // reset cursor
                return 0;
            case '-':
                exposure -= 10;
                exposure = (exposure>0) ? exposure : 0;
                break;
            case '=':
                exposure += 10;
                exposure = (exposure<254) ? exposure : 254;
                break;
            case '[':
                printf("\e[2J \n"); // reset cursor
                rows /= 1.1;
                cols /= 1.1;
                rows = (rows>1) ? rows : 1;
                cols = (cols>1) ? cols : 1;
                break;
            case ']':
                printf("\e[2J \n"); // reset cursor
                rows *= 1.1;
                cols *= 1.1;
                break;
            case 'f':
                switch (f){ // Small -> Big -> EdgeDet -> Color -> Small ...
                    case Small:
                        f = Big;
                        mapsize = sizeof(ASCII_map_big)-2;
                        break;
                    case Big:
                        f = EdgeDet;
                        mapsize = sizeof(ASCII_map_small)-2;
                        break;
                    case EdgeDet:
                        f = Color;
                        mapsize = sizeof(ASCII_map_small)-2;
                        break;
                    case Color:
                        f = Small;
                        mapsize = sizeof(ASCII_map_small)-2;
                        break;
                }
                break;
        }

        // Reset cursor to top of image
        printf("\e[%iF", rows); 
    }
}

void DisplayASCIIImage(Mat image, char* ASCII_map, int mapsize, int exposure){
    for(int i = 0; i < image.rows; i++){
        for(int j = 0; j < image.cols; j++){
            int index = image.at<uchar>(i,j)*mapsize/(255-exposure);
            index = (index < mapsize) ? index : mapsize;
            putchar(ASCII_map[index]);
        }
        putchar('\n');
    }

}