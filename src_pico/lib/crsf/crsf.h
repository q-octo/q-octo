#pragma once
#include <stdint.h>
#include "pico/stdlib.h"

// BEGIN gen_frames.dart
typedef struct crsf_battery_sensor_s
{
    // voltage in dV (Big Endian)
    uint16_t voltage;
    // current in dA (Big Endian)
    uint16_t current;
    // used capacity in mAh
    uint32_t capacity;
    // estimated battery remaining in percent (%)
    uint8_t percent;
} crsf_payload_battery_sensor_t;

typedef struct crsf_link_statistics_s
{
    // Uplink RSSI Ant. 1 ( dBm * -1 )
    uint8_t uplink_rssi_ant_1;
    // Uplink RSSI Ant. 2 ( dBm * -1 )
    uint8_t uplink_rssi_ant_2;
    // Uplink Package success rate / Link quality ( % )
    uint8_t uplink_package_success_rate;
    // Uplink SNR ( dB, or dB*4 for TBS I believe )
    int8_t uplink_snr;
    // Diversity active antenna ( enum ant. 1 = 0, ant. 2 = 1 )
    uint8_t diversity_active_antenna;
    // RF Mode ( 500Hz, 250Hz etc, varies based on ELRS Band or TBS )
    uint8_t rf_mode;
    // Uplink TX Power ( enum 0mW = 0, 10mW, 25 mW, 100 mW, 500 mW, 1000 mW, 2000mW, 50mW )
    uint8_t uplink_tx_power;
    // Downlink RSSI ( dBm * -1 )
    uint8_t downlink_rssi;
    // Downlink package success rate / Link quality ( % )
    uint8_t downlink_package_success_rate;
    // Downlink SNR ( dB )
    int8_t downlink_snr;
} crsf_payload_link_statistics_t;

typedef struct telemetry_s
{
    crsf_payload_battery_sensor_t battery_sensor;
    crsf_payload_link_statistics_t link_statistics;
} telemetry_t;

typedef enum frame_type_e
{
    CRSF_FRAMETYPE_BATTERY_SENSOR = 0x08,
    CRSF_FRAMETYPE_LINK_STATISTICS = 0x14,

} frame_type_t;
// END gen_frames.dart

typedef struct
{
    uint8_t *buffer;
    size_t capacity;
    size_t offset;
} stream_buffer_t;

typedef struct link_statistics_s
{
    uint8_t rssi;
    uint8_t link_quality;
    int8_t snr;
    uint16_t tx_power;
} link_statistics_t;

struct rc_channels_s
{
    uint16_t channel0 : 11;
    uint16_t channel1 : 11;
    uint16_t channel2 : 11;
    uint16_t channel3 : 11;
    uint16_t channel4 : 11;
    uint16_t channel5 : 11;
    uint16_t channel6 : 11;
    uint16_t channel7 : 11;
    uint16_t channel8 : 11;
    uint16_t channel9 : 11;
    uint16_t channel10 : 11;
    uint16_t channel11 : 11;
    uint16_t channel12 : 11;
    uint16_t channel13 : 11;
    uint16_t channel14 : 11;
    uint16_t channel15 : 11;
} __attribute__((packed));

typedef struct rc_channels_s rc_channels_t;

#define TICKS_TO_US(x) ((x - 992) * 5 / 8 + 1500)

#ifdef __cplusplus
extern "C" {
#endif

void crsf_telem_set_battery_data(uint16_t voltage, uint16_t current, uint32_t capacity, uint8_t percent);
void crsf_set_link_quality_threshold(uint8_t threshold);
void crsf_set_rssi_threshold(uint8_t threshold);
void crsf_set_on_rc_channels(void (*callback)(const uint16_t channels[16]));
void crsf_set_on_link_statistics(void (*callback)(const link_statistics_t link_stats));
void crsf_set_on_failsafe(void (*callback)(const bool failsafe));
void crsf_begin(uart_inst_t *uart, uint8_t rx, uint8_t tx);
void crsf_end();
void crsf_process_frames();

#ifdef __cplusplus
}
#endif