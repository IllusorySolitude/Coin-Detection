#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <vector>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>

using namespace cv;
using namespace std;

int main() {

	//IMPORTING THE IMAGE//

	Mat image, img_gray;

	string path = "Resources/projetImage1.jpg"; // Medium Image
	//string path = "Resources/imput.jpg"; // Small Image
	//string path = "Resources/input.jpg"; // Large Image


	image = imread(path);
	resize(image, image, Size(500, 500), INTER_LINEAR);

	if (image.empty()) {
		return EXIT_FAILURE;
	}

	//----------------------------------------------------//

	//CONVERT TO GRAY IMAGE//

	cvtColor(image, img_gray, COLOR_BGR2GRAY);
	//imshow("Converted to Gray", img_gray);

	//----------------------------------------------------//

	//USING A MEDIAN FILTER//

	medianBlur(img_gray, img_gray, 5);
	//imshow("Applying Median Filter", img_gray);

	//----------------------------------------------------//

	//ERODE AND DILATE//

	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
	int iteration = 9;
	morphologyEx(img_gray, img_gray, MORPH_ERODE, element, Point(-1, -1), iteration);
	morphologyEx(img_gray, img_gray, MORPH_DILATE, element, Point(-1, -1), iteration);
	//imshow("After eroding and dilating", img_gray);
	
	//----------------------------------------------------//

	//NORMALIZE AND EQUALIZE//

	normalize(img_gray, img_gray, 0, 200, NORM_MINMAX);
	//equalizeHist(img_gray, img_gray);
	//imshow("After normalizing", img_gray);

	//----------------------------------------------------//

	//CANNY DETECTION//

	//Canny(img_gray, img_gray, 100, 200);
	imshow("Final image before Hough", img_gray);

	//----------------------------------------------------//

	//DRAWING HOUGH CIRCLES//

	vector<Vec3f> coins;
	HoughCircles(img_gray, coins, HOUGH_GRADIENT, 1, img_gray.rows / 16, 100, 30, 1, img_gray.rows / 4);

	int l = coins.size();

	cout << "\n There are " << l << " coins detected.\n" << endl;

	float TotalMoney = 0;
	float Maxradius = 0;
	float ratio;

	for (size_t i = 0; i < l; i++)
	{
		Vec3i c = coins[i];
		float radius = c[2];
		cout << "Radius " << i << " is " << c[2] << ".\n" << endl;
		if (Maxradius < radius) { Maxradius = radius; }

	}

	//----------------------------------------------------//

	//IDENTIFYING THE COIN VALUES AND THE TOTAL VALUE//

	for (size_t i = 0; i < l; i++)
	{
		Vec3i c = coins[i];
		Point center = Point(c[0], c[1]);
		
		float radius= c[2];
		ratio = radius / Maxradius;

		if (ratio > 0.6) { circle(image, center, 3, Scalar(0, 255, 0), -1, 8, 0); }

		if (ratio > 0.6) { circle(image, center, radius, Scalar(0, 0, 255), 3, 8, 0); }

		//imshow("Hough Circles", image);

		cout << "\n Ratio " << i << " is equal to " << ratio << ".\n" << endl;

		if (ratio < 0.6) { putText(image, " ", Point(center.x - radius, center.y + radius + 15), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 0, 0), 0.4, LINE_AA); }
		if (ratio > 0.6 && ratio <= 0.675) { putText(image, "1 Cent", Point(center.x - radius, center.y + radius + 15), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(50, 205, 50), 0.4, LINE_AA); TotalMoney += 0.01; }
		if (ratio > 0.675 && ratio <= 0.75) { putText(image, "2 Cents", Point(center.x - radius, center.y + radius + 15), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(50, 205, 50), 0.4, LINE_AA); TotalMoney += 0.02; }
		if (ratio > 0.8 && ratio <= 0.84) { putText(image, "5 Cents", Point(center.x - radius, center.y + radius + 15), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(50, 205, 50), 0.4, LINE_AA); TotalMoney += 0.05; }
		if (ratio > 0.75 && ratio <= 0.8) { putText(image, "10 Cents", Point(center.x - radius, center.y + radius + 15), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(50, 205, 50), 0.4, LINE_AA); TotalMoney += 0.1; }
		if (ratio > 0.84 && ratio <= 0.875) { putText(image, "20 Cents", Point(center.x - radius, center.y + radius + 15), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(50, 205, 50), 0.4, LINE_AA); TotalMoney += 0.2; }
		if (ratio > 0.925 && ratio <= 0.975) { putText(image, "50 Cents", Point(center.x - radius, center.y + radius + 15), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(50, 205, 50), 0.4, LINE_AA); TotalMoney += 0.5; }
		if (ratio > 0.875 && ratio <= 0.925) { putText(image, "1 Euro", Point(center.x - radius, center.y + radius + 15), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(50, 205, 50), 0.4, LINE_AA); TotalMoney += 1; }
		if (ratio > 0.975) { putText(image, "2 Euros", Point(center.x - radius, center.y + radius + 15), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(50, 205, 50), 0.4, LINE_AA); TotalMoney += 2; }
		if (ratio > 1.05) { putText(image, " ", Point(center.x - radius, center.y + radius + 15), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(50, 205, 50), 0.4, LINE_AA); }
	}

	putText(image, "Total value is " + to_string(TotalMoney) + " euros.", Point(image.cols / 10, image.rows / 10), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 0, 255), 0.6, LINE_AA);

	//----------------------------------------------------//

	imshow("Images depicting the value of each coin along with the total value", image);
	waitKey();

	return EXIT_SUCCESS;
}
