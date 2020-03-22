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
        commands/node/cmd_node_start.cpp \
        commands/player/cmd_play_from_pos.cpp \
        commands/player/cmd_play_loop.cpp \
        commands/player/cmd_play_pause.cpp \
        commands/player/cmd_play_realtime.cpp \
        commands/player/cmd_play_reverse.cpp \
        commands/player/cmd_play_speed.cpp \
        commands/player/cmd_play_start.cpp \
        commands/player/cmd_play_step.cpp \
        commands/player/cmd_play_stop.cpp \
        dss_client.cpp \
        handler_node_real.cpp \
        handler_node_simula.cpp \
        handler_player.cpp \
        main.cpp \
        player_handler.cpp

HEADERS += \
    commands/cmd_context_close.h \
    commands/cmd_context_open.h \
    commands/cmd_ping.h \
    commands/cmd_register.h \
    commands/i_command.h \
    commands/node/cmd_node_start.h \
    commands/player/cmd_play_from_pos.h \
    commands/player/cmd_play_loop.h \
    commands/player/cmd_play_pause.h \
    commands/player/cmd_play_realtime.h \
    commands/player/cmd_play_reverse.h \
    commands/player/cmd_play_speed.h \
    commands/player/cmd_play_start.h \
    commands/player/cmd_play_step.h \
    commands/player/cmd_play_stop.h \
    common_types_private.h \
    common_types_public.h \
    common_utils.h \
    common_vars.h \
    dss_client.h \
    handler_node_real.h \
    handler_node_simula.h \
    handler_player.h \
    player_handler.h
