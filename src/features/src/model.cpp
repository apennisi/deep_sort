#include "model.h"
#include <algorithm>


enum DETECTBOX_IDX {IDX_X = 0, IDX_Y, IDX_W, IDX_H };

static void splitStr(const std::string& inputStr, const std::string &key, std::vector<std::string>& outStrVec) {
	if (inputStr == "") {
		return;
	}
	int pos = inputStr.find(key);
	int oldpos = 0;
	if (pos > 0) {
		std::string tmp = inputStr.substr(0, pos);
		outStrVec.push_back(tmp);
	}
	while (1) 
	{
		if (pos < 0) 
		{
			break;
		}
		oldpos = pos;
		int newpos = inputStr.find(key, pos + key.length());
		std::string tmp = inputStr.substr(pos + key.length(), newpos - pos - key.length());
		outStrVec.push_back(tmp);
		pos = newpos;
	}
	int tmplen = 0;
	if (outStrVec.size() > 0) 
	{
		tmplen = outStrVec.at(outStrVec.size() - 1).length();
	}

	if (oldpos + tmplen < int(inputStr.length()) - 1) 
	{
		std::string tmp = inputStr.substr(oldpos + key.length());
		outStrVec.push_back(tmp);
	}
}

static std::string trim(std::string &s) 
{
	if (s.empty()) 
	{
		return s;
	}

	s.erase(0, s.find_first_not_of(" "));
	s.erase(s.find_last_not_of(" ") + 1);
	return s;
}

static int toInt(const std::string &in)
{
	int re = 0;
	sscanf(in.c_str(), "%d", &re);
	return re;
}

static float toFloat(const std::string &in) 
{
	float re = 0;
	sscanf(in.c_str(), "%f", &re);
	return re;
}

bool 
ModelDetection::getFrameDetections(cv::Mat& frame, DETECTIONS& res)
{
	if(loadFromFile && counter < maxImgNum) 
    {
		res = data[counter];
        std::stringstream ss;
        ss << imgFolder << "/" << counter << ".jpg";
        frame = cv::imread(ss.str());
        counter++;
		return true;
	}
	return false;
}

bool 
ModelDetection::getFrameDetections(int frame_idx, DETECTIONS& res)
{
	if(loadFromFile == true) 
    {
		res = data[frame_idx];
		return true;
	}
	return false;
}


void ModelDetection::dataMoreConf(float min_confidence, DETECTIONS &d)
{
	DETECTIONS::iterator it;
	for(it = d.begin(); it != d.end();) {
		if((*it).confidence < min_confidence) it = d.erase(it);
		else ++it;
	}
}

void ModelDetection::dataPreprocessing(float max_bbox_overlap, DETECTIONS &d)
{
	int size = int(d.size());
	if(size == 0) return;

	//generate idx:
	std::vector<int> idxs;
	idxs.reserve(size);

	std::vector<bool> idx_status;
	idx_status.reserve(size);
	for(auto i = 0; i < size; ++i) 
    {
		idxs.push_back(int(i));
		idx_status.push_back(false);        
	}

	//get areas:
	std::vector<double> areas;
	areas.reserve(size);
	for(auto i = 0; i < size; ++i) {
		double tmp = (d[i].tlwh(IDX_W)+1)*(d[i].tlwh(IDX_H)+1);
		areas.push_back(tmp);
	}

	//sort idxs by scores ==>quick sort:
	_Qsort(d, idxs, 0, size-1);

	//get delete detections:
	std::vector<int> delIdxs;
	while(true) {//get compare idx;
		int i = -1;
		for(int j = size-1; j>0; --j) {
			if(idx_status[j] == false) {
				i = j;
				idx_status[i] = true;
			}
		}
		if(i == -1) break; //end circle

		//get standard area:
		int xx1 = d[idxs[i]].tlwh(IDX_X); //max
		int yy1 = d[idxs[i]].tlwh(IDX_Y); //max
		int xx2 = d[idxs[i]].tlwh(IDX_X) + d[idxs[i]].tlwh(IDX_W); //min
		int yy2 = d[idxs[i]].tlwh(IDX_Y) + d[idxs[i]].tlwh(IDX_H);//min
		for(auto j = 0; j < size; j++) 
		{
			if(idx_status[j] == true) continue;
			xx1 = int(d[idxs[j]].tlwh(IDX_X) > xx1?d[idxs[j]].tlwh(IDX_X):xx1);
			yy1 = int(d[idxs[j]].tlwh(IDX_Y) > yy1?d[idxs[j]].tlwh(IDX_Y):yy1);
			int tmp = d[idxs[j]].tlwh(IDX_X) + d[idxs[j]].tlwh(IDX_W);
			xx2 = (tmp < xx2? tmp:xx2);
			tmp = d[idxs[j]].tlwh(IDX_Y) + d[idxs[j]].tlwh(IDX_H);
			yy2 = (tmp < yy2?tmp:yy2);
		}
		//standard area = w*h;
		int w = xx2-xx1+1; w = (w > 0?w:0);
		int h = yy2-yy1+1; h = (h > 0?h:0);
		//get delIdx;
		for(auto j = 0; j < size; j++) 
		{
			if(idx_status[j] == true) continue;
			double tmp = w*h*1.0/areas[idxs[j]];
			if(tmp > max_bbox_overlap) 
			{
				delIdxs.push_back(idxs[j]);
				idx_status[j] = true;
			}
		}//end
	}
	//delete from detections:
	for(size_t i = 0; i < delIdxs.size(); ++i) 
    {
		DETECTIONS::iterator it = d.begin() + delIdxs[i];
		d.erase(it);
	}
}

void ModelDetection::ReadFileContent(const std::string &file, std::string &content)
{
	FILE *fl = fopen(file.c_str(), "rb");
	if(fl == NULL)
    {
		return;
	}
	fseek(fl, 0, SEEK_END);
	int len = ftell(fl);
	if(len <= 0)
    {
		return;
	}
	fseek(fl, 0, SEEK_SET);
	char *buf = new char[len+1];
	memset(buf, 0, len+1);
	fread(buf, 1, len, fl);
	content = std::string(buf);
	delete []buf;
	fclose(fl);
}

void ModelDetection::ReadRcFileTotal(const std::string &file) 
{
	std::string content = "";
	ReadFileContent(file, content);


	std::vector<std::string> lines;
	splitStr(content, "\n", lines);
	DETECTIONS rcs;
	int num = -1;
	int tmpNum = -1;
	for (size_t i = 0; i < lines.size(); i++) 
    {
		std::vector<std::string> cols;
		// std::cout << lines[i] << std::endl;
		splitStr(lines[i], ",", cols);
		if (cols.size() < 6) {
			continue;
		}
		tmpNum = toInt(trim(cols[0]));
		if (num!=-1 && tmpNum!=num) 
        {
			data.insert(std::make_pair(num, rcs));
			rcs.clear();
			num = tmpNum;
		}
		if (num == -1) 
		{
			num = tmpNum;
		}
		
        DETECTION_ROW tmpRow;
		tmpRow.class_num = toInt(trim(cols[5]));
		tmpRow.confidence = toFloat(trim(cols[6]));
		tmpRow.tlwh = DETECTBOX(toInt(trim(cols[1])), toInt(trim(cols[2])), toInt(trim(cols[3])), toInt(trim(cols[4])));

		rcs.push_back(tmpRow);
	}
	
    if (!rcs.empty()) 
    {
		data.insert(std::make_pair(tmpNum, rcs));
	}
}


//load detection data from outer file
ModelDetection::ModelDetection(const std::string& txt_filename, const std::string& img_path)
{
    ReadRcFileTotal(txt_filename);
    maxImgNum = data.size();
    counter = 0;
	imgFolder = img_path;
    loadFromFile = true;
}

void ModelDetection::_Qsort(DETECTIONS d, std::vector<int>& a, int low, int high)
{
	if(low >= high) return;
	int first = low;
	int last = high;

	int key_idx = a[first];
	while(first < last) 
    {
		while(first < last && d[a[last]].confidence >= d[key_idx].confidence) 
            --last;
		a[first] = a[last];
		while(first < last && d[a[first]].confidence <= d[key_idx].confidence) 
            ++first;
		a[last] = a[first];
	}
	a[first] = key_idx;
	_Qsort(d, a, low, first-1);
	_Qsort(d, a, first+1, high);
}


DETECTBOX DETECTION_ROW::to_xyah() const
{//(centerx, centery, ration, h)
	DETECTBOX ret = tlwh;
	ret(0,IDX_X) += (ret(0, IDX_W)*0.5);
	ret(0, IDX_Y) += (ret(0, IDX_H)*0.5);
	ret(0, IDX_W) /= ret(0, IDX_H);
	return ret;
}

DETECTBOX DETECTION_ROW::to_tlbr() const
{//(x,y,xx,yy)
	DETECTBOX ret = tlwh;
	ret(0, IDX_X) += ret(0, IDX_W);
	ret(0, IDX_Y) += ret(0, IDX_H);
	return ret;
}

