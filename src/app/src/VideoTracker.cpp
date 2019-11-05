/*
 * VideoTracker.cpp
 *
 *  Created on: Dec 15, 2017
 *      Author: zy
 */
#include "VideoTracker.h"
#include "opencv2/opencv.hpp"
#include "tracker.h"
#include "FeatureTensor.h"

using namespace cv;
using namespace std;

VideoTracker::VideoTracker(const DeepSortParam& tracker_params) : params(tracker_params)
{
	modelDetection = std::shared_ptr<ModelDetection>(new ModelDetection(params.detections(), params.images()));
}



bool VideoTracker::run()
{
	tracker mytracker(params);
	std::shared_ptr<FeatureTensor> featureTensor(new FeatureTensor(params.metric_model(), params.feature_model()));
	
	while(true) 
	{
		Mat frame;

		DETECTIONS detections;
		if(modelDetection->getFrameDetections(frame, detections) == false) 
		{
			this->errorMsg = "No more frames!";
			return false;
		}
		
		//modelDetection->dataMoreConf(args_min_confidence, detections);
		//modelDetection->dataPreprocessing(args_nms_max_overlap, detections);
		
		//TENSORFLOW get rect's feature.
		if(featureTensor->getRectsFeature(frame, detections) == false) 
		{
			this->errorMsg = "Tensorflow get feature failed!";
			return false;
		}

		mytracker.predict();
		mytracker.update(detections);
		std::vector<RESULT_DATA> result;
		for(Track& track : mytracker.tracks) 
		{
			if(!track.is_confirmed() || track.time_since_update > 1) continue;
			result.push_back(std::make_pair(std::make_pair(track.track_id, track.detection_class), std::make_pair(track.to_tlwh(), track.color)));
		}

		if(params.show_detections())
		{
			for(unsigned int k = 0; k < detections.size(); k++) 
			{
				DETECTBOX tmpbox = detections[k].tlwh;
				Rect rect(tmpbox(0), tmpbox(1), tmpbox(2), tmpbox(3));
				rectangle(frame, rect, Scalar(0,0,255), 4);
			}
		}

		std::stringstream ss;
		for(unsigned int k = 0; k < result.size(); k++) 
		{
			DETECTBOX tmp = result[k].second.first;
			std::string det_class = result[k].first.second;
			cv::Scalar color = result[k].second.second;
			Rect rect = Rect(tmp(0), tmp(1), tmp(2), tmp(3));
			rectangle(frame, rect, color, 2);
			ss << result[k].first.first << " - " << det_class;
			putText(frame, ss.str(), Point(rect.x, rect.y), CV_FONT_HERSHEY_SIMPLEX, 0.8, color, 2);
			ss.str("");
		}
		imshow("DeepSortTracking", frame);
		waitKey(1);
	}//end while;
	return true;
}

std::string VideoTracker::showErrMsg() 
{
	return this->errorMsg;
}