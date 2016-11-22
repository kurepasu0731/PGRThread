#include <boost/thread/thread.hpp>
#include <opencv2\opencv.hpp>


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
		image = img;
	}


private:
	boost::shared_mutex image_mutex;

	cv::Mat image;

};