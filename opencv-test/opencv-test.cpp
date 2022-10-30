#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <opencv2/features2d.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <string>
#include <windows.h>



using namespace cv;
using namespace std;

int findLargestArea(std::vector<std::vector<cv::Point>> contours){
	double largest = 0;
	int ind = 0;
	for (int i = 0; i < contours.size(); i++) {
		if (cv::contourArea(contours[i]) > largest) {
			ind = i;
			largest = cv::contourArea(contours[i]);
		}
	}
	return ind;
}

int main()
{
	VideoCapture cap(1);
	Mat frame;

	int count = 0;
	int count2 = 0;
	bool blinked = false;

	while (1) {
		cap >> frame;

		if (frame.empty()) {
			break;
		}

		cv::Mat threshed;

		cvtColor(frame, frame, cv::COLOR_RGB2GRAY);

		GaussianBlur(frame, frame, Size(17, 17), 0);

		threshold(frame, threshed, 20, 255, THRESH_BINARY_INV);

		vector<vector<Point>> contours;
		vector<Vec4i> hierarchy;
		findContours(threshed, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
		// draw contours on the original image
		Mat image_copy = frame.clone();
		stringstream text;

		int x1 = 0;
		int y1 = 0;

		namedWindow("Contour: ", WINDOW_NORMAL);

		int x = GetSystemMetrics(SM_CXSCREEN);
		int y = GetSystemMetrics(SM_CYSCREEN);

		if (!contours.empty()) {
			drawContours(image_copy, contours, findLargestArea(contours), Scalar(255, 0, 0), 3);

			cv::Moments M = cv::moments(contours[findLargestArea(contours)]);
			cv::Point center(M.m10 / M.m00, M.m01 / M.m00);

			circle(image_copy, center, 25, (255, 255, 255), -1);

			cv::Point origin(0, 10);

			x1 = center.x;
			y1 = center.y;

			text << "x: " << center.x << "y: " << center.y;

			putText(image_copy, text.str(), origin, FONT_HERSHEY_PLAIN, 1, (0, 255, 0), 1);

			SetCursorPos(center.x*3, center.y*3);

			count = 0;
		}

		count += 1;
		count2 += 1;

		if (count > 20 && blinked == false && count2 > 10) {
			
			mouse_event(MOUSEEVENTF_LEFTDOWN, x1, y1, 0, 0);
			mouse_event(MOUSEEVENTF_LEFTUP, x1, y1, 0, 0);
			blinked = true;
			count2 = 0;

		}


		imshow("Contour: ", image_copy);
		resizeWindow("Contour: ", 600, 600);
		

		char c = (char)waitKey(25);

		if (c == 27) {
			break;
		}
		waitKey(50);
	}
	return 0;
}