#ifndef _VISUALMODEL_H_
#define _VISUALMODEL_H_

#include <opencv2/opencv.hpp>
#include <vector>
#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")

using namespace cv;
using namespace std;

class VisualModel
{
private:
	bool frame_valid_flag_;
	Mat frame_mat_;
	vector<Point2f> vertex_list_;
	string video_name_;
	string wav_name_;

	Mat * video_frame_set_;
	Mat output_mat_;
	unsigned int frame_stamp_;
	long max_frame_num_;
	
	void GenerateVideoMat(vector<Point2f> vertex_list);
	void LoadAviVideo(string video_name);

public:
	VisualModel(string wav_name, string video_name);
	~VisualModel();

	int InputDataFile(bool frame_valid_flag,
		Mat frame_mat = Mat(),
		vector<Point2f> vertex_list = vector<Point2f>());
	Mat GetOutputMat();
};

#endif
