# Deep Sort

A refactoring and tested version of <a href="https://github.com/bitzy/DeepSort">DeepSort</a>.

## Requirements

* OpenCV (>= 3.4)
* Eigen3
* Boost
* CUDA (>= 10.0)
* Tensorflow ( = 1.14)

## Tensorflow Installation 

For installing the C++ API, I followed the following <a href="https://github.com/FloopCZ/tensorflow_cc">guide</a>. I included the repository inside the folder *tensorflow_cc*, where I modified some file for installing the 1.14 version of tensorflow.

For compiling it, after satysfying the requirements listed in the <a href="https://github.com/FloopCZ/tensorflow_cc">official repository</a>, execute the following command:

```
cmake -DTENSORFLOW_STATIC=OFF -DTENSORFLOW_SHARED=ON ..
make
sudo make install
```

## How to compile

Deep Sort works under Linux environments. I recommend a so-called out of source build which can be achieved by the following command sequence:

* mkdir build
* cd build
* cmake ../
* make -j<number-of-cores+1>

## How to use

Go to the bin diretory and launch the program with the following commands:
```bash
./deep_sort /path/to/the/config/file
e.g.:
./deep_sort ../config/deepsort_config.txt
```

If you want to use it as library, please add *tensorflow_framework* as well in the *target_link_libraries* command as showed in the CMakeLists file.

## Params

The param file has to be created like the following one:
```
[DETECTION_FILE]
/path/to/the/detection/file
[IMAGE_DIRECTORY]
/path/to/the/image/folder
[METRIC_MODEL]
/path/to/the/similarity/model
[FEATURE_MODEL]
/path/to/the/feature/extractor
[ARGS_NN_BUDGET]
100
[ARGS_MAX_COSINE_DISTANCE]
0.2
[DT]
1.0
[MAX_IOU_DISTANCE]
0.9
[MAX_AGE]
30
[N_INIT]
3
[CLASSES]
/path/to/the/class/file
[SHOW_DETECTIONS] # 1 = true 0 = false
1
```
### Detection File

The detection file is a txt file containing the detections in the following format:

```
#frame_number,#top_left_x,#top_left_y,#width,#height,#class,#confidence
e.g. 0,623,281,16,15,1,0.294832
```


### Class File
The class file has to include the names of all the classes detected by your detector. 

E.g.:
```
class1
class2
class3
...
classN
```

Please, for any comment send me an email to andrea[dot]pennisi[at]gmail[dot]com.
