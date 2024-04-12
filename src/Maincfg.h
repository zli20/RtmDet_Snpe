#ifndef CVDL_MAINCFG_H
#define CVDL_MAINCFG_H

#include <iostream>
#include <string>
#include <vector>
#include <map>

class Maincfg {
public:
    // 单例类添加标志位，允许创建多个不同的实例, 为了方便多个方法同时使用
    static Maincfg& instance(const std::string& identifier="default");
    Maincfg(const Maincfg&) = delete;
    Maincfg& operator=(const Maincfg) = delete;

    void init();

    bool LoadCfg(const std::string & path);

private:
    explicit Maincfg(const std::string& identifier="default");
    ~Maincfg();

public:
    std::string model_path;
    std::vector<std::string> model_input_layer_name;
    std::vector<std::string> model_output_layer_name;
    std::vector<std::string> model_output_tensor_name;

    int model_input_width{-1};
    int model_input_hight{-1};

    std::vector<int> moudel_input_shape;
    std::vector<std::vector<int>> model_output_shape;

    int framework{-1};
    int runtime{-1};

    float target_conf_th{0};
    float nms_th{0};

    std::vector<std::string> udo_list;
private:
    static int instance_count; // 记录实例数量的静态成员变量
    std::string instance_identifier; // 实例标识符

};


#endif //CVDL_MAINCFG_H
