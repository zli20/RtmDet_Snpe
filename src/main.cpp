#include <iostream>

#include "RtmDet.h"

int plantform;

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0]  << " int <platdorm>" << " string <cfg path> " << " string <input path> " << std::endl;
        return -1;
    }

    plantform = std::stoi(argv[1]);

    std::string cfg_path;
    cfg_path = argv[2];
    std::cout << "cfg path: " << cfg_path << std::endl;
    Maincfg& cfg = Maincfg::instance();
    cfg.LoadCfg(cfg_path);

    std::string img_path;
    img_path = argv[3];
    std::cout << "inputpath: " << img_path << std::endl;
    // std::string img_path = "/yolov8_pose_snpe/images/a.jpg";

    bool model_withnms = (cfg.model_path.find("nonms") == std::string::npos);

    auto *_engine = new RtmDet();
    cv::Mat cvmat = cv::imread(img_path, cv::IMREAD_COLOR);

    cv::Mat result_mat = cvmat.clone();
    std::vector<DET_RESULT> result;

    double start = static_cast<double>(cv::getTickCount());
    std::cout << "---------Star inference" << std::endl;

    _engine->getInference(cvmat, result,model_withnms);
    double end = static_cast<double>(cv::getTickCount());
    double time_cost = (end - start) / cv::getTickFrequency() * 1000;
    std::cout << "---------Time cost : " << time_cost << "ms" << std::endl;

    _engine->drawResult(result_mat, result);

     // imshow("yolov8n-pose", result_mat);
     // cv::waitKey(0);

     // 图像显示时，由于分辨率的问题，显示不全，所以resize到640*640显示
     cv::Mat resized_img;
     if (result_mat.cols != 640 || result_mat.rows != 640) {
         int new_height, new_width;
         float im_ratio = float(result_mat.rows) / float(result_mat.cols);
         if (im_ratio > 1) {
             new_height = 640;
             new_width = int(new_height / im_ratio);
         }
         else {
             new_width = 640;
             new_height = int(new_width * im_ratio);
         }

         cv::resize(result_mat, resized_img, cv::Size(new_width, new_height));
     }
     imshow("yolov8n-pose", resized_img);
     cv::waitKey(0);
     delete _engine;
    _engine = nullptr;

     return 0;
}
