#include "imgproc.cuh"

int main(int argc, char** argv)
{
	/*
	
	CvCapture* capture = cvCaptureFromCAM(CV_CAP_ANY);
	if (!capture) {
		fprintf(stderr, "ERROR: capture is NULL \n");
		getchar();
		return -1;
	}
	// Create a window in which the captured images will be presented
	cvNamedWindow("mywindow", CV_WINDOW_AUTOSIZE);
	// Show the image captured from the camera in the window and repeat
	while (1) {
		// Get one frame
		IplImage* frame = cvQueryFrame(capture);
		if (!frame) {
			fprintf(stderr, "ERROR: frame is null...\n");
			getchar();
			break;
		}
		cvShowImage("mywindow", frame);
		// Do not release the frame!

		if ((cvWaitKey(10) & 255) == 's') {
			CvSize size = cvGetSize(frame);
			IplImage* img = cvCreateImage(size, IPL_DEPTH_16S, 1);
			img = frame;
			cvSaveImage("my.png", &img);
		}
		if (cvWaitKey(10) < 0)
			cvSaveImage("my.png", frame);
	}
	// Release the capture device housekeeping
	cvReleaseCapture(&capture);
	cvDestroyWindow("mywindow");
	*/

	namedWindow("src", WINDOW_AUTOSIZE);
	namedWindow("src", WINDOW_AUTOSIZE);
	string filename = "lena_rgb.png";
	Mat src, dst;
	src = imread(filename, CV_LOAD_IMAGE_GRAYSCALE);
	src.convertTo(src, CV_8UC1);
	clock_t start, stop;
	start = clock();
	medianGPU(src, dst);
	imwrite("lena_median.jpg", dst);

	stop = clock();
	cout << "All cost time is " << static_cast<double>(stop - start) * 1000 / CLOCKS_PER_SEC << " ms" << endl;
	dst = dst.rowRange(1, dst.rows - 1).colRange(1, dst.cols - 1);

	imshow("src", src);
	imshow("dst", dst);
	waitKey(0);
	return 0;
}
