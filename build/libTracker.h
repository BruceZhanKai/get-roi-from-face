
#ifndef _LIB_TRACKER_H_
#define _LIB_TRACKER_H_

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <memory>

struct Information
{
    cv::Mat FeartureImage;
    cv::Mat DisplayImage;
    int MultiFace;
    cv::Rect_<double> RectNow;
};

class FacialTracker
{
public:
    FacialTracker();
    ~FacialTracker();
    bool Initial(int argc, char **argv);
    Information Update(const cv::Mat& Image);

private:
    class TrackerClass;
    std::auto_ptr<TrackerClass> trackerpClass;
};

#endif // _LIB_TRACKER_H_
