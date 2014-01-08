#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>
#include <time.h> // to calculate time needed
#include <limits.h> // to get INT_MAX, to protect against overflow


int main(int argc, char *argv[]) {
    CvCapture* capture = cvCaptureFromCAM( 1 );
    if (!capture){
        fprintf(stderr, "ERROR: Capture is null\nPress any key to exit\n");
        getchar();
        return -1;
    }

    // fps counter begin
    time_t start, end;
    int counter = 0;
    double sec;
    double fps;
    // fps counter end

    cvNamedWindow("Original", CV_WINDOW_AUTOSIZE);

    while(1){
        // fps counter begin
        if (counter == 0){
            time(&start);
        }
        // fps counter end

        IplImage* origImage = cvQueryFrame(capture);
        cvShowImage("Original", origImage);

        // do your stuff here

        // fps counter begin
        time(&end);
        counter++;
        sec = difftime(end, start);
        fps = counter/sec;
        if (counter > 30)
            printf("%.2f fps\n", fps);
        // overflow protection
        if (counter == (INT_MAX - 1000))
            counter = 0;
        // fps counter end

        // will exit when ESC button is pressed
        if ( (cvWaitKey(10) & 255) == 27 ) break;
    }

    cvReleaseCapture(&capture);
    cvDestroyWindow("Original");
    return 0;
}
