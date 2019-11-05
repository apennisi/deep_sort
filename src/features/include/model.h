#ifndef MODEL_H
#define MODEL_H
#include "dataType.h"
#include "utility.h"
#include <map>
#include <opencv2/opencv.hpp>

/**
 * Get each image's rects & corresponding features.
 * Method of filter conf.
 * Method of preprocessing.
 */
class ModelDetection
{

    public:
        ModelDetection() { ; }
        ModelDetection(const std::string& txt_filename, const std::string& img_path);
        ModelDetection(const ModelDetection&);
        ModelDetection& operator =(const ModelDetection&);
        bool getFrameDetections(int frame_idx, DETECTIONS& res);
        bool getFrameDetections(cv::Mat& frame, DETECTIONS& res);
        void dataMoreConf(float min_confidence, DETECTIONS& d);
        void dataPreprocessing(float max_bbox_overlap, DETECTIONS& d);

    private:
        void ReadFileContent(const std::string &file, std::string &content);
        void ReadRcFileTotal(const std::string &file);

    private:
        using AREAPAIR = std::pair<int, double>;
        struct cmp {
            bool operator()(const AREAPAIR a, const AREAPAIR b) 
            {
                return a.second < b.second;
            }
        };

    private:
        
        std::map<int, DETECTIONS> data;
        std::vector<std::string> names;
        void _Qsort(DETECTIONS d, std::vector<int>& a, int low, int high);
        bool loadFromFile;
        std::string imgFolder;
        int maxImgNum;
        int counter = 0;
};

#endif // MODEL_H
