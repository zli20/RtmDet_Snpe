//==============================================================================
// Auto Generated Code for HardSigmodUdoPackage
//==============================================================================
#include "QnnCpuOpPackage.h"
#include "CustomOpPackage.hpp"

using namespace qnn::custom;
using namespace qnn::custom::macros;

static Qnn_ErrorHandle_t HardSigmodUdoPackageInitialize(
  QnnOpPackage_GlobalInfrastructure_t globalInfrastructure) {

  QNN_CUSTOM_BE_ENSURE(!(CustomOpPackage::getIsInitialized()),QNN_OP_PACKAGE_ERROR_LIBRARY_ALREADY_INITIALIZED);

  INIT_BE_OP_PACKAGE(HardSigmodUdoPackage)

  REGISTER_PACKAGE_OP(HardSigmoid)

  // INIT_BE_PACKAGE_OPTIMIZATIONS();

  CustomOpPackage::setIsInitialized(true);

  return QNN_SUCCESS;
}

static Qnn_ErrorHandle_t HardSigmodUdoPackageGetInfo(const QnnOpPackage_Info_t** info) {
  auto opPkg = CustomOpPackage::getInstance();

  QNN_CUSTOM_BE_ENSURE(opPkg, QNN_OP_PACKAGE_ERROR_LIBRARY_NOT_INITIALIZED);

  QNN_CUSTOM_BE_ENSURE_STATUS(opPkg->getPackageInfo(info));

  return QNN_SUCCESS;
}

static Qnn_ErrorHandle_t HardSigmodUdoPackageValidateOpConfig(Qnn_OpConfig_t opConfig) {
  auto opPkg = CustomOpPackage::getInstance();

  QNN_CUSTOM_BE_ENSURE(opPkg, QNN_OP_PACKAGE_ERROR_LIBRARY_NOT_INITIALIZED);

  auto opRegistration = opPkg->getOpRegistration(opConfig.v1.typeName);

  QNN_CUSTOM_BE_ENSURE(opRegistration, QNN_OP_PACKAGE_ERROR_VALIDATION_FAILURE)

  QNN_CUSTOM_BE_ENSURE_STATUS(opRegistration->validateOpConfig(opConfig));

return QNN_SUCCESS;
}

static Qnn_ErrorHandle_t HardSigmodUdoPackageCreateOpImpl(
   QnnOpPackage_GraphInfrastructure_t graphInfrastructure,
   QnnOpPackage_Node_t node,
   QnnOpPackage_OpImpl_t* opImpl) {
  auto opPkg = CustomOpPackage::getInstance();

  QNN_CUSTOM_BE_ENSURE(opPkg, QNN_OP_PACKAGE_ERROR_LIBRARY_NOT_INITIALIZED);

  QNN_CUSTOM_BE_ENSURE_STATUS(
    opPkg->createOpImpl(graphInfrastructure, node, opImpl));

  return QNN_SUCCESS;
}

static Qnn_ErrorHandle_t HardSigmodUdoPackageFreeOpImpl(
   QnnCpuOpPackage_OpImpl_t* opImpl) {
  auto opPkg = CustomOpPackage::getInstance();

  QNN_CUSTOM_BE_ENSURE(opPkg, QNN_OP_PACKAGE_ERROR_LIBRARY_NOT_INITIALIZED);

  QNN_CUSTOM_BE_ENSURE_STATUS(opPkg->freeOpImpl(opImpl));

  return QNN_SUCCESS;
}

static Qnn_ErrorHandle_t HardSigmodUdoPackageTerminate() {
  auto opPkg = CustomOpPackage::getInstance();

  CustomOpPackage::destroyInstance();
  opPkg->freeResolver();

  return QNN_SUCCESS;
}

static Qnn_ErrorHandle_t HardSigmodUdoPackageLogInitialize(
QnnLog_Callback_t callback, QnnLog_Level_t maxLogLevel) {
// function should be used if at least two backends support it
// USER SHOULD NOTE THIS FUNCTION IS UNUSED BY BE

  return QNN_SUCCESS;
}

static Qnn_ErrorHandle_t HardSigmodUdoPackageLogSetLevel(
QnnLog_Level_t maxLogLevel) {
// USER SHOULD NOTE THIS FUNCTION IS UNUSED BY CPU BE

return QNN_SUCCESS;
}

static Qnn_ErrorHandle_t HardSigmodUdoPackageLogTerminate() {
// USER SHOULD NOTE THIS FUNCTION IS UNUSED BY CPU BE

  return QNN_SUCCESS;
}


extern "C" QNN_API Qnn_ErrorHandle_t HardSigmodUdoPackageInterfaceProvider(
   QnnOpPackage_Interface_t* interface) {
  interface->interfaceVersion.major = 1;
  interface->interfaceVersion.minor = 4;
  interface->interfaceVersion.patch = 0;
  interface->v1_4.init              = HardSigmodUdoPackageInitialize;
  interface->v1_4.terminate         = HardSigmodUdoPackageTerminate;
  interface->v1_4.getInfo           = HardSigmodUdoPackageGetInfo;
  interface->v1_4.validateOpConfig  = HardSigmodUdoPackageValidateOpConfig;
  interface->v1_4.createOpImpl     =  HardSigmodUdoPackageCreateOpImpl;
  interface->v1_4.freeOpImpl        = HardSigmodUdoPackageFreeOpImpl;
  interface->v1_4.logInitialize     = HardSigmodUdoPackageLogInitialize;
  interface->v1_4.logSetLevel       = HardSigmodUdoPackageLogSetLevel;
  interface->v1_4.logTerminate      = HardSigmodUdoPackageLogTerminate;
  return QNN_SUCCESS;
}

