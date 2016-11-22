#include <boost/thread/thread.hpp>
#include <opencv2\opencv.hpp>


class criticalSection
{
public:
	criticalSection()
	{
		image = boost::shared_ptr<cv::Mat>(new cv::Mat);
	}

	~criticalSection(){};

	inline boost::shared_ptr<cv::Mat> getImage()
	{
		boost::shared_lock<boost::shared_mutex> read_lock(image_mutex);
		return image;
	}

	inline void setImage(const boost::shared_ptr<cv::Mat> &img)
	{
		boost::upgrade_lock<boost::shared_mutex> up_lock(image_mutex);
		boost::upgrade_to_unique_lock<boost::shared_mutex> write_lock(up_lock);
		*image = img->clone();
	}


private:
	boost::shared_mutex image_mutex;

	boost::shared_ptr<cv::Mat> image;

};