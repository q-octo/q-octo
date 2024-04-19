// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_WSUPDATE_FBS_H_
#define FLATBUFFERS_GENERATED_WSUPDATE_FBS_H_

#include "flatbuffers/flatbuffers.h"

// Ensure the included flatbuffers.h is the same version as when this file was
// generated, otherwise it may not be compatible.
static_assert(FLATBUFFERS_VERSION_MAJOR == 24 &&
              FLATBUFFERS_VERSION_MINOR == 3 &&
              FLATBUFFERS_VERSION_REVISION == 25,
             "Non-compatible flatbuffers version included");

namespace fbs {

struct UpdateBatteries;
struct UpdateBatteriesBuilder;

struct UpdateLowVoltageThreshold;
struct UpdateLowVoltageThresholdBuilder;

struct UpdateCriticalVoltageThreshold;
struct UpdateCriticalVoltageThresholdBuilder;

struct UpdateReferenceWheelAngle;
struct UpdateReferenceWheelAngleBuilder;

struct UpdateFoldWheels;
struct UpdateFoldWheelsBuilder;

struct UpdateEnableRover;
struct UpdateEnableRoverBuilder;

struct Update;
struct UpdateBuilder;

enum UpdateUnion : uint8_t {
  UpdateUnion_NONE = 0,
  UpdateUnion_UpdateBatteries = 1,
  UpdateUnion_UpdateLowVoltageThreshold = 2,
  UpdateUnion_UpdateCriticalVoltageThreshold = 3,
  UpdateUnion_UpdateReferenceWheelAngle = 4,
  UpdateUnion_UpdateFoldWheels = 5,
  UpdateUnion_UpdateEnableRover = 6,
  UpdateUnion_MIN = UpdateUnion_NONE,
  UpdateUnion_MAX = UpdateUnion_UpdateEnableRover
};

inline const UpdateUnion (&EnumValuesUpdateUnion())[7] {
  static const UpdateUnion values[] = {
    UpdateUnion_NONE,
    UpdateUnion_UpdateBatteries,
    UpdateUnion_UpdateLowVoltageThreshold,
    UpdateUnion_UpdateCriticalVoltageThreshold,
    UpdateUnion_UpdateReferenceWheelAngle,
    UpdateUnion_UpdateFoldWheels,
    UpdateUnion_UpdateEnableRover
  };
  return values;
}

inline const char * const *EnumNamesUpdateUnion() {
  static const char * const names[8] = {
    "NONE",
    "UpdateBatteries",
    "UpdateLowVoltageThreshold",
    "UpdateCriticalVoltageThreshold",
    "UpdateReferenceWheelAngle",
    "UpdateFoldWheels",
    "UpdateEnableRover",
    nullptr
  };
  return names;
}

inline const char *EnumNameUpdateUnion(UpdateUnion e) {
  if (::flatbuffers::IsOutRange(e, UpdateUnion_NONE, UpdateUnion_UpdateEnableRover)) return "";
  const size_t index = static_cast<size_t>(e);
  return EnumNamesUpdateUnion()[index];
}

template<typename T> struct UpdateUnionTraits {
  static const UpdateUnion enum_value = UpdateUnion_NONE;
};

template<> struct UpdateUnionTraits<fbs::UpdateBatteries> {
  static const UpdateUnion enum_value = UpdateUnion_UpdateBatteries;
};

template<> struct UpdateUnionTraits<fbs::UpdateLowVoltageThreshold> {
  static const UpdateUnion enum_value = UpdateUnion_UpdateLowVoltageThreshold;
};

template<> struct UpdateUnionTraits<fbs::UpdateCriticalVoltageThreshold> {
  static const UpdateUnion enum_value = UpdateUnion_UpdateCriticalVoltageThreshold;
};

template<> struct UpdateUnionTraits<fbs::UpdateReferenceWheelAngle> {
  static const UpdateUnion enum_value = UpdateUnion_UpdateReferenceWheelAngle;
};

template<> struct UpdateUnionTraits<fbs::UpdateFoldWheels> {
  static const UpdateUnion enum_value = UpdateUnion_UpdateFoldWheels;
};

template<> struct UpdateUnionTraits<fbs::UpdateEnableRover> {
  static const UpdateUnion enum_value = UpdateUnion_UpdateEnableRover;
};

bool VerifyUpdateUnion(::flatbuffers::Verifier &verifier, const void *obj, UpdateUnion type);
bool VerifyUpdateUnionVector(::flatbuffers::Verifier &verifier, const ::flatbuffers::Vector<::flatbuffers::Offset<void>> *values, const ::flatbuffers::Vector<uint8_t> *types);

struct UpdateBatteries FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef UpdateBatteriesBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_BATTERIES = 4
  };
  int32_t batteries() const {
    return GetField<int32_t>(VT_BATTERIES, 0);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<int32_t>(verifier, VT_BATTERIES, 4) &&
           verifier.EndTable();
  }
};

struct UpdateBatteriesBuilder {
  typedef UpdateBatteries Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_batteries(int32_t batteries) {
    fbb_.AddElement<int32_t>(UpdateBatteries::VT_BATTERIES, batteries, 0);
  }
  explicit UpdateBatteriesBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<UpdateBatteries> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<UpdateBatteries>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<UpdateBatteries> CreateUpdateBatteries(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    int32_t batteries = 0) {
  UpdateBatteriesBuilder builder_(_fbb);
  builder_.add_batteries(batteries);
  return builder_.Finish();
}

struct UpdateLowVoltageThreshold FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef UpdateLowVoltageThresholdBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_LOW_VOLTAGE_THRESHOLD = 4
  };
  float low_voltage_threshold() const {
    return GetField<float>(VT_LOW_VOLTAGE_THRESHOLD, 0.0f);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<float>(verifier, VT_LOW_VOLTAGE_THRESHOLD, 4) &&
           verifier.EndTable();
  }
};

struct UpdateLowVoltageThresholdBuilder {
  typedef UpdateLowVoltageThreshold Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_low_voltage_threshold(float low_voltage_threshold) {
    fbb_.AddElement<float>(UpdateLowVoltageThreshold::VT_LOW_VOLTAGE_THRESHOLD, low_voltage_threshold, 0.0f);
  }
  explicit UpdateLowVoltageThresholdBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<UpdateLowVoltageThreshold> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<UpdateLowVoltageThreshold>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<UpdateLowVoltageThreshold> CreateUpdateLowVoltageThreshold(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    float low_voltage_threshold = 0.0f) {
  UpdateLowVoltageThresholdBuilder builder_(_fbb);
  builder_.add_low_voltage_threshold(low_voltage_threshold);
  return builder_.Finish();
}

struct UpdateCriticalVoltageThreshold FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef UpdateCriticalVoltageThresholdBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_CRITICAL_VOLTAGE_THRESHOLD = 4
  };
  float critical_voltage_threshold() const {
    return GetField<float>(VT_CRITICAL_VOLTAGE_THRESHOLD, 0.0f);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<float>(verifier, VT_CRITICAL_VOLTAGE_THRESHOLD, 4) &&
           verifier.EndTable();
  }
};

struct UpdateCriticalVoltageThresholdBuilder {
  typedef UpdateCriticalVoltageThreshold Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_critical_voltage_threshold(float critical_voltage_threshold) {
    fbb_.AddElement<float>(UpdateCriticalVoltageThreshold::VT_CRITICAL_VOLTAGE_THRESHOLD, critical_voltage_threshold, 0.0f);
  }
  explicit UpdateCriticalVoltageThresholdBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<UpdateCriticalVoltageThreshold> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<UpdateCriticalVoltageThreshold>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<UpdateCriticalVoltageThreshold> CreateUpdateCriticalVoltageThreshold(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    float critical_voltage_threshold = 0.0f) {
  UpdateCriticalVoltageThresholdBuilder builder_(_fbb);
  builder_.add_critical_voltage_threshold(critical_voltage_threshold);
  return builder_.Finish();
}

struct UpdateReferenceWheelAngle FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef UpdateReferenceWheelAngleBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_REFERENCE_WHEEL_ANGLE = 4
  };
  int32_t reference_wheel_angle() const {
    return GetField<int32_t>(VT_REFERENCE_WHEEL_ANGLE, 0);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<int32_t>(verifier, VT_REFERENCE_WHEEL_ANGLE, 4) &&
           verifier.EndTable();
  }
};

struct UpdateReferenceWheelAngleBuilder {
  typedef UpdateReferenceWheelAngle Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_reference_wheel_angle(int32_t reference_wheel_angle) {
    fbb_.AddElement<int32_t>(UpdateReferenceWheelAngle::VT_REFERENCE_WHEEL_ANGLE, reference_wheel_angle, 0);
  }
  explicit UpdateReferenceWheelAngleBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<UpdateReferenceWheelAngle> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<UpdateReferenceWheelAngle>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<UpdateReferenceWheelAngle> CreateUpdateReferenceWheelAngle(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    int32_t reference_wheel_angle = 0) {
  UpdateReferenceWheelAngleBuilder builder_(_fbb);
  builder_.add_reference_wheel_angle(reference_wheel_angle);
  return builder_.Finish();
}

struct UpdateFoldWheels FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef UpdateFoldWheelsBuilder Builder;
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           verifier.EndTable();
  }
};

struct UpdateFoldWheelsBuilder {
  typedef UpdateFoldWheels Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  explicit UpdateFoldWheelsBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<UpdateFoldWheels> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<UpdateFoldWheels>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<UpdateFoldWheels> CreateUpdateFoldWheels(
    ::flatbuffers::FlatBufferBuilder &_fbb) {
  UpdateFoldWheelsBuilder builder_(_fbb);
  return builder_.Finish();
}

struct UpdateEnableRover FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef UpdateEnableRoverBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_ENABLE_ROVER = 4
  };
  bool enable_rover() const {
    return GetField<uint8_t>(VT_ENABLE_ROVER, 0) != 0;
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint8_t>(verifier, VT_ENABLE_ROVER, 1) &&
           verifier.EndTable();
  }
};

struct UpdateEnableRoverBuilder {
  typedef UpdateEnableRover Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_enable_rover(bool enable_rover) {
    fbb_.AddElement<uint8_t>(UpdateEnableRover::VT_ENABLE_ROVER, static_cast<uint8_t>(enable_rover), 0);
  }
  explicit UpdateEnableRoverBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<UpdateEnableRover> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<UpdateEnableRover>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<UpdateEnableRover> CreateUpdateEnableRover(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    bool enable_rover = false) {
  UpdateEnableRoverBuilder builder_(_fbb);
  builder_.add_enable_rover(enable_rover);
  return builder_.Finish();
}

struct Update FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef UpdateBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_UPDATE_TYPE = 4,
    VT_UPDATE = 6
  };
  fbs::UpdateUnion update_type() const {
    return static_cast<fbs::UpdateUnion>(GetField<uint8_t>(VT_UPDATE_TYPE, 0));
  }
  const void *update() const {
    return GetPointer<const void *>(VT_UPDATE);
  }
  template<typename T> const T *update_as() const;
  const fbs::UpdateBatteries *update_as_UpdateBatteries() const {
    return update_type() == fbs::UpdateUnion_UpdateBatteries ? static_cast<const fbs::UpdateBatteries *>(update()) : nullptr;
  }
  const fbs::UpdateLowVoltageThreshold *update_as_UpdateLowVoltageThreshold() const {
    return update_type() == fbs::UpdateUnion_UpdateLowVoltageThreshold ? static_cast<const fbs::UpdateLowVoltageThreshold *>(update()) : nullptr;
  }
  const fbs::UpdateCriticalVoltageThreshold *update_as_UpdateCriticalVoltageThreshold() const {
    return update_type() == fbs::UpdateUnion_UpdateCriticalVoltageThreshold ? static_cast<const fbs::UpdateCriticalVoltageThreshold *>(update()) : nullptr;
  }
  const fbs::UpdateReferenceWheelAngle *update_as_UpdateReferenceWheelAngle() const {
    return update_type() == fbs::UpdateUnion_UpdateReferenceWheelAngle ? static_cast<const fbs::UpdateReferenceWheelAngle *>(update()) : nullptr;
  }
  const fbs::UpdateFoldWheels *update_as_UpdateFoldWheels() const {
    return update_type() == fbs::UpdateUnion_UpdateFoldWheels ? static_cast<const fbs::UpdateFoldWheels *>(update()) : nullptr;
  }
  const fbs::UpdateEnableRover *update_as_UpdateEnableRover() const {
    return update_type() == fbs::UpdateUnion_UpdateEnableRover ? static_cast<const fbs::UpdateEnableRover *>(update()) : nullptr;
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint8_t>(verifier, VT_UPDATE_TYPE, 1) &&
           VerifyOffset(verifier, VT_UPDATE) &&
           VerifyUpdateUnion(verifier, update(), update_type()) &&
           verifier.EndTable();
  }
};

template<> inline const fbs::UpdateBatteries *Update::update_as<fbs::UpdateBatteries>() const {
  return update_as_UpdateBatteries();
}

template<> inline const fbs::UpdateLowVoltageThreshold *Update::update_as<fbs::UpdateLowVoltageThreshold>() const {
  return update_as_UpdateLowVoltageThreshold();
}

template<> inline const fbs::UpdateCriticalVoltageThreshold *Update::update_as<fbs::UpdateCriticalVoltageThreshold>() const {
  return update_as_UpdateCriticalVoltageThreshold();
}

template<> inline const fbs::UpdateReferenceWheelAngle *Update::update_as<fbs::UpdateReferenceWheelAngle>() const {
  return update_as_UpdateReferenceWheelAngle();
}

template<> inline const fbs::UpdateFoldWheels *Update::update_as<fbs::UpdateFoldWheels>() const {
  return update_as_UpdateFoldWheels();
}

template<> inline const fbs::UpdateEnableRover *Update::update_as<fbs::UpdateEnableRover>() const {
  return update_as_UpdateEnableRover();
}

struct UpdateBuilder {
  typedef Update Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_update_type(fbs::UpdateUnion update_type) {
    fbb_.AddElement<uint8_t>(Update::VT_UPDATE_TYPE, static_cast<uint8_t>(update_type), 0);
  }
  void add_update(::flatbuffers::Offset<void> update) {
    fbb_.AddOffset(Update::VT_UPDATE, update);
  }
  explicit UpdateBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<Update> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<Update>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<Update> CreateUpdate(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    fbs::UpdateUnion update_type = fbs::UpdateUnion_NONE,
    ::flatbuffers::Offset<void> update = 0) {
  UpdateBuilder builder_(_fbb);
  builder_.add_update(update);
  builder_.add_update_type(update_type);
  return builder_.Finish();
}

inline bool VerifyUpdateUnion(::flatbuffers::Verifier &verifier, const void *obj, UpdateUnion type) {
  switch (type) {
    case UpdateUnion_NONE: {
      return true;
    }
    case UpdateUnion_UpdateBatteries: {
      auto ptr = reinterpret_cast<const fbs::UpdateBatteries *>(obj);
      return verifier.VerifyTable(ptr);
    }
    case UpdateUnion_UpdateLowVoltageThreshold: {
      auto ptr = reinterpret_cast<const fbs::UpdateLowVoltageThreshold *>(obj);
      return verifier.VerifyTable(ptr);
    }
    case UpdateUnion_UpdateCriticalVoltageThreshold: {
      auto ptr = reinterpret_cast<const fbs::UpdateCriticalVoltageThreshold *>(obj);
      return verifier.VerifyTable(ptr);
    }
    case UpdateUnion_UpdateReferenceWheelAngle: {
      auto ptr = reinterpret_cast<const fbs::UpdateReferenceWheelAngle *>(obj);
      return verifier.VerifyTable(ptr);
    }
    case UpdateUnion_UpdateFoldWheels: {
      auto ptr = reinterpret_cast<const fbs::UpdateFoldWheels *>(obj);
      return verifier.VerifyTable(ptr);
    }
    case UpdateUnion_UpdateEnableRover: {
      auto ptr = reinterpret_cast<const fbs::UpdateEnableRover *>(obj);
      return verifier.VerifyTable(ptr);
    }
    default: return true;
  }
}

inline bool VerifyUpdateUnionVector(::flatbuffers::Verifier &verifier, const ::flatbuffers::Vector<::flatbuffers::Offset<void>> *values, const ::flatbuffers::Vector<uint8_t> *types) {
  if (!values || !types) return !values && !types;
  if (values->size() != types->size()) return false;
  for (::flatbuffers::uoffset_t i = 0; i < values->size(); ++i) {
    if (!VerifyUpdateUnion(
        verifier,  values->Get(i), types->GetEnum<UpdateUnion>(i))) {
      return false;
    }
  }
  return true;
}

inline const fbs::Update *GetUpdate(const void *buf) {
  return ::flatbuffers::GetRoot<fbs::Update>(buf);
}

inline const fbs::Update *GetSizePrefixedUpdate(const void *buf) {
  return ::flatbuffers::GetSizePrefixedRoot<fbs::Update>(buf);
}

inline const char *UpdateIdentifier() {
  return "UPDT";
}

inline bool UpdateBufferHasIdentifier(const void *buf) {
  return ::flatbuffers::BufferHasIdentifier(
      buf, UpdateIdentifier());
}

inline bool SizePrefixedUpdateBufferHasIdentifier(const void *buf) {
  return ::flatbuffers::BufferHasIdentifier(
      buf, UpdateIdentifier(), true);
}

inline bool VerifyUpdateBuffer(
    ::flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<fbs::Update>(UpdateIdentifier());
}

inline bool VerifySizePrefixedUpdateBuffer(
    ::flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<fbs::Update>(UpdateIdentifier());
}

inline void FinishUpdateBuffer(
    ::flatbuffers::FlatBufferBuilder &fbb,
    ::flatbuffers::Offset<fbs::Update> root) {
  fbb.Finish(root, UpdateIdentifier());
}

inline void FinishSizePrefixedUpdateBuffer(
    ::flatbuffers::FlatBufferBuilder &fbb,
    ::flatbuffers::Offset<fbs::Update> root) {
  fbb.FinishSizePrefixed(root, UpdateIdentifier());
}

}  // namespace fbs

#endif  // FLATBUFFERS_GENERATED_WSUPDATE_FBS_H_
