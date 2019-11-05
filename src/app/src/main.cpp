#include <iostream>
#include "VideoTracker.h"
#include "param.h"

int main(int argc, char **argv)
{
	if(argc != 2)
	{
		std::cerr << "Usage: " << argv[0] << "/path/to/the/config/file" << std::endl;
		exit(-1);
	}

	DeepSortParam params;
	params.read(argv[1]);
	
	VideoTracker t(params);
	if(t.run() == false) 
	{
		std::cout << t.showErrMsg() << std::endl;
	}
	return 0;
}
