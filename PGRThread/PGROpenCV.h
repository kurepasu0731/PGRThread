#ifndef PGROPENCV_H
#define PGROPENCV_H


#define PGR_PARAMETER_FILE "./parameter.ini"

#pragma once

#include <FlyCapture2.h>
#include <opencv2\opencv.hpp>
#include <cstdio>
#include <iostream>
#include "Timer.h"

#include <boost/thread/thread.hpp>

//#include "criticalSection.h"

class criticalSection
{
public:
	criticalSection()
	{
		//image = boost::shared_ptr<cv::Mat>(new cv::Mat);
	}

	~criticalSection(){};

	inline cv::Mat getImage()
	{
		boost::shared_lock<boost::shared_mutex> read_lock(image_mutex);
		return image;
	}

	inline void setImage(const cv::Mat &img)
	{
		boost::upgrade_lock<boost::shared_mutex> up_lock(image_mutex);
		boost::upgrade_to_unique_lock<boost::shared_mutex> write_lock(up_lock);
		image = img.clone();
	}


private:
	boost::shared_mutex image_mutex;

	cv::Mat image;

};
class TPGROpenCV
{
private:
	FlyCapture2::Error			fc2Error;
	FlyCapture2::BusManager		fc2BusMgr;
	FlyCapture2::PGRGuid		fc2Guid;
	FlyCapture2::CameraInfo		fc2CamInfo;
	FlyCapture2::Camera			fc2Cam;
	FlyCapture2::Property		fc2Prop;
	FlyCapture2::PixelFormat	fc2PixelFormat;
	FlyCapture2::Image			fc2Image;

	FlyCapture2::ColorProcessingAlgorithm fc2CPA;
	unsigned int				numCameras;
	unsigned int				useCamIndex;

	float						ShutterSpeed;
	float						Gain;
	float						Gamma;
	float						Brightness;
	float						Framerate;
	unsigned int				Wb_Red;
	unsigned int				Wb_Blue;
	cv::Mat	fc2Mat;
	//boost::shared_ptr<unsigned char>	fc2Mat_data;

	float						delay;
	
	void loadParameters();

public:
	TPGROpenCV(int _useCameraIndex = 0);
	~TPGROpenCV();
	int init( FlyCapture2::PixelFormat _format = FlyCapture2::PIXEL_FORMAT_BGR, int ColorProcessingAlgorithm = FlyCapture2::ColorProcessingAlgorithm::HQ_LINEAR);
	void PrintBuildInfo();
	void PrintError( FlyCapture2::Error error );
	void PrintCameraInfo( FlyCapture2::CameraInfo* pCamInfo );
	void InitCameraParameter();
	int PixelFormatInOpenCV();
	int start();
	int queryFrame();
	int stop();
	int release();
	std::string windowNameCamera;

	// パラメータをセットするメソッド
	void setShutterSpeed(float shutterSpeed);
	void setGain(float gain);
	void setWhiteBalance(int r, int b);
	void setPixelFormat( FlyCapture2::PixelFormat format );
	void setColorProcessingAlgorithm( FlyCapture2::ColorProcessingAlgorithm algorithm );
	void setGamma(float gamma);
	void setBrightness(float brightness);
	void setFrameRate(float framerate);

	// パラメータを取得するメソッド
	float getShutterSpeed();
	float getGain();
	void getWhiteBalance(int &r, int &b);
	void showCapImg(cv::Mat cap = cv::Mat());	//撮影画像を表示
	void CameraCapture(cv::Mat &image);			// 撮影画像をMatで取得

	cv::Mat getVideo(){ return fc2Mat; };

	Timer tm;

protected:
	boost::thread thread;
	mutable boost::mutex mutex;

	void threadFunction();

	bool quit;
	bool running;

	//! スレッド間の共有クラス
	boost::shared_ptr<criticalSection> critical_section;


};

#endif