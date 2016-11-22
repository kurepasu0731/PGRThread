#include <opencv2\opencv.hpp>

#include <thread>
#include <mutex>
#include <memory>

class criticalSection
{
public:
	criticalSection()
	{
		image = std::shared_ptr<cv::Mat>(new cv::Mat);
	}

	~criticalSection(){};

	inline std::shared_ptr<cv::Mat> getImage()
	{
		std::shared_lock<std::mutex> read_lock(image_mutex);
	}


private:
	std::mutex image_mutex;

	std::shared_ptr<cv::Mat> image;

};