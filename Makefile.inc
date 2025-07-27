mkfile_path := $(dir $(lastword $(MAKEFILE_LIST)))

# Shellスクリプトで、app,unitディレクトリ中のソースコード名をオブジェクトファイル名に変換している
APPL_CXXOBJS += $(shell ls ${mkfile_path}unit/*.cpp | sed -r 's/.*\/(.*)\.cpp/\1.o/g')
APPL_CXXOBJS += $(shell ls ${mkfile_path}app/*.cpp | sed -r 's/.*\/(.*)\.cpp/\1.o/g')

SRCLANG := c++

ifdef CONFIG_EV3RT_APPLICATION

# Include libraries
include $(EV3RT_SDK_LIB_DIR)/libcpp-spike/Makefile

endif

APPL_DIRS += $(mkfile_path)app $(mkfile_path)unit

INCLUDES += \
    -I$(mkfile_path)app \
    -I$(mkfile_path)unit
