#include <opencv2\opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

Mat img2hash(Mat orgImg)
{
	Mat Hash;
	resize(orgImg, Hash, Size(16, 16));

	Mat gray(Hash.rows, Hash.cols, CV_8UC1);
	for (int i = 1; i < Hash.rows; i++) {
		for (int j = 1; j < Hash.cols; j++) {
			gray.at<uchar>(i, j) = (Hash.at<Vec3b>(i, j)[0] + Hash.at<Vec3b>(i, j)[1] + Hash.at<Vec3b>(i, j)[2]) / 3;
		}
	}
	
	Scalar avg = mean(gray);

	Mat averageHash(gray.rows, gray.cols, CV_8UC1);
	for (int i = 0; i < gray.rows; i++) {
		for (int j = 0; j < gray.cols; j++) {
			if (gray.at<uchar>(i, j) > avg[0]) {
				averageHash.at<uchar>(i, j) = 1;
			}
			else {
				averageHash.at<uchar>(i, j) = 0;
			}
		}
	}
	return averageHash;
}


int hamming_distance(Mat orgHash, Mat imgHash) {
	int distance = 0;
	for (int i = 0; i < orgHash.rows; i++) {
		for (int j = 0; j < orgHash.cols; j++) {
			if (orgHash.at<uchar>(i, j) != imgHash.at<uchar>(i, j)) {
				distance += 1;
			}
		}
	}
	return distance;
}


int main()
{
	Mat orgImg = imread("C:/Users/juzero/Desktop/OrgImg/grape.jpg");
	imshow("Input image", orgImg);
	moveWindow("Input image", 1000, 0);
	waitKey(2000);

	Mat orgHash = img2hash(orgImg);
	cout << orgHash << endl;

	vector<string> path;
	glob("C:/Users/juzero/Desktop/Img/*.jpg", path, false);

	vector<Mat> Img;

	size_t count = path.size();
	for (size_t i = 0; i < count; i++) {
		Img.push_back(imread(path[i]));
		imshow(". . . Searching . . .", Img[i]);
		moveWindow(". . . Searching . . .", 1000, 300);
		waitKey(500);

		Mat imgHash = img2hash(Img[i]);

		Mat Color;
		resize(Img[i], Color, Size(16, 16));
		
		int distance = hamming_distance(orgHash, imgHash);
		if (float(distance) / 256 < 0.25) {
			cout << path[i] << "->" << float(distance) / 256 << endl;
			cout << imgHash << endl;
			cout << float(distance) / 256 << endl;
			imshow(path[i], Img[i]);
			moveWindow(path[i], 0, 0);

			for (int i = 0; i < imgHash.rows; i++) {
				for (int j = 0; j < imgHash.cols; j++) {
					if (imgHash.at<uchar>(i, j) == 0) {
						Color.at<Vec3b>(i, j)[0] = 255;
						Color.at<Vec3b>(i, j)[1] = 0;
						Color.at<Vec3b>(i, j)[2] = 255;
					}
					else {
						Color.at<Vec3b>(i, j)[0] = 255;
						Color.at<Vec3b>(i, j)[1] = 255;
						Color.at<Vec3b>(i, j)[2] = 255;
					}
				}
			}

			Mat big;
			resize(Color, big, Size(400, 400));

			imshow(". . . Searching Grape . . .", big);
			moveWindow(". . . Searching Grape . . .", 1500, 300);
			waitKey(500);
		}
	}
	waitKey(0);
	return 0;
}