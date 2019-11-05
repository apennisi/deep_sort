#ifndef _VIDEOTRACKER_H_
#define _VIDEOTRACKER_H_

#include <string>
#include <future>
#include "model.h"
#include "param.h"

/**
 * VideoTracker
 *
 * run:
 * -vpath: the video's path.
 * -showSwitch: whether show the tracking result.
 *
 * run_sequenceWithGT:
 * -motDir: the path of MOT directory.
 * -showSwitch: show or not.
 *
 * run_sequence:
 * -motDir: the path of MOT directory.
 * -showSwitch: show or not.
 */
class VideoTracker 
{
	public:
		VideoTracker(const DeepSortParam& tracker_params);
		bool run();
		std::string showErrMsg();

	private:
		DeepSortParam params;
		std::string errorMsg;
		std::shared_ptr<ModelDetection> modelDetection;
};


#endif /* VIDEOTRACKER_H_ */

