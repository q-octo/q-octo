#pragma once

#include "robot_state_generated.h"
#include "flatbuffers/flatbuffers.h"
#include "task_data_manager.h"
using namespace fbs;

void stateToFlatbuffer(const TaskMessage::State &robotState, RobotT &robot);