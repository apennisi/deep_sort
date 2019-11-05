
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <future>
#include <tensorflow/core/public/session.h>
#include <tensorflow/core/protobuf/meta_graph.pb.h>

#include "utility.h"


typedef unsigned char uint8;

class FeatureTensor
{
public:
	// FeatureTensor();
	FeatureTensor(const std::string& model_meta, const std::string& model_data);
	FeatureTensor(const FeatureTensor&);
	FeatureTensor& operator = (const FeatureTensor&);
	bool getRectsFeature(const cv::Mat& img, DETECTIONS& d);
	~FeatureTensor();

private:
	bool init();

	void tobuffer(const std::vector<cv::Mat> &imgs, uint8 *buf);

	int feature_dim;
	std::shared_ptr<tensorflow::Session> session;
	std::vector<tensorflow::Tensor> output_tensors;
	std::vector<tensorflow::string> outnames;
	tensorflow::string input_layer;
	std::string tf_model_meta;
	std::string tf_model_data;
};
