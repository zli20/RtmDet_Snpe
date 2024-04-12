//==============================================================================
// Auto Generated Code for HardSigmodUdoPackage
//==============================================================================

#pragma once

#include "utils/UdoUtil.hpp"

class HardSigmoidDspValidationFunction : public UdoUtil::ImplValidationFunction {
public:

    HardSigmoidDspValidationFunction()
            : ImplValidationFunction() {}

    SnpeUdo_ErrorType_t
    validateOperation(SnpeUdo_OpDefinition_t* def) override;
};
