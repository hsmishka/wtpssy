#include <opencv2/opencv.hpp>
#include <opencv2/video/background_segm.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "ServoCtrl.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include "time.h"

using namespace cv;
using namespace std;

const string WINDOW_DIFF = "Diff";
const string WINDOW_REFINED = "Ref";

const string VIDEO_OUT_PATH = "capture.avi";

bool saving_video = false;

VideoCapture  inputVideo;
VideoWriter   outputVideo; // Open the output
ServoController servoController("/dev/servoblaster");

const int capture_width = 640,
          capture_height = 480;

Scalar crosshair_color(0, 0, 200);

struct timespec ts;

void initInput() {
    inputVideo.open(0);
    sleep(1);

    if (!inputVideo.isOpened())
    {
        printf("\nCan not open camera or video file\n");
        exit(-1);
    }
}

void initOutput() {
    //capture_width = (int)inputVideo.get(CV_CAP_PROP_FRAME_WIDTH);   // Acquire input size
    //capture_height = (int)inputVideo.get(CV_CAP_PROP_FRAME_HEIGHT);

    Size S = Size(
        capture_width,    // Acquire input size
        capture_height);

    int ex = -1;     // Ask for Codec Type

    outputVideo.open(VIDEO_OUT_PATH, ex, inputVideo.get(CV_CAP_PROP_FPS), S, true);
    if (!outputVideo.isOpened())
    {
        cout << "Could not open the output video for write: " << VIDEO_OUT_PATH << endl;
        return exit(-1);
    }
}

long lastTime = 0;

void timemsg(string msg) {
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts);
  long msec = ts.tv_nsec / 1000000;
  cout << msg << " at " << msec - lastTime << endl;
  lastTime = msec;
}

int main(int argc, char** argv)
{
/*    servoController.setPosition(0,0);
    sleep(1);
    servoController.setPosition(1,1);
    sleep(1);
    servoController.setPosition(0.5,0.5);
*/

    initInput();
    //initOutput();

    initModule_video();
    setUseOptimized(true);
    setNumThreads(8);

    namedWindow("FG Segmentation", WINDOW_NORMAL);

    Ptr<BackgroundSubtractorGMG> fgbg = Algorithm::create<BackgroundSubtractorGMG>("BackgroundSubtractor.GMG");
    if (fgbg.empty())
    {
        std::cerr << "Failed to create BackgroundSubtractor.GMG Algorithm." << std::endl;
        return -1;
    }

    fgbg->set("initializationFrames", 10);
    fgbg->set("decisionThreshold", 0.9);

    Mat frame, fgmask, segm, erosion_dst, dilation_dst, convex_hull;
/*
    int erosion_type = MORPH_RECT;
    int erosion_size = 8;
    Mat element = getStructuringElement(erosion_type,
        Size(2 * erosion_size + 1, 2 * erosion_size + 1),
        Point(erosion_size, erosion_size));

    RNG rng(12345);
    Scalar rect_color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
*/
    for (;;)
    {
        static int fn = 0;
        timemsg("capturing frame");
        inputVideo >> frame;
        timemsg("captured frame");

        if (frame.empty())
            break;
        fn++;
        cout << "frame #" << fn << endl;

        (*fgbg)(frame, fgmask);
        timemsg("(*fgbg)(frame, fgmask);");

        frame.copyTo(segm);

//        erode(fgmask, erosion_dst, element);
//        dilate(erosion_dst, dilation_dst, element);

        add(frame, Scalar(0, 100, 100), segm, erosion_dst);

        timemsg("done erode-dilate");

        int min_x = capture_width, min_y = capture_height, max_x = 0, max_y = 0;
        for (int i = 0; i < capture_width; i++) {
            for (int j = 0; j < capture_height; j++) {
                if (dilation_dst.data[j * capture_width + i] > 0) {
                    min_x = min(min_x, i);
                    min_y = min(min_y, j);
                    max_x = max(max_x, i);
                    max_y = max(max_y, j);
                }
            }
        }

        cout << "min_x:" << min_x << " min_y:" << min_y << " max_x:" << max_x << " max_y" << max_y << endl;

        int radius = max(max_x - min_x, max_y - min_y) / 2; ;
        Point2f center(min_x + radius, min_y + radius);
        timemsg("got radius");
        if (radius > 0) {
          circle(segm, center, radius, crosshair_color, 3, 8, 0);
          circle(segm, center, 3, crosshair_color, 3, 8, 0);
          line(segm, Point2f(min_x, min_y + radius), Point2f(min_x + radius * 2, min_y + radius), crosshair_color, 1, 8, 0);
          line(segm, Point2f(min_x + radius, min_y), Point2f(min_x + radius, min_y + radius * 2), crosshair_color, 1, 8, 0);
          servoController.setPosition(((float)center.x) / capture_width, ((float)center.y) / capture_height);
        }

        timemsg("before imgshow");
        imshow("FG Segmentation", segm);
        timemsg("after imgshow");
        cout << "gotFrame" << endl;
        int key = waitKey(20); // was 20 !!!!!!!!!!!
        if (key == 27) {
            break;
        }
    }

    return 0;
}
