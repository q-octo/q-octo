#include "storage.h"
#include "task_rc.h"
#include "CRC.h"
#include "config.h"

/**
 * Bear in mind that the Pi Pico does not have a real EEPROM.
 * A 4kb flash block is used to emulate EEPROM and thus the write cycle limit
 * is significantly lower than a real EEPROM.
 */

void Storage::init()
{
    const size_t size = sizeof(StateWithCRC);
    if (size > 4096)
    {
        Serial.println("[ERROR] State struct cannot fit in EEPROM");
        return;
    }
    EEPROM.begin(size);
    if (CFG_DEBUG_WIPE_STORAGE) {
        save();
    }
    EEPROM.get(0, stateWithCRC);
    if (isCrcValid())
    {
        state = stateWithCRC.state;
    }
    else
    {
        Serial.println("CRC mismatch, using default state");
        // Stores the default state in 'EEPROM'
        save();
    }
}

bool Storage::isCrcValid()
{
    return calculateCRC(stateWithCRC.state) == stateWithCRC.crc;
}

uint32_t Storage::calculateCRC(StorageState &state)
{
    const uint8_t *data = (uint8_t *)&state;
    return CRC::Calculate(data, sizeof(StorageState), CRC::CRC_32());
}

void Storage::save()
{
    const uint32_t crc = calculateCRC(state);
    stateWithCRC = {state, crc};
    EEPROM.put(0, stateWithCRC);
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