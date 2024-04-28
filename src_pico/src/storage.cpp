#include "storage.h"
#include "task_rc.h"

void Storage::init()
{
    EEPROM.begin(sizeof(State));
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