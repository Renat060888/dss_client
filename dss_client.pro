ROOT_DIR=../

TEMPLATE = lib
#TEMPLATE = app
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
    -lrabbitmq \
    -ljsoncpp \

# NOTE: paths for dev environment ( all projects sources in one dir )
INCLUDEPATH +=  \
    $${PWD}/from_ms_common/ \

SOURCES += \
    commands/dss_core/cmd_core_context_close.cpp \
    commands/dss_core/cmd_core_context_open.cpp \
    commands/dss_core/cmd_core_ping.cpp \
    commands/dss_core/cmd_core_register.cpp \
    commands/i_command.cpp \
    commands/node/cmd_node_start.cpp \
    commands/player/cmd_play_from_pos.cpp \
    commands/player/cmd_play_live.cpp \
    commands/player/cmd_play_loop.cpp \
    commands/player/cmd_play_pause.cpp \
    commands/player/cmd_play_reverse.cpp \
    commands/player/cmd_play_speed.cpp \
    commands/player/cmd_play_start.cpp \
    commands/player/cmd_play_step.cpp \
    commands/player/cmd_play_stop.cpp \
    commands/player/cmd_player_context_close.cpp \
    commands/player/cmd_player_context_open.cpp \
    commands/player/cmd_player_ping.cpp \
    commands/player/cmd_player_register.cpp \
    dss_client.cpp \
    fakeobjrepr.cpp \
    handler_node_real.cpp \
    handler_node_simula.cpp \
    main.cpp \
    player_handler.cpp \
    from_ms_common/communication/amqp_client_c.cpp \
    from_ms_common/communication/amqp_controller.cpp \
    from_ms_common/communication/network_interface.cpp \
    from_ms_common/system/logger_astra.cpp \
    from_ms_common/system/logger_normal.cpp \
    from_ms_common/system/logger_simple.cpp

HEADERS += \
    commands/dss_core/cmd_core_context_close.h \
    commands/dss_core/cmd_core_context_open.h \
    commands/dss_core/cmd_core_ping.h \
    commands/dss_core/cmd_core_register.h \
    commands/i_command.h \
    commands/node/cmd_node_start.h \
    commands/player/cmd_play_from_pos.h \
    commands/player/cmd_play_live.h \
    commands/player/cmd_play_loop.h \
    commands/player/cmd_play_pause.h \
    commands/player/cmd_play_reverse.h \
    commands/player/cmd_play_speed.h \
    commands/player/cmd_play_start.h \
    commands/player/cmd_play_step.h \
    commands/player/cmd_play_stop.h \
    commands/player/cmd_player_context_close.h \
    commands/player/cmd_player_context_open.h \
    commands/player/cmd_player_ping.h \
    commands/player/cmd_player_register.h \
    common_types_private.h \
    common_types_public.h \
    common_utils.h \
    common_vars.h \
    dss_client.h \
    fakeobjrepr.h \
    handler_node_real.h \
    handler_node_simula.h \
    player_handler.h \
    from_ms_common/common/ms_common_types.h \
    from_ms_common/common/ms_common_utils.h \
    from_ms_common/communication/amqp_client_c.h \
    from_ms_common/communication/amqp_controller.h \
    from_ms_common/communication/network_interface.h \
    from_ms_common/system/logger_astra.h \
    from_ms_common/system/logger_common.h \
    from_ms_common/system/logger_normal.h \
    from_ms_common/system/logger_simple.h \
    from_ms_common/system/logger.h
