#ifndef RTM_POSE_SNPE_RTMDET_H
#define RTM_POSE_SNPE_RTMDET_H

#include "SnpeEngine.h"
#include "Datatype.h"
#include "Maincfg.h"

extern int plantform;

class RtmDet : public SnpeEngine{
public:
    using ANCHOR = struct anchor{
        float cx;
        float cy;
    };
    explicit  RtmDet() : SnpeEngine(Maincfg::instance().model_input_width, Maincfg::instance().model_input_hight)
    {
        this->setOutName(Maincfg::instance().model_output_layer_name);

        this->setUdoPkg(Maincfg::instance().udo_list);
        // initialization models
//        if (init(Maincfg::instance().model_path, Maincfg::instance().runtime) != 0) {
//            throw std::runtime_error("Faild to init model");
//        }
        if (init(Maincfg::instance().model_path, plantform) != 0) {
            throw std::runtime_error("Faild to init model");
        }

        this->target_conf_th = Maincfg::instance().target_conf_th;
        this->nms_th = Maincfg::instance().nms_th;
    }

    ~RtmDet() override= default;

    void postProcessing(std::vector<DET_RESULT> & _results, const float det_scale, bool padding, bool with_nms);

    void getInference(const cv::Mat& img, std::vector<DET_RESULT>& results, bool with_nms);

    void drawResult(cv::Mat& img, const std::vector<DET_RESULT>& results);

    static void create_anchor_rtmdet(std::vector<ANCHOR> &anchor, int w, int h);
    cv::Size img_input_size;
    float target_conf_th ;
    float nms_th;
    int box_node_nums = 4;
    int score_node_nums = 1;
};


#endif //RTM_POSE_SNPE_RTMDET_H
