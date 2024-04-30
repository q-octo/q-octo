cmake_minimum_required(VERSION 3.28)
project(isolated C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(isolated
        canard.c
        canard.h
        canard_internals.h
        main.c
        uavcan.equipment.power.BatteryInfo.c
        uavcan.equipment.power.BatteryInfo.h
        uavcan.protocol.dynamic_node_id.Allocation.c
        uavcan.protocol.dynamic_node_id.Allocation.h
        uavcan.protocol.NodeStatus.c
        uavcan.protocol.NodeStatus.h
)
