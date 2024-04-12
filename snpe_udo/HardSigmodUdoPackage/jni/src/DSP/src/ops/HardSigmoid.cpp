//==============================================================================
// Auto Generated Code for HardSigmodUdoPackage
//==============================================================================

#include <string.h>
#include <stdlib.h>

#include "DSP/QnnDspOpPackage.h"
#include "DspOps.hpp"

// operations info
char g_hardsigmoidOpType [] = "HardSigmoid";
uint32_t g_hardsigmoidStaticParamsNum = 0;
uint32_t g_hardsigmoidInputsNum = 1;
uint32_t g_hardsigmoidOutputsNum = 1;
Udo_QuantizationType_t g_hardsigmoidInputQuantizationTypes [] = {UDO_QUANTIZATION_TF};
Udo_QuantizationType_t g_hardsigmoidOutputQuantizationTypes [] =  {UDO_QUANTIZATION_TF};
Udo_HexNNTensorLayout_t* g_hardsigmoidLayout = NULL;

Udo_ErrorType_t
hardsigmoid_createOpFactory (QnnOpPackage_GlobalInfrastructure_t globalInfra,
    Udo_CoreType_t udoCoreType, void *perFactoryInfrastructure,
    Udo_String_t operationType, uint32_t numOfStaticParams,
    Udo_Param_t *staticParams, Udo_OpFactory_t *opFactory)
{
    if(operationType == NULL || opFactory == NULL) {
        return UDO_INVALID_ARGUMENT;
    }
    if(strcmp(operationType, g_hardsigmoidOpType) == 0) {
        hardsigmoidOpFactory_t* thisFactory = (hardsigmoidOpFactory_t *)(*(globalInfra->dspGlobalInfra->hexNNv2Infra.udoMalloc))(sizeof(hardsigmoidOpFactory_t));
        int size = strlen(operationType) + 1; // +1 to hold the '\0' character
        thisFactory->opType = (Udo_String_t)(*(globalInfra->dspGlobalInfra->hexNNv2Infra.udoMalloc))(size);
        strlcpy((thisFactory->opType), operationType, size);
        thisFactory->numOfStaticParams = numOfStaticParams;
        /*
         * if this op has static params, add code here
         */
        *opFactory = (Udo_OpFactory_t)thisFactory;
    } else {
        return UDO_INVALID_ARGUMENT;
    }
    return UDO_NO_ERROR;
}

Udo_ErrorType_t
hardsigmoid_releaseOpFactory(QnnOpPackage_GlobalInfrastructure_t globalInfra,
                                              Udo_OpFactory_t opFactory)
{
    if(opFactory == NULL) {
        return UDO_INVALID_ARGUMENT;
    }
    hardsigmoidOpFactory_t* thisFactory = (hardsigmoidOpFactory_t *)(opFactory);
    (*(globalInfra->dspGlobalInfra->hexNNv2Infra.udoFree))((thisFactory->opType));
    (*(globalInfra->dspGlobalInfra->hexNNv2Infra.udoFree))(thisFactory);
    /*
     * if this op has static params, add code here
     */
    return UDO_NO_ERROR;
}

Udo_ErrorType_t
hardsigmoid_validateOperation (Udo_String_t operationType, uint32_t numOfStaticParams,
    const Udo_Param_t *staticParams) {
    if(strcmp(operationType, g_hardsigmoidOpType) == 0) {
        if (numOfStaticParams != g_hardsigmoidStaticParamsNum) {
            return UDO_INVALID_ARGUMENT;
        }
        /*
         * If this op should validate others, add code here
         */
    } else {
        return UDO_INVALID_ARGUMENT;
    }
    return UDO_NO_ERROR;
}

Udo_ErrorType_t hardsigmoid_executeOp(QnnOpPackage_GlobalInfrastructure_t globalInfra,
                                      Udo_Operation_t operation, bool blocking, const uint32_t ID,
                                      Udo_ExternalNotify_t notifyFunc) {
    if (operation == NULL) {
        return UDO_INVALID_ARGUMENT;
    }
    OpParams_t *m_Operation = (OpParams_t *)operation;
    const char *opType = ((hardsigmoidOpFactory_t *)(m_Operation->opFactory))->opType;
    if (opType == NULL) {
        return UDO_INVALID_ARGUMENT;
    }
    if (strcmp(opType, g_hardsigmoidOpType) == 0) {
        Udo_TensorParam_t *input = &(m_Operation->InputParams[0]);
        Udo_TensorParam_t *out = m_Operation->outputParams;

        if (input->layout == UDO_LAYOUT_NULL || out->layout == UDO_LAYOUT_NULL) {
            return UDO_UNSUPPORTED_FEATURE;
        }

        uint32_t inputLen = sizeof(uint8_t);
        for (int k = 0; k < input->tensorRank; k++) {
            inputLen *= input->currDimensions[k];
            out->currDimensions[k] = input->currDimensions[k];
        }

        float inputMin = input->quantizeParams.TFParams.minValue;
        float inputMax = input->quantizeParams.TFParams.maxValue;

        float outputMin = out->quantizeParams.TFParams.minValue;
        float outputMax = out->quantizeParams.TFParams.maxValue;

        uint8_t *inputTensorData = (uint8_t *)(input->tensorData);
        uint8_t *outputTensorData = (uint8_t *)(out->tensorData);

        out->dataType = UDO_DATATYPE_FIXED_8;
        // required to set output tensor sizes
        if ((*(globalInfra->dspGlobalInfra->hexNNv2Infra.udoSetOutputTensorSize))(m_Operation->opInfra, 0, inputLen) != 0) {
            return UDO_UNSUPPORTED_FEATURE;
        }

        // Perform Hard Sigmoid operation
        for (uint32_t i = 0; i < inputLen; i++) {
            float x = (inputTensorData[i] - inputMin) / (inputMax - inputMin); // Normalize input
            float result = fmin(1.0f, fmax(0.0f, 0.2f * x + 0.5f));             // Hard Sigmoid operation
            outputTensorData[i] = (uint8_t)(result * 255);                       // Quantize output
        }

        return UDO_NO_ERROR;
    } else {
        return UDO_INVALID_ARGUMENT;
    }
}


Udo_ErrorType_t hardsigmoid_queryOperation (
    Udo_String_t operationType, uint32_t numOfStaticParams,
    const Udo_Param_t *staticParams, uint32_t *numOfInputs,
    Udo_QuantizationType_t **inputsQuantTypes,
    Udo_HexNNTensorLayout_t **inputsLayouts, uint32_t *numOfOutputs,
    Udo_QuantizationType_t **outputsQuantTypes,
    Udo_HexNNTensorLayout_t **outputsLayouts) {
    if(strcmp(operationType, g_hardsigmoidOpType) == 0) {
        *numOfInputs = g_hardsigmoidInputsNum;
        *inputsQuantTypes = g_hardsigmoidInputQuantizationTypes;
        *inputsLayouts = g_hardsigmoidLayout;
        *numOfOutputs = g_hardsigmoidOutputsNum;
        *outputsQuantTypes = g_hardsigmoidOutputQuantizationTypes;
        *outputsLayouts = g_hardsigmoidLayout;
    } else {
        return UDO_WRONG_OPERATION;
    }
    return UDO_NO_ERROR;
}

UdoDspShared* new_hardsigmoid(QnnOpPackage_GlobalInfrastructure_t globalInfra) {
    UdoDspShared* pOpObj = (UdoDspShared*)(*(globalInfra->dspGlobalInfra->hexNNv2Infra.udoMalloc))(sizeof(UdoDspShared));
    if (pOpObj == NULL) {
        return NULL;
    }
    pOpObj->opType = g_hardsigmoidOpType;
    pOpObj->numOfStaticParams = g_hardsigmoidStaticParamsNum;
    pOpObj->numOfInputs = g_hardsigmoidInputsNum;
    pOpObj->numOfOutputs = g_hardsigmoidOutputsNum;

    pOpObj->createOpFactory = hardsigmoid_createOpFactory;
    pOpObj->releaseOpFactory = hardsigmoid_releaseOpFactory;
    pOpObj->validateOp = hardsigmoid_validateOperation;
    pOpObj->executeOp = hardsigmoid_executeOp;
    pOpObj->queryOp = hardsigmoid_queryOperation;
    return pOpObj;
}

Udo_ErrorType_t free_hardsigmoid(QnnOpPackage_GlobalInfrastructure_t globalInfra, UdoDspShared* opInfo) {
    if (opInfo == NULL) {
        return UDO_NO_ERROR;
    }
    (*(globalInfra->dspGlobalInfra->hexNNv2Infra.udoFree))(opInfo);
    return UDO_NO_ERROR;
}
