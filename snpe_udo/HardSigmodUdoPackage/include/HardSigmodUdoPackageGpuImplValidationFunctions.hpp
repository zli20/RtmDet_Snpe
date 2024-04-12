//==============================================================================
// Auto Generated Code for HardSigmodUdoPackage
//==============================================================================

#pragma once

#include "utils/UdoUtil.hpp"

class HardSigmoidGpuValidationFunction : public UdoUtil::ImplValidationFunction {
public:

    HardSigmoidGpuValidationFunction()
            : ImplValidationFunction() {}

    SnpeUdo_ErrorType_t
    validateOperation(SnpeUdo_OpDefinition_t* def) override;
};
