#include "VisualModel.h"



void VisualModel::GenerateVideoMat(vector<Point2f> vertex_list)
{
	// Get Perspective Change Matrix
	Size video_frame_size = this->video_frame_set_[0].size();
	vector<Point2f> video_vertex_list;
	video_vertex_list.push_back(Point2f(0, 0));
	video_vertex_list.push_back(Point2f(0, video_frame_size.height));
	video_vertex_list.push_back(Point2f(video_frame_size.width, video_frame_size.height));
	video_vertex_list.push_back(Point2f(video_frame_size.width, 0));
	Mat transform_mat = getPerspectiveTransform(vertex_list, video_vertex_list);

	// Get Bounding box of points
	Size frame_size = this->frame_mat_.size();
	int min_x_idx = frame_size.width;
	int max_x_idx = 0;
	int min_y_idx = frame_size.height;
	int max_y_idx = 0;
	for (int i = 0; i < 4; i++) {
		if (vertex_list[i].x < min_x_idx) {
			min_x_idx = vertex_list[i].x;
		}
		if (vertex_list[i].x > max_x_idx) {
			max_x_idx = vertex_list[i].x;
		}
		if (vertex_list[i].y < min_y_idx) {
			min_y_idx = vertex_list[i].y;
		}
		if (vertex_list[i].y > max_y_idx) {
			max_y_idx = vertex_list[i].y;
		}
	}

	// Draw images
	this->output_mat_.create(frame_size, CV_8UC3);
	this->output_mat_.setTo(0);
	vector<Point2f> points_frame;
	vector<Point2f> points_video;
	for (int h = min_y_idx; h <= max_y_idx; h++) {
		for (int w = min_x_idx; w <= max_x_idx; w++) {
			points_frame.push_back(Point2f(w, h));
		}
	}
	perspectiveTransform(points_frame, points_video, transform_mat);
	for (int i = 0; i < points_video.size(); i++) {
		int h_v = points_video[i].y;
		int w_v = points_video[i].x;
		if ((h_v >= 0) && (h_v < video_frame_size.height) 
			&& (w_v >= 0) && (w_v < video_frame_size.width)) {
			this->output_mat_.at<Vec3b>(points_frame[i].y, points_frame[i].x)
				= this->video_frame_set_[this->frame_stamp_].at<Vec3b>(h_v, w_v);
		}
	}

	// time_stamp++;
	this->frame_stamp_ += 1;
	if (this->frame_stamp_ >= this->max_frame_num_) {
		this->frame_stamp_ = 0;
	}
}

void VisualModel::LoadAviVideo(string video_name)
{
	if (this->video_frame_set_ != NULL)	{
		delete[]video_frame_set_;
		this->video_frame_set_ = NULL;
	}

	this->video_name_ = video_name;
	VideoCapture capture(this->video_name_);
	this->frame_stamp_ = 0;
	this->max_frame_num_ = static_cast<long>(capture.get(CV_CAP_PROP_FRAME_COUNT));
	this->max_frame_num_ -= 1;
	this->video_frame_set_ = new Mat[this->max_frame_num_];
	for (int i = 0; i < this->max_frame_num_; i++) {
		capture >> this->video_frame_set_[i];
	}
	capture.release();
}

VisualModel::VisualModel()
{
	this->frame_valid_flag_ = true;
	this->frame_stamp_ = 0;
	this->max_frame_num_ = 0;
	this->video_frame_set_ = NULL;
}

VisualModel::~VisualModel()
{
	if (this->video_frame_set_ != NULL)
	{
		delete[]video_frame_set_;
		this->video_frame_set_ = NULL;
	}
}

// InputDataFile:
// Input the parameters for visualization.
// Parameters:
//     frame_valid_flag - set false if the pattern is undetected.
//     frame_mat - image that the camera captured
//     vertex_list - a 4 point2f vector. The 4 vertex should be anti-clockwise, from the left-up point.
//                   left-up, left-down, right-down, right-up
//                   Coordinate: Point2f.x for width, Point2f.y for height.
//     video_name - string. The file name of the avi file.
//     wav_name - not used.
int VisualModel::InputDataFile(bool frame_valid_flag,
	Mat frame_mat,
	vector<Point2f> vertex_list,
	string video_name,
	string wav_name)
{
	if (frame_valid_flag) {
		frame_mat.copyTo(this->frame_mat_);

		// Check open the video or not
		if (this->video_name_ != video_name) {
			this->LoadAviVideo(video_name);
		}

		this->wav_name_ = wav_name;
		this->GenerateVideoMat(vertex_list);
	}
	return 0;
}

// GetOutputMat:
// Get a mat which has the same size of camera image.
// CV_8UC3. 0 is the background.
Mat VisualModel::GetOutputMat()
{
	return this->output_mat_;
}
