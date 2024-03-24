#include <Arduino.h> // Always include this first

#include "FreeRTOS.h"
#include "task.h"

#include "CRSFforArduino.hpp"

CRSFforArduino *crsf = nullptr;

/* A flag to hold the fail-safe status. */
bool isFailsafeActive = false;

/* RC Channels data. */
int rcChannelCount = 8;
// 16 channels
const char *rcChannelNames[] = {
    "A",
    "E",
    "T",
    "R",
    "Aux1",
    "Aux2",
    "Aux3",
    "Aux4",

    "Aux5", // Failsafe Channel
    "Aux6",
    "Aux7",
    "Aux8",
    "Aux9",
    "Aux10",
    "Aux11",
    "Aux12"};

void terminateCrsf();
/* RC Channels Event Callback. */
void onReceiveRcChannels(serialReceiverLayer::rcChannels_t *rcData);
void onLinkStatisticsUpdate(serialReceiverLayer::link_statistics_t);

void taskCRSF(void *pvParameters)
{
    (void)pvParameters; //  To avoid warnings
    crsf = new CRSFforArduino();

    /* Initialise CRSF for Arduino, and clean up
    any allocated resources if initialisation fails. */
    if (crsf->begin())
    {
        crsf->setLinkStatisticsCallback(onLinkStatisticsUpdate);
        crsf->setRcChannelsCallback(onReceiveRcChannels);
        /* Constrain the STATE_RC Channels Count to the maximum number
          of channels that are specified by The Crossfire Protocol.*/
        rcChannelCount = rcChannelCount > crsfProtocol::RC_CHANNEL_COUNT ? crsfProtocol::RC_CHANNEL_COUNT : rcChannelCount;
        // crsf->telemetryWriteBattery(0, 0, 0, 71);
    }
    else
    {
        /* CRSF for Arduino failed to initialise.
        Clean-up the resources that it previously allocated, and then free up the memory it allocated. */
        Serial.println("CRSF init failed.");
        terminateCrsf();
        return;
    }
    

    for (;;)
    {
        crsf->update();
        vTaskDelay(pdMS_TO_TICKS(1));
    }
}

void terminateCrsf()
{
    crsf->end();
    delete crsf;
    crsf = nullptr;
}

void onLinkStatisticsUpdate(serialReceiverLayer::link_statistics_t linkStatistics)
{
    // WARNING: Treat as an ISR callback!


    /* This is your Link Statistics Event Callback.
    By using the linkStatistics parameter that's passed in,
    you have access to the following:
    - linkStatistics.rssi
    - linkStatistics.lqi
    - linkStatistics.snr
    - linkStatistics.tx_power

    For the purposes of this example, these values are simply
    printed to the Serial Monitor at a rate of 5 Hz. */

    static unsigned long lastPrint = 0;
    if (millis() - lastPrint >= 500)
    {
        lastPrint = millis();
        Serial.print("Link Statistics: ");
        Serial.print("RSSI: ");
        Serial.print(linkStatistics.rssi);
        Serial.print(", Link Quality: ");
        Serial.print(linkStatistics.lqi);
        Serial.print(", Signal-to-Noise Ratio: ");
        Serial.print(linkStatistics.snr);
        Serial.print(", Transmitter Power: ");
        Serial.println(linkStatistics.tx_power);
    }
}

void onReceiveRcChannels(serialReceiverLayer::rcChannels_t *rcData)
{
    // WARNING: Treat as an ISR callback!

    /* This is your STATE_RC Channels Event Callback.
    Here, you have access to all 16 11-bit channels,
    plus an additional "failsafe" flag that tells you whether-or-not
    your receiver is connected to your controller's transmitter module.

    Using the rcData parameter that was passed in,
    you have access to the following:

    - failsafe - A boolean flag indicating the "Fail-safe" status.
    - value[16] - An array consisting of all 16 received STATE_RC channels.
      NB: STATE_RC Channels are RAW values and are NOT in "microseconds" units.

    For the purposes of this example, the fail-safe flag is used to centre
    all channels except for Channel 5 (AKA Aux1). Aux1 is set to the
    "Disarmed" position.
    The STATE_RC Channels themselves are all converted to "microseconds" for
    visualisation purposes, and printed to the Serial Monitor at a rate
    of 100 Hz. */

    if (rcData->failsafe)
    {
        if (!isFailsafeActive)
        {
            isFailsafeActive = true;

            /* Centre all STATE_RC Channels, except for Channel 5 (Aux1). */
            for (int i = 0; i < rcChannelCount; i++)
            {
                if (i != 4)
                {
                    rcData->value[i] = 992;
                }
            }

            /* Set Channel 5 (Aux1) to its minimum value. */
            rcData->value[4] = 191;

            Serial.println("[Sketch | WARN]: Failsafe detected.");
        }
    }
    else
    {
        /* Set the failsafe status to false. */
        if (isFailsafeActive)
        {
            isFailsafeActive = false;
            Serial.println("[Sketch | INFO]: Failsafe cleared.");
        }
    }

    /* Here is where you may write your STATE_RC channels implementation.
    For this example, STATE_RC Channels are simply sent to the Serial Monitor. */
    static uint32_t lastPrint = millis();

    if (millis() - lastPrint >= 200)
    {
        lastPrint = millis();

        Serial.print("[Sketch | INFO]: STATE_RC Channels: <");
        for (int i = 0; i < rcChannelCount; i++)
        {
            Serial.print(rcChannelNames[i]);
            Serial.print(": ");
            Serial.print(crsf->rcToUs(rcData->value[i]));

            if (i < (rcChannelCount - 1))
            {
                Serial.print(", ");
            }
        }
        Serial.println(">");
    }
}
