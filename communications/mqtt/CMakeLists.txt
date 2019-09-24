cmake_minimum_required (VERSION 3.5)

set(MQTT_LIB_FILES
    src/MQTTClient.cpp
    src/MQTTConnectPacket.cpp
    src/MQTTConnectAckPacket.cpp
    src/MQTTDisconnectPacket.cpp
    src/MQTTFactory.cpp
    src/MQTTInstance.cpp
    src/MQTTPacket.cpp
    src/MQTTPacketOptions.cpp
    src/MQTTPingRequestPacket.cpp
    src/MQTTPingResponsePacket.cpp
    src/MQTTPublishPacket.cpp
    src/MQTTPublishAckPacket.cpp
    src/MQTTPublishCompletedPacket.cpp
    src/MQTTPublishReceivedPacket.cpp
    src/MQTTPublishReleasedPacket.cpp
    src/MQTTServer.cpp
    src/MQTTSubscribePacket.cpp
    src/MQTTSubscribeAckPacket.cpp
    src/MQTTTrackedPacket.cpp
    src/MQTTUnsubscribePacket.cpp
    src/MQTTUnsubscribeAckPacket.cpp
)

set(MQTT_SRC_FILES
    test/main.cpp
)

include_directories (
    .
    internal
    ${CMAKE_SOURCE_DIR}
    ${JSON_INCLUDE}
    ${SOCKET_INCLUDE}
    ${COMMON_INCLUDE}
)

link_directories (
    ${LIB_DIR}
    ${SOCKET_LIB}
    ${COMMON_LIB}
)

add_library (MQTT ${MQTT_LIB_FILES})
add_executable (mqttClientTest ${MQTT_SRC_FILES})

target_link_libraries(
    mqttClientTest
    MQTT
    AFMSocket
    AFMCommon
    pthread
)

install (TARGETS mqttClientTest DESTINATION ${BIN_DIR})

