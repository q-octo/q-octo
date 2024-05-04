
#include "display.h"
#include <vector>
#include <string>

#define SET_PEN_RED() graphics.set_pen(244, 67, 54);
#define SET_PEN_GREEN() graphics.set_pen(76, 176, 80);
#define SET_PEN_ORANGE() graphics.set_pen(255, 152, 0);
#define SET_PEN_BLACK() graphics.set_pen(0, 0, 0);
#define SET_PEN_WHITE() graphics.set_pen(255, 255, 255);

void Display::loop()
{
  static unsigned long lastBlinkMillis = 0;
  static unsigned long lastRepaintMillis = 0;

  st7789.set_backlight(255);
  led.set_brightness(0); // Turn LED off.

  // Display is 240x135 pixels

  // Blink the LED every second to show that the data is live
  // TODO blink this red if the status is not OK.
  const uint32_t currentMillis = millis();
  if (currentMillis - lastBlinkMillis >= 1000)
  {
    lastBlinkMillis = currentMillis;
    blinkLED();
  }

  // Repaint the screen every 32ms (30fps)
  if (currentMillis - lastRepaintMillis >= 32)
  {
    lastRepaintMillis = currentMillis;
    repaintDisplay();
  }

    // Check for button presses

    // Y press changes the page
    if (button_y.read())
    {
      nextPage();
    }
    // The rest of the buttons are just for toggles
    if (button_a.read())
    {
        button_a_callback();
    }
    if (button_b.read())
    {
        button_b_callback();
    }
    if (button_x.read())
    {
        button_x_callback();
    }
}

void Display::blinkLED()
{
  led.set_rgb(76, 176, 80); // Green
  led.set_brightness(ledState ? 0 : 100);
  ledState = !ledState;
}

void Display::paintPage1()
{
// An example of how to access state.
    //  state.batteries;

    // bitmap6, bitmap8, bitmap14_outline

    // For building strings
    std::ostringstream oss;

    // Set white background
    SET_PEN_WHITE()

    // Top Bar - Control Source and WIFI
    // Height 30px
    graphics.set_font("bitmap14_outline");
    SET_PEN_WHITE()
    Rect topBar(0, 0, 240, 30);
    graphics.rectangle(topBar);
    topBar.deflate(2);

    oss.str("");
    oss << EnumNamesControlSource()[state.control_source];
    SET_PEN_BLACK()
    graphics.text(oss.str(), Point(topBar.x, topBar.y), topBar.w);

    // Web Server running
    graphics.text("WIFI:", Point(topBar.x + 110, topBar.y), topBar.w);
    int wifiwidth = graphics.measure_text("WIFI:", 2, 1, false);

    if (!webServerIsRunningState) {
        SET_PEN_RED();
        graphics.text("OFF", Point(topBar.x + 110 + wifiwidth, topBar.y), topBar.w);
    } else {
        SET_PEN_GREEN();
        graphics.text("ON", Point(topBar.x + 110 + wifiwidth, topBar.y), topBar.w);
    }

    // Middle1 - Status Code in the center
    // Height 31px
    graphics.set_font("bitmap14_outline");

    // Center the text
    // Text is 14px
    // Measure length of text first
    const char *status;

    status = EnumNamesStatus()[state.status];

    switch (state.status) {
        case Status_OK:
            SET_PEN_GREEN();
            break;
        case Status_INIT:
            SET_PEN_ORANGE()
            break;
        case Status_NOTX:
            SET_PEN_RED()
            break;
        case Status_BAT:
            SET_PEN_RED()
            break;
        case Status_TEMP:
            SET_PEN_ORANGE()
            break;
        case Status_OFF:
            SET_PEN_RED()
            break;
    }

    Rect middle1(0, 30, 240, 31);
    graphics.rectangle(middle1);
    middle1.deflate(2);

    int32_t textWidth = graphics.measure_text(status, 2, 1, false);
    SET_PEN_WHITE()
    graphics.text(status, Point(middle1.x + (middle1.w - textWidth) / 2, middle1.y), middle1.w);

    // Middle2 - battery bar
    // Height 20px
    SET_PEN_BLACK()
    Rect middle2(0, 61, 240, 20);
    graphics.rectangle(middle2);
    middle2.deflate(2);

    if (state.fuel < 0.1) {
        SET_PEN_RED()
    } else if (state.fuel < 0.2) {
        SET_PEN_ORANGE()
    } else {
        SET_PEN_GREEN()
    }

    int padding = 2;
    graphics.rectangle(Rect(0 + padding, 61 + padding, (int)((240-padding) * state.fuel), 20 - padding * 2));


    // Bottom1 - Voltage and number of batteries
    graphics.set_font("bitmap8");
    // Height 27px
    SET_PEN_WHITE()
    Rect bottom1(0, 81, 240, 27);
    graphics.rectangle(bottom1);
    bottom1.deflate(2);

    SET_PEN_BLACK()
    // TODO: Make this color conditional on voltage
    graphics.text("VOLTAGE:", Point(bottom1.x, bottom1.y), bottom1.w);

    if (state.voltage < 11.5) {
        SET_PEN_RED()
    } else if (state.voltage < 12.5) {
        SET_PEN_ORANGE()
    } else {
        SET_PEN_GREEN()
    }

    // Formatted string with voltage
    oss.str("");
    oss << state.voltage << "V";

    graphics.text(oss.str(), Point(bottom1.x + 100, bottom1.y), bottom1.w);

    // Draw a battery icon with 2 rectangles
    SET_PEN_BLACK()
    graphics.rectangle(Rect(bottom1.x + 185, bottom1.y, 24, 18));
    graphics.rectangle(Rect(bottom1.x + 209, bottom1.y + 6, 8, 8));

    oss.str("");
    oss << "#" << state.batteries ;

    graphics.text(oss.str(), Point(bottom1.x + 160, bottom1.y), bottom1.w);

    // Bottom2 - Current and number of battery percentage
    // Height 27px
    SET_PEN_WHITE()
    Rect bottom2(0, 108, 240, 27);
    graphics.rectangle(bottom2);
    bottom2.deflate(2);

    SET_PEN_BLACK()
    // Make this color conditional on current
    graphics.text("CURRENT:", Point(bottom2.x, bottom2.y), bottom2.w);

    if (state.current < 0) {
        SET_PEN_RED()
    } else if (state.current < 1) {
        SET_PEN_ORANGE()
    } else {
        SET_PEN_GREEN()
    }

    oss.str("");
    oss << state.current << "A";
    graphics.text(oss.str(), Point(bottom2.x + 100, bottom2.y), bottom2.w);

    if (state.fuel < 0.1) {
        SET_PEN_RED()
    } else if (state.fuel < 0.2) {
        SET_PEN_ORANGE()
    } else {
        SET_PEN_GREEN()
    }

    oss.str("");
    oss << state.fuel * 100 << "%";

    graphics.text(oss.str(), Point(bottom2.x + 160, bottom2.y), bottom2.w);
}

void Display::paintStack(const std::vector<std::string>& items) {
    // Clear the display to start with a clean slate
    SET_PEN_WHITE();
    graphics.clear();

    graphics.set_font("bitmap8");

    SET_PEN_BLACK();

    // Determine the vertical spacing for each item
    int total_height = 135;  // Total height of the display
    int num_items = items.size();
    int space_per_item = total_height / num_items;

    // Draw each item in the stack
    for (int i = 0; i < num_items; ++i) {
        int y_position = i * space_per_item + 5; // Add 5px margin to the top

        // Cutoff text > 25 chars
        std::string shortened = items[i];
        if (items[i].length() > 25) {
            shortened = items[i].substr(0, 25);
        }

        // Draw text at the calculated position
        graphics.text(shortened, Point(10, y_position), 230);  // Text position adjusted for margins

        // Draw a horizontal line after the text, if not the last item
        if (i < num_items - 1) {
            graphics.line(Point(0, y_position + space_per_item - 1), Point(240, y_position + space_per_item - 1));
        }
    }

    // Update the display with the new graphics
    st7789.update(&graphics);
}


void Display::paintPage2() {

    // Set white background
    SET_PEN_WHITE();
    graphics.clear();

    graphics.set_font("bitmap6");

    // Draw horizontal lines to separate sections
    SET_PEN_BLACK();
    graphics.line(Point(0, 20), Point(239, 20)); // After RSSI
    graphics.line(Point(0, 40), Point(239, 40));
    graphics.line(Point(0, 60), Point(239, 60));
    graphics.line(Point(0, 80), Point(239, 80));
    graphics.line(Point(0, 100), Point(239, 100));
    graphics.line(Point(0, 120), Point(239, 120));

    // For building strings
    std::ostringstream oss;

    if (state.crsf_data == nullptr) {
        graphics.text("No CRSF data.", Point(5, 5), 220);
        return;
    } else{
        // Line1 - RSSI and Link Quality
        float rssi = state.crsf_data->telemetry->rssi;
        rssi = roundf(rssi * 100) / 100;
        float link = state.crsf_data->telemetry->link_quality;
        link = roundf(link * 100) / 100;
        float snr = state.crsf_data->telemetry->snr;
        snr = roundf(snr * 100) / 100;
        oss << "RSSI:" << rssi << "dBm" << " Link:" << link << "%" << " SNR:" << snr;
        graphics.text(oss.str(), Point(5, 5), 230);
        oss.str("");
    }




    if (state.motors == nullptr) {
        graphics.text("No motor info.", Point(5, 25), 220);
    } else {
        // Motor1 Temperature, RPS, Angle
        oss << "M1: " << state.motors->motor1->temperature << "C " << state.motors->motor1->rps << "RPS " << state.motors->motor1->angle << "deg";
        graphics.text(oss.str(), Point(5, 25), 220);
        oss.str("");

        // Motor2 Temperature, RPS, Angle
        oss << "M2: " << state.motors->motor2->temperature << "C " << state.motors->motor2->rps << "RPS " << state.motors->motor2->angle << "deg";
        graphics.text(oss.str(), Point(5, 45), 220);
        oss.str("");
    }

    // Channels
    if(state.crsf_data == nullptr) {
        graphics.text("No CRSF data.", Point(5, 65), 220);
    } else {
        oss.str("");

        auto channels = state.crsf_data->channels->data();

        // Make an array of the channels
        auto channelData = state.crsf_data->channels->data();
        auto chan1 = channelData[0].data();
        auto chan2 = channelData[1].data();
        auto chan3 = channelData[2].data();
        auto chan4 = channelData[3].data();
        auto chan5 = channelData[4].data();
        auto chan6 = channelData[5].data();
        auto chan7 = channelData[6].data();
        auto chan8 = channelData[7].data();
        auto chan9 = channelData[8].data();
        auto chan10 = channelData[9].data();
        auto chan11 = channelData[10].data();
        auto chan12 = channelData[11].data();
        auto chan13 = channelData[12].data();
        auto chan14 = channelData[13].data();
        auto chan15 = channelData[14].data();
        auto chan16 = channelData[15].data();

        oss.str("");
        oss << "1-4:" << chan1 << " " << chan2 << " " << chan3 << " " << chan4;
        graphics.text(oss.str(), Point(5, 65), 220);
        oss.str("");
        oss << "5-8:" << chan5 << " " << chan6 << " " << chan7 << " " << chan8;
        graphics.text(oss.str(), Point(5, 85), 220);
        oss.str("");
        oss << "9-12:" << chan9 << " " << chan10 << " " << chan11 << " " << chan12;
        graphics.text(oss.str(), Point(5, 105), 220);
        oss.str("");
        oss << "13-16:" << chan13 << " " << chan14 << " " << chan15 << " " << chan16;
        graphics.text(oss.str(), Point(5, 125), 220);

    }
}

void Display::paintPage3() {
    // Build strings for
    // Low battery voltage, critical battery voltage, rssithreshold, linkqualitythreshold
    std::ostringstream oss;

    // Set white background
    SET_PEN_WHITE()

    paintStack({
        "Low Volt: " + std::to_string(state.low_voltage_threshold),
        "Crit Volt: " + std::to_string(state.critical_voltage_threshold),
        "RSSI Thres: " + std::to_string(state.rssi_threshold),
        "Lnk Q. Thres: " + std::to_string(state.link_quality_threshold)
    });
}

void Display::paintPage4() {
    // Set white background
    SET_PEN_WHITE()

    if (state.display_messages == nullptr) {
        paintStack({"No messages."});
        return;
    }
    
    paintStack({
        state.display_messages->message1,
        state.display_messages->message2,
        state.display_messages->message3,
        state.display_messages->message4,
        state.display_messages->message5,
        state.display_messages->message6,
        state.display_messages->message7,
    });

}



void Display::repaintDisplay()
{
    graphics.clear();
    switch (current_page)
    {
    case 0:
        paintPage1();
        break;
    case 1:
        paintPage2();
        break;
    case 2:
        paintPage3();
        break;
    case 3:
        paintPage4();
        break;
    }
  st7789.update(&graphics);
}