#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

const int MAT_RESOLUTION_HEIGHT = 800;
const int MAT_RESOLUTION_WIDTH = 1280;

Mat CreateLineMat(vector<Point2f> start_point, vector<Point2f>end_point)
{
	int line_num = start_point.size();

	// Find the vanish point from line_1 and line_2
	Point2f vanish_point;
	float y1_1 = start_point[0].y;
	float x1_1 = start_point[0].x;
	float y2_1 = end_point[0].y;
	float x2_1 = end_point[0].x;
	float k_1 = (y2_1 - y1_1) / (x2_1 - x1_1);
	float b_1 = y1_1 - k_1 * x1_1;
	float y1_2 = start_point[1].y;
	float x1_2 = start_point[1].x;
	float y2_2 = end_point[1].y;
	float x2_2 = end_point[1].x;
	float k_2 = (y2_2 - y1_2) / (x2_2 - x1_2);
	float b_2 = y1_2 - k_2 * x1_2;
	vanish_point.x = -(b_2 - b_1) / (k_2 - k_1);
	vanish_point.y = k_1 * vanish_point.x + b_1;

	// Draw lines in the picture
	Mat line_mat;
	line_mat.create(Size(MAT_RESOLUTION_WIDTH, MAT_RESOLUTION_HEIGHT), CV_8UC3);
	line_mat.setTo(0);
	for (int idx = 0; idx < line_num; idx++)
	{
		line(line_mat, start_point[idx], vanish_point, CV_RGB(255, 0, 0), 5);
	}

	// Return result
	return line_mat;
}

int LocateCarPosition(vector<Point2f> world_start_point, vector<Point2f> world_end_point)
{
	int line_num = world_start_point.size();

	// Find line position
	int car_position = 0;
	for (int idx = 0; idx < line_num; idx++)
	{
		float x1 = world_start_point[idx].x;
		float y1 = world_start_point[idx].y;
		float x2 = world_end_point[idx].x;
		float y2 = world_end_point[idx].y;
		if (x1 == x2)
		{
			if (x1 > 0)
			{
				break;
			}
		}
		else
		{
			float k = (y2 - y1) / (x2 - x1);
			float b = y1 - k * x1;
			if (-b / k > 0)
				break;
		}
		car_position += 1;
	}

	return car_position;
}

Mat CreateSafeMat(vector<Point2f> start_point, 
	vector<Point2f> end_point, 
	vector<Point2f> car_location,
	vector<float> car_size,
	int car_position)
{
	// Set draw line
	int line_num = start_point.size();
	int car_left_num = car_position - 1;
	int car_right_num = car_position + 1;

	// Calculate lines function
	vector<float> line_k;
	vector<float> line_b;
	for (int idx = 0; idx < line_num; idx++)
	{
		float x1 = start_point[idx].x;
		float y1 = start_point[idx].y;
		float x2 = end_point[idx].x;
		float y2 = end_point[idx].y;
		float k = (y2 - y1) / (x2 - x1);
		float b = y1 - k * x1;
		line_k.push_back(k);
		line_b.push_back(b);
	}

	// Find the vanish point from line_1 and line_2
	Point2f vanish_point;
	vanish_point.x = -(line_b[1] - line_b[0]) / (line_k[1] - line_k[0]);
	vanish_point.y = line_k[0] * vanish_point.x + line_b[0];

	// Create safe mat
	Mat safe_mat;
	safe_mat.create(Size(MAT_RESOLUTION_WIDTH, MAT_RESOLUTION_HEIGHT), CV_8UC3);

	// Draw left
	if (car_left_num >= 0)
	{
		// Get safe valuation
		Scalar safe_color = CV_RGB(255, 128, 128);

		int start_h = vanish_point.y;
		int end_h = MAT_RESOLUTION_HEIGHT;
		for (int h = start_h; h < end_h; h++)
		{
			int start_w = 0;
			int end_w = 0;
			if (car_left_num > 0)
			{
				float b1 = line_b[car_left_num - 1];
				float k1 = line_k[car_left_num - 1];
				start_w = (h - b1) / k1;
			}
			float b2 = line_b[car_left_num];
			float k2 = line_k[car_left_num];
			end_w = (h - b2) / k2;
			line(safe_mat, Point2f(start_w, h), Point2f(end_w, h), safe_color);
		}
	}

	// Draw right
	if (car_right_num <= line_num)
	{
		// Get safe valuation
		Scalar safe_color = CV_RGB(128, 255, 128);

		int start_h = vanish_point.y;
		int end_h = MAT_RESOLUTION_HEIGHT;
		for (int h = start_h; h < end_h; h++)
		{
			int start_w = 0;
			int end_w = 0;
			float b1 = line_b[car_right_num - 1];
			float k1 = line_k[car_right_num - 1];
			start_w = (h - b1) / k1;
			if (car_right_num < line_num)
			{
				float b2 = line_b[car_right_num];
				float k2 = line_k[car_right_num];
				end_w = (h - b2) / k2;
			}
			line(safe_mat, Point2f(start_w, h), Point2f(end_w, h), safe_color);
		}
	}

	return safe_mat;
}

Mat CreateDirectionMat(int direction_cmd)
{
	Mat direction_mat;
	direction_mat.create(Size(MAT_RESOLUTION_WIDTH, MAT_RESOLUTION_HEIGHT), CV_8UC3);

	return direction_mat;
}

int main()
{
	// Input Background Image
	Mat background_mat;
	background_mat = imread("Background.png");

	// Input line list
	vector<Point2f> world_start_point;
	vector<Point2f> world_end_point;
	vector<Point2f> start_point;	// start from the edge
	vector<Point2f> end_point;
	start_point.push_back(Point2f(200.0, 800.0));
	end_point.push_back(Point2f(640.0, 512.0));
	start_point.push_back(Point2f(500.0, 800.0));
	end_point.push_back(Point2f(640.0, 512.0));
	start_point.push_back(Point2f(700.0, 800.0));
	end_point.push_back(Point2f(640.0, 512.0));
	start_point.push_back(Point2f(1000.0, 800.0));
	end_point.push_back(Point2f(640.0, 512.0));

	world_start_point.push_back(Point2f(-1.0, -1.0));
	world_end_point.push_back(Point2f(-1.0, 1.0));
	world_start_point.push_back(Point2f(-3.0, -1.0));
	world_end_point.push_back(Point2f(-3.0, 1.0));
	world_start_point.push_back(Point2f(1.0, -1.0));
	world_end_point.push_back(Point2f(1.0, 1.0));
	world_start_point.push_back(Point2f(3.0, -1.0));
	world_end_point.push_back(Point2f(3.0, 1.0));
	
	// Input car place
	vector<Point2f> car_location;
	vector<float> car_size;

	// Input direction command
	int direction_cmd;
	direction_cmd = 1; // 0,1,2

	// Create line mat
	Mat line_mat;
	line_mat = CreateLineMat(start_point, end_point);

	// Get car position
	int car_position;
	car_position = LocateCarPosition(world_start_point, world_end_point);

	// Create safe mat
	Mat safe_mat;
	safe_mat = CreateSafeMat(start_point, end_point, car_location, car_size, car_position);
	namedWindow("Test");
	imshow("Test", safe_mat);
	waitKey(0);

	// Create direction mat
	Mat direction_mat;
	direction_mat = CreateDirectionMat(direction_cmd);

	return 0;

}