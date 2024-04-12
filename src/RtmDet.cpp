#include "RtmDet.h"
#include "ImageProcessing.h"

void RtmDet::postProcessing(std::vector<DET_RESULT> &_results, const float det_scale, bool padding, bool with_nms) {
    float higth_scale = 1.0;
    float width_scale = 1.0;
    if(!padding){
        higth_scale = static_cast<float>(this->_model_input_height) / this->img_input_size.height;
        width_scale = static_cast<float>(this->_model_input_width) / this->img_input_size.width;
    }

    std::vector<float> confidences;
    std::vector<cv::Rect> boxes;
    std::vector<int> labels;
    if(with_nms) {
        auto *boxptr = this->_out_data_ptr[Maincfg::instance().model_output_tensor_name[0]];
        auto box_shape = this->_output_shapes[Maincfg::instance().model_output_tensor_name[0]];  // 1*100*5
        std::cout << "box_shape: " << box_shape[0] << " " << box_shape[1] << " " << box_shape[2] << std::endl;

        auto *clsptr = this->_out_data_ptr[Maincfg::instance().model_output_tensor_name[1]];
        auto cls_shape = this->_output_shapes[Maincfg::instance().model_output_tensor_name[1]];  // 1*100
        std::cout << "cls_shape: " << cls_shape[0] << " " << box_shape[1] << std::endl;
        for (int i = 0; i < Maincfg::instance().model_output_shape[1][1]; i++) {
            float box_t = boxptr[i * 5] / width_scale / det_scale;;
            float box_l = boxptr[i * 5 + 1] / higth_scale / det_scale;
            float box_w = boxptr[i * 5 + 2] / width_scale / det_scale - box_t;
            float box_h = boxptr[i * 5 + 3] / higth_scale / det_scale - box_l;
            float scor = boxptr[i * 5 + 4];
            int cls = clsptr[i];

            confidences.push_back(scor);
            labels.push_back(cls);
            boxes.emplace_back(box_t, box_l, box_w, box_h);
        }
    }
    else{
        auto *boxptr = this->_out_data_ptr[Maincfg::instance().model_output_tensor_name[0]];
        auto box_shape = this->_output_shapes[Maincfg::instance().model_output_tensor_name[0]];
        std::cout << "box_shape: " << box_shape[0] << " " << box_shape[1] << " " << box_shape[2] << std::endl;

        auto *clsptr = this->_out_data_ptr[Maincfg::instance().model_output_tensor_name[1]];
        auto cls_shape = this->_output_shapes[Maincfg::instance().model_output_tensor_name[1]];
        std::cout << "cls_shape: " << cls_shape[0] << " " << cls_shape[1] << " " << cls_shape[2]<< std::endl;

        const cv::Mat boxes_mat = cv::Mat(cv::Size(box_shape[1], box_shape[2]), CV_32F, boxptr);
        cv::Mat scores_mat = cv::Mat(cv::Size(cls_shape[1], cls_shape[2]), CV_32F, clsptr);
        cvSigmoid_(scores_mat);

        auto boxes_pdata = reinterpret_cast<float*>(boxes_mat.data);
        auto scores_pdata = reinterpret_cast<float*>(scores_mat.data);

        std::vector<ANCHOR> anchors;
        create_anchor_rtmdet(anchors, this->_model_input_width,this->_model_input_height);

        for (int r = 0; r < scores_mat.cols; ++r) {
            cv::Mat scores(1, score_node_nums, CV_32FC1, scores_pdata);

            float x0 = anchors[r].cx - boxes_pdata[0] ;
            float y0 = anchors[r].cy - boxes_pdata[1];
            float x1 = anchors[r].cx + boxes_pdata[2];
            float y1 = anchors[r].cy + boxes_pdata[3];

            float box_t = x0 / width_scale /det_scale;
            float box_l = y0 / higth_scale /det_scale;
            float box_w = x1 / width_scale /det_scale - box_t;
            float box_h = y1 / higth_scale /det_scale - box_l;

            cv::Point classIdPoint;
            double max_class_socre;
            cv::minMaxLoc(scores, nullptr, &max_class_socre, nullptr, &classIdPoint);
            max_class_socre = static_cast<float>(max_class_socre);

            if(max_class_socre >= 0.5 && max_class_socre <= 1){

                confidences.push_back(max_class_socre);
                labels.push_back(classIdPoint.x);
                boxes.emplace_back(box_t, box_l, box_w, box_h);

//                std::cout << "box_tlwh_score: " << box_t << " " << box_l << " " << box_w << " " << box_h << " " << max_class_socre << std::endl;
            }

            boxes_pdata += box_node_nums;
            scores_pdata += score_node_nums;

        }
    }
    std::vector<int> nms_result;
    cv::dnn::NMSBoxes(boxes, confidences, target_conf_th, nms_th, nms_result);

    for (auto idx : nms_result) {
        DET_RESULT result;
        result.confidence = confidences[idx];
        result.box = boxes[idx];
        result.label = labels[idx];
        _results.push_back(result);
    }
//        std::cout << "box_xywh: "<< bx << " " <<by << " "<< bw << " "<< bh << " " <<  scor << std::endl;
//        std::cout << "cls: "<< cls << std::endl;
}

void RtmDet::getInference(const cv::Mat &img, std::vector<DET_RESULT> &results, bool with_nms) {
    this->img_input_size = img.size();
    cv::Mat input_mat(img);

    float det_scale;
    preProcessing(input_mat, det_scale, false, true, false, false);

    build_tensor(input_mat);

    this->inference();

    results.clear();
    postProcessing(results, det_scale, false, with_nms);
}

void RtmDet::drawResult(cv::Mat &img, const std::vector<DET_RESULT> &results) {
    if (results.empty()) {
        return ;
    }
    for (auto& result : results) {
        int  left, top, width, height;
        left = result.box.x;
        top = result.box.y;
        width = result.box.width;
        height = result.box.height;

        cv::Rect boxxs;
        boxxs.x = left;
        boxxs.y = top;
        boxxs.width = width;
        boxxs.height = height;

        cv::rectangle(img, boxxs, cv::Scalar(0, 0, 255), 1, 8);

        // 在目标框左上角标识目标类别以及概率
        if (result.label >= 0 && result.label < static_cast<int>(cocoClassNamesList.size())) {
            std::string label = cocoClassNamesList[result.label] + ":" + std::to_string(result.confidence);
            int baseLine;
            cv::Size labelSize = cv::getTextSize(label, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
            top = std::max(top, labelSize.height);
            putText(img, label, cv::Point(left, top), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255), 2);
        }
        else {
            // 如果 result.label 不在合法范围内，输出错误信息
            std::cout << "Error: Invalid label index!" << std::endl;
        }
    }
}

void RtmDet::create_anchor_rtmdet(std::vector<ANCHOR> &anchor, int w, int h) {
//    anchor.reserve(num_boxes);
    anchor.clear();
    std::vector<std::vector<int>> feature_map(3), min_sizes(3);
    int steps[] = {8, 16, 32};
    for (size_t  i = 0; i < feature_map.size(); ++i) {
        feature_map[i].push_back(ceil(h/steps[i]));
        feature_map[i].push_back(ceil(w/steps[i]));
    }

    for (size_t  k = 0; k < feature_map.size(); ++k)
    {
        std::vector<int> min_size = min_sizes[k];
        for (int i = 0; i < feature_map[k][0]; ++i)
        {
            for (int j = 0; j < feature_map[k][1]; ++j)
            {
                    float cx = j  * steps[k];
                    float cy = i  * steps[k];
                    ANCHOR axil = {cx, cy};
                    anchor.push_back(axil);
            }
        }

    }
}
