//==============================================================================
// Auto Generated Code for HardSigmodUdoPackage
//==============================================================================
#include <iostream>
#include "utils/UdoUtil.hpp"
#include "HardSigmodUdoPackageCpuImplValidationFunctions.hpp"
#include "HardSigmodUdoPackageDspImplValidationFunctions.hpp"
#include "HardSigmodUdoPackageGpuImplValidationFunctions.hpp"

#ifndef UDO_LIB_NAME_CPU
#define UDO_LIB_NAME_CPU "libUdoHardSigmodUdoPackageImplCpu.so"
#endif
#ifndef UDO_LIB_NAME_DSP
#define UDO_LIB_NAME_DSP "libUdoHardSigmodUdoPackageImplDsp.so"
#endif
#ifndef UDO_LIB_NAME_GPU
#define UDO_LIB_NAME_GPU "libUdoHardSigmodUdoPackageImplGpu.so"
#endif

extern "C"
{

std::unique_ptr<UdoUtil::UdoVersion> regLibraryVersion;
std::unique_ptr<UdoUtil::UdoRegLibrary> regLibraryInfo;

SnpeUdo_ErrorType_t
SnpeUdo_initRegLibrary(void)
{
    regLibraryInfo.reset(new UdoUtil::UdoRegLibrary("HardSigmodUdoPackage",
                                                   SNPE_UDO_CORETYPE_CPU | SNPE_UDO_CORETYPE_DSP | SNPE_UDO_CORETYPE_GPU));

    regLibraryVersion.reset(new UdoUtil::UdoVersion);

    regLibraryVersion->setUdoVersion(1, 0, 0);


    /*
    ** User should fill in implementation library path here as needed.
    ** Note: The Implementation library path set here is relative, meaning each library to be used
    ** must be discoverable by the linker.
    */
    regLibraryInfo->addImplLib(UDO_LIB_NAME_CPU, SNPE_UDO_CORETYPE_CPU); //adding implementation libraries
    regLibraryInfo->addImplLib(UDO_LIB_NAME_DSP, SNPE_UDO_CORETYPE_DSP); //adding implementation libraries
    regLibraryInfo->addImplLib(UDO_LIB_NAME_GPU, SNPE_UDO_CORETYPE_GPU); //adding implementation libraries

    //==============================================================================
    // Auto Generated Code for HardSigmoid
    //==============================================================================
    auto HardSigmoidInfo = regLibraryInfo->addOperation("HardSigmoid", SNPE_UDO_CORETYPE_CPU | SNPE_UDO_CORETYPE_GPU | SNPE_UDO_CORETYPE_DSP, 1, 1);

    HardSigmoidInfo->addCoreInfo(SNPE_UDO_CORETYPE_CPU, SNPE_UDO_DATATYPE_FLOAT_16 | SNPE_UDO_DATATYPE_FLOAT_32); //adding core info
    HardSigmoidInfo->addCoreInfo(SNPE_UDO_CORETYPE_GPU, SNPE_UDO_DATATYPE_INT_8 | SNPE_UDO_DATATYPE_INT_16); //adding core info
    HardSigmoidInfo->addCoreInfo(SNPE_UDO_CORETYPE_DSP, SNPE_UDO_DATATYPE_FLOAT_16 | SNPE_UDO_DATATYPE_FLOAT_32); //adding core info



    //inputs and outputs need to be added as tensor params

    HardSigmoidInfo->addInputTensorInfo("HardSigmod_input", {{SNPE_UDO_CORETYPE_CPU, SNPE_UDO_DATATYPE_FLOAT_32}, {SNPE_UDO_CORETYPE_GPU, SNPE_UDO_DATATYPE_FLOAT_32}, {SNPE_UDO_CORETYPE_DSP, SNPE_UDO_DATATYPE_UINT_8}}, SNPE_UDO_LAYOUT_LAST, 0, 0); //adding tensor info

    //adding outputs
    HardSigmoidInfo->addOutputTensorInfo("HardSigmod_output", {{SNPE_UDO_CORETYPE_CPU, SNPE_UDO_DATATYPE_FLOAT_32}, {SNPE_UDO_CORETYPE_GPU, SNPE_UDO_DATATYPE_FLOAT_32}, {SNPE_UDO_CORETYPE_DSP, SNPE_UDO_DATATYPE_UINT_8}}, SNPE_UDO_LAYOUT_LAST, 0); //adding tensor info

    // adding validation functions
    UDO_VALIDATE_RETURN_STATUS(regLibraryInfo->registerValidationFunction("HardSigmoid",
                                                SNPE_UDO_CORETYPE_CPU,
                                                std::unique_ptr<HardSigmoidCpuValidationFunction>
                                                    (new HardSigmoidCpuValidationFunction())))

    UDO_VALIDATE_RETURN_STATUS(regLibraryInfo->registerValidationFunction("HardSigmoid",
                                                SNPE_UDO_CORETYPE_GPU,
                                                std::unique_ptr<HardSigmoidGpuValidationFunction>
                                                    (new HardSigmoidGpuValidationFunction())))

    UDO_VALIDATE_RETURN_STATUS(regLibraryInfo->registerValidationFunction("HardSigmoid",
                                                SNPE_UDO_CORETYPE_DSP,
                                                std::unique_ptr<HardSigmoidDspValidationFunction>
                                                    (new HardSigmoidDspValidationFunction())))

    UDO_VALIDATE_RETURN_STATUS(regLibraryInfo->createRegInfoStruct())

    return SNPE_UDO_NO_ERROR;
}

SnpeUdo_ErrorType_t
SnpeUdo_getVersion(SnpeUdo_LibVersion_t** version) {

    UDO_VALIDATE_RETURN_STATUS(regLibraryVersion->getLibraryVersion(version))

    return SNPE_UDO_NO_ERROR;
}

SnpeUdo_ErrorType_t
SnpeUdo_getRegInfo(SnpeUdo_RegInfo_t** registrationInfo) {

    UDO_VALIDATE_RETURN_STATUS(regLibraryInfo->getLibraryRegInfo(registrationInfo))

    return SNPE_UDO_NO_ERROR;
}

SnpeUdo_ErrorType_t
SnpeUdo_terminateRegLibrary(void) {
    regLibraryInfo.reset();
    regLibraryVersion.reset();

    return SNPE_UDO_NO_ERROR;
}

SnpeUdo_ErrorType_t
SnpeUdo_validateOperation(SnpeUdo_OpDefinition_t* opDefinition) {
    UDO_VALIDATE_RETURN_STATUS(regLibraryInfo->snpeUdoValidateOperation(opDefinition))

    return SNPE_UDO_NO_ERROR;
}
}; //extern C
