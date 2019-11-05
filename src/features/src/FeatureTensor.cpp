#include "FeatureTensor.h"

using namespace tensorflow;

FeatureTensor::FeatureTensor(const std::string& model_meta, const std::string& model_data) 
	: tf_model_meta(model_meta), tf_model_data(model_data)
{
	//prepare model:
	bool status = init();
	if(status == false) exit(1);
}

FeatureTensor::~FeatureTensor() 
{
	session->Close();
	output_tensors.clear();
	outnames.clear();
}

bool FeatureTensor::init() {
	
	tensorflow::SessionOptions sessOptions;
	sessOptions.config.mutable_gpu_options()->set_allow_growth(true);
	session = std::shared_ptr<tensorflow::Session>(NewSession(sessOptions));
	if(session == nullptr) return false;

	// const tensorflow::string pathToGraph = TENSORFLOW_MODEL_META;
	const tensorflow::string pathToGraph = tf_model_meta;
	Status status;
	MetaGraphDef graph_def;
	status = ReadBinaryProto(tensorflow::Env::Default(), pathToGraph, &graph_def);
	if(status.ok() == false) return false;

	status = session->Create(graph_def.graph_def());
	if(status.ok() == false) return false;

	// const tensorflow::string checkpointPath = TENSORFLOW_MODEL;
	const tensorflow::string checkpointPath = tf_model_data;
	Tensor checkpointTensor(DT_STRING, TensorShape());
	checkpointTensor.scalar<std::string>()() = checkpointPath;
	status = session->Run(
			{ {graph_def.saver_def().filename_tensor_name(), checkpointTensor}, },
			{}, {graph_def.saver_def().restore_op_name()}, nullptr );
	if(status.ok() == false) return false;

	input_layer = "Placeholder:0";
	outnames.push_back("truediv:0");
	feature_dim = 128;
	return true;
}

bool FeatureTensor::getRectsFeature(const cv::Mat& img, DETECTIONS& d) 
{
	std::vector<cv::Mat> mats;
	for(auto& dbox : d) 
	{
		auto rc = cv::Rect(int(dbox.tlwh(0)), int(dbox.tlwh(1)),
				int(dbox.tlwh(2)), int(dbox.tlwh(3)));

		if(rc.x < 0)
        {
            rc.width -= rc.x;
            rc.x = 0;
        }
        
        if(rc.y < 0)
        {
            rc.height -= rc.y;
            rc.y = 0;
        }
        
        if(rc.width + rc.x >=  img.cols)
        {
            rc.width -= (rc.width + rc.x -  img.cols);
        }
        
        if(rc.height + rc.y >= img.rows)
        {
             rc.height -= (rc.height + rc.y - img.rows);
        }

		cv::Mat mattmp = img(rc).clone();
		cv::resize(mattmp, mattmp, cv::Size(64, 128));
		mats.push_back(mattmp);
	}

	int count = mats.size();

	Tensor input_tensor(DT_UINT8, TensorShape({count, 128, 64, 3}));
	tobuffer(mats, input_tensor.flat<uint8>().data());
	std::vector<std::pair<tensorflow::string, Tensor>> feed_dict = {
			{input_layer, input_tensor},
	};
	Status status = session->Run(feed_dict, outnames, {}, &output_tensors);
	if(status.ok() == false) return false;
	float* tensor_buffer = output_tensors[0].flat<float>().data();
	int i = 0;
	
	for(auto& dbox : d) 
	{
		for(int j = 0; j < feature_dim; j++)
		{
			dbox.feature[j] = tensor_buffer[i*feature_dim+j];
		}
		i++;
	}
	return true;
}

void FeatureTensor::tobuffer(const std::vector<cv::Mat> &imgs, uint8 *buf) 
{
	int pos = 0;
	for(const cv::Mat& img : imgs) 
	{
		int Lenth = img.rows * img.cols * 3;
		int nr = img.rows;
		int nc = img.cols;
		if(img.isContinuous()) 
		{
			nr = 1;
			nc = Lenth;
		}
		for(int i = 0; i < nr; i++) 
		{
			const uchar* inData = img.ptr<uchar>(i);
			for(int j = 0; j < nc; j++) 
			{
				buf[pos] = *inData++;
				pos++;
			}
		}//end for
	}//end imgs;
}
