/**
 * Class: DFR0534
 *
 * Description:
 * Class for controlling a DFR0534 audio module (https://wiki.dfrobot.com/Voice_Module_SKU__DFR0534)
 * by SoftwareSerial
 *
 * License: 2-Clause BSD License
 * Copyright (c) 2024 codingABI
 * For details see: LICENSE.txt
 *
 * Home: https://github.com/codingABI/DFR0534
 *
 * @author codingABI https://github.com/codingABI/
 * @copyright 2-Clause BSD License
 * @file DFR0534.h
 * @version 1.0.1
 */
#pragma once

/** Library version */
#define DFR0534_VERSION "1.0.1"

#include <arduino.h>
#include <Stream.h>

#define STARTINGCODE 0xAA

/**@brief
 * Class for a DFR0534 audio module
 */
class DFR0534 {
  public:
    /** Supported input channels */
    enum DFR0534CHANNELS
    {
      CHANNELMP3, /**< Use MP3 input channel for DAC output (=default after device startup) */
      CHANNELAUX, /**< Use AUX input (P26 and P27) for DAC output */
      CHANNELMP3AUX, /**< Combines MP3 and AUX audio from P26 and P27 for DAC output */
      CHANNELUNKNOWN /**< Unknown */
    };
    /** Supported drives */
    enum DFR0534DRIVE
    {
      DRIVEUSB, /**< USB drive */
      DRIVESD, /**< SD card */
      DRIVEFLASH, /**< Flash memory chip */
      DRIVEUNKNOWN, /**< Unknown */
      DRIVENO = 0xff /**< No drive */
    };
    /** Loop modes */
    enum DFR0534LOOPMODE
    {
      LOOPBACKALL, /**< Every file on drive in "file copy order" and loop afterwards */
      SINGLEAUDIOLOOP, /**< Repeat current file */
      SINGLEAUDIOSTOP, /**< Stops after single file (=default after device startup) */
      PLAYRANDOM, /**< Random play order */
      DIRECTORYLOOP, /**< Every file in current director in "file copy order" and loop afterwards */
      RANDOMINDIRECTORY, /**< Random play order in current directory */
      SEQUENTIALINDIRECTORY, /**< Every file in current directory in "file copy order" without loop */
      SEQUENTIAL, /**< Every file on drive in "file copy order" without loop */
      PLAYMODEUNKNOWN /**< Unknown */
    };
    /** EQ modes */
    enum DFR0534EQ
    {
      NORMAL, /**< (=default after device startup) */
      POP,
      ROCK,
      JAZZ ,
      CLASSIC,
      EQUNKNOWN
    };
    /** Modul states */
    enum DFR0534STATUS
    {
      STOPPED, /**< Audio module is idle */
      PLAYING, /**< Audio module is playing a file */
      PAUSED, /**< Audio module is paused */
      STATUSUNKNOWN /**< Unkown */
    };
    /**@brief
     * Constructor of a the DFR0534 audio module
     *
     * @param[in] stream  Serial connection object, like SoftwareSerial
     */
    DFR0534(Stream &stream)
    {
      m_ptrStream = &stream;
    }
    void decreaseVolume();
    void fastBackwardDuration(word seconds);
    void fastForwardDuration(word seconds);
    byte getDrive();
    byte getDrivesStates();
    bool getDuration(byte &hour, byte &minute, byte &second);
    bool getFileName(char *name);
    word getFileNumber();
    int getFirstFileNumberInCurrentDirectory();
    bool getRuntime(byte &hour, byte &minute, byte &second);
    byte getStatus();
    int getTotalFiles();
    int getTotalFilesInCurrentDirectory();
    void increaseVolume();
    void insertFileByNumber(word track, byte drive=DRIVEFLASH);
    void pause();
    void play();
    void playCombined(char* list);
    void playFileByName(char *path, byte drive=DRIVEFLASH);
    void playFileByNumber(word track);
    void playLastInDirectory();
    void playNext();
    void playNextDirectory();
    void playPrevious();
    void prepareFileByNumber(word track);
    void repeatPart(byte startMinute, byte startSecond, byte stopMinute, byte stopSecond );
    void setChannel(byte channel);
    void setDirectory(char *path, byte drive=DRIVEFLASH);
    void setDrive(byte drive);
    void setEqualizer(byte mode);
    void setLoopMode(byte mode);
    void setRepeatLoops(word loops);
    void setVolume(byte volume);
    void stop();
    void stopInsertedFile();
    void startSendingRuntime();
    void stopCombined();
    void stopRepeatPart();
    void stopSendingRuntime();
  private:
    void sendStartingCode() {
      m_checksum=STARTINGCODE;
      m_ptrStream->write((byte)STARTINGCODE);
    }
    void sendDataByte(byte data) {
      m_checksum +=data;
      m_ptrStream->write((byte)data);
    }
    void sendCheckSum() {
      m_ptrStream->write((byte)m_checksum);
    }
    byte m_checksum;
    Stream *m_ptrStream = NULL;
};
