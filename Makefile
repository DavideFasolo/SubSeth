RACK_DIR ?= ../Rack-SDK

SOURCES += src/plugin.cpp
SOURCES += src/UPolM.cpp
SOURCES += src/UPolS.cpp
SOURCES += src/UMult4.cpp
SOURCES += src/UMult4L.cpp
SOURCES += src/UMult6.cpp
SOURCES += src/UMult6L.cpp
SOURCES += src/UMult8.cpp
SOURCES += src/UMult8L.cpp
SOURCES += src/UMult12.cpp
SOURCES += src/UMult14.cpp
SOURCES += src/shared/SubPorts.cpp

DISTRIBUTABLES += res

SOURCES += src/UMat1604.cpp
include $(RACK_DIR)/plugin.mk
