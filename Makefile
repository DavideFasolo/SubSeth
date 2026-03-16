RACK_DIR ?= ../Rack-SDK

SOURCES += src/plugin.cpp
SOURCES += src/UPolM.cpp
SOURCES += src/shared/SubPorts.cpp

DISTRIBUTABLES += res

include $(RACK_DIR)/plugin.mk
