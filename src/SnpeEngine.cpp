#include "SnpeEngine.h"
#include <fstream>
#include "SNPE/SNPEFactory.hpp"
//#include "DlSystem/DlEnums.hpp"
#include "ImageProcessing.h"

void SnpeEngine::checkRuntime(){
    static zdl::DlSystem::Version_t Version = zdl::SNPE::SNPEFactory::getLibraryVersion();
    std::cout << "SNPE Version: " << Version.asString().c_str() << std::endl;
    if (zdl::SNPE::SNPEFactory::isRuntimeAvailable(zdl::DlSystem::Runtime_t::AIP_FIXED_TF)) {
        const char *aip_runtime_string = zdl::DlSystem::RuntimeList::runtimeToString(zdl::DlSystem::Runtime_t::AIP_FIXED_TF);
        std::cout << "Current SNPE runtime Support :   " << aip_runtime_string << std::endl;
    }
    else {
        std::cout << "Current SNPE runtime Not Support :  AIP" << std::endl;
    }

    if (zdl::SNPE::SNPEFactory::isRuntimeAvailable(zdl::DlSystem::Runtime_t::GPU)) {
        const char *gpu_runtime_string = zdl::DlSystem::RuntimeList::runtimeToString(zdl::DlSystem::Runtime_t::GPU);
        std::cout << "Current SNPE runtime Support :   " << gpu_runtime_string << std::endl;
    }
    else {
        std::cout << "Current SNPE runtime Not Support :   GPU" << std::endl;
    }

    if(zdl::SNPE::SNPEFactory::isRuntimeAvailable(zdl::DlSystem::Runtime_t::DSP)) {
        const char *dsp_runtime_string = zdl::DlSystem::RuntimeList::runtimeToString(zdl::DlSystem::Runtime_t::DSP);
        std::cout << "Current SNPE runtime Support :   " << dsp_runtime_string << std::endl;
    }
    else {
        std::cout << "Current SNPE runtime Not Support :   DSP" << std::endl;
    }
}

void SnpeEngine::setruntime(const int platform) {
    _runtime_list.clear();
    this->_platform = static_cast<Platform>(platform);
    switch (platform) {
        case 3:
            _runtime_list.add(zdl::DlSystem::Runtime_t::AIP_FIXED_TF);
        case 2:
            _runtime_list.add(zdl::DlSystem::Runtime_t::DSP);
        case 1:
            _runtime_list.add(zdl::DlSystem::Runtime_t::GPU);
            // break;
        case 0:
        default:
            _runtime_list.add(zdl::DlSystem::Runtime_t::CPU);
            break;
    }
}

void SnpeEngine::preProcessing(cv::Mat &img, float & det_scale, bool padding, bool normalize, bool mean, bool bgr2rgb) const{
    det_scale=1.0;
    if(img.size() != cv::Size(_model_input_width, _model_input_height)) {
        if(padding){utils::resize_padding(img, det_scale, cv::Size(_model_input_width, _model_input_height));}
        else{cv::resize(img, img, cv::Size(_model_input_height, _model_input_width));}

    }
    // cv::imshow("YOLOv8: ", img);
    // cv::waitKey();

    if(mean){
        cv::Scalar meanValues(103.0, 117.0, 123.0);
        img -= meanValues;
    }
    if(normalize){ img.convertTo(img, CV_32F, 1.0 / 255);}
    else{img.convertTo(img, CV_32F, 1.0);}

    // BGR to RGB
    if(bgr2rgb){cv::cvtColor(img, img, cv::COLOR_BGR2RGB);}

}

void SnpeEngine::setOutName(const std::vector<std::string>& out_names) {
    for (const auto& name : out_names) {
        _output_tensor_names.append(name.c_str());
    }
}

int SnpeEngine::init(const std::string &model_path, int platdorm) {
    // 1.set runtime
    setruntime(platdorm);

    // 2. load model
    _container = zdl::DlContainer::IDlContainer::open(model_path);

    if (_container == nullptr) {
        std::cout << "load model error : " << zdl::DlSystem::getLastErrorString() << std::endl;
        return -1;
    }

    if(!_udo_paths_list.empty()){
        for (const auto &u : _udo_paths_list)
        {
            if (!zdl::SNPE::SNPEFactory::addOpPackage(u))
            {
                std::cerr << "Error while loading UDO package: "<< u << std::endl;
                return -1;
            }
        }
    }
    // zdl::DlSystem::TensorShapeMap inputShapeMap;
    // inputShapeMap.add("images", {1UL, static_cast<unsigned long>(input_image_size.width), static_cast<unsigned long>(input_image_size.height), 3UL});
    // 3. build engine
    zdl::SNPE::SNPEBuilder snpe_builder(_container.get());
    zdl::DlSystem::PerformanceProfile_t profile = zdl::DlSystem::PerformanceProfile_t::HIGH_PERFORMANCE;

    if (_output_tensor_names.size() != 0){
        snpe_builder.setOutputLayers(_output_tensor_names);
    }
    _engine = snpe_builder
            .setRuntimeProcessorOrder(_runtime_list)
            .setPerformanceProfile(profile)
            // .setCPUFallbackMode(true)
            .build();

    if (_engine == nullptr) {
        std::cout << "build engine error : " << zdl::DlSystem::getLastErrorString() << std::endl;
        return -1;
    }

    std::cout << "init success..." << std::endl;
    return 0;
}

void SnpeEngine::build_tensor(const cv::Mat &mat) {
    zdl::DlSystem::Dimension dims[4];
    dims[0] = 1;
    dims[1] = _model_input_width;
    dims[2] = _model_input_height;
    dims[3] = 3;
    zdl::DlSystem::TensorShape tensorShape(dims, 4);
    _input_tensor = zdl::SNPE::SNPEFactory::getTensorFactory().createTensor(tensorShape);

    // std::vector<float> input_data((float*)mat.datastart, (float*)mat.dataend);
    // std::copy(input_data.begin(), input_data.end(), _input_tensor->begin());

    size_t dataSize = (char*)mat.dataend - (char*)mat.datastart;
    std::copy_n(reinterpret_cast<const float*>(mat.datastart), dataSize / sizeof(float), _input_tensor->begin());

}

int SnpeEngine::inference()
{
    _output_tensor_map.clear();
    bool ret = _engine->execute(_input_tensor.get(), _output_tensor_map);
    if (!ret) {
        std::cerr << "engine inference error : " << zdl::DlSystem::getLastErrorString() << std::endl;
        return -1;
    }

    zdl::DlSystem::StringList tensorNames = _output_tensor_map.getTensorNames();
    if (tensorNames.size() == 0) {
        std::cerr << "No output tensors found" << std::endl;
        return -1;
    }

    for (const auto& tensorName : tensorNames) {
        zdl::DlSystem::ITensor* tensor = _output_tensor_map.getTensor(tensorName);
        if (tensor != nullptr) {
            auto *pdata = reinterpret_cast<float *>(&(*tensor->begin()));
            zdl::DlSystem::TensorShape shape = tensor->getShape();
            _out_data_ptr[tensorName] = pdata;
            _output_shapes[tensorName] = shape;
        } else {
            std::cerr << "Tensor not found: " << tensorName << std::endl;
        }
    }
    return 0;
}

void SnpeEngine::setUdoPkg(const std::vector<std::string> & udo_paths_list) {
    this->_udo_paths_list = udo_paths_list;
}

//int SnpeEngine::inference(const cv::Mat &cv_mat, std::map<std::string, float *> &out_tensor) {
//    return 0;
//}

