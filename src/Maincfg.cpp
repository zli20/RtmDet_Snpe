#include "Maincfg.h"
#include <opencv2/opencv.hpp>

int Maincfg::instance_count = 0;

//Maincfg::Maincfg(const std::string& identifier): instance_identifier(identifier)
//{
//    ++instance_count;
//}

Maincfg::Maincfg(const std::string& identifier): instance_identifier(identifier) // NOLINT(*-pass-by-value)
{
    ++instance_count;
}

Maincfg::~Maincfg(){
    --instance_count;
}


Maincfg& Maincfg::instance(const std::string& identifier) {
    static Maincfg instance(identifier);
    return instance;
}


bool Maincfg::LoadCfg(const std::string & path) {

    cv::FileStorage fs(path, cv::FileStorage::READ);
    if (!fs.isOpened()) {
        std::cerr << "Failed to read config file" <<std::endl;
        return false;
    }

    fs["model_path"] >> model_path;

    model_input_layer_name.clear();
    for (const auto& n : fs["model_input_layer_name"]) {
        std::string str;
        n >> str;
        model_input_layer_name.push_back(str);
    }

    model_output_layer_name.clear();
    for (const auto& n : fs["model_output_layer_name"]) {
        std::string str;
        n >> str;
        model_output_layer_name.push_back(str);
    }

    model_output_tensor_name.clear();
    for (const auto& n : fs["model_output_tensor_name"]) {
        std::string str;
        n >> str;
        model_output_tensor_name.push_back(str);
    }

    fs["model_input_width"] >> model_input_width;
    fs["model_input_hight"] >> model_input_hight;

    moudel_input_shape.clear();
    for (const auto& n : fs["moudel_input_shape"]) {
        int number;
        n >> number;
        moudel_input_shape.push_back(number);
    }

    model_output_shape.clear();
    for (const auto& shape_node : fs["moudel_output_shape"]) {
        std::vector<int> shape;
        if (!shape_node.isSeq()) {
            std::cerr << "Invalid data format!" << std::endl;
            return false;
        }

        for (const auto& value_node : shape_node) {
            shape.push_back((int)value_node);
        }
        model_output_shape.push_back(shape);
    }

    fs["framework"] >> framework;
    fs["runtime"] >> runtime;

    // 可选参数，部分方法会用到
    if (!fs["target_conf_th"].empty()) {
        fs["target_conf_th"] >> target_conf_th;
    }
    if (!fs["nms_th"].empty()) {
        fs["nms_th"] >> nms_th;
    }

    if (!fs["udo_list"].empty()) {
        udo_list.clear();
        for (const auto& n : fs["udo_list"]) {
            std::string str;
            n >> str;
            udo_list.push_back(str);
        }
    }
    return true;
}

void Maincfg::init() {

}


