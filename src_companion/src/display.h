#pragma once

namespace Display
{
    typedef enum
    {
        STATE_UPDATE
    } MessageType;

    typedef struct
    {
        MessageType type;
        union
        {
            // TaskMessage::State state;
        } as;
    } Message;
}


void loopDisplay();
