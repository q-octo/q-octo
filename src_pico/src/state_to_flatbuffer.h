#pragma once

#include "task_data_manager.h"
#include "robot_state_generated.h"
#include "flatbuffers/flatbuffers.h"
using namespace fbs;

void stateToFlatbuffer(const DataManager::State &robotState, RobotT &robot);