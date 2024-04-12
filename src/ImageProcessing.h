#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H

#include<fstream>
#include"opencv2/opencv.hpp"
#include<string>
namespace utils{
// coco 80 class namees
    extern const std::vector<std::string> cocoClassNamesList;

// coco pose 17 keypoints line and colors
//          1       2
//      3       0       4
//           
//          5       6
//     
//      7               8 
//     
//    9                  10 
//         11      12
//     
//         13      14  
//    
//         15      16
    extern const std::vector<std::vector<unsigned int>> SKELETON;
    extern const std::vector<std::vector<unsigned int>> KPS_COLORS;
    extern const std::vector<std::vector<unsigned int>> LIMB_COLORS;

    cv::Mat hwc_to_chw(const cv::Mat &image);

    void chw_to_hwc(cv::InputArray src, cv::OutputArray dst);

// resize image with padding 
    void resize_padding(cv::Mat &img, float &det_scale, cv::Size img_size);

    void box_nms(std::vector<cv::Rect> &input_boxes, std::vector<float> confidences,
                 std::vector<std::vector<float>> landmarks, std::vector<int> labels, float NMS_THRESH);

    static inline void softmax_(const float *x, float *y, int length) {
        // Calculate exponential values and sum
        float sum_exp = 0.0f;
        for (int i = 0; i < length; i++) {
            y[i] = expf(x[i]);
            sum_exp += y[i];
        }

        // Normalize using sum
        for (int i = 0; i < length; i++) {
            y[i] /= sum_exp;
        }
    }

    static inline float sigmoid_x(float x) {
        return static_cast<float>(1.f / (1.f + exp(-x)));
    }

    static inline void sigmoid_mat(cv::Mat &mat) {
        cv::exp(-mat, mat);
        mat += 1.0;
        cv::divide(1.0, mat, mat);
    }
}
#endif // IMAGE_PROCESSING_H
