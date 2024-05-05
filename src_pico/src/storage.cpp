#include "storage.h"
#include "task_rc.h"

void Storage::init()
{
    const size_t size = sizeof(State);
    if (size > 4096) {
        Serial.println("[ERROR] State struct cannot fit in EEPROM");
        return;
    }
    EEPROM.begin(size);
    // TODO remove this workaround as it makes this system redundant
    // We shuold figure out how to check if this is the first read or not
    save();
    EEPROM.get(0, state);
}

void Storage::save()
{
    EEPROM.put(0, state);
    if (EEPROM.commit())
    {
        // Seeing this too often? The EEPROM has a limited number of writes so an
        // effort should be made to reduce the number of writes.
        Serial.println("Saved state to EEPROM");
        notifyStateUpdate();
    }
    else
    {
        Serial.println("Failed to save state to EEPROM");
    }
}

void Storage::notifyStateUpdate()
{
    DataManager::Message message = {.type = DataManager::Type::STORAGE_UPDATE};
    DataManager::receiveMessage(message);
}