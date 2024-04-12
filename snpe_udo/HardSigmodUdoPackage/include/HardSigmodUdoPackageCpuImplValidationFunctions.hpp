//==============================================================================
// Auto Generated Code for HardSigmodUdoPackage
//==============================================================================

#pragma once

#include "utils/UdoUtil.hpp"

class HardSigmoidCpuValidationFunction : public UdoUtil::ImplValidationFunction {
public:

    HardSigmoidCpuValidationFunction()
            : ImplValidationFunction() {}

    SnpeUdo_ErrorType_t
    validateOperation(SnpeUdo_OpDefinition_t* def) override;
};
