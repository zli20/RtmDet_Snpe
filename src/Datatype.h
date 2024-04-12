#ifndef DATATYPE_H
#define DATATYPE_H

#include <iostream>
#include <vector>
#include "opencv2/opencv.hpp"

using DET_RESULT = struct det{
    cv::Rect_<float> box;
    int label = 0;
    float confidence = 0.0;
};

using FACE_RESULT = struct face{
    cv::Rect_<float> box;
    int label = 0;
    float confidence = 0.0;
    std::vector<float> landmark; //points for det，such as retinaface, yolov8face
    std::vector<float> mulkeypoints; // muilt points such as 68
};
// using FACE_RESULTs = std::vector<FACE_RESULT>;

using POSE_RESULT = struct pose{
    std::vector<float> landmark;
    cv::Rect_<float> box;
    int label = 0;
    float confidence = 0.0;
};



#endif //DATATYPE_H
