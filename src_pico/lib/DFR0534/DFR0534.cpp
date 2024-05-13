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
 * Notes for DFR0534 audio module:
 * - Consumes about 20mA when idle (Vcc = 5V)
 * - Creates a short "click" noise, when Vcc is powered on
 * - Should be used with a 1k resistor on TX when your MCU runs on 5V,
 *   because the DFR0534 uses 3.3V logic (and 5V on TX causes clicks/noise)
 * - Can be controlled by a RX/TX serial connection (9600 baud) or one-wire protocol
 * - Can play WAV and MP3 audiofiles
 * - Can "insert" audiofiles while another audiofile is running.
 *   In this case to original audiofile is paused and will be
 *   resumed after the "inserted" audiofile
 * - Can play files in a playlist like mode called "combined"
 *   for files stored in a directory /ZH
 * - Can select the file to play by a file number* or file name**
 *   *File number is independent from file name. The first WAV or MP3 copied to
 *   the DFR0534 gets file number 1 and so on. To play a file by number
 *   use playFileByNumber()
 *   **File name is a little bit like a 8+3 file path and
 *   can be used with playFileByName(), but have special rules (see playFileByName() for details)
 * - Can send automatically the file runtime every second (when enabled)
 * - Has a NS8002 amplifier, JQ8400 Audio chip, W25Q64JVSIQ flash memory
 * - Has a Sleep mode 0x1B and this mode only works with one-wire protocol (https://github.com/arduino12/mp3_player_module_wire)
 *   and does not work for me without additional electric modifications (e.g. disconnecting speakers)
 *   => Switching off DFR0534 with a FET is a better solution
 *
 * Home: https://github.com/codingABI/DFR0534
 *
 * @author codingABI https://github.com/codingABI/
 * @copyright 2-Clause BSD License
 * @file DFR0534.cpp
 * @version 1.0.1
 */
#include "DFR0534.h"

/**@brief
 * Get module status
 *
 * @retval DFR0534::STOPPED        Audio module is idle
 * @retval DFR0534::PLAYING        Audio module is playing a file
 * @retval DFR0534::PAUSED         Audio module is paused
 * @retval DFR0534::STATUSUNKNOWN  Error (for example request timeout)
 */
byte DFR0534::getStatus()
{
  #define COMMAND 0x01
  #define RECEIVEBYTETIMEOUTMS 100
  #define RECEIVEGLOBALTIMEOUTMS 500
  #define RECEIVEFAILED STATUSUNKNOWN
  #define RECEIVEHEADERLENGTH 2 // startingcode+command

  if (m_ptrStream == NULL) return RECEIVEFAILED; // Should not happen
  sendStartingCode();
  sendDataByte(COMMAND);;
  sendDataByte(0x00);;
  sendCheckSum();

  // Receive
  int i=0;
  byte data, firstByte = 0, sum, length=0xff, result = 0;
  unsigned long receiveStartMS = millis();
  do {
    byte dataReady = 0;
    unsigned long lastMS = millis();
    // Wait for response or timeout
    while ((millis()-lastMS < RECEIVEBYTETIMEOUTMS) && (dataReady==0)) dataReady = m_ptrStream->available();

    if (dataReady == 0) return RECEIVEFAILED; // Timeout
    data = m_ptrStream->read();

    if (i==0) { // Begin of transmission
      firstByte=data;
      sum = 0;
    }
    if ((i == 1) && (data != COMMAND)) {
      // Invalid signal => reset receive
      i=0;
      firstByte = 0;
    }
    if (i == RECEIVEHEADERLENGTH) {
      length = data; // Length of receiving data
      if (length != 1) {
        // Invalid length => reset receive
        i=0;
        firstByte = 0;
      }
    }
    if ((i > RECEIVEHEADERLENGTH) && (i-RECEIVEHEADERLENGTH-1<length)) {
      result = data;
    }
    if (firstByte == STARTINGCODE) {
      if (i-RECEIVEHEADERLENGTH<=length) sum+=data; // Update checksum
      i++;
    }
    if (millis()-receiveStartMS > RECEIVEGLOBALTIMEOUTMS) return RECEIVEFAILED; // Timeout
  } while (i<length+RECEIVEHEADERLENGTH+2);

  if (data != sum) return RECEIVEFAILED; // Does checksum matches?
  return result;
}

/**@brief
 * Set equalizer to NORMAL, POP, ROCK, JAZZ or CLASSIC
 *
 * @param[in] mode  EQ mode: DFR0534::NORMAL, DFR0534::POP, DFR0534::ROCK, DFR0534::JAZZ or DFR0534::CLASSIC
 */
void DFR0534::setEqualizer(byte mode)
{
  if (m_ptrStream == NULL) return; // Should not happen
  if (mode >= EQUNKNOWN) return;
  sendStartingCode();
  sendDataByte(0x1A);
  sendDataByte(0x01);
  sendDataByte(mode);
  sendCheckSum();
}

/**@brief
 * Play audio file by number
 *
 * File number order is "file copy order":
 * First audio file copied to the drive gets number 1, second audio file copied gets  number 2... )
 *
 * @param[in] track  File number
 */
void DFR0534::playFileByNumber(word track)
{
  if (m_ptrStream == NULL) return; // Should not happen
  if (track <=0) return;
  sendStartingCode();
  sendDataByte(0x07);
  sendDataByte(0x02);
  sendDataByte((track >> 8) & 0xff);
  sendDataByte(track & 0xff);
  sendCheckSum();
}

/**@brief
 * Set volume
 *
 * Volumen levels 0-30 are allowed. Audio module starts always with level 20.
 *
 * @param[in] volume  Volume level
 */
void DFR0534::setVolume(byte volume)
{
  if (m_ptrStream == NULL) return; // Should not happen
  if (volume > 30) volume = 30;
  sendStartingCode();
  sendDataByte(0x13);
  sendDataByte(0x01);
  sendDataByte(volume);
  sendCheckSum();
}

/**@brief
 * Play the current selected file
 */
void DFR0534::play()
{
  if (m_ptrStream == NULL) return; // Should not happen
  sendStartingCode();
  sendDataByte(0x02);
  sendDataByte(0x00);
  sendCheckSum();
}

/**@brief
 * Pause the current file
 */
void DFR0534::pause()
{
  if (m_ptrStream == NULL) return; // Should not happen
  sendStartingCode();
  sendDataByte(0x03);
  sendDataByte(0x00);
  sendCheckSum();
}

/**@brief
 * Stop the current file
 */
void DFR0534::stop()
{
  if (m_ptrStream == NULL) return; // Should not happen
  sendStartingCode();
  sendDataByte(0x04);
  sendDataByte(0x00);
  sendCheckSum();
}

/**@brief
 * Play previous file (in "file copy order")
 */
void DFR0534::playPrevious()
{
  if (m_ptrStream == NULL) return; // Should not happen
  sendStartingCode();
  sendDataByte(0x05);
  sendDataByte(0x00);
  sendCheckSum();
}

/**@brief
 * Play next file (in "file copy order")
 */
void DFR0534::playNext()
{
  if (m_ptrStream == NULL) return; // Should not happen
  sendStartingCode();
  sendDataByte(0x06);
  sendDataByte(0x00);
  sendCheckSum();
}

/**@brief
 * Play audio file by file name/path
 *
 * The file name/path is the full path of the audio file to be played
 * in format which looks like a special unix 8+3 format:
 * - Without the dot for the file extension
 * - All characters in upper case
 * - Every file and folder whose length is shorter then 8 chars must be filled up to the 8 chars length by spaces.
 * - Only WAV and MP3 files are supported
 * Wildcards * (=multiple arbitrary characters) and ? (=one single arbitrary character) are allowed and can be used to reduce filling spaces.
 *
 * Valid examples:
 * - "/01      WAV" for file 01.wav
 * - "/99-AFR~1MP3" for a file /99-Africa.mp3
 * - "/99-AFR*MP3" for first file matching /99-Afr*.mp3
 * - "/10*" for first audio file matching /10*.*
 * - "/10      /20      WAV" for the file /10/20.wav
 *
 * @param[in] path   Full path of the audio file
 * @param[in] drive  Drive, where file is stored: Drive DFR0534::DRIVEUSB, DFR0534::DRIVESD or DFR0534::DRIVEFLASH (=default)
 */
void DFR0534::playFileByName(char *path, byte drive)
{
  if (m_ptrStream == NULL) return; // Should not happen
  if (path == NULL) return;
  if (drive >= DRIVEUNKNOWN) return;
  sendStartingCode();
  sendDataByte(0x08);
  sendDataByte(strlen(path)+1);
  sendDataByte(drive);
  for (int i=0;i<strlen(path);i++) {
    sendDataByte(path[i]);
  }
  sendCheckSum();
}

/**@brief
 * Checks which drives are ready/online
 *
 * Returned value is a bit pattern that shows which drives are ready/online (1=online,0=offline):
 * - Bit 0 = DFR0534::DRIVEUSB
 * - Bit 1 = DFR0534::DRIVESD
 * - Bit 2 = DFR0534::DRIVEFLASH

 * @returns Bit pattern for drives
 * @retval DFR0534::DRIVEUNKNOWN  Error (for example request timeout)
 */
byte DFR0534::getDrivesStates()
{
  #define COMMAND 0x09
  #define RECEIVEBYTETIMEOUTMS 100
  #define RECEIVEGLOBALTIMEOUTMS 500
  #define RECEIVEFAILED DRIVEUNKNOWN
  #define RECEIVEHEADERLENGTH 2 // startingcode+command

  if (m_ptrStream == NULL) return RECEIVEFAILED; // Should not happen
  sendStartingCode();
  sendDataByte(COMMAND);
  sendDataByte(0x00);
  sendCheckSum();

  // Receive
  int i=0;
  byte data, firstByte = 0, sum, length=0xff, result = 0;
  unsigned long receiveStartMS = millis();
  do {
    byte dataReady = 0;
    unsigned long lastMS = millis();
    // Wait for response or timeout
    while ((millis()-lastMS < RECEIVEBYTETIMEOUTMS) && (dataReady==0)) dataReady = m_ptrStream->available();

    if (dataReady == 0) return RECEIVEFAILED; // Timeout
    data = m_ptrStream->read();

    if (i==0) { // Begin of transmission
      firstByte=data;
      sum = 0;
    }
    if ((i == 1) && (data != COMMAND)) {
      // Invalid signal => reset receive
      i=0;
      firstByte = 0;
    }
    if (i == RECEIVEHEADERLENGTH) {
      length = data; // Length of receiving data
      if (length != 1) {
        // Invalid length => reset receive
        i=0;
        firstByte = 0;
      }
    }
    if ((i > RECEIVEHEADERLENGTH) && (i-RECEIVEHEADERLENGTH-1<length)) {
      result = data;
    }
    if (firstByte == STARTINGCODE) {
      if (i-RECEIVEHEADERLENGTH<=length) sum+=data; // Update checksum
      i++;
    }
    if (millis()-receiveStartMS > RECEIVEGLOBALTIMEOUTMS) return RECEIVEFAILED; // Timeout
  } while (i<length+RECEIVEHEADERLENGTH+2);

  if (data != sum) return RECEIVEFAILED; // Does checksum matches?
  return result;
}

/**@brief
 * Get current drive
 *
 * @retval DFR0534::DRIVEUSB      USB drive
 * @retval DFR0534::DRIVESD       SD card
 * @retval DFR0534::DRIVEFLASH    Flash memory chip
 * @retval DFR0534::DRIVENO       No drive found
 * @retval DFR0534::DRIVEUNKNOWN  Error (for example request timeout)
 */
byte DFR0534::getDrive()
{
  #define COMMAND 0x0A
  #define RECEIVEBYTETIMEOUTMS 100
  #define RECEIVEGLOBALTIMEOUTMS 500
  #define RECEIVEFAILED DRIVEUNKNOWN
  #define RECEIVEHEADERLENGTH 2 // startingcode+command

  if (m_ptrStream == NULL) return RECEIVEFAILED; // Should not happen
  sendStartingCode();
  sendDataByte(COMMAND);
  sendDataByte(0x00);
  sendCheckSum();

  // Receive
  int i=0;
  byte data, firstByte = 0, sum, length=0xff, result = 0;
  unsigned long receiveStartMS = millis();
  do {
    byte dataReady = 0;
    unsigned long lastMS = millis();
    // Wait for response or timeout
    while ((millis()-lastMS < RECEIVEBYTETIMEOUTMS) && (dataReady==0)) dataReady = m_ptrStream->available();

    if (dataReady == 0) return RECEIVEFAILED; // Timeout
    data = m_ptrStream->read();

    if (i==0) { // Begin of transmission
      firstByte=data;
      sum = 0;
    }
    if ((i == 1) && (data != COMMAND)) {
      // Invalid signal => reset receive
      i=0;
      firstByte = 0;
    }
    if (i == RECEIVEHEADERLENGTH) {
      length = data; // Length of receiving data
      if (length != 1) {
        // Invalid length => reset receive
        i=0;
        firstByte = 0;
      }
    }
    if ((i > RECEIVEHEADERLENGTH) && (i-RECEIVEHEADERLENGTH-1<length)) {
      result = data;
    }
    if (firstByte == STARTINGCODE) {
      if (i-RECEIVEHEADERLENGTH<=length) sum+=data; // Update checksum
      i++;
    }
    if (millis()-receiveStartMS > RECEIVEGLOBALTIMEOUTMS) return RECEIVEFAILED; // Timeout
  } while (i<length+RECEIVEHEADERLENGTH+2);

  if (data != sum) return RECEIVEFAILED; // Does checksum matches?
  return result;
}

/**@brief
* Switch to drive
*
* @param[in] drive  Drive DFR0534::DRIVEUSB, DFR0534::DRIVESD or DFR0534::DRIVEFLASH
*/
void DFR0534::setDrive(byte drive)
{
  if (m_ptrStream == NULL) return; // Should not happen
  if (drive >= DRIVEUNKNOWN) return;
  sendStartingCode();
  sendDataByte(0x0B);
  sendDataByte(0x01);
  sendDataByte(drive);
  sendCheckSum();
}

/**@brief
 * Get file number of current file
 *
 * File number is in "file copy order". First audio file copied to the drive get number 1...
 *
 * @returns File number
 * @retval 0  Error (for example request timeout)
 */
word DFR0534::getFileNumber()
{
  #define COMMAND 0x0D
  #define RECEIVEFAILED 0
  #define RECEIVEBYTETIMEOUTMS 100
  #define RECEIVEGLOBALTIMEOUTMS 500
  #define RECEIVEHEADERLENGTH 2 // startingcode+command

  if (m_ptrStream == NULL) return RECEIVEFAILED; // Should not happen
  sendStartingCode();
  sendDataByte(COMMAND);
  sendDataByte(0x00);
  sendCheckSum();

  // Receive
  int i=0;
  byte data, firstByte = 0, sum, length=0xff;
  word result = 0;
  unsigned long receiveStartMS = millis();
  do {
    byte dataReady = 0;
    unsigned long lastMS = millis();
    // Wait for response or timeout
    while ((millis()-lastMS < RECEIVEBYTETIMEOUTMS) && (dataReady==0)) dataReady = m_ptrStream->available();

    if (dataReady == 0) return RECEIVEFAILED; // Timeout
    data = m_ptrStream->read();

    if (i==0) { // Begin of transmission
      firstByte=data;
      sum = 0;
    }
    if ((i == 1) && (data != COMMAND)) {
      // Invalid signal => reset receive
      i=0;
      firstByte = 0;
    }
    if (i == RECEIVEHEADERLENGTH) {
      length = data; // Length of receiving data
      if (length != 2) {
        // Invalid length => reset receive
        i=0;
        firstByte = 0;
      }
    }
    if ((i > RECEIVEHEADERLENGTH) && (i-RECEIVEHEADERLENGTH-1<length)) {
      switch (i-RECEIVEHEADERLENGTH-1) {
        case 0:
          result=data<<8;
          break;
        case 1:
          result+=data;
          break;
      }
    }
    if (firstByte == STARTINGCODE) {
      if (i-RECEIVEHEADERLENGTH<=length) sum+=data; // Update checksum
      i++;
    }
    if (millis()-receiveStartMS > RECEIVEGLOBALTIMEOUTMS) return RECEIVEFAILED; // Timeout
  } while (i<length+RECEIVEHEADERLENGTH+2);

  if (data != sum) return RECEIVEFAILED; // Does checksum matches?
  return result;
}

/**@brief
 * Get total number of supported audio files on current drive
 *
 * @returns Number of files
 * @retval -1 Error (for example request timeout)
 */
int DFR0534::getTotalFiles()
{
  #define COMMAND 0x0C
  #define RECEIVEFAILED -1
  #define RECEIVEBYTETIMEOUTMS 100
  #define RECEIVEGLOBALTIMEOUTMS 500
  #define RECEIVEHEADERLENGTH 2 // startingcode+command

  if (m_ptrStream == NULL) return RECEIVEFAILED; // Should not happen
  sendStartingCode();
  sendDataByte(COMMAND);
  sendDataByte(0x00);
  sendCheckSum();

  // Receive
  int i=0;
  byte data, firstByte = 0, sum, length=0xff;
  word result = 0;
  unsigned long receiveStartMS = millis();
  do {
    byte dataReady = 0;
    unsigned long lastMS = millis();
    // Wait for response or timeout
    while ((millis()-lastMS < RECEIVEBYTETIMEOUTMS) && (dataReady==0)) dataReady = m_ptrStream->available();

    if (dataReady == 0) return RECEIVEFAILED; // Timeout
    data = m_ptrStream->read();

    if (i==0) { // Begin of transmission
      firstByte=data;
      sum = 0;
    }
    if ((i == 1) && (data != COMMAND)) {
      // Invalid signal => reset receive
      i=0;
      firstByte = 0;
    }
    if (i == RECEIVEHEADERLENGTH) {
      length = data; // Length of receiving data
      if (length != 2) {
        // Invalid length => reset receive
        i=0;
        firstByte = 0;
      }
    }
    if ((i > RECEIVEHEADERLENGTH) && (i-RECEIVEHEADERLENGTH-1<length)) {
      switch (i-RECEIVEHEADERLENGTH-1) {
        case 0:
          result=data<<8;
          break;
        case 1:
          result+=data;
          break;
      }
    }
    if (firstByte == STARTINGCODE) {
      if (i-RECEIVEHEADERLENGTH<=length) sum+=data; // Update checksum
      i++;
    }
    if (millis()-receiveStartMS > RECEIVEGLOBALTIMEOUTMS) return RECEIVEFAILED; // Timeout
  } while (i<length+RECEIVEHEADERLENGTH+2);

  if (data != sum) return RECEIVEFAILED; // Does checksum matches?
  return result;
}

/**@brief
 * Play last file in directory (in "file copy order")
 */
void DFR0534::playLastInDirectory()
{
  if (m_ptrStream == NULL) return; // Should not happen
  sendStartingCode();
  sendDataByte(0x0E);
  sendDataByte(0x00);
  sendCheckSum();
}

/**@brief
 * Play first file in next directory (in "file copy order")
 */
void DFR0534::playNextDirectory()
{
  if (m_ptrStream == NULL) return; // Should not happen
  sendStartingCode();
  sendDataByte(0x0F);
  sendDataByte(0x00);
  sendCheckSum();
}

/**@brief
 * Get number of first file in current directory
 *
 * @returns File number
 * @retval -1  Error (for example request timeout)
 */
int DFR0534::getFirstFileNumberInCurrentDirectory()
{
  #define COMMAND 0x11
  #define RECEIVEFAILED -1
  #define RECEIVEBYTETIMEOUTMS 100
  #define RECEIVEGLOBALTIMEOUTMS 500
  #define RECEIVEHEADERLENGTH 2 // startingcode+command

  if (m_ptrStream == NULL) RECEIVEFAILED; // Should not happen
  sendStartingCode();
  sendDataByte(COMMAND);
  sendDataByte(0x00);
  sendCheckSum();

  // Receive
  int i=0;
  byte data, firstByte = 0, sum, length=0xff;
  word result = 0;
  unsigned long receiveStartMS = millis();
  do {
    byte dataReady = 0;
    unsigned long lastMS = millis();
    // Wait for response or timeout
    while ((millis()-lastMS < RECEIVEBYTETIMEOUTMS) && (dataReady==0)) dataReady = m_ptrStream->available();

    if (dataReady == 0) return RECEIVEFAILED; // Timeout
    data = m_ptrStream->read();

    if (i==0) { // Begin of transmission
      firstByte=data;
      sum = 0;
    }
    if ((i == 1) && (data != COMMAND)) {
      // Invalid signal => reset receive
      i=0;
      firstByte = 0;
    }
    if (i == RECEIVEHEADERLENGTH) {
      length = data; // Length of receiving data
      if (length != 2) {
        // Invalid length => reset receive
        i=0;
        firstByte = 0;
      }
    }
    if ((i > RECEIVEHEADERLENGTH) && (i-RECEIVEHEADERLENGTH-1<length)) {
      switch (i-RECEIVEHEADERLENGTH-1) {
        case 0:
          result=data<<8;
          break;
        case 1:
          result+=data;
          break;
      }
    }
    if (firstByte == STARTINGCODE) {
      if (i-RECEIVEHEADERLENGTH<=length) sum+=data; // Update checksum
      i++;
    }
    if (millis()-receiveStartMS > RECEIVEGLOBALTIMEOUTMS) return RECEIVEFAILED; // Timeout
  } while (i<length+RECEIVEHEADERLENGTH+2);

  if (data != sum) return RECEIVEFAILED; // Does checksum matches?
  return result;
}

/**@brief
 * Count all audio files for the current directory
 *
 * @returns File count
 * @retval -1  Error (for example request timeout)
 */
int DFR0534::getTotalFilesInCurrentDirectory()
{
  #define COMMAND 0x12
  #define RECEIVEFAILED -1
  #define RECEIVEBYTETIMEOUTMS 100
  #define RECEIVEGLOBALTIMEOUTMS 500
  #define RECEIVEHEADERLENGTH 2 // startingcode+command

  if (m_ptrStream == NULL) RECEIVEFAILED; // Should not happen
  sendStartingCode();
  sendDataByte(COMMAND);
  sendDataByte(0x00);
  sendCheckSum();

  // Receive
  int i=0;
  byte data, firstByte = 0, sum, length=0xff;
  word result = 0;
  unsigned long receiveStartMS = millis();
  do {
    byte dataReady = 0;
    unsigned long lastMS = millis();
    // Wait for response or timeout
    while ((millis()-lastMS < RECEIVEBYTETIMEOUTMS) && (dataReady==0)) dataReady = m_ptrStream->available();

    if (dataReady == 0) return RECEIVEFAILED; // Timeout
    data = m_ptrStream->read();

    if (i==0) { // Begin of transmission
      firstByte=data;
      sum = 0;
    }
    if ((i == 1) && (data != COMMAND)) {
      // Invalid signal => reset receive
      i=0;
      firstByte = 0;
    }
    if (i == RECEIVEHEADERLENGTH) {
      length = data; // Length of receiving data
      if (length != 2) {
        // Invalid length => reset receive
        i=0;
        firstByte = 0;
      }
    }
    if ((i > RECEIVEHEADERLENGTH) && (i-RECEIVEHEADERLENGTH-1<length)) {
      switch (i-RECEIVEHEADERLENGTH-1) {
        case 0:
          result=data<<8;
          break;
        case 1:
          result+=data;
          break;
      }
    }
    if (firstByte == STARTINGCODE) {
      if (i-RECEIVEHEADERLENGTH<=length) sum+=data; // Update checksum
      i++;
    }
    if (millis()-receiveStartMS > RECEIVEGLOBALTIMEOUTMS) return RECEIVEFAILED; // Timeout
  } while (i<length+RECEIVEHEADERLENGTH+2);

  if (data != sum) return RECEIVEFAILED; // Does checksum matches?
  return result;
}

/**@brief
 * Increase volume by one step
 */
void DFR0534::increaseVolume()
{
  if (m_ptrStream == NULL) return; // Should not happen
  sendStartingCode();
  sendDataByte(0x14);
  sendDataByte(0x00);
  sendCheckSum();
}

/**@brief
 * Decrease volume by one step
 */
void DFR0534::decreaseVolume()
{
  if (m_ptrStream == NULL) return; // Should not happen
  sendStartingCode();
  sendDataByte(0x15);
  sendDataByte(0x00);
  sendCheckSum();
}

/**@brief
 * Pause current file and play another file by number
 *
 * File number order is "file copy order". Continue original file when this file stops
 *
 * @param[in] track  File number of the audio file
 * @param[in] drive  Drive, where file is stored: Drive DFR0534::DRIVEUSB, DFR0534::DRIVESD or DFR0534::DRIVEFLASH (=default)
 */
void DFR0534::insertFileByNumber(word track, byte drive)
{
  if (m_ptrStream == NULL) return; // Should not happen
  if (drive >= DRIVEUNKNOWN) return;
  sendStartingCode();
  sendDataByte(0x16);
  sendDataByte(0x03);
  sendDataByte(drive);
  sendDataByte((track >> 8) & 0xff);
  sendDataByte(track & 0xff);
  sendCheckSum();
}

/**@brief
 * Stop inserted file
 *
 * Continue original file
 */
void DFR0534::stopInsertedFile()
{
  if (m_ptrStream == NULL) return; // Should not happen
  sendStartingCode();
  sendDataByte(0x10);
  sendDataByte(0x00);
  sendCheckSum();
}

/**@brief
 * Should set directory, but does not work for me
 *
 * @param[in] path   Directory
 * @param[in] drive  Drive, where directory is stored: Drive DFR0534::DRIVEUSB, DFR0534::DRIVESD or DFR0534::DRIVEFLASH (=default)
 */
void DFR0534::setDirectory(char *path, byte drive)
{
  if (m_ptrStream == NULL) return; // Should not happen
  if (path == NULL) return;
  if (drive >= DRIVEUNKNOWN) return;
  sendStartingCode();
  sendDataByte(0x17);
  sendDataByte(strlen(path)+1);
  sendDataByte(drive);
  for (int i=0;i<strlen(path);i++) {
    sendDataByte(path[i]);
  }
  sendCheckSum();
}

/**@brief
 * Set loop mode
 *
 * @param[in] mode  Loop mode: DFR0534::LOOPBACKALL, DFR0534::SINGLEAUDIOLOOP, DFR0534::SINGLEAUDIOSTOP, DFR0534::PLAYRANDOM, DFR0534::DIRECTORYLOOP, DFR0534::RANDOMINDIRECTORY, DFR0534::SEQUENTIALINDIRECTORY or DFR0534::SEQUENTIAL
 */
void DFR0534::setLoopMode(byte mode)
{
  if (m_ptrStream == NULL) return; // Should not happen
  if (mode >= PLAYMODEUNKNOWN) return;
  sendStartingCode();
  sendDataByte(0x18);
  sendDataByte(0x01);
  sendDataByte(mode);
  sendCheckSum();
}

/**@brief
 * Set repeat loops
 *
 * Only valid for loop modes DFR0534::LOOPBACKALL, DFR0534::SINGLEAUDIOLOOP or DFR0534::DIRECTORYLOOP
 *
 * @param[in] loops  Number of loops
 */
void DFR0534::setRepeatLoops(word loops)
{
  if (m_ptrStream == NULL) return; // Should not happen
  sendStartingCode();
  sendDataByte(0x19);
  sendDataByte(0x02);
  sendDataByte((loops >> 8) & 0xff);
  sendDataByte(loops & 0xff);
  sendCheckSum();
}

/**@brief
 * Combined/concatenated play of files
 *
 * Combined is like a playlist, for example playCombined("0103") for
 * the two files 01 and 03.
 * The Filenames must be two chars long and the files must
 * be in a directory called /ZH
 * Combined playback ignores loop mode and stops after last file.
 *
 * @param[in] list  Concatenated list of all files to play
 */
void DFR0534::playCombined(char* list)
{
  if (m_ptrStream == NULL) return; // Should not happen
  if (list == NULL) return;
  if ((strlen(list) % 2) != 0) return;

  sendStartingCode();
  sendDataByte(0x1B);
  sendDataByte(strlen(list));
  for (int i=0;i<strlen(list);i++) {
    sendDataByte(list[i]);
  }
  sendCheckSum();
}

/**@brief
 * Stop combined play (playlist)
 */
void DFR0534::stopCombined()
{
  if (m_ptrStream == NULL) return; // Should not happen
  sendStartingCode();
  sendDataByte(0x1C);
  sendDataByte(0x00);
  sendCheckSum();
}

/**@brief
 * Set output for DAC to channel MP3, AUX or both
 *
 * I found not P26/P27 for AUX on my DFR0534 => Only DFR0534::CHANNELMP3 makes sense (and is already set by default)
 * Perhaps this function works on other audio modules with the same chip.
 *
 * @param[in] channel Output channel: DFR0534::CHANNELMP3, DFR0534::CHANNELAUX or DFR0534::CHANNELMP3AUX
 */
void DFR0534::setChannel(byte channel)
{
  if (m_ptrStream == NULL) return; // Should not happen
  if (channel >= CHANNELUNKNOWN) return;
  sendStartingCode();
  sendDataByte(0x1D);
  sendDataByte(0x01);
  sendDataByte(channel);
  sendCheckSum();
}

/**@brief
 * Get name for current file
 *
 * File name is in 8+3 format in upper case, with spaces
 * without the dot "." between name and extension,
 * e.g. "TEST   WAV" for the file test.wav
 *
 * @param[out] name  Filename. You have to allocate at least 12 chars memory for this variable.
 */
bool DFR0534::getFileName(char *name)
{
  #define COMMAND 0x1E
  #define RECEIVEBYTETIMEOUTMS 100
  #define RECEIVEGLOBALTIMEOUTMS 500
  #define RECEIVEFAILED false
  #define RECEIVEHEADERLENGTH 2 // startingcode+command

  if (m_ptrStream == NULL) return false; // Should not happen
  if (name == NULL) return false;
  name[0] = '\0';

  sendStartingCode();
  sendDataByte(COMMAND);
  sendDataByte(0x00);
  sendCheckSum();

  // Receive
  int i=0;
  byte data, firstByte = 0, sum, length=0xff;
  unsigned long receiveStartMS = millis();
  do {
    byte dataReady = 0;
    unsigned long lastMS = millis();
    // Wait for response or timeout
    while ((millis()-lastMS < RECEIVEBYTETIMEOUTMS) && (dataReady==0)) dataReady = m_ptrStream->available();

    if (dataReady == 0) return RECEIVEFAILED; // Timeout
    data = m_ptrStream->read();
    if (i==0) { // Begin of transmission
      firstByte=data;
      sum = 0;
    }
    if ((i == 1) && (data != COMMAND)) {
      // Invalid signal => reset receive
      i=0;
      firstByte = 0;
    }
    if (i == RECEIVEHEADERLENGTH) length = data; // Length of receiving string
    if ((i > RECEIVEHEADERLENGTH) && (i-RECEIVEHEADERLENGTH-1<length)) {
      if ((i-RECEIVEHEADERLENGTH) < 12) { // I expect no longer file names than 8+3 chars plus '\0'
        name[i-RECEIVEHEADERLENGTH-1] = data;
        name[i-RECEIVEHEADERLENGTH] = '\0';
      }
    }
    if (firstByte == STARTINGCODE) {
      if (i-RECEIVEHEADERLENGTH<=length) sum+=data; // Update checksum
      i++;
    }
    if (millis()-receiveStartMS > RECEIVEGLOBALTIMEOUTMS) return RECEIVEFAILED; // Timeout
  } while (i<length+RECEIVEHEADERLENGTH+2);
  return (data == sum); // Does checksum matches?
}

/**@brief
 * Select file by number, but not start playing
 *
 * @param[in] track  Number for file
 */
void DFR0534::prepareFileByNumber(word track)
{
  if (m_ptrStream == NULL) return; // Should not happen
  sendStartingCode();
  sendDataByte(0x1F);
  sendDataByte(0x02);
  sendDataByte((track >> 8) & 0xff);
  sendDataByte(track & 0xff);
  sendCheckSum();
}

/**@brief
 * Repeat part of the current file
 *
 * Repeat between time start and stop position
 *
 * @param[in] startMinute  Minute for start position
 * @param[in] startSecond  Second for start position
 * @param[in] stopMinute   Minute for stop position
 * @param[in] stopSecond   Seconde for stop position
 */
void DFR0534::repeatPart(byte startMinute, byte startSecond, byte stopMinute, byte stopSecond )
{
  if (m_ptrStream == NULL) return; // Should not happen
  sendStartingCode();
  sendDataByte(0x20);
  sendDataByte(0x04);
  sendDataByte(startMinute);
  sendDataByte(startSecond);
  sendDataByte(stopMinute);
  sendDataByte(stopSecond);
  sendCheckSum();
}

/**@brief
 * Stop repeating part of the current file
 */
void DFR0534::stopRepeatPart()
{
  if (m_ptrStream == NULL) return; // Should not happen
  sendStartingCode();
  sendDataByte(0x21);
  sendDataByte(0x00);
  sendCheckSum();
}

/**@brief
 * Fast backward
 *
 * Fast backward in seconds
 *
 * @param[in] seconds  Seconds to go backward
 */
void DFR0534::fastBackwardDuration(word seconds)
{
  if (m_ptrStream == NULL) return; // Should not happen
  sendStartingCode();
  sendDataByte(0x22);
  sendDataByte(0x02);
  sendDataByte((seconds >> 8) & 0xff);
  sendDataByte(seconds & 0xff);
  sendCheckSum();
}

/**@brief
 *
 * Fast forward in seconds
 *
 * @param[in] seconds  Seconds to go forward
 */
void DFR0534::fastForwardDuration(word seconds)
{
  if (m_ptrStream == NULL) return; // Should not happen
  sendStartingCode();
  sendDataByte(0x23);
  sendDataByte(0x02);
  sendDataByte((seconds >> 8) & 0xff);
  sendDataByte(seconds & 0xff);
  sendCheckSum();
}

/**@brief
 * Get duration/length of current file
 *
 * Get duration/length of current file in hours:minutes:seconds
 *
 * @param[out] hour   Hours
 * @param[out] minute Minutes
 * @param[out] second Seconds
 *
 * @retval true  Request was successful
 * @retval false Request failed
 */
bool DFR0534::getDuration(byte &hour, byte &minute, byte &second)
{
  #define COMMAND 0x24
  #define RECEIVEFAILED false
  #define RECEIVEBYTETIMEOUTMS 100
  #define RECEIVEGLOBALTIMEOUTMS 500
  #define RECEIVEHEADERLENGTH 2 // startingcode+command

  if (m_ptrStream == NULL) return false; // Should not happen
  sendStartingCode();
  sendDataByte(COMMAND);
  sendDataByte(0x00);
  sendCheckSum();

  // Receive
  int i=0;
  byte data, firstByte = 0, sum, length=0xff;
  word result = 0;
  unsigned long receiveStartMS = millis();
  do {
    byte dataReady = 0;
    unsigned long lastMS = millis();
    // Wait for response or timeout
    while ((millis()-lastMS < RECEIVEBYTETIMEOUTMS) && (dataReady==0)) dataReady = m_ptrStream->available();

    if (dataReady == 0) return RECEIVEFAILED; // Timeout
    data = m_ptrStream->read();

    if (i==0) { // Begin of transmission
      firstByte=data;
      sum = 0;
    }
    if ((i == 1) && (data != COMMAND)) {
      // Invalid signal => reset receive
      i=0;
      firstByte = 0;
    }
    if (i == RECEIVEHEADERLENGTH) {
      length = data; // Length of receiving data
      if (length != 3) {
        // Invalid length => reset receive
        i=0;
        firstByte = 0;
      }
    }
    if ((i > RECEIVEHEADERLENGTH) && (i-RECEIVEHEADERLENGTH-1<length)) {
      switch (i-RECEIVEHEADERLENGTH-1) {
        case 0:
          hour=data;
          break;
        case 1:
          minute=data;
          break;
        case 2:
          second=data;
          break;
      }
    }
    if (firstByte == STARTINGCODE) {
      if (i-RECEIVEHEADERLENGTH<=length) sum+=data; // Update checksum
      i++;
    }
    if (millis()-receiveStartMS > RECEIVEGLOBALTIMEOUTMS) return RECEIVEFAILED; // Timeout
  } while (i<length+RECEIVEHEADERLENGTH+2);

  return (data == sum); // Does checksum matches?
}

/**@brief
 * Start sending elapsed runtime every 1 second
 */
void DFR0534::startSendingRuntime()
{
  if (m_ptrStream == NULL) return; // Should not happen
  sendStartingCode();
  sendDataByte(0x25);
  sendDataByte(0x00);
  sendCheckSum();
}

/**@brief
 * Get elapsed runtime/duration of the current file
 *
 * Runtime is in hours:minutes:seconds. You have to call startSendingRuntime() before runtimes can
 * be received.
 *
 * @param[out] hour   Hours
 * @param[out] minute Minutes
 * @param[out] second Seconds
 *
 * @retval true  Request was successful
 * @retval false Request failed
 */
bool DFR0534::getRuntime(byte &hour, byte &minute, byte &second)
{
  #define COMMAND 0x25
  #define RECEIVEFAILED false
  #define RECEIVEBYTETIMEOUTMS 100
  #define RECEIVEGLOBALTIMEOUTMS 500
  #define RECEIVEHEADERLENGTH 2 // startingcode+command

  if (m_ptrStream == NULL) return false; // Should not happen

  // Receive
  int i=0;
  byte data, firstByte = 0, sum, length=0xff;
  word result = 0;
  unsigned long receiveStartMS = millis();
  do {
    byte dataReady = 0;
    unsigned long lastMS = millis();
    // Wait for response or timeout
    while ((millis()-lastMS < RECEIVEBYTETIMEOUTMS) && (dataReady==0)) dataReady = m_ptrStream->available();

    if (dataReady == 0) return RECEIVEFAILED; // Timeout
    data = m_ptrStream->read();

    if (i==0) { // Begin of transmission
      firstByte=data;
      sum = 0;
    }
    if ((i == 1) && (data != COMMAND)) {
      // Invalid signal => reset receive
      i=0;
      firstByte = 0;
    }
    if (i == RECEIVEHEADERLENGTH) {
      length = data; // Length of receiving data
      if (length != 3) {
        // Invalid length => reset receive
        i=0;
        firstByte = 0;
      }
    }
    if ((i > RECEIVEHEADERLENGTH) && (i-RECEIVEHEADERLENGTH-1<length)) {
      switch (i-RECEIVEHEADERLENGTH-1) {
        case 0:
          hour=data;
          break;
        case 1:
          minute=data;
          break;
        case 2:
          second=data;
          break;
      }
    }
    if (firstByte == STARTINGCODE) {
      if (i-RECEIVEHEADERLENGTH<=length) sum+=data; // Update checksum
      i++;
    }
    if (millis()-receiveStartMS > RECEIVEGLOBALTIMEOUTMS) return RECEIVEFAILED; // Timeout
  } while (i<length+RECEIVEHEADERLENGTH+2);

  return (data == sum); // Does checksum matches?
}

/**@brief
 * Stop sending runtime
 */
void DFR0534::stopSendingRuntime()
{
  if (m_ptrStream == NULL) return; // Should not happen
  sendStartingCode();
  sendDataByte(0x26);
  sendDataByte(0x00);
  sendCheckSum();
}
