# SNPE UDO使用
SNPE支持用户能够以udo（User-Defined Operations）的形式插入框架本身可能不支持的自定义神经网络操作。  
这里只是列出简单的步骤，详细信息参照官方文档：https://developer.qualcomm.com/sites/default/files/docs/snpe/usergroup2.html

## 关于snpe版本问题
开始想用1.68的snpe，然而在使用udo时，发现需要指定QNN目录，而QNN1.x的版本并没有发布，从qnn的官网上可以找到的最低版本时2.10.40.4，于是使用这个版本的qnn和snpe1.68进行尝试。  
由于snpe2.x版本接口产生了改动，果然编译过程中产生了错误，通过对照报错提示，改了一些接口后，编译是通过了，但调用时，仍然报错：  
build engine error : error_code=1106; error_message=UDO Package Registration Failed error_code=1106; error_message=UDO Package Registration Failed Failed to register UDO package : 4005; error_component=User-defined Layer; line_no=161; thread_id=140737142949632; error_component=User-defined Layer; line_no=342; thread_id=140737146014592  
无奈，放弃snpe1.68，改用 2.10。

## 环境
* ubuntu 18.04  
* snpe2.10  
* qnn2.10  
* android-ndk（如果需要在安卓下使用）  

## config文件

第一步需要先定义udo包的配置文件，采用json的格式定义包名、输出输出参数、运行平台等信息。HardSigmod的配置参数，可以参照snpe_udo/HardSigmodUdoPackage/config/Hardsigmoid.json 。    

## 模型转换

模型转换比较简单，借助snpe的转换工具，加上 --udo参数，转换时会自动忽略snpe不支持但json文件中定义的层。  
`snpe-onnx-to-dlc -i rtmxxx.onnx -o rtmdet_168.dlc --udo ./udo/hard_sigmod/HardSigmodUdoPackage/config/Hardsigmoid.json`

## 包生成

使用snpe-udo-package-generator工具生成创建库，也就是这一步需要用qnn。  
snpe-udo-package-generator是一个python脚本， 实际就是调用SNPE_ROOT/lib/python/qti.aisw.converters.backend.custom_ops.snpe_udo_package_generator.py，根据qnn目录下的模版、makfile等生成编译使用的接口hpp、cpp和编译makefile等。  
用法如下：  
`snpe-udo-package-generator -p ./config/Hardsigmoid.json -o . `
以上命令将创建一个UDO包，该包将是一个目录，由框架代码和构建文件组成，可以用来将包内容编译成独立的共享库。
在这个框架中，接口是已经写好的的，只需要在相应的地方填充代码即可，如：cpu需要在./HardSigmodUdoPackage/jni/src/CPU/src/ops/HardSigmoid.cpp中Qnn_ErrorHandle_t execute(CustomOp* operation)填充相关操作。

## 编译

在完成包生成后，就需要编译生成动态库了。编译执行make就可以，需要注意的是环境导入。  
`export SNPE_UDO_ROOT=<absolute_path_to_SnpeUdo_headers_directory>  # snpe路径下share/SnpeUdo `  
如果要编译安卓下使用的库，还需要nkd路径：       
`export NDK_BUILD=<absolute_path_to_android_ndk_directory> `  
编译安卓gpu的版，需要设置opencl环境：   
`export CL_LIBRARY_PATH=<absolute_path_to_OpenCL_library>`  
然后执行make命令即可：  
`make cpu_x86  # linux`  
`make cpu_android # 安卓cpu`  
`make gpu_android # 安卓gpu`  
编译完成后，就会在相应目录下生成，动态库。  
The UDO CPU implementation library: <UDO-Package>/libs/x86-64_linux_clang/libUdo<UDO-Package>ImplCpu.so  
The UDO package registration library: <UDO-Package>/libs/x86-64_linux_clang/libUdo<UDO-Package>Reg.so  

## 使用
在snpe-net-run工具中使用时，需要指定udo_package_path：  
`snpe-net-run --container <path_to_dlc> --input_list <path_to_input_list> --udo_package_path <path_to_registration_lib>`  
在c++接口中使用时，需要在初始化模型时，调用bool SNPEFactory::addOpPackage( const std::string& regLibraryPath ); 加入udo库。参照SnpeEngine::init。  
这里需要加入的是Reg.so，ImplCpu.so是无法加载的，要在cmakelist使用target_link_libraries 链接。

