/*
    Code produced for the Q-Octo project.

    Copyright (c) 2024 Britannio Jarrett, <oss@britannio.dev>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#pragma once

#include "task_data_manager.h"

class SystemStatus {
public:


  static DataManager::Status getStatus();

  static void receiveDataManagerMessage(const DataManager::Message &message);

private:
  static inline bool lowBattery;
  static inline bool noTxSignal;
  // TODO set these fields
  static inline bool motorError;
  static inline bool disarmed;
};
