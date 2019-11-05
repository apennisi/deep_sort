#ifndef _UTILS_
#define _UTILS_

#include <opencv2/opencv.hpp>
#include "dataType.h"

class DETECTION_ROW 
{
    public:
        DETECTBOX tlwh; //np.float
        float confidence; //float
        int class_num;
        cv::Scalar color;
        FEATURE feature; //np.float32
        DETECTBOX to_xyah() const;
        DETECTBOX to_tlbr() const;
};

typedef std::vector<DETECTION_ROW> DETECTIONS;

#endif