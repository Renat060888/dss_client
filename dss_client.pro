ROOT_DIR=../

#TEMPLATE = lib
TEMPLATE = app
TARGET = dss_client

include($${ROOT_DIR}pri/common.pri)

CONFIG -= qt
CONFIG += plugin
#CONFIG += release

QMAKE_CXXFLAGS += -Wno-unused-parameter
QMAKE_CXXFLAGS += -Wno-unused-variable

DEFINES += \
    SWITCH_LOGGER_SIMPLE \

LIBS += \
    -lpthread \
    -ljsoncpp \
    -lmicroservice_common \

# NOTE: paths for dev environment ( all projects sources in one dir )
INCLUDEPATH +=  \
    $${ROOT_DIR}/microservice_common/ \

SOURCES += \
        commands/cmd_context_close.cpp \
        commands/cmd_context_open.cpp \
        commands/cmd_ping.cpp \
        commands/cmd_register.cpp \
        commands/i_command.cpp \
        dss_client.cpp \
        handler_node_real.cpp \
        handler_node_simula.cpp \
        handler_player.cpp \
        main.cpp

HEADERS += \
    commands/cmd_context_close.h \
    commands/cmd_context_open.h \
    commands/cmd_ping.h \
    commands/cmd_register.h \
    commands/i_command.h \
    common_types_private.h \
    common_types_public.h \
    common_utils.h \
    common_vars.h \
    dss_client.h \
    handler_node_real.h \
    handler_node_simula.h \
    handler_player.h
