LOCAL_PATH := $(call my-dir)
SUPPORTED_TARGET_ABI := arm64-v8a armeabi-v7a x86 x86_64

#============================ Verify Target Info and Application Variables =========================================
ifneq ($(filter $(TARGET_ARCH_ABI),$(SUPPORTED_TARGET_ABI)),)
    ifneq ($(APP_STL), c++_shared)
        $(error Unsupported APP_STL: "$(APP_STL)")
    endif
else
    $(error Unsupported TARGET_ARCH_ABI: '$(TARGET_ARCH_ABI)')
endif

#============================ Define Common Variables ===============================================================
PACKAGE_C_INCLUDES        := /home/zli/snpe/snpe-2.10.0.4541/udo/HardSigmodUdoPackage/include
PACKAGE_C_INCLUDES += /home/zli/snpe/snpe-2.10.0.4541/udo/HardSigmodUdoPackage/include/utils
# if SNPE_ROOT is set and points to the SDK path, it will be used. Otherwise the build environment variable ZDL_ROOT
# will be used
ifdef SNPE_ROOT
PACKAGE_C_INCLUDES += -I $(SNPE_ROOT)/include/zdl
else ifdef ZDL_ROOT
PACKAGE_C_INCLUDES += -I $(ZDL_ROOT)/x86_64-linux-clang/include/zdl
else
$(error SNPE_ROOT: Please set SNPE_ROOT or ZDL_ROOT to obtain Udo headers necessary to compile the package)
endif

#============================ Define GPU Conditional Common Variables =================================================
ifdef CL_INCLUDE_PATH
PACKAGE_C_INCLUDES        += $(CL_INCLUDE_PATH) # Should be the location such that CL/cl.h is accessible
endif
ifndef CL_LIBRARY_PATH
CL_LIBRARY_PATH           := # Note: should be the location where libOpenCL.so is discoverable
endif

#========================== Define Registration Library Build Variables =============================================
include $(CLEAR_VARS)
LOCAL_C_INCLUDES               := $(PACKAGE_C_INCLUDES)
MY_SRC_FILES                    = $(wildcard $(LOCAL_PATH)/src/reg/*.cpp) $(wildcard $(LOCAL_PATH)/src/utils/*.cpp)
LOCAL_MODULE                   := UdoHardSigmodUdoPackageReg
LOCAL_SRC_FILES                := $(subst jni/,,$(MY_SRC_FILES))
#LOCAL_CPP_FEATURES            := rtti exceptions
LOCAL_LDLIBS                   := -lGLESv2 -lEGL
include $(BUILD_SHARED_LIBRARY)

#========================== Define Cpu Library Build Variables ==================================
include $(CLEAR_VARS)
LOCAL_C_INCLUDES               := $(PACKAGE_C_INCLUDES)
LOCAL_MODULE                   := UdoHardSigmodUdoPackageImplCpu
MY_SRC_FILES                    = $(wildcard $(LOCAL_PATH)/src/CPU/*.cpp) $(wildcard $(LOCAL_PATH)/src/utils/*.cpp)
LOCAL_SRC_FILES                := $(subst jni/,,$(MY_SRC_FILES))
#LOCAL_CPP_FEATURES            := rtti exceptions
LOCAL_LDLIBS                   := -lGLESv2 -lEGL
include $(BUILD_SHARED_LIBRARY)

#========================== Define Gpu Library Build Variables ==================================
include $(CLEAR_VARS)
LOCAL_C_INCLUDES               := $(PACKAGE_C_INCLUDES)
LOCAL_MODULE                   := UdoHardSigmodUdoPackageImplGpu
MY_SRC_FILES                    = $(wildcard $(LOCAL_PATH)/src/GPU/*.cpp) $(wildcard $(LOCAL_PATH)/src/utils/*.cpp)
# MY_SRC_FILES                   += $(wildcard $(LOCAL_PATH)/src/utils/GPU/*.cpp)
LOCAL_SRC_FILES                := $(subst jni/,,$(MY_SRC_FILES))
#LOCAL_CPP_FEATURES            := rtti exceptions
LOCAL_LDLIBS                   := -lGLESv2 -lEGL
# LOCAL_SHARED_LIBRARIES         := libOpenCL
include $(BUILD_SHARED_LIBRARY)

# ifeq ($(APP_ALLOW_MISSING_DEPS),false)
# #============================ Include Runtime Specific Shared Libraries  =================================================
# include $(CLEAR_VARS)
# LOCAL_MODULE                  := libOpenCL
# LOCAL_SRC_FILES               := $(CL_LIBRARY_PATH)/libOpenCL.so # Note LibOpenCL is target-specific.
# include $(PREBUILT_SHARED_LIBRARY)
# endif

