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
SOURCES += src/UMat1604.cpp
SOURCES += src/UMat1601E.cpp
SOURCES += src/UMat1601.cpp

SOURCES += src/UB20.cpp
DISTRIBUTABLES += res

include $(RACK_DIR)/plugin.mk
