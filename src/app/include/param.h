#ifndef _PARAM_H_
#define _PARAM_H_

#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <boost/algorithm/string.hpp>


class DeepSortParam
{
    public:
        DeepSortParam() { ; }
        void read(const std::string& filename);
        const std::string detections() const
        {
        return detection_file;
        }

        const std::string images() const
        {
        return image_directory;
        }

        const float dt() const
        {
        return dt_value;
        }

        const std::string metric_model() const
        {
        return metric_model_file;
        }

        const std::string feature_model() const
        {
        return feature_model_file;
        }

        const float args_nn_budget() const
        {
            return args_nn_budget_value;
        }

        const float args_max_cosine_distance() const
        {
            return args_max_cosine_distance_value;
        }
        
        const float max_iou_distance() const
        {
            return max_iou_distance_value;
        }

        const int max_age() const
        {
            return max_age_value;
        }

        const int n_init() const
        {
            return n_init_value;
        }

        const std::vector<std::string> classes() const
        {
            return detection_classes;
        }

        const bool show_detections() const
        {
            return show_detection_value;
        }

        DeepSortParam& operator=(const DeepSortParam& _param)
        {
            this->detection_file = _param.detections();
            this->image_directory = _param.images();
            this->metric_model_file = _param.metric_model();
            this->feature_model_file = _param.feature_model();
            this->args_nn_budget_value = _param.args_nn_budget();
            this->args_max_cosine_distance_value = _param.args_max_cosine_distance();
            this->dt_value = _param.dt();
            this->max_iou_distance_value = _param.max_iou_distance();
            this->max_age_value = _param.max_age();
            this->n_init_value = _param.n_init();
            this->show_detection_value = _param.show_detections();
            return *this;
        }

        const void print() const
        {
            std::cout << "[DETECTION_FILE]: " << detection_file << std::endl;
            std::cout << "[IMAGE_DIRECTORY]: " << image_directory << std::endl;
            std::cout << "[METRIC_MODEL]: " << metric_model_file << std::endl;
            std::cout << "[FEATURE_MODEL]: " << feature_model_file << std::endl;
            std::cout << "[ARGS_NN_BUDGET]: " << args_nn_budget_value << std::endl;
            std::cout << "[ARGS_MAX_COSINE_DISTANCE]: " << args_max_cosine_distance_value << std::endl;
            std::cout << "[DT]: " << dt_value << std::endl;
            std::cout << "[MAX_IOU_DISTANCE]: " << max_iou_distance_value << std::endl;
            std::cout << "[MAX_AGE]: " << max_age_value << std::endl;
            std::cout << "[N_INIT]: "  << n_init_value << std::endl;
            if(detection_classes.size() != 0)
            {
                std::cout << "[CLASSES]: ";
                for(const auto& c : detection_classes)
                {
                    std::cout << c << ", ";
                }
                std::cout << "total=" << detection_classes.size() << " classes" << std::endl;
            }
            std::cout << "[SHOW_DETECTIONS]: "  << show_detection_value << std::endl;
        }

    private:
        std::string detection_file;
        std::string image_directory;
        std::string metric_model_file;
        std::string feature_model_file;
        float args_nn_budget_value;
        float args_max_cosine_distance_value;
        float dt_value;
        float max_iou_distance_value;
        int max_age_value;
        int n_init_value;
        bool show_detection_value;
        std::vector<std::string> detection_classes;
};


#endif