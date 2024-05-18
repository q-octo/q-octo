#pragma once

class EnableWifi
{
public:
    static void init();
    static void onPhysicalSwitchChange(bool on);
    static void onTxSwitchChange(bool on);
    static void onTxFailsafed();
    static void onTxFailsafeCleared();
    static void onWifiButtonPress();
    
private:
    static void setWifiState(bool on);
    static bool txOnOrFailsafed() { return txSwitchOn || failsafed; }
    static bool shouldInit() { return !switchesInitialised && txSwitchInitialised && physicalSwitchInitialised; }
    static void onSwitchesInitialised();
    static inline bool physicalSwitchOn = false;
    static inline bool txSwitchOn = false;
    static inline bool failsafed = false;
    static inline bool wifiEnabled = false;
    static inline bool switchesInitialised = false;
    static inline bool txSwitchInitialised = false;
    static inline bool physicalSwitchInitialised = false;
};