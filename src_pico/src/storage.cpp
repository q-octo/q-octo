#include "storage.h"
#include "task_rc.h"

void Storage::init()
{
    EEPROM.begin(sizeof(State));
    // TODO remove this workaround as it makes this system redundant
    // We shuold figure out how to check if this is the first read or not
    save();
    EEPROM.get(0, state);
}

void Storage::save()
{
    EEPROM.put(0, state);
    EEPROM.commit();
    // Seeing this too often? The EEPROM has a limited number of writes so an
    // effort should be made to reduce the number of writes.
    Serial.println("Saved state to EEPROM");
    notifyStateUpdate();
}

void Storage::notifyStateUpdate()
{
    DataManager::Message message = {.type = DataManager::Type::STORAGE_UPDATE};
    DataManager::receiveMessage(message);
}