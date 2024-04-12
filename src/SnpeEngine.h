#ifndef SNPE_SNPEENGINE_H
#define SNPE_SNPEENGINE_H

#include <vector>
#include <iostream>
#include <any>

#include "DlContainer/IDlContainer.hpp"
#include "DlSystem/DlError.hpp"
#include "DlSystem/ITensorFactory.hpp"
#include "DlSystem/RuntimeList.hpp"
#include "SNPE/SNPEBuilder.hpp"

#include "SNPE/SNPE.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/core/ocl.hpp"

class SnpeEngine
{
public:
    enum Platform {CPU=0, GPU, DSP, AIP  };

    explicit SnpeEngine(int model_input_width, int _model_input_hight): _model_input_width(model_input_width), _model_input_height(_model_input_hight)
    {
        checkRuntime();
    };

    virtual ~SnpeEngine(){
        _engine.reset();
        std:: cout << "deinit success..." << std::endl;
    };

    int init(const std::string &model_path, int platform=CPU);

    void preProcessing(cv::Mat &img, float & det_scale, bool padding=false, bool normalize=true, bool mean=true, bool bgr2rgb=false) const;

    void setOutName(const std::vector<std::string>& out_names);

    void setUdoPkg(const std::vector<std::string> & udo_paths_list);

    void build_tensor(const cv::Mat& mat);

    int inference();

    int _model_input_width;
    int _model_input_height;
    Platform _platform = CPU;

    // snpe model
    std::unique_ptr<zdl::SNPE::SNPE> _engine;
    std::unique_ptr<zdl::DlContainer::IDlContainer> _container;

    // snpe input & output
    zdl::DlSystem::StringList _output_tensor_names;

    std::unique_ptr<zdl::DlSystem::ITensor> _input_tensor;
    zdl::DlSystem::TensorMap _output_tensor_map;
    std::map<std::string, float*> _out_data_ptr;
    std::map<std::string, zdl::DlSystem::TensorShape > _output_shapes;

    // snpe builder config
    zdl::DlSystem::RuntimeList _runtime_list;

    std::vector<std::string> _udo_paths_list;

    void setruntime(int platform);

    static  void checkRuntime();

private:

    // void Preprocessing(cv::Mat &img, float &detScale) const;
    //
    // void Postprocessing(zdl::DlSystem::ITensor *boxes_outTensor, zdl::DlSystem::ITensor *scores_outTensor, std::vector<Yolov8OutPut>& outPut, float det_scale) const;
    //
    // void Postprocessing(zdl::DlSystem::ITensor *outTensor, std::vector<Yolov8OutPut>& outPut, float det_scale) const;



};

#endif //SNPE_SNPEENGINE_H
