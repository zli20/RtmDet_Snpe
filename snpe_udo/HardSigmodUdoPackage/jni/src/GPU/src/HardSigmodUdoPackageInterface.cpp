//==============================================================================
// Auto Generated Code for HardSigmodUdoPackage
//==============================================================================

#include <memory>
#include <mutex>

#include "GPU/QnnGpuOpPackage.h"
#include "GpuCustomOpPackage.hpp"
#include "QnnOpDef.h"
#include "Operation.hpp"

static std::unique_ptr<OpPackage> sg_opPackage;
static std::mutex sg_mutex;
QnnLog_Callback_t g_callback;
QnnLog_Level_t g_maxLogLevel;

__attribute__((unused)) static Qnn_ErrorHandle_t HardSigmodUdoPackage_initialize(
    QnnOpPackage_GlobalInfrastructure_t globalInfrastructure) {
  std::lock_guard<std::mutex> locker(sg_mutex);

  if (sg_opPackage) {
    return QNN_OP_PACKAGE_ERROR_LIBRARY_ALREADY_INITIALIZED;
  }

  if (!globalInfrastructure) {
    return QNN_OP_PACKAGE_ERROR_LIBRARY_NOT_INITIALIZED;
  }

  auto opPkg = OpPackage::create("HardSigmodUdoPackage", globalInfrastructure->deviceProperties);
  if (!opPkg) {
    return QNN_OP_PACKAGE_ERROR_LIBRARY_NOT_INITIALIZED;
  }

  opPkg->registerOperation(HardSigmoidOperation::s_operationType, HardSigmoidOperation::create);

  sg_opPackage = std::move(opPkg);
  return QNN_SUCCESS;
}

__attribute__((unused)) static Qnn_ErrorHandle_t HardSigmodUdoPackage_getInfo(
    const QnnOpPackage_Info_t** info) {
  if (!sg_opPackage) {
    return QNN_OP_PACKAGE_ERROR_LIBRARY_NOT_INITIALIZED;
  }

  return sg_opPackage->getPackageInfo(info);
}

__attribute__((unused)) static Qnn_ErrorHandle_t HardSigmodUdoPackage_validateOpConfig(
    Qnn_OpConfig_t opConfig) {
  if (!sg_opPackage) {
    return QNN_OP_PACKAGE_ERROR_LIBRARY_NOT_INITIALIZED;
  }
  return sg_opPackage->operationExists(opConfig.v1.typeName);
}

__attribute__((unused)) static Qnn_ErrorHandle_t HardSigmodUdoPackage_createOpImpl(
    QnnOpPackage_GraphInfrastructure_t graphInfrastructure,
    QnnOpPackage_Node_t node,
    QnnOpPackage_OpImpl_t* operation) {
  if (!graphInfrastructure || !node || !operation) {
    return QNN_OP_PACKAGE_ERROR_INVALID_ARGUMENT;
  }

  if (!sg_opPackage) {
    return QNN_OP_PACKAGE_ERROR_LIBRARY_NOT_INITIALIZED;
  }

  return sg_opPackage->createOperation(graphInfrastructure, node, operation);
}

__attribute__((unused)) static Qnn_ErrorHandle_t HardSigmodUdoPackage_freeOpImpl(
    QnnOpPackage_OpImpl_t operation) {
  if (!sg_opPackage) {
    return QNN_OP_PACKAGE_ERROR_LIBRARY_NOT_INITIALIZED;
  }

  return sg_opPackage->freeOperation(operation);
}

__attribute__((unused)) static Qnn_ErrorHandle_t HardSigmodUdoPackage_terminate() {
  sg_opPackage.reset();
  return QNN_SUCCESS;
}

__attribute__((unused)) static Qnn_ErrorHandle_t HardSigmodUdoPackage_logInitialize(
    QnnLog_Callback_t callback, QnnLog_Level_t maxLogLevel) {
  g_callback    = callback;
  g_maxLogLevel = maxLogLevel;
  return QNN_SUCCESS;
}

__attribute__((unused)) static Qnn_ErrorHandle_t HardSigmodUdoPackage_logSetLevel(
    QnnLog_Level_t maxLogLevel) {
  return QNN_SUCCESS;
}

__attribute__((unused)) static Qnn_ErrorHandle_t HardSigmodUdoPackage_logTerminate(void) {
  return QNN_SUCCESS;
}

extern "C" QNN_API Qnn_ErrorHandle_t
HardSigmodUdoPackageInterfaceProvider(QnnOpPackage_Interface_t* interface) {
  interface->interfaceVersion.major = 1;
  interface->interfaceVersion.minor = 4;
  interface->interfaceVersion.patch = 0;
  interface->v1_4.init              = HardSigmodUdoPackage_initialize;
  interface->v1_4.terminate         = HardSigmodUdoPackage_terminate;
  interface->v1_4.getInfo           = HardSigmodUdoPackage_getInfo;
  interface->v1_4.validateOpConfig  = HardSigmodUdoPackage_validateOpConfig;
  interface->v1_4.createOpImpl      = HardSigmodUdoPackage_createOpImpl;
  interface->v1_4.freeOpImpl        = HardSigmodUdoPackage_freeOpImpl;
  interface->v1_4.logInitialize     = HardSigmodUdoPackage_logInitialize;
  interface->v1_4.logSetLevel       = HardSigmodUdoPackage_logSetLevel;
  interface->v1_4.logTerminate      = HardSigmodUdoPackage_logTerminate;
  return QNN_SUCCESS;
}

extern "C" QNN_API Qnn_ErrorHandle_t QnnGpuOpPackage_getKernelBinary(const char* name,
                                                                     const uint8_t** binary,
                                                                     uint32_t* numBytes) {
  (void)name;
  (void)binary;
  (void)numBytes;
  return QNN_OP_PACKAGE_ERROR_UNSUPPORTED_FEATURE;
}
