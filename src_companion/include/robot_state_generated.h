// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_ROBOTSTATE_FBS_H_
#define FLATBUFFERS_GENERATED_ROBOTSTATE_FBS_H_

#include "flatbuffers/flatbuffers.h"

// Ensure the included flatbuffers.h is the same version as when this file was
// generated, otherwise it may not be compatible.
static_assert(FLATBUFFERS_VERSION_MAJOR == 24 &&
              FLATBUFFERS_VERSION_MINOR == 3 &&
              FLATBUFFERS_VERSION_REVISION == 25,
             "Non-compatible flatbuffers version included");

namespace fbs {

struct Motor;
struct MotorBuilder;
struct MotorT;

struct Motors;
struct MotorsBuilder;
struct MotorsT;

struct DisplayMessages;
struct DisplayMessagesBuilder;
struct DisplayMessagesT;

struct Robot;
struct RobotBuilder;
struct RobotT;

enum ControlSource : int8_t {
  ControlSource_Manual = 0,
  ControlSource_FlightController = 1,
  ControlSource_OnboardComputer = 2,
  ControlSource_RC = 3,
  ControlSource_MIN = ControlSource_Manual,
  ControlSource_MAX = ControlSource_RC
};

inline const ControlSource (&EnumValuesControlSource())[4] {
  static const ControlSource values[] = {
    ControlSource_Manual,
    ControlSource_FlightController,
    ControlSource_OnboardComputer,
    ControlSource_RC
  };
  return values;
}

inline const char * const *EnumNamesControlSource() {
  static const char * const names[5] = {
    "Manual",
    "FlightController",
    "OnboardComputer",
    "RC",
    nullptr
  };
  return names;
}

inline const char *EnumNameControlSource(ControlSource e) {
  if (::flatbuffers::IsOutRange(e, ControlSource_Manual, ControlSource_RC)) return "";
  const size_t index = static_cast<size_t>(e);
  return EnumNamesControlSource()[index];
}

enum Status : int8_t {
  Status_OK = 0,
  Status_INIT = 1,
  Status_NOTX = 2,
  Status_BAT = 3,
  Status_TEMP = 4,
  Status_OFF = 5,
  Status_MIN = Status_OK,
  Status_MAX = Status_OFF
};

inline const Status (&EnumValuesStatus())[6] {
  static const Status values[] = {
    Status_OK,
    Status_INIT,
    Status_NOTX,
    Status_BAT,
    Status_TEMP,
    Status_OFF
  };
  return values;
}

inline const char * const *EnumNamesStatus() {
  static const char * const names[7] = {
    "OK",
    "INIT",
    "NOTX",
    "BAT",
    "TEMP",
    "OFF",
    nullptr
  };
  return names;
}

inline const char *EnumNameStatus(Status e) {
  if (::flatbuffers::IsOutRange(e, Status_OK, Status_OFF)) return "";
  const size_t index = static_cast<size_t>(e);
  return EnumNamesStatus()[index];
}

struct MotorT : public ::flatbuffers::NativeTable {
  typedef Motor TableType;
  float temperature = 0.0f;
  float rps = 0.0f;
  float angle = 0.0f;
  float torque = 0.0f;
};

struct Motor FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef MotorT NativeTableType;
  typedef MotorBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_TEMPERATURE = 4,
    VT_RPS = 6,
    VT_ANGLE = 8,
    VT_TORQUE = 10
  };
  float temperature() const {
    return GetField<float>(VT_TEMPERATURE, 0.0f);
  }
  float rps() const {
    return GetField<float>(VT_RPS, 0.0f);
  }
  float angle() const {
    return GetField<float>(VT_ANGLE, 0.0f);
  }
  float torque() const {
    return GetField<float>(VT_TORQUE, 0.0f);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<float>(verifier, VT_TEMPERATURE, 4) &&
           VerifyField<float>(verifier, VT_RPS, 4) &&
           VerifyField<float>(verifier, VT_ANGLE, 4) &&
           VerifyField<float>(verifier, VT_TORQUE, 4) &&
           verifier.EndTable();
  }
  MotorT *UnPack(const ::flatbuffers::resolver_function_t *_resolver = nullptr) const;
  void UnPackTo(MotorT *_o, const ::flatbuffers::resolver_function_t *_resolver = nullptr) const;
  static ::flatbuffers::Offset<Motor> Pack(::flatbuffers::FlatBufferBuilder &_fbb, const MotorT* _o, const ::flatbuffers::rehasher_function_t *_rehasher = nullptr);
};

struct MotorBuilder {
  typedef Motor Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_temperature(float temperature) {
    fbb_.AddElement<float>(Motor::VT_TEMPERATURE, temperature, 0.0f);
  }
  void add_rps(float rps) {
    fbb_.AddElement<float>(Motor::VT_RPS, rps, 0.0f);
  }
  void add_angle(float angle) {
    fbb_.AddElement<float>(Motor::VT_ANGLE, angle, 0.0f);
  }
  void add_torque(float torque) {
    fbb_.AddElement<float>(Motor::VT_TORQUE, torque, 0.0f);
  }
  explicit MotorBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<Motor> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<Motor>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<Motor> CreateMotor(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    float temperature = 0.0f,
    float rps = 0.0f,
    float angle = 0.0f,
    float torque = 0.0f) {
  MotorBuilder builder_(_fbb);
  builder_.add_torque(torque);
  builder_.add_angle(angle);
  builder_.add_rps(rps);
  builder_.add_temperature(temperature);
  return builder_.Finish();
}

::flatbuffers::Offset<Motor> CreateMotor(::flatbuffers::FlatBufferBuilder &_fbb, const MotorT *_o, const ::flatbuffers::rehasher_function_t *_rehasher = nullptr);

struct MotorsT : public ::flatbuffers::NativeTable {
  typedef Motors TableType;
  std::unique_ptr<fbs::MotorT> motor1{};
  std::unique_ptr<fbs::MotorT> motor2{};
  MotorsT() = default;
  MotorsT(const MotorsT &o);
  MotorsT(MotorsT&&) FLATBUFFERS_NOEXCEPT = default;
  MotorsT &operator=(MotorsT o) FLATBUFFERS_NOEXCEPT;
};

struct Motors FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef MotorsT NativeTableType;
  typedef MotorsBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_MOTOR1 = 4,
    VT_MOTOR2 = 6
  };
  const fbs::Motor *motor1() const {
    return GetPointer<const fbs::Motor *>(VT_MOTOR1);
  }
  const fbs::Motor *motor2() const {
    return GetPointer<const fbs::Motor *>(VT_MOTOR2);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_MOTOR1) &&
           verifier.VerifyTable(motor1()) &&
           VerifyOffset(verifier, VT_MOTOR2) &&
           verifier.VerifyTable(motor2()) &&
           verifier.EndTable();
  }
  MotorsT *UnPack(const ::flatbuffers::resolver_function_t *_resolver = nullptr) const;
  void UnPackTo(MotorsT *_o, const ::flatbuffers::resolver_function_t *_resolver = nullptr) const;
  static ::flatbuffers::Offset<Motors> Pack(::flatbuffers::FlatBufferBuilder &_fbb, const MotorsT* _o, const ::flatbuffers::rehasher_function_t *_rehasher = nullptr);
};

struct MotorsBuilder {
  typedef Motors Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_motor1(::flatbuffers::Offset<fbs::Motor> motor1) {
    fbb_.AddOffset(Motors::VT_MOTOR1, motor1);
  }
  void add_motor2(::flatbuffers::Offset<fbs::Motor> motor2) {
    fbb_.AddOffset(Motors::VT_MOTOR2, motor2);
  }
  explicit MotorsBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<Motors> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<Motors>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<Motors> CreateMotors(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    ::flatbuffers::Offset<fbs::Motor> motor1 = 0,
    ::flatbuffers::Offset<fbs::Motor> motor2 = 0) {
  MotorsBuilder builder_(_fbb);
  builder_.add_motor2(motor2);
  builder_.add_motor1(motor1);
  return builder_.Finish();
}

::flatbuffers::Offset<Motors> CreateMotors(::flatbuffers::FlatBufferBuilder &_fbb, const MotorsT *_o, const ::flatbuffers::rehasher_function_t *_rehasher = nullptr);

struct DisplayMessagesT : public ::flatbuffers::NativeTable {
  typedef DisplayMessages TableType;
  std::string message1{};
  std::string message2{};
  std::string message3{};
  std::string message4{};
  std::string message5{};
  std::string message6{};
  std::string message7{};
};

struct DisplayMessages FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef DisplayMessagesT NativeTableType;
  typedef DisplayMessagesBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_MESSAGE1 = 4,
    VT_MESSAGE2 = 6,
    VT_MESSAGE3 = 8,
    VT_MESSAGE4 = 10,
    VT_MESSAGE5 = 12,
    VT_MESSAGE6 = 14,
    VT_MESSAGE7 = 16
  };
  const ::flatbuffers::String *message1() const {
    return GetPointer<const ::flatbuffers::String *>(VT_MESSAGE1);
  }
  const ::flatbuffers::String *message2() const {
    return GetPointer<const ::flatbuffers::String *>(VT_MESSAGE2);
  }
  const ::flatbuffers::String *message3() const {
    return GetPointer<const ::flatbuffers::String *>(VT_MESSAGE3);
  }
  const ::flatbuffers::String *message4() const {
    return GetPointer<const ::flatbuffers::String *>(VT_MESSAGE4);
  }
  const ::flatbuffers::String *message5() const {
    return GetPointer<const ::flatbuffers::String *>(VT_MESSAGE5);
  }
  const ::flatbuffers::String *message6() const {
    return GetPointer<const ::flatbuffers::String *>(VT_MESSAGE6);
  }
  const ::flatbuffers::String *message7() const {
    return GetPointer<const ::flatbuffers::String *>(VT_MESSAGE7);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_MESSAGE1) &&
           verifier.VerifyString(message1()) &&
           VerifyOffset(verifier, VT_MESSAGE2) &&
           verifier.VerifyString(message2()) &&
           VerifyOffset(verifier, VT_MESSAGE3) &&
           verifier.VerifyString(message3()) &&
           VerifyOffset(verifier, VT_MESSAGE4) &&
           verifier.VerifyString(message4()) &&
           VerifyOffset(verifier, VT_MESSAGE5) &&
           verifier.VerifyString(message5()) &&
           VerifyOffset(verifier, VT_MESSAGE6) &&
           verifier.VerifyString(message6()) &&
           VerifyOffset(verifier, VT_MESSAGE7) &&
           verifier.VerifyString(message7()) &&
           verifier.EndTable();
  }
  DisplayMessagesT *UnPack(const ::flatbuffers::resolver_function_t *_resolver = nullptr) const;
  void UnPackTo(DisplayMessagesT *_o, const ::flatbuffers::resolver_function_t *_resolver = nullptr) const;
  static ::flatbuffers::Offset<DisplayMessages> Pack(::flatbuffers::FlatBufferBuilder &_fbb, const DisplayMessagesT* _o, const ::flatbuffers::rehasher_function_t *_rehasher = nullptr);
};

struct DisplayMessagesBuilder {
  typedef DisplayMessages Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_message1(::flatbuffers::Offset<::flatbuffers::String> message1) {
    fbb_.AddOffset(DisplayMessages::VT_MESSAGE1, message1);
  }
  void add_message2(::flatbuffers::Offset<::flatbuffers::String> message2) {
    fbb_.AddOffset(DisplayMessages::VT_MESSAGE2, message2);
  }
  void add_message3(::flatbuffers::Offset<::flatbuffers::String> message3) {
    fbb_.AddOffset(DisplayMessages::VT_MESSAGE3, message3);
  }
  void add_message4(::flatbuffers::Offset<::flatbuffers::String> message4) {
    fbb_.AddOffset(DisplayMessages::VT_MESSAGE4, message4);
  }
  void add_message5(::flatbuffers::Offset<::flatbuffers::String> message5) {
    fbb_.AddOffset(DisplayMessages::VT_MESSAGE5, message5);
  }
  void add_message6(::flatbuffers::Offset<::flatbuffers::String> message6) {
    fbb_.AddOffset(DisplayMessages::VT_MESSAGE6, message6);
  }
  void add_message7(::flatbuffers::Offset<::flatbuffers::String> message7) {
    fbb_.AddOffset(DisplayMessages::VT_MESSAGE7, message7);
  }
  explicit DisplayMessagesBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<DisplayMessages> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<DisplayMessages>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<DisplayMessages> CreateDisplayMessages(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    ::flatbuffers::Offset<::flatbuffers::String> message1 = 0,
    ::flatbuffers::Offset<::flatbuffers::String> message2 = 0,
    ::flatbuffers::Offset<::flatbuffers::String> message3 = 0,
    ::flatbuffers::Offset<::flatbuffers::String> message4 = 0,
    ::flatbuffers::Offset<::flatbuffers::String> message5 = 0,
    ::flatbuffers::Offset<::flatbuffers::String> message6 = 0,
    ::flatbuffers::Offset<::flatbuffers::String> message7 = 0) {
  DisplayMessagesBuilder builder_(_fbb);
  builder_.add_message7(message7);
  builder_.add_message6(message6);
  builder_.add_message5(message5);
  builder_.add_message4(message4);
  builder_.add_message3(message3);
  builder_.add_message2(message2);
  builder_.add_message1(message1);
  return builder_.Finish();
}

inline ::flatbuffers::Offset<DisplayMessages> CreateDisplayMessagesDirect(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    const char *message1 = nullptr,
    const char *message2 = nullptr,
    const char *message3 = nullptr,
    const char *message4 = nullptr,
    const char *message5 = nullptr,
    const char *message6 = nullptr,
    const char *message7 = nullptr) {
  auto message1__ = message1 ? _fbb.CreateString(message1) : 0;
  auto message2__ = message2 ? _fbb.CreateString(message2) : 0;
  auto message3__ = message3 ? _fbb.CreateString(message3) : 0;
  auto message4__ = message4 ? _fbb.CreateString(message4) : 0;
  auto message5__ = message5 ? _fbb.CreateString(message5) : 0;
  auto message6__ = message6 ? _fbb.CreateString(message6) : 0;
  auto message7__ = message7 ? _fbb.CreateString(message7) : 0;
  return fbs::CreateDisplayMessages(
      _fbb,
      message1__,
      message2__,
      message3__,
      message4__,
      message5__,
      message6__,
      message7__);
}

::flatbuffers::Offset<DisplayMessages> CreateDisplayMessages(::flatbuffers::FlatBufferBuilder &_fbb, const DisplayMessagesT *_o, const ::flatbuffers::rehasher_function_t *_rehasher = nullptr);

struct RobotT : public ::flatbuffers::NativeTable {
  typedef Robot TableType;
  int32_t batteries = 4;
  fbs::ControlSource control_source = fbs::ControlSource_RC;
  fbs::Status status = fbs::Status_OK;
  std::unique_ptr<fbs::MotorsT> motors{};
  float voltage = 0.0f;
  float current = 0.0f;
  float fuel = 0.0f;
  float rssi = 0.0f;
  int32_t link_quality = 0;
  float max_speed = 0.0f;
  float low_voltage_threshold = 12.0f;
  float critical_voltage_threshold = 16.0f;
  int32_t rssi_threshold = 0;
  int32_t link_quality_threshold = 0;
  int32_t reference_wheel_angle = 0;
  std::string motor_error_code{};
  bool wheels_folded = false;
  bool enable_rover = false;
  std::unique_ptr<fbs::DisplayMessagesT> display_messages{};
  RobotT() = default;
  RobotT(const RobotT &o);
  RobotT(RobotT&&) FLATBUFFERS_NOEXCEPT = default;
  RobotT &operator=(RobotT o) FLATBUFFERS_NOEXCEPT;
};

struct Robot FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef RobotT NativeTableType;
  typedef RobotBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_BATTERIES = 4,
    VT_CONTROL_SOURCE = 6,
    VT_STATUS = 8,
    VT_MOTORS = 10,
    VT_VOLTAGE = 12,
    VT_CURRENT = 14,
    VT_FUEL = 16,
    VT_RSSI = 18,
    VT_LINK_QUALITY = 20,
    VT_MAX_SPEED = 22,
    VT_LOW_VOLTAGE_THRESHOLD = 24,
    VT_CRITICAL_VOLTAGE_THRESHOLD = 26,
    VT_RSSI_THRESHOLD = 28,
    VT_LINK_QUALITY_THRESHOLD = 30,
    VT_REFERENCE_WHEEL_ANGLE = 32,
    VT_MOTOR_ERROR_CODE = 34,
    VT_WHEELS_FOLDED = 36,
    VT_ENABLE_ROVER = 38,
    VT_DISPLAY_MESSAGES = 40
  };
  int32_t batteries() const {
    return GetField<int32_t>(VT_BATTERIES, 4);
  }
  fbs::ControlSource control_source() const {
    return static_cast<fbs::ControlSource>(GetField<int8_t>(VT_CONTROL_SOURCE, 3));
  }
  fbs::Status status() const {
    return static_cast<fbs::Status>(GetField<int8_t>(VT_STATUS, 0));
  }
  const fbs::Motors *motors() const {
    return GetPointer<const fbs::Motors *>(VT_MOTORS);
  }
  float voltage() const {
    return GetField<float>(VT_VOLTAGE, 0.0f);
  }
  float current() const {
    return GetField<float>(VT_CURRENT, 0.0f);
  }
  float fuel() const {
    return GetField<float>(VT_FUEL, 0.0f);
  }
  float rssi() const {
    return GetField<float>(VT_RSSI, 0.0f);
  }
  int32_t link_quality() const {
    return GetField<int32_t>(VT_LINK_QUALITY, 0);
  }
  float max_speed() const {
    return GetField<float>(VT_MAX_SPEED, 0.0f);
  }
  float low_voltage_threshold() const {
    return GetField<float>(VT_LOW_VOLTAGE_THRESHOLD, 12.0f);
  }
  float critical_voltage_threshold() const {
    return GetField<float>(VT_CRITICAL_VOLTAGE_THRESHOLD, 16.0f);
  }
  int32_t rssi_threshold() const {
    return GetField<int32_t>(VT_RSSI_THRESHOLD, 0);
  }
  int32_t link_quality_threshold() const {
    return GetField<int32_t>(VT_LINK_QUALITY_THRESHOLD, 0);
  }
  int32_t reference_wheel_angle() const {
    return GetField<int32_t>(VT_REFERENCE_WHEEL_ANGLE, 0);
  }
  const ::flatbuffers::String *motor_error_code() const {
    return GetPointer<const ::flatbuffers::String *>(VT_MOTOR_ERROR_CODE);
  }
  bool wheels_folded() const {
    return GetField<uint8_t>(VT_WHEELS_FOLDED, 0) != 0;
  }
  bool enable_rover() const {
    return GetField<uint8_t>(VT_ENABLE_ROVER, 0) != 0;
  }
  const fbs::DisplayMessages *display_messages() const {
    return GetPointer<const fbs::DisplayMessages *>(VT_DISPLAY_MESSAGES);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<int32_t>(verifier, VT_BATTERIES, 4) &&
           VerifyField<int8_t>(verifier, VT_CONTROL_SOURCE, 1) &&
           VerifyField<int8_t>(verifier, VT_STATUS, 1) &&
           VerifyOffset(verifier, VT_MOTORS) &&
           verifier.VerifyTable(motors()) &&
           VerifyField<float>(verifier, VT_VOLTAGE, 4) &&
           VerifyField<float>(verifier, VT_CURRENT, 4) &&
           VerifyField<float>(verifier, VT_FUEL, 4) &&
           VerifyField<float>(verifier, VT_RSSI, 4) &&
           VerifyField<int32_t>(verifier, VT_LINK_QUALITY, 4) &&
           VerifyField<float>(verifier, VT_MAX_SPEED, 4) &&
           VerifyField<float>(verifier, VT_LOW_VOLTAGE_THRESHOLD, 4) &&
           VerifyField<float>(verifier, VT_CRITICAL_VOLTAGE_THRESHOLD, 4) &&
           VerifyField<int32_t>(verifier, VT_RSSI_THRESHOLD, 4) &&
           VerifyField<int32_t>(verifier, VT_LINK_QUALITY_THRESHOLD, 4) &&
           VerifyField<int32_t>(verifier, VT_REFERENCE_WHEEL_ANGLE, 4) &&
           VerifyOffset(verifier, VT_MOTOR_ERROR_CODE) &&
           verifier.VerifyString(motor_error_code()) &&
           VerifyField<uint8_t>(verifier, VT_WHEELS_FOLDED, 1) &&
           VerifyField<uint8_t>(verifier, VT_ENABLE_ROVER, 1) &&
           VerifyOffset(verifier, VT_DISPLAY_MESSAGES) &&
           verifier.VerifyTable(display_messages()) &&
           verifier.EndTable();
  }
  RobotT *UnPack(const ::flatbuffers::resolver_function_t *_resolver = nullptr) const;
  void UnPackTo(RobotT *_o, const ::flatbuffers::resolver_function_t *_resolver = nullptr) const;
  static ::flatbuffers::Offset<Robot> Pack(::flatbuffers::FlatBufferBuilder &_fbb, const RobotT* _o, const ::flatbuffers::rehasher_function_t *_rehasher = nullptr);
};

struct RobotBuilder {
  typedef Robot Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_batteries(int32_t batteries) {
    fbb_.AddElement<int32_t>(Robot::VT_BATTERIES, batteries, 4);
  }
  void add_control_source(fbs::ControlSource control_source) {
    fbb_.AddElement<int8_t>(Robot::VT_CONTROL_SOURCE, static_cast<int8_t>(control_source), 3);
  }
  void add_status(fbs::Status status) {
    fbb_.AddElement<int8_t>(Robot::VT_STATUS, static_cast<int8_t>(status), 0);
  }
  void add_motors(::flatbuffers::Offset<fbs::Motors> motors) {
    fbb_.AddOffset(Robot::VT_MOTORS, motors);
  }
  void add_voltage(float voltage) {
    fbb_.AddElement<float>(Robot::VT_VOLTAGE, voltage, 0.0f);
  }
  void add_current(float current) {
    fbb_.AddElement<float>(Robot::VT_CURRENT, current, 0.0f);
  }
  void add_fuel(float fuel) {
    fbb_.AddElement<float>(Robot::VT_FUEL, fuel, 0.0f);
  }
  void add_rssi(float rssi) {
    fbb_.AddElement<float>(Robot::VT_RSSI, rssi, 0.0f);
  }
  void add_link_quality(int32_t link_quality) {
    fbb_.AddElement<int32_t>(Robot::VT_LINK_QUALITY, link_quality, 0);
  }
  void add_max_speed(float max_speed) {
    fbb_.AddElement<float>(Robot::VT_MAX_SPEED, max_speed, 0.0f);
  }
  void add_low_voltage_threshold(float low_voltage_threshold) {
    fbb_.AddElement<float>(Robot::VT_LOW_VOLTAGE_THRESHOLD, low_voltage_threshold, 12.0f);
  }
  void add_critical_voltage_threshold(float critical_voltage_threshold) {
    fbb_.AddElement<float>(Robot::VT_CRITICAL_VOLTAGE_THRESHOLD, critical_voltage_threshold, 16.0f);
  }
  void add_rssi_threshold(int32_t rssi_threshold) {
    fbb_.AddElement<int32_t>(Robot::VT_RSSI_THRESHOLD, rssi_threshold, 0);
  }
  void add_link_quality_threshold(int32_t link_quality_threshold) {
    fbb_.AddElement<int32_t>(Robot::VT_LINK_QUALITY_THRESHOLD, link_quality_threshold, 0);
  }
  void add_reference_wheel_angle(int32_t reference_wheel_angle) {
    fbb_.AddElement<int32_t>(Robot::VT_REFERENCE_WHEEL_ANGLE, reference_wheel_angle, 0);
  }
  void add_motor_error_code(::flatbuffers::Offset<::flatbuffers::String> motor_error_code) {
    fbb_.AddOffset(Robot::VT_MOTOR_ERROR_CODE, motor_error_code);
  }
  void add_wheels_folded(bool wheels_folded) {
    fbb_.AddElement<uint8_t>(Robot::VT_WHEELS_FOLDED, static_cast<uint8_t>(wheels_folded), 0);
  }
  void add_enable_rover(bool enable_rover) {
    fbb_.AddElement<uint8_t>(Robot::VT_ENABLE_ROVER, static_cast<uint8_t>(enable_rover), 0);
  }
  void add_display_messages(::flatbuffers::Offset<fbs::DisplayMessages> display_messages) {
    fbb_.AddOffset(Robot::VT_DISPLAY_MESSAGES, display_messages);
  }
  explicit RobotBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<Robot> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<Robot>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<Robot> CreateRobot(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    int32_t batteries = 4,
    fbs::ControlSource control_source = fbs::ControlSource_RC,
    fbs::Status status = fbs::Status_OK,
    ::flatbuffers::Offset<fbs::Motors> motors = 0,
    float voltage = 0.0f,
    float current = 0.0f,
    float fuel = 0.0f,
    float rssi = 0.0f,
    int32_t link_quality = 0,
    float max_speed = 0.0f,
    float low_voltage_threshold = 12.0f,
    float critical_voltage_threshold = 16.0f,
    int32_t rssi_threshold = 0,
    int32_t link_quality_threshold = 0,
    int32_t reference_wheel_angle = 0,
    ::flatbuffers::Offset<::flatbuffers::String> motor_error_code = 0,
    bool wheels_folded = false,
    bool enable_rover = false,
    ::flatbuffers::Offset<fbs::DisplayMessages> display_messages = 0) {
  RobotBuilder builder_(_fbb);
  builder_.add_display_messages(display_messages);
  builder_.add_motor_error_code(motor_error_code);
  builder_.add_reference_wheel_angle(reference_wheel_angle);
  builder_.add_link_quality_threshold(link_quality_threshold);
  builder_.add_rssi_threshold(rssi_threshold);
  builder_.add_critical_voltage_threshold(critical_voltage_threshold);
  builder_.add_low_voltage_threshold(low_voltage_threshold);
  builder_.add_max_speed(max_speed);
  builder_.add_link_quality(link_quality);
  builder_.add_rssi(rssi);
  builder_.add_fuel(fuel);
  builder_.add_current(current);
  builder_.add_voltage(voltage);
  builder_.add_motors(motors);
  builder_.add_batteries(batteries);
  builder_.add_enable_rover(enable_rover);
  builder_.add_wheels_folded(wheels_folded);
  builder_.add_status(status);
  builder_.add_control_source(control_source);
  return builder_.Finish();
}

inline ::flatbuffers::Offset<Robot> CreateRobotDirect(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    int32_t batteries = 4,
    fbs::ControlSource control_source = fbs::ControlSource_RC,
    fbs::Status status = fbs::Status_OK,
    ::flatbuffers::Offset<fbs::Motors> motors = 0,
    float voltage = 0.0f,
    float current = 0.0f,
    float fuel = 0.0f,
    float rssi = 0.0f,
    int32_t link_quality = 0,
    float max_speed = 0.0f,
    float low_voltage_threshold = 12.0f,
    float critical_voltage_threshold = 16.0f,
    int32_t rssi_threshold = 0,
    int32_t link_quality_threshold = 0,
    int32_t reference_wheel_angle = 0,
    const char *motor_error_code = nullptr,
    bool wheels_folded = false,
    bool enable_rover = false,
    ::flatbuffers::Offset<fbs::DisplayMessages> display_messages = 0) {
  auto motor_error_code__ = motor_error_code ? _fbb.CreateString(motor_error_code) : 0;
  return fbs::CreateRobot(
      _fbb,
      batteries,
      control_source,
      status,
      motors,
      voltage,
      current,
      fuel,
      rssi,
      link_quality,
      max_speed,
      low_voltage_threshold,
      critical_voltage_threshold,
      rssi_threshold,
      link_quality_threshold,
      reference_wheel_angle,
      motor_error_code__,
      wheels_folded,
      enable_rover,
      display_messages);
}

::flatbuffers::Offset<Robot> CreateRobot(::flatbuffers::FlatBufferBuilder &_fbb, const RobotT *_o, const ::flatbuffers::rehasher_function_t *_rehasher = nullptr);

inline MotorT *Motor::UnPack(const ::flatbuffers::resolver_function_t *_resolver) const {
  auto _o = std::unique_ptr<MotorT>(new MotorT());
  UnPackTo(_o.get(), _resolver);
  return _o.release();
}

inline void Motor::UnPackTo(MotorT *_o, const ::flatbuffers::resolver_function_t *_resolver) const {
  (void)_o;
  (void)_resolver;
  { auto _e = temperature(); _o->temperature = _e; }
  { auto _e = rps(); _o->rps = _e; }
  { auto _e = angle(); _o->angle = _e; }
  { auto _e = torque(); _o->torque = _e; }
}

inline ::flatbuffers::Offset<Motor> Motor::Pack(::flatbuffers::FlatBufferBuilder &_fbb, const MotorT* _o, const ::flatbuffers::rehasher_function_t *_rehasher) {
  return CreateMotor(_fbb, _o, _rehasher);
}

inline ::flatbuffers::Offset<Motor> CreateMotor(::flatbuffers::FlatBufferBuilder &_fbb, const MotorT *_o, const ::flatbuffers::rehasher_function_t *_rehasher) {
  (void)_rehasher;
  (void)_o;
  struct _VectorArgs { ::flatbuffers::FlatBufferBuilder *__fbb; const MotorT* __o; const ::flatbuffers::rehasher_function_t *__rehasher; } _va = { &_fbb, _o, _rehasher}; (void)_va;
  auto _temperature = _o->temperature;
  auto _rps = _o->rps;
  auto _angle = _o->angle;
  auto _torque = _o->torque;
  return fbs::CreateMotor(
      _fbb,
      _temperature,
      _rps,
      _angle,
      _torque);
}

inline MotorsT::MotorsT(const MotorsT &o)
      : motor1((o.motor1) ? new fbs::MotorT(*o.motor1) : nullptr),
        motor2((o.motor2) ? new fbs::MotorT(*o.motor2) : nullptr) {
}

inline MotorsT &MotorsT::operator=(MotorsT o) FLATBUFFERS_NOEXCEPT {
  std::swap(motor1, o.motor1);
  std::swap(motor2, o.motor2);
  return *this;
}

inline MotorsT *Motors::UnPack(const ::flatbuffers::resolver_function_t *_resolver) const {
  auto _o = std::unique_ptr<MotorsT>(new MotorsT());
  UnPackTo(_o.get(), _resolver);
  return _o.release();
}

inline void Motors::UnPackTo(MotorsT *_o, const ::flatbuffers::resolver_function_t *_resolver) const {
  (void)_o;
  (void)_resolver;
  { auto _e = motor1(); if (_e) { if(_o->motor1) { _e->UnPackTo(_o->motor1.get(), _resolver); } else { _o->motor1 = std::unique_ptr<fbs::MotorT>(_e->UnPack(_resolver)); } } else if (_o->motor1) { _o->motor1.reset(); } }
  { auto _e = motor2(); if (_e) { if(_o->motor2) { _e->UnPackTo(_o->motor2.get(), _resolver); } else { _o->motor2 = std::unique_ptr<fbs::MotorT>(_e->UnPack(_resolver)); } } else if (_o->motor2) { _o->motor2.reset(); } }
}

inline ::flatbuffers::Offset<Motors> Motors::Pack(::flatbuffers::FlatBufferBuilder &_fbb, const MotorsT* _o, const ::flatbuffers::rehasher_function_t *_rehasher) {
  return CreateMotors(_fbb, _o, _rehasher);
}

inline ::flatbuffers::Offset<Motors> CreateMotors(::flatbuffers::FlatBufferBuilder &_fbb, const MotorsT *_o, const ::flatbuffers::rehasher_function_t *_rehasher) {
  (void)_rehasher;
  (void)_o;
  struct _VectorArgs { ::flatbuffers::FlatBufferBuilder *__fbb; const MotorsT* __o; const ::flatbuffers::rehasher_function_t *__rehasher; } _va = { &_fbb, _o, _rehasher}; (void)_va;
  auto _motor1 = _o->motor1 ? CreateMotor(_fbb, _o->motor1.get(), _rehasher) : 0;
  auto _motor2 = _o->motor2 ? CreateMotor(_fbb, _o->motor2.get(), _rehasher) : 0;
  return fbs::CreateMotors(
      _fbb,
      _motor1,
      _motor2);
}

inline DisplayMessagesT *DisplayMessages::UnPack(const ::flatbuffers::resolver_function_t *_resolver) const {
  auto _o = std::unique_ptr<DisplayMessagesT>(new DisplayMessagesT());
  UnPackTo(_o.get(), _resolver);
  return _o.release();
}

inline void DisplayMessages::UnPackTo(DisplayMessagesT *_o, const ::flatbuffers::resolver_function_t *_resolver) const {
  (void)_o;
  (void)_resolver;
  { auto _e = message1(); if (_e) _o->message1 = _e->str(); }
  { auto _e = message2(); if (_e) _o->message2 = _e->str(); }
  { auto _e = message3(); if (_e) _o->message3 = _e->str(); }
  { auto _e = message4(); if (_e) _o->message4 = _e->str(); }
  { auto _e = message5(); if (_e) _o->message5 = _e->str(); }
  { auto _e = message6(); if (_e) _o->message6 = _e->str(); }
  { auto _e = message7(); if (_e) _o->message7 = _e->str(); }
}

inline ::flatbuffers::Offset<DisplayMessages> DisplayMessages::Pack(::flatbuffers::FlatBufferBuilder &_fbb, const DisplayMessagesT* _o, const ::flatbuffers::rehasher_function_t *_rehasher) {
  return CreateDisplayMessages(_fbb, _o, _rehasher);
}

inline ::flatbuffers::Offset<DisplayMessages> CreateDisplayMessages(::flatbuffers::FlatBufferBuilder &_fbb, const DisplayMessagesT *_o, const ::flatbuffers::rehasher_function_t *_rehasher) {
  (void)_rehasher;
  (void)_o;
  struct _VectorArgs { ::flatbuffers::FlatBufferBuilder *__fbb; const DisplayMessagesT* __o; const ::flatbuffers::rehasher_function_t *__rehasher; } _va = { &_fbb, _o, _rehasher}; (void)_va;
  auto _message1 = _o->message1.empty() ? 0 : _fbb.CreateString(_o->message1);
  auto _message2 = _o->message2.empty() ? 0 : _fbb.CreateString(_o->message2);
  auto _message3 = _o->message3.empty() ? 0 : _fbb.CreateString(_o->message3);
  auto _message4 = _o->message4.empty() ? 0 : _fbb.CreateString(_o->message4);
  auto _message5 = _o->message5.empty() ? 0 : _fbb.CreateString(_o->message5);
  auto _message6 = _o->message6.empty() ? 0 : _fbb.CreateString(_o->message6);
  auto _message7 = _o->message7.empty() ? 0 : _fbb.CreateString(_o->message7);
  return fbs::CreateDisplayMessages(
      _fbb,
      _message1,
      _message2,
      _message3,
      _message4,
      _message5,
      _message6,
      _message7);
}

inline RobotT::RobotT(const RobotT &o)
      : batteries(o.batteries),
        control_source(o.control_source),
        status(o.status),
        motors((o.motors) ? new fbs::MotorsT(*o.motors) : nullptr),
        voltage(o.voltage),
        current(o.current),
        fuel(o.fuel),
        rssi(o.rssi),
        link_quality(o.link_quality),
        max_speed(o.max_speed),
        low_voltage_threshold(o.low_voltage_threshold),
        critical_voltage_threshold(o.critical_voltage_threshold),
        rssi_threshold(o.rssi_threshold),
        link_quality_threshold(o.link_quality_threshold),
        reference_wheel_angle(o.reference_wheel_angle),
        motor_error_code(o.motor_error_code),
        wheels_folded(o.wheels_folded),
        enable_rover(o.enable_rover),
        display_messages((o.display_messages) ? new fbs::DisplayMessagesT(*o.display_messages) : nullptr) {
}

inline RobotT &RobotT::operator=(RobotT o) FLATBUFFERS_NOEXCEPT {
  std::swap(batteries, o.batteries);
  std::swap(control_source, o.control_source);
  std::swap(status, o.status);
  std::swap(motors, o.motors);
  std::swap(voltage, o.voltage);
  std::swap(current, o.current);
  std::swap(fuel, o.fuel);
  std::swap(rssi, o.rssi);
  std::swap(link_quality, o.link_quality);
  std::swap(max_speed, o.max_speed);
  std::swap(low_voltage_threshold, o.low_voltage_threshold);
  std::swap(critical_voltage_threshold, o.critical_voltage_threshold);
  std::swap(rssi_threshold, o.rssi_threshold);
  std::swap(link_quality_threshold, o.link_quality_threshold);
  std::swap(reference_wheel_angle, o.reference_wheel_angle);
  std::swap(motor_error_code, o.motor_error_code);
  std::swap(wheels_folded, o.wheels_folded);
  std::swap(enable_rover, o.enable_rover);
  std::swap(display_messages, o.display_messages);
  return *this;
}

inline RobotT *Robot::UnPack(const ::flatbuffers::resolver_function_t *_resolver) const {
  auto _o = std::unique_ptr<RobotT>(new RobotT());
  UnPackTo(_o.get(), _resolver);
  return _o.release();
}

inline void Robot::UnPackTo(RobotT *_o, const ::flatbuffers::resolver_function_t *_resolver) const {
  (void)_o;
  (void)_resolver;
  { auto _e = batteries(); _o->batteries = _e; }
  { auto _e = control_source(); _o->control_source = _e; }
  { auto _e = status(); _o->status = _e; }
  { auto _e = motors(); if (_e) { if(_o->motors) { _e->UnPackTo(_o->motors.get(), _resolver); } else { _o->motors = std::unique_ptr<fbs::MotorsT>(_e->UnPack(_resolver)); } } else if (_o->motors) { _o->motors.reset(); } }
  { auto _e = voltage(); _o->voltage = _e; }
  { auto _e = current(); _o->current = _e; }
  { auto _e = fuel(); _o->fuel = _e; }
  { auto _e = rssi(); _o->rssi = _e; }
  { auto _e = link_quality(); _o->link_quality = _e; }
  { auto _e = max_speed(); _o->max_speed = _e; }
  { auto _e = low_voltage_threshold(); _o->low_voltage_threshold = _e; }
  { auto _e = critical_voltage_threshold(); _o->critical_voltage_threshold = _e; }
  { auto _e = rssi_threshold(); _o->rssi_threshold = _e; }
  { auto _e = link_quality_threshold(); _o->link_quality_threshold = _e; }
  { auto _e = reference_wheel_angle(); _o->reference_wheel_angle = _e; }
  { auto _e = motor_error_code(); if (_e) _o->motor_error_code = _e->str(); }
  { auto _e = wheels_folded(); _o->wheels_folded = _e; }
  { auto _e = enable_rover(); _o->enable_rover = _e; }
  { auto _e = display_messages(); if (_e) { if(_o->display_messages) { _e->UnPackTo(_o->display_messages.get(), _resolver); } else { _o->display_messages = std::unique_ptr<fbs::DisplayMessagesT>(_e->UnPack(_resolver)); } } else if (_o->display_messages) { _o->display_messages.reset(); } }
}

inline ::flatbuffers::Offset<Robot> Robot::Pack(::flatbuffers::FlatBufferBuilder &_fbb, const RobotT* _o, const ::flatbuffers::rehasher_function_t *_rehasher) {
  return CreateRobot(_fbb, _o, _rehasher);
}

inline ::flatbuffers::Offset<Robot> CreateRobot(::flatbuffers::FlatBufferBuilder &_fbb, const RobotT *_o, const ::flatbuffers::rehasher_function_t *_rehasher) {
  (void)_rehasher;
  (void)_o;
  struct _VectorArgs { ::flatbuffers::FlatBufferBuilder *__fbb; const RobotT* __o; const ::flatbuffers::rehasher_function_t *__rehasher; } _va = { &_fbb, _o, _rehasher}; (void)_va;
  auto _batteries = _o->batteries;
  auto _control_source = _o->control_source;
  auto _status = _o->status;
  auto _motors = _o->motors ? CreateMotors(_fbb, _o->motors.get(), _rehasher) : 0;
  auto _voltage = _o->voltage;
  auto _current = _o->current;
  auto _fuel = _o->fuel;
  auto _rssi = _o->rssi;
  auto _link_quality = _o->link_quality;
  auto _max_speed = _o->max_speed;
  auto _low_voltage_threshold = _o->low_voltage_threshold;
  auto _critical_voltage_threshold = _o->critical_voltage_threshold;
  auto _rssi_threshold = _o->rssi_threshold;
  auto _link_quality_threshold = _o->link_quality_threshold;
  auto _reference_wheel_angle = _o->reference_wheel_angle;
  auto _motor_error_code = _o->motor_error_code.empty() ? 0 : _fbb.CreateString(_o->motor_error_code);
  auto _wheels_folded = _o->wheels_folded;
  auto _enable_rover = _o->enable_rover;
  auto _display_messages = _o->display_messages ? CreateDisplayMessages(_fbb, _o->display_messages.get(), _rehasher) : 0;
  return fbs::CreateRobot(
      _fbb,
      _batteries,
      _control_source,
      _status,
      _motors,
      _voltage,
      _current,
      _fuel,
      _rssi,
      _link_quality,
      _max_speed,
      _low_voltage_threshold,
      _critical_voltage_threshold,
      _rssi_threshold,
      _link_quality_threshold,
      _reference_wheel_angle,
      _motor_error_code,
      _wheels_folded,
      _enable_rover,
      _display_messages);
}

inline const fbs::Robot *GetRobot(const void *buf) {
  return ::flatbuffers::GetRoot<fbs::Robot>(buf);
}

inline const fbs::Robot *GetSizePrefixedRobot(const void *buf) {
  return ::flatbuffers::GetSizePrefixedRoot<fbs::Robot>(buf);
}

inline const char *RobotIdentifier() {
  return "ROBO";
}

inline bool RobotBufferHasIdentifier(const void *buf) {
  return ::flatbuffers::BufferHasIdentifier(
      buf, RobotIdentifier());
}

inline bool SizePrefixedRobotBufferHasIdentifier(const void *buf) {
  return ::flatbuffers::BufferHasIdentifier(
      buf, RobotIdentifier(), true);
}

inline bool VerifyRobotBuffer(
    ::flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<fbs::Robot>(RobotIdentifier());
}

inline bool VerifySizePrefixedRobotBuffer(
    ::flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<fbs::Robot>(RobotIdentifier());
}

inline void FinishRobotBuffer(
    ::flatbuffers::FlatBufferBuilder &fbb,
    ::flatbuffers::Offset<fbs::Robot> root) {
  fbb.Finish(root, RobotIdentifier());
}

inline void FinishSizePrefixedRobotBuffer(
    ::flatbuffers::FlatBufferBuilder &fbb,
    ::flatbuffers::Offset<fbs::Robot> root) {
  fbb.FinishSizePrefixed(root, RobotIdentifier());
}

inline std::unique_ptr<fbs::RobotT> UnPackRobot(
    const void *buf,
    const ::flatbuffers::resolver_function_t *res = nullptr) {
  return std::unique_ptr<fbs::RobotT>(GetRobot(buf)->UnPack(res));
}

inline std::unique_ptr<fbs::RobotT> UnPackSizePrefixedRobot(
    const void *buf,
    const ::flatbuffers::resolver_function_t *res = nullptr) {
  return std::unique_ptr<fbs::RobotT>(GetSizePrefixedRobot(buf)->UnPack(res));
}

}  // namespace fbs

#endif  // FLATBUFFERS_GENERATED_ROBOTSTATE_FBS_H_
