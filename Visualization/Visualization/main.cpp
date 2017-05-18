//#include <al.h>
//#include <alc.h>
//#include <iostream>
//#include <string>
//#include <fstream>
//#include <cstdio>
//#include <Windows.h>
#include "VisualModel.h"

using namespace std;

int endWithError(string msg, int error_code = 0)
{
	cout << msg << endl;
	system("PAUSE");
	return error_code;
}

int main()
{
	VisualModel vm;
	
	// frame_mat input:
	Mat background = imread("./background.png");

	// video_name:
	string video_name = "./test.avi";
	string wav_name = "";

	namedWindow("Test");
	for (int idx = 0; idx < 500; idx++)
	{
		Point2f point_LU = Point2f(200.0 + idx % 100, 100.0 + idx % 100);
		Point2f point_LD = Point2f(200.0 - idx % 100, 900.0 + idx % 100);
		Point2f point_RD = Point2f(1100.0 + idx % 100, 900.0 - idx % 100);
		Point2f point_RU = Point2f(1100.0 - idx % 100, 100 - idx % 100);
		vector<Point2f> vertex_list;
		vertex_list.push_back(point_LU);
		vertex_list.push_back(point_LD);
		vertex_list.push_back(point_RD);
		vertex_list.push_back(point_RU);
		vm.InputDataFile(true,
			background,
			vertex_list,
			video_name);
		Mat tmp;
		tmp = vm.GetOutputMat();
		imshow("Test", tmp);
		waitKey(10);
	}
	destroyAllWindows();

	// Read wave file
	//FILE *fp = NULL;
	//fp = fopen("./music.wav", "rb");
	//char type[5];
	//type[4] = '\0';
	//DWORD size, chunkSize;
	//short formatType, channels;
	//DWORD sampleRate, avgBytesPerSec;
	//short bytesPerSample, bitsPerSample;
	//DWORD dataSize;
	//fread(type, sizeof(char), 4, fp);
	//if (string(type) != "RIFF") {
	//	return endWithError("No RIFF");
	//}
	//fread(&size, sizeof(DWORD), 1, fp);
	//fread(type, sizeof(char), 4, fp);
	//if (string(type) != "WAVE") {
	//	return endWithError("Not WAVE");
	//}
	//fread(type, sizeof(char), 4, fp);
	//if (string(type) != "fmt ") {
	//	return endWithError("not fmt ");
	//}
	//fread(&chunkSize, sizeof(DWORD), 1, fp);
	//fread(&formatType, sizeof(short), 1, fp);
	//fread(&channels, sizeof(short), 1, fp);
	//fread(&sampleRate, sizeof(DWORD), 1, fp);
	//fread(&avgBytesPerSec, sizeof(DWORD), 1, fp);
	//fread(&bytesPerSample, sizeof(short), 1, fp);
	//fread(&bitsPerSample, sizeof(short), 1, fp);
	//fread(type, sizeof(char), 4, fp);
	//if (string(type) != "data") {
	//	return endWithError("Missing Data");
	//}
	//fread(&dataSize, sizeof(DWORD), 1, fp);
	//unsigned char * music_buffer = new unsigned char[dataSize];
	//fread(music_buffer, sizeof(BYTE), dataSize, fp);
	//fclose(fp);

	//// Initialize openal
	//ALCdevice *device;
	//ALCcontext *context;
	//device = alcOpenDevice(NULL);
	//if (!device) {
	//	return endWithError("No sound device found");
	//}
	//context = alcCreateContext(device, NULL);
	//alcMakeContextCurrent(context);
	//if (!context) {
	//	return endWithError("No sound context");
	//}

	//// generate source
	//ALuint source;
	//ALuint buffer;
	//ALuint frequency = sampleRate;
	//ALenum format = 0;
	//alGenBuffers(1, &buffer);
	//alGenSources(1, &source);
	//if (bitsPerSample == 8) {
	//	if (channels == 1) {
	//		format = AL_FORMAT_MONO8;
	//	}
	//	else if (channels == 2) {
	//		format = AL_FORMAT_STEREO8;
	//	}
	//}
	//else if (bitsPerSample == 16) {
	//	if (channels == 1) {
	//		format = AL_FORMAT_MONO16;
	//	}
	//	else if (channels = 2) {
	//		format = AL_FORMAT_STEREO16;
	//	}
	//}



	return 0;
}