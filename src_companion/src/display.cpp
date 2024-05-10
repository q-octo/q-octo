
#include "display.h"
#include <vector>
#include <string>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#define SET_PEN_RED() graphics.set_pen(244, 67, 54);
#define SET_PEN_GREEN() graphics.set_pen(76, 176, 80);
#define SET_PEN_ORANGE() graphics.set_pen(255, 152, 0);
#define SET_PEN_BLACK() graphics.set_pen(0, 0, 0);
#define SET_PEN_WHITE() graphics.set_pen(255, 255, 255);

void Display::init() {
  st7789.set_backlight(255);
  led.set_brightness(0); // Turn LED off.
}

void Display::loop()
{
  static unsigned long lastBlinkMillis = 0;
  static unsigned long lastRepaintMillis = 0;


  // Display is 240x135 pixels

  // Blink the LED every second to show that the data is live
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
  const bool stateOK = state.status == Status_OK;
  if (stateOK)
  {
      led.set_rgb(0, 255, 0); // Green
  }
  else
  {
      led.set_rgb(255, 0, 0); // Red
  }
  led.set_brightness(ledState ? 0 : 100);
  ledState = !ledState;
}

std::string Display::fmtFloat(const float& number, const int& precision) {
    // Use stringstream to format the float
    std::stringstream formatted;
    formatted << std::fixed << std::setprecision(precision) << number; // Set to two decimal places
    return formatted.str();
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

    if (state.web_server_enabled) {
        SET_PEN_GREEN();
        graphics.text("ON", Point(topBar.x + 110 + wifiwidth, topBar.y), topBar.w);
    } else {
        SET_PEN_RED();
        graphics.text("OFF", Point(topBar.x + 110 + wifiwidth, topBar.y), topBar.w);
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
        case Status_NO_TX_SIGNAL:
        case Status_DISARMED:
            SET_PEN_RED()
            break;
        case Status_LOW_BATTERY:
        case Status_MOTOR_ERROR:
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
        graphics.text(shortened, Point(4, y_position), 230);  // Text position adjusted for margins

        // Draw a horizontal line after the text, if not the last item
        if (i < num_items - 1) {
            graphics.line(Point(0, y_position + space_per_item - 3), Point(240, y_position + space_per_item - 3));
        }
    }

    // Update the display with the new graphics
    st7789.update(&graphics);
}

void Display::paintPage2() {
    std::vector<std::string> items; // Vector to store the lines to be displayed

    if (state.crsf_data == nullptr) {
        items.push_back("No CRSF data.");
    } else {
        // Format and construct the first line with RSSI, Link, and SNR
        std::ostringstream oss;
        oss << "RSSI:-" << state.crsf_data->telemetry->rssi << "dBm"
            << " LINK:" << state.crsf_data->telemetry->link_quality << "%";
            // << " SNR:" << state.crsf_data->telemetry->snr;
        items.push_back(oss.str());
        oss.str(""); // Clear the stringstream for next use
    }

    if (state.motors == nullptr) {
        items.push_back("No motor info.");
    } else {
        // Format and construct lines for motor info
        std::ostringstream oss;
        oss << "L MOT " << state.motors->motor1->temperature << "°C "
            << fmtFloat(state.motors->motor1->rps) << "rad/s "
            << state.motors->motor1->angle << "°";
        items.push_back(oss.str());
        oss.str("");

        oss << "R MOT: " << state.motors->motor2->temperature << "°C "
            << fmtFloat(state.motors->motor2->rps) << "rad/s "
            << state.motors->motor2->angle << "°";
        items.push_back(oss.str());
    }

    if (state.crsf_data == nullptr) {
        items.push_back("No CRSF data.");
    } else {
        // Process channel data
        uint16_t channelData[16];
        channelData[0] = state.crsf_data->channels->c1;
        channelData[1] = state.crsf_data->channels->c2;
        channelData[2] = state.crsf_data->channels->c3;
        channelData[3] = state.crsf_data->channels->c4;
        channelData[4] = state.crsf_data->channels->c5;
        channelData[5] = state.crsf_data->channels->c6;
        channelData[6] = state.crsf_data->channels->c7;
        channelData[7] = state.crsf_data->channels->c8;
        channelData[8] = state.crsf_data->channels->c9;
        channelData[9] = state.crsf_data->channels->c10;
        channelData[10] = state.crsf_data->channels->c11;
        channelData[11] = state.crsf_data->channels->c12;
        channelData[12] = state.crsf_data->channels->c13;
        channelData[13] = state.crsf_data->channels->c14;
        channelData[14] = state.crsf_data->channels->c15;
        channelData[15] = state.crsf_data->channels->c16;
        std::ostringstream oss;

        // Construct lines for channel data groups
        for (int i = 0; i < 4; i++) {
            int baseIndex = i * 4;
            oss << std::to_string(baseIndex + 1) << "-" << std::to_string(baseIndex + 4);
            for (int j = 0; j < 4; j++) {
                oss << " " << channelData[baseIndex + j];
            }
            items.push_back(oss.str());
            oss.str(""); // Clear for next group
        }
    }

    paintStack(items);
}


void Display::paintPage3() {
    // Build strings for
    // Low battery voltage, critical battery voltage, rssithreshold, linkqualitythreshold
    std::ostringstream oss;

    // Set white background
    SET_PEN_WHITE()

    paintStack({
        "DISARM THRESH: " + fmtFloat(state.low_voltage_threshold) + "V",
        "SHUTDOWN THRES: " + fmtFloat(state.critical_voltage_threshold) + "V",
        "RSSI THRESH: " + fmtFloat(state.rssi_threshold),
        "LINK Q. THRESH: " + fmtFloat(state.link_quality_threshold),
        "",
        "",
        "",
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