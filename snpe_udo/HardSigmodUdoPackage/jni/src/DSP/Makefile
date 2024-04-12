#================================================================================
# Auto Generated Code for HardSigmodUdoPackage
#================================================================================

# define relevant directories
SRC_DIR := ./

# NOTE:
# - this Makefile is going to be used only to create DSP skels, so no need for android.min


HEXAGON_SDK_VERSION = 3.5.2
HEXAGON_TOOLS_VERSION = 8.3.07
ifndef HEXAGON_SDK_ROOT
$(error "HEXAGON_SDK_ROOT needs to be defined to compile a dsp library. Please set HEXAGON_SDK_ROOT to hexagon sdk installation.(Supported Version :$(HEXAGON_SDK_VERSION))"
endif

ifndef HEXAGON_TOOLS_ROOT
$(error "HEXAGON_TOOLS_ROOT needs to be defined to compile a dsp library. Please set HEXAGON_TOOLS_ROOT to HEXAGON_SDK_ROOT/tools/HEXAGON_Tools/$(HEXAGON_TOOLS_VERSION)")
endif


ifndef SDK_SETUP_ENV
$(error "SDK_SETUP_ENV needs to be defined to compile a dsp library. Please set SDK_SETUP_ENV=Done")
endif

# define variant as V=hexagon_Release_dynamic_toolv83_v60 - it can be hardcoded too
ifndef V
V = hexagon_Release_dynamic_toolv83_v60
endif

V_TARGET = $(word 1,$(subst _, ,$(V)))
ifneq ($(V_TARGET),hexagon)
$(error Unsupported target '$(V_TARGET)' in variant '$(V)')
endif

# define package include paths and check API header path
# set package include paths, note package root will take precedence
# if includes are already present in package
UDO_PACKAGE_ROOT =/home/zli/snpe/snpe-2.10.0.4541/udo/HardSigmodUdoPackage
PKG_NAME = HardSigmodUdoPackage

# must list all variants supported by this project
SUPPORTED_VS = $(default_VS)

# must list all the dependencies of this project
DEPENDENCIES = ATOMIC RPCMEM TEST_MAIN TEST_UTIL

# each dependency needs a directory definition
#  the form is <DEPENDENCY NAME>_DIR
#  for example:
#    DEPENDENCIES = FOO
#    FOO_DIR = $(HEXAGON_SDK_ROOT)/examples/common/foo
#

ATOMIC_DIR = $(HEXAGON_SDK_ROOT)/libs/common/atomic
RPCMEM_DIR = $(HEXAGON_SDK_ROOT)/libs/common/rpcmem
TEST_MAIN_DIR = $(HEXAGON_SDK_ROOT)/test/common/test_main
TEST_UTIL_DIR = $(HEXAGON_SDK_ROOT)/test/common/test_util

include $(HEXAGON_SDK_ROOT)/build/make.d/$(V_TARGET)_vs.min
include $(HEXAGON_SDK_ROOT)/build/defines.min

# set include paths as compiler flags
CC_FLAGS += -I $(UDO_PACKAGE_ROOT)/include

# if SNPE_ROOT is set and points to the SDK path, it will be used. Otherwise ZDL_ROOT will be assumed to point
# to a build directory
ifdef SNPE_ROOT
CC_FLAGS += -I $(SNPE_ROOT)/include/zdl
else ifdef ZDL_ROOT
CC_FLAGS += -I $(ZDL_ROOT)/x86_64-linux-clang/include/zdl
else
$(error SNPE_ROOT: Please set SNPE_ROOT or ZDL_ROOT to obtain Udo headers necessary to compile the package)
endif

ifndef QNN_INCLUDE
$(info "INFO: Qnn include not explicitly defined, attempting to use QNN_SDK_ROOT if it is valid")
QNN_INCLUDE := $(QNN_SDK_ROOT)/include
endif
ifeq ($(wildcard $(QNN_INCLUDE)),)
$(error "ERROR: QNN_INCLUDE path is not set. QNN include paths must be set to obtain BE headers necessary to compile the package")
endif

ifndef PACKAGE_NAME
PACKAGE_NAME := $(notdir $(shell pwd))
$(info "INFO: No package name defined. Using current directory name: $(PACKAGE_NAME) as the package name")
endif

# set include paths as compiler flags
CC_FLAGS += -I $(SRC_DIR)/include
CC_FLAGS += -I $(QNN_INCLUDE)/

# only build the shared object if dynamic option specified in the variant
ifeq (1,$(V_dynamic))
BUILD_DLLS = libUdoHardSigmodUdoPackageImplDsp
endif

OP_SOURCES = $(wildcard $(SRC_DIR)/src/ops/*.cpp)
OTHER_SOURCES = $(wildcard $(SRC_DIR)/src/*.cpp)

# sources for the DSP implementation library in src directory
libUdoHardSigmodUdoPackageImplDsp.C_SRCS := $(wildcard $(OP_SOURCES)) $(OTHER_SOURCES)


# copy final build products to the ship directory
BUILD_COPIES = $(DLLS) $(EXES) $(LIBS) $(UDO_PACKAGE_ROOT)/libs/dsp_v60/

# always last
include $(RULES_MIN)

# define destination library directory, and copy files into lib/dsp
# this code will create it
SHIP_LIBS_DIR   := $(CURDIR)/$(V)
LIB_DIR         := $(UDO_PACKAGE_ROOT)/libs/dsp_v60
OBJ_DIR         := $(UDO_PACKAGE_ROOT)/obj/local/dsp_v60

.PHONY: dsp

dsp: tree
	mkdir -p ${OBJ_DIR};  \
	cp -Rf ${SHIP_LIBS_DIR}/. ${OBJ_DIR} ;\
	rm -rf ${SHIP_LIBS_DIR};


