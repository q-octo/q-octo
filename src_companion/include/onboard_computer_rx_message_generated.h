// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_ONBOARDCOMPUTERRXMESSAGE_FBS_H_
#define FLATBUFFERS_GENERATED_ONBOARDCOMPUTERRXMESSAGE_FBS_H_

#include "flatbuffers/flatbuffers.h"

// Ensure the included flatbuffers.h is the same version as when this file was
// generated, otherwise it may not be compatible.
static_assert(FLATBUFFERS_VERSION_MAJOR == 24 &&
              FLATBUFFERS_VERSION_MINOR == 3 &&
              FLATBUFFERS_VERSION_REVISION == 25,
             "Non-compatible flatbuffers version included");

#include "robot_state_generated.h"

namespace fbs {

struct CrsfTelemetry;
struct CrsfTelemetryBuilder;
struct CrsfTelemetryT;

struct CrsfChannels;

struct CrsfData;
struct CrsfDataBuilder;
struct CrsfDataT;

struct OnboardComputerRx;
struct OnboardComputerRxBuilder;
struct OnboardComputerRxT;

enum OnboardComputerRxUnion : uint8_t {
  OnboardComputerRxUnion_NONE = 0,
  OnboardComputerRxUnion_CrsfData = 1,
  OnboardComputerRxUnion_Robot = 2,
  OnboardComputerRxUnion_MIN = OnboardComputerRxUnion_NONE,
  OnboardComputerRxUnion_MAX = OnboardComputerRxUnion_Robot
};

inline const OnboardComputerRxUnion (&EnumValuesOnboardComputerRxUnion())[3] {
  static const OnboardComputerRxUnion values[] = {
    OnboardComputerRxUnion_NONE,
    OnboardComputerRxUnion_CrsfData,
    OnboardComputerRxUnion_Robot
  };
  return values;
}

inline const char * const *EnumNamesOnboardComputerRxUnion() {
  static const char * const names[4] = {
    "NONE",
    "CrsfData",
    "Robot",
    nullptr
  };
  return names;
}

inline const char *EnumNameOnboardComputerRxUnion(OnboardComputerRxUnion e) {
  if (::flatbuffers::IsOutRange(e, OnboardComputerRxUnion_NONE, OnboardComputerRxUnion_Robot)) return "";
  const size_t index = static_cast<size_t>(e);
  return EnumNamesOnboardComputerRxUnion()[index];
}

template<typename T> struct OnboardComputerRxUnionTraits {
  static const OnboardComputerRxUnion enum_value = OnboardComputerRxUnion_NONE;
};

template<> struct OnboardComputerRxUnionTraits<fbs::CrsfData> {
  static const OnboardComputerRxUnion enum_value = OnboardComputerRxUnion_CrsfData;
};

template<> struct OnboardComputerRxUnionTraits<fbs::Robot> {
  static const OnboardComputerRxUnion enum_value = OnboardComputerRxUnion_Robot;
};

template<typename T> struct OnboardComputerRxUnionUnionTraits {
  static const OnboardComputerRxUnion enum_value = OnboardComputerRxUnion_NONE;
};

template<> struct OnboardComputerRxUnionUnionTraits<fbs::CrsfDataT> {
  static const OnboardComputerRxUnion enum_value = OnboardComputerRxUnion_CrsfData;
};

template<> struct OnboardComputerRxUnionUnionTraits<fbs::RobotT> {
  static const OnboardComputerRxUnion enum_value = OnboardComputerRxUnion_Robot;
};

struct OnboardComputerRxUnionUnion {
  OnboardComputerRxUnion type;
  void *value;

  OnboardComputerRxUnionUnion() : type(OnboardComputerRxUnion_NONE), value(nullptr) {}
  OnboardComputerRxUnionUnion(OnboardComputerRxUnionUnion&& u) FLATBUFFERS_NOEXCEPT :
    type(OnboardComputerRxUnion_NONE), value(nullptr)
    { std::swap(type, u.type); std::swap(value, u.value); }
  OnboardComputerRxUnionUnion(const OnboardComputerRxUnionUnion &);
  OnboardComputerRxUnionUnion &operator=(const OnboardComputerRxUnionUnion &u)
    { OnboardComputerRxUnionUnion t(u); std::swap(type, t.type); std::swap(value, t.value); return *this; }
  OnboardComputerRxUnionUnion &operator=(OnboardComputerRxUnionUnion &&u) FLATBUFFERS_NOEXCEPT
    { std::swap(type, u.type); std::swap(value, u.value); return *this; }
  ~OnboardComputerRxUnionUnion() { Reset(); }

  void Reset();

  template <typename T>
  void Set(T&& val) {
    typedef typename std::remove_reference<T>::type RT;
    Reset();
    type = OnboardComputerRxUnionUnionTraits<RT>::enum_value;
    if (type != OnboardComputerRxUnion_NONE) {
      value = new RT(std::forward<T>(val));
    }
  }

  static void *UnPack(const void *obj, OnboardComputerRxUnion type, const ::flatbuffers::resolver_function_t *resolver);
  ::flatbuffers::Offset<void> Pack(::flatbuffers::FlatBufferBuilder &_fbb, const ::flatbuffers::rehasher_function_t *_rehasher = nullptr) const;

  fbs::CrsfDataT *AsCrsfData() {
    return type == OnboardComputerRxUnion_CrsfData ?
      reinterpret_cast<fbs::CrsfDataT *>(value) : nullptr;
  }
  const fbs::CrsfDataT *AsCrsfData() const {
    return type == OnboardComputerRxUnion_CrsfData ?
      reinterpret_cast<const fbs::CrsfDataT *>(value) : nullptr;
  }
  fbs::RobotT *AsRobot() {
    return type == OnboardComputerRxUnion_Robot ?
      reinterpret_cast<fbs::RobotT *>(value) : nullptr;
  }
  const fbs::RobotT *AsRobot() const {
    return type == OnboardComputerRxUnion_Robot ?
      reinterpret_cast<const fbs::RobotT *>(value) : nullptr;
  }
};

bool VerifyOnboardComputerRxUnion(::flatbuffers::Verifier &verifier, const void *obj, OnboardComputerRxUnion type);
bool VerifyOnboardComputerRxUnionVector(::flatbuffers::Verifier &verifier, const ::flatbuffers::Vector<::flatbuffers::Offset<void>> *values, const ::flatbuffers::Vector<uint8_t> *types);

FLATBUFFERS_MANUALLY_ALIGNED_STRUCT(2) CrsfChannels FLATBUFFERS_FINAL_CLASS {
 private:
  uint16_t data_[16];

 public:
  CrsfChannels()
      : data_() {
  }
  CrsfChannels(::flatbuffers::span<const uint16_t, 16> _data) {
    ::flatbuffers::CastToArray(data_).CopyFromSpan(_data);
  }
  const ::flatbuffers::Array<uint16_t, 16> *data() const {
    return &::flatbuffers::CastToArray(data_);
  }
};
FLATBUFFERS_STRUCT_END(CrsfChannels, 32);

struct CrsfTelemetryT : public ::flatbuffers::NativeTable {
  typedef CrsfTelemetry TableType;
  uint8_t rssi = 0;
  uint8_t link_quality = 0;
  int8_t snr = 0;
  uint16_t tx_power = 0;
};

struct CrsfTelemetry FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef CrsfTelemetryT NativeTableType;
  typedef CrsfTelemetryBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_RSSI = 4,
    VT_LINK_QUALITY = 6,
    VT_SNR = 8,
    VT_TX_POWER = 10
  };
  /// Signal strength indicator
  uint8_t rssi() const {
    return GetField<uint8_t>(VT_RSSI, 0);
  }
  /// Link quality
  uint8_t link_quality() const {
    return GetField<uint8_t>(VT_LINK_QUALITY, 0);
  }
  /// Signal to noise ratio
  int8_t snr() const {
    return GetField<int8_t>(VT_SNR, 0);
  }
  /// Transmission power
  uint16_t tx_power() const {
    return GetField<uint16_t>(VT_TX_POWER, 0);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint8_t>(verifier, VT_RSSI, 1) &&
           VerifyField<uint8_t>(verifier, VT_LINK_QUALITY, 1) &&
           VerifyField<int8_t>(verifier, VT_SNR, 1) &&
           VerifyField<uint16_t>(verifier, VT_TX_POWER, 2) &&
           verifier.EndTable();
  }
  CrsfTelemetryT *UnPack(const ::flatbuffers::resolver_function_t *_resolver = nullptr) const;
  void UnPackTo(CrsfTelemetryT *_o, const ::flatbuffers::resolver_function_t *_resolver = nullptr) const;
  static ::flatbuffers::Offset<CrsfTelemetry> Pack(::flatbuffers::FlatBufferBuilder &_fbb, const CrsfTelemetryT* _o, const ::flatbuffers::rehasher_function_t *_rehasher = nullptr);
};

struct CrsfTelemetryBuilder {
  typedef CrsfTelemetry Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_rssi(uint8_t rssi) {
    fbb_.AddElement<uint8_t>(CrsfTelemetry::VT_RSSI, rssi, 0);
  }
  void add_link_quality(uint8_t link_quality) {
    fbb_.AddElement<uint8_t>(CrsfTelemetry::VT_LINK_QUALITY, link_quality, 0);
  }
  void add_snr(int8_t snr) {
    fbb_.AddElement<int8_t>(CrsfTelemetry::VT_SNR, snr, 0);
  }
  void add_tx_power(uint16_t tx_power) {
    fbb_.AddElement<uint16_t>(CrsfTelemetry::VT_TX_POWER, tx_power, 0);
  }
  explicit CrsfTelemetryBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<CrsfTelemetry> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<CrsfTelemetry>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<CrsfTelemetry> CreateCrsfTelemetry(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    uint8_t rssi = 0,
    uint8_t link_quality = 0,
    int8_t snr = 0,
    uint16_t tx_power = 0) {
  CrsfTelemetryBuilder builder_(_fbb);
  builder_.add_tx_power(tx_power);
  builder_.add_snr(snr);
  builder_.add_link_quality(link_quality);
  builder_.add_rssi(rssi);
  return builder_.Finish();
}

::flatbuffers::Offset<CrsfTelemetry> CreateCrsfTelemetry(::flatbuffers::FlatBufferBuilder &_fbb, const CrsfTelemetryT *_o, const ::flatbuffers::rehasher_function_t *_rehasher = nullptr);

struct CrsfDataT : public ::flatbuffers::NativeTable {
  typedef CrsfData TableType;
  std::unique_ptr<fbs::CrsfChannels> channels{};
  std::unique_ptr<fbs::CrsfTelemetryT> telemetry{};
  bool failsafe = true;
  CrsfDataT() = default;
  CrsfDataT(const CrsfDataT &o);
  CrsfDataT(CrsfDataT&&) FLATBUFFERS_NOEXCEPT = default;
  CrsfDataT &operator=(CrsfDataT o) FLATBUFFERS_NOEXCEPT;
};

struct CrsfData FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef CrsfDataT NativeTableType;
  typedef CrsfDataBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_CHANNELS = 4,
    VT_TELEMETRY = 6,
    VT_FAILSAFE = 8
  };
  const fbs::CrsfChannels *channels() const {
    return GetStruct<const fbs::CrsfChannels *>(VT_CHANNELS);
  }
  /// Telemetry data associated with the RC frame
  const fbs::CrsfTelemetry *telemetry() const {
    return GetPointer<const fbs::CrsfTelemetry *>(VT_TELEMETRY);
  }
  /// Failsafe flag with a default value of true
  bool failsafe() const {
    return GetField<uint8_t>(VT_FAILSAFE, 1) != 0;
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<fbs::CrsfChannels>(verifier, VT_CHANNELS, 2) &&
           VerifyOffset(verifier, VT_TELEMETRY) &&
           verifier.VerifyTable(telemetry()) &&
           VerifyField<uint8_t>(verifier, VT_FAILSAFE, 1) &&
           verifier.EndTable();
  }
  CrsfDataT *UnPack(const ::flatbuffers::resolver_function_t *_resolver = nullptr) const;
  void UnPackTo(CrsfDataT *_o, const ::flatbuffers::resolver_function_t *_resolver = nullptr) const;
  static ::flatbuffers::Offset<CrsfData> Pack(::flatbuffers::FlatBufferBuilder &_fbb, const CrsfDataT* _o, const ::flatbuffers::rehasher_function_t *_rehasher = nullptr);
};

struct CrsfDataBuilder {
  typedef CrsfData Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_channels(const fbs::CrsfChannels *channels) {
    fbb_.AddStruct(CrsfData::VT_CHANNELS, channels);
  }
  void add_telemetry(::flatbuffers::Offset<fbs::CrsfTelemetry> telemetry) {
    fbb_.AddOffset(CrsfData::VT_TELEMETRY, telemetry);
  }
  void add_failsafe(bool failsafe) {
    fbb_.AddElement<uint8_t>(CrsfData::VT_FAILSAFE, static_cast<uint8_t>(failsafe), 1);
  }
  explicit CrsfDataBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<CrsfData> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<CrsfData>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<CrsfData> CreateCrsfData(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    const fbs::CrsfChannels *channels = nullptr,
    ::flatbuffers::Offset<fbs::CrsfTelemetry> telemetry = 0,
    bool failsafe = true) {
  CrsfDataBuilder builder_(_fbb);
  builder_.add_telemetry(telemetry);
  builder_.add_channels(channels);
  builder_.add_failsafe(failsafe);
  return builder_.Finish();
}

::flatbuffers::Offset<CrsfData> CreateCrsfData(::flatbuffers::FlatBufferBuilder &_fbb, const CrsfDataT *_o, const ::flatbuffers::rehasher_function_t *_rehasher = nullptr);

struct OnboardComputerRxT : public ::flatbuffers::NativeTable {
  typedef OnboardComputerRx TableType;
  fbs::OnboardComputerRxUnionUnion message{};
};

struct OnboardComputerRx FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef OnboardComputerRxT NativeTableType;
  typedef OnboardComputerRxBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_MESSAGE_TYPE = 4,
    VT_MESSAGE = 6
  };
  fbs::OnboardComputerRxUnion message_type() const {
    return static_cast<fbs::OnboardComputerRxUnion>(GetField<uint8_t>(VT_MESSAGE_TYPE, 0));
  }
  const void *message() const {
    return GetPointer<const void *>(VT_MESSAGE);
  }
  template<typename T> const T *message_as() const;
  const fbs::CrsfData *message_as_CrsfData() const {
    return message_type() == fbs::OnboardComputerRxUnion_CrsfData ? static_cast<const fbs::CrsfData *>(message()) : nullptr;
  }
  const fbs::Robot *message_as_Robot() const {
    return message_type() == fbs::OnboardComputerRxUnion_Robot ? static_cast<const fbs::Robot *>(message()) : nullptr;
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint8_t>(verifier, VT_MESSAGE_TYPE, 1) &&
           VerifyOffset(verifier, VT_MESSAGE) &&
           VerifyOnboardComputerRxUnion(verifier, message(), message_type()) &&
           verifier.EndTable();
  }
  OnboardComputerRxT *UnPack(const ::flatbuffers::resolver_function_t *_resolver = nullptr) const;
  void UnPackTo(OnboardComputerRxT *_o, const ::flatbuffers::resolver_function_t *_resolver = nullptr) const;
  static ::flatbuffers::Offset<OnboardComputerRx> Pack(::flatbuffers::FlatBufferBuilder &_fbb, const OnboardComputerRxT* _o, const ::flatbuffers::rehasher_function_t *_rehasher = nullptr);
};

template<> inline const fbs::CrsfData *OnboardComputerRx::message_as<fbs::CrsfData>() const {
  return message_as_CrsfData();
}

template<> inline const fbs::Robot *OnboardComputerRx::message_as<fbs::Robot>() const {
  return message_as_Robot();
}

struct OnboardComputerRxBuilder {
  typedef OnboardComputerRx Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_message_type(fbs::OnboardComputerRxUnion message_type) {
    fbb_.AddElement<uint8_t>(OnboardComputerRx::VT_MESSAGE_TYPE, static_cast<uint8_t>(message_type), 0);
  }
  void add_message(::flatbuffers::Offset<void> message) {
    fbb_.AddOffset(OnboardComputerRx::VT_MESSAGE, message);
  }
  explicit OnboardComputerRxBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<OnboardComputerRx> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<OnboardComputerRx>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<OnboardComputerRx> CreateOnboardComputerRx(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    fbs::OnboardComputerRxUnion message_type = fbs::OnboardComputerRxUnion_NONE,
    ::flatbuffers::Offset<void> message = 0) {
  OnboardComputerRxBuilder builder_(_fbb);
  builder_.add_message(message);
  builder_.add_message_type(message_type);
  return builder_.Finish();
}

::flatbuffers::Offset<OnboardComputerRx> CreateOnboardComputerRx(::flatbuffers::FlatBufferBuilder &_fbb, const OnboardComputerRxT *_o, const ::flatbuffers::rehasher_function_t *_rehasher = nullptr);

inline CrsfTelemetryT *CrsfTelemetry::UnPack(const ::flatbuffers::resolver_function_t *_resolver) const {
  auto _o = std::unique_ptr<CrsfTelemetryT>(new CrsfTelemetryT());
  UnPackTo(_o.get(), _resolver);
  return _o.release();
}

inline void CrsfTelemetry::UnPackTo(CrsfTelemetryT *_o, const ::flatbuffers::resolver_function_t *_resolver) const {
  (void)_o;
  (void)_resolver;
  { auto _e = rssi(); _o->rssi = _e; }
  { auto _e = link_quality(); _o->link_quality = _e; }
  { auto _e = snr(); _o->snr = _e; }
  { auto _e = tx_power(); _o->tx_power = _e; }
}

inline ::flatbuffers::Offset<CrsfTelemetry> CrsfTelemetry::Pack(::flatbuffers::FlatBufferBuilder &_fbb, const CrsfTelemetryT* _o, const ::flatbuffers::rehasher_function_t *_rehasher) {
  return CreateCrsfTelemetry(_fbb, _o, _rehasher);
}

inline ::flatbuffers::Offset<CrsfTelemetry> CreateCrsfTelemetry(::flatbuffers::FlatBufferBuilder &_fbb, const CrsfTelemetryT *_o, const ::flatbuffers::rehasher_function_t *_rehasher) {
  (void)_rehasher;
  (void)_o;
  struct _VectorArgs { ::flatbuffers::FlatBufferBuilder *__fbb; const CrsfTelemetryT* __o; const ::flatbuffers::rehasher_function_t *__rehasher; } _va = { &_fbb, _o, _rehasher}; (void)_va;
  auto _rssi = _o->rssi;
  auto _link_quality = _o->link_quality;
  auto _snr = _o->snr;
  auto _tx_power = _o->tx_power;
  return fbs::CreateCrsfTelemetry(
      _fbb,
      _rssi,
      _link_quality,
      _snr,
      _tx_power);
}

inline CrsfDataT::CrsfDataT(const CrsfDataT &o)
      : channels((o.channels) ? new fbs::CrsfChannels(*o.channels) : nullptr),
        telemetry((o.telemetry) ? new fbs::CrsfTelemetryT(*o.telemetry) : nullptr),
        failsafe(o.failsafe) {
}

inline CrsfDataT &CrsfDataT::operator=(CrsfDataT o) FLATBUFFERS_NOEXCEPT {
  std::swap(channels, o.channels);
  std::swap(telemetry, o.telemetry);
  std::swap(failsafe, o.failsafe);
  return *this;
}

inline CrsfDataT *CrsfData::UnPack(const ::flatbuffers::resolver_function_t *_resolver) const {
  auto _o = std::unique_ptr<CrsfDataT>(new CrsfDataT());
  UnPackTo(_o.get(), _resolver);
  return _o.release();
}

inline void CrsfData::UnPackTo(CrsfDataT *_o, const ::flatbuffers::resolver_function_t *_resolver) const {
  (void)_o;
  (void)_resolver;
  { auto _e = channels(); if (_e) _o->channels = std::unique_ptr<fbs::CrsfChannels>(new fbs::CrsfChannels(*_e)); }
  { auto _e = telemetry(); if (_e) { if(_o->telemetry) { _e->UnPackTo(_o->telemetry.get(), _resolver); } else { _o->telemetry = std::unique_ptr<fbs::CrsfTelemetryT>(_e->UnPack(_resolver)); } } else if (_o->telemetry) { _o->telemetry.reset(); } }
  { auto _e = failsafe(); _o->failsafe = _e; }
}

inline ::flatbuffers::Offset<CrsfData> CrsfData::Pack(::flatbuffers::FlatBufferBuilder &_fbb, const CrsfDataT* _o, const ::flatbuffers::rehasher_function_t *_rehasher) {
  return CreateCrsfData(_fbb, _o, _rehasher);
}

inline ::flatbuffers::Offset<CrsfData> CreateCrsfData(::flatbuffers::FlatBufferBuilder &_fbb, const CrsfDataT *_o, const ::flatbuffers::rehasher_function_t *_rehasher) {
  (void)_rehasher;
  (void)_o;
  struct _VectorArgs { ::flatbuffers::FlatBufferBuilder *__fbb; const CrsfDataT* __o; const ::flatbuffers::rehasher_function_t *__rehasher; } _va = { &_fbb, _o, _rehasher}; (void)_va;
  auto _channels = _o->channels ? _o->channels.get() : nullptr;
  auto _telemetry = _o->telemetry ? CreateCrsfTelemetry(_fbb, _o->telemetry.get(), _rehasher) : 0;
  auto _failsafe = _o->failsafe;
  return fbs::CreateCrsfData(
      _fbb,
      _channels,
      _telemetry,
      _failsafe);
}

inline OnboardComputerRxT *OnboardComputerRx::UnPack(const ::flatbuffers::resolver_function_t *_resolver) const {
  auto _o = std::unique_ptr<OnboardComputerRxT>(new OnboardComputerRxT());
  UnPackTo(_o.get(), _resolver);
  return _o.release();
}

inline void OnboardComputerRx::UnPackTo(OnboardComputerRxT *_o, const ::flatbuffers::resolver_function_t *_resolver) const {
  (void)_o;
  (void)_resolver;
  { auto _e = message_type(); _o->message.type = _e; }
  { auto _e = message(); if (_e) _o->message.value = fbs::OnboardComputerRxUnionUnion::UnPack(_e, message_type(), _resolver); }
}

inline ::flatbuffers::Offset<OnboardComputerRx> OnboardComputerRx::Pack(::flatbuffers::FlatBufferBuilder &_fbb, const OnboardComputerRxT* _o, const ::flatbuffers::rehasher_function_t *_rehasher) {
  return CreateOnboardComputerRx(_fbb, _o, _rehasher);
}

inline ::flatbuffers::Offset<OnboardComputerRx> CreateOnboardComputerRx(::flatbuffers::FlatBufferBuilder &_fbb, const OnboardComputerRxT *_o, const ::flatbuffers::rehasher_function_t *_rehasher) {
  (void)_rehasher;
  (void)_o;
  struct _VectorArgs { ::flatbuffers::FlatBufferBuilder *__fbb; const OnboardComputerRxT* __o; const ::flatbuffers::rehasher_function_t *__rehasher; } _va = { &_fbb, _o, _rehasher}; (void)_va;
  auto _message_type = _o->message.type;
  auto _message = _o->message.Pack(_fbb);
  return fbs::CreateOnboardComputerRx(
      _fbb,
      _message_type,
      _message);
}

inline bool VerifyOnboardComputerRxUnion(::flatbuffers::Verifier &verifier, const void *obj, OnboardComputerRxUnion type) {
  switch (type) {
    case OnboardComputerRxUnion_NONE: {
      return true;
    }
    case OnboardComputerRxUnion_CrsfData: {
      auto ptr = reinterpret_cast<const fbs::CrsfData *>(obj);
      return verifier.VerifyTable(ptr);
    }
    case OnboardComputerRxUnion_Robot: {
      auto ptr = reinterpret_cast<const fbs::Robot *>(obj);
      return verifier.VerifyTable(ptr);
    }
    default: return true;
  }
}

inline bool VerifyOnboardComputerRxUnionVector(::flatbuffers::Verifier &verifier, const ::flatbuffers::Vector<::flatbuffers::Offset<void>> *values, const ::flatbuffers::Vector<uint8_t> *types) {
  if (!values || !types) return !values && !types;
  if (values->size() != types->size()) return false;
  for (::flatbuffers::uoffset_t i = 0; i < values->size(); ++i) {
    if (!VerifyOnboardComputerRxUnion(
        verifier,  values->Get(i), types->GetEnum<OnboardComputerRxUnion>(i))) {
      return false;
    }
  }
  return true;
}

inline void *OnboardComputerRxUnionUnion::UnPack(const void *obj, OnboardComputerRxUnion type, const ::flatbuffers::resolver_function_t *resolver) {
  (void)resolver;
  switch (type) {
    case OnboardComputerRxUnion_CrsfData: {
      auto ptr = reinterpret_cast<const fbs::CrsfData *>(obj);
      return ptr->UnPack(resolver);
    }
    case OnboardComputerRxUnion_Robot: {
      auto ptr = reinterpret_cast<const fbs::Robot *>(obj);
      return ptr->UnPack(resolver);
    }
    default: return nullptr;
  }
}

inline ::flatbuffers::Offset<void> OnboardComputerRxUnionUnion::Pack(::flatbuffers::FlatBufferBuilder &_fbb, const ::flatbuffers::rehasher_function_t *_rehasher) const {
  (void)_rehasher;
  switch (type) {
    case OnboardComputerRxUnion_CrsfData: {
      auto ptr = reinterpret_cast<const fbs::CrsfDataT *>(value);
      return CreateCrsfData(_fbb, ptr, _rehasher).Union();
    }
    case OnboardComputerRxUnion_Robot: {
      auto ptr = reinterpret_cast<const fbs::RobotT *>(value);
      return CreateRobot(_fbb, ptr, _rehasher).Union();
    }
    default: return 0;
  }
}

inline OnboardComputerRxUnionUnion::OnboardComputerRxUnionUnion(const OnboardComputerRxUnionUnion &u) : type(u.type), value(nullptr) {
  switch (type) {
    case OnboardComputerRxUnion_CrsfData: {
      value = new fbs::CrsfDataT(*reinterpret_cast<fbs::CrsfDataT *>(u.value));
      break;
    }
    case OnboardComputerRxUnion_Robot: {
      value = new fbs::RobotT(*reinterpret_cast<fbs::RobotT *>(u.value));
      break;
    }
    default:
      break;
  }
}

inline void OnboardComputerRxUnionUnion::Reset() {
  switch (type) {
    case OnboardComputerRxUnion_CrsfData: {
      auto ptr = reinterpret_cast<fbs::CrsfDataT *>(value);
      delete ptr;
      break;
    }
    case OnboardComputerRxUnion_Robot: {
      auto ptr = reinterpret_cast<fbs::RobotT *>(value);
      delete ptr;
      break;
    }
    default: break;
  }
  value = nullptr;
  type = OnboardComputerRxUnion_NONE;
}

inline const fbs::OnboardComputerRx *GetOnboardComputerRx(const void *buf) {
  return ::flatbuffers::GetRoot<fbs::OnboardComputerRx>(buf);
}

inline const fbs::OnboardComputerRx *GetSizePrefixedOnboardComputerRx(const void *buf) {
  return ::flatbuffers::GetSizePrefixedRoot<fbs::OnboardComputerRx>(buf);
}

inline const char *OnboardComputerRxIdentifier() {
  return "OCRX";
}

inline bool OnboardComputerRxBufferHasIdentifier(const void *buf) {
  return ::flatbuffers::BufferHasIdentifier(
      buf, OnboardComputerRxIdentifier());
}

inline bool SizePrefixedOnboardComputerRxBufferHasIdentifier(const void *buf) {
  return ::flatbuffers::BufferHasIdentifier(
      buf, OnboardComputerRxIdentifier(), true);
}

inline bool VerifyOnboardComputerRxBuffer(
    ::flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<fbs::OnboardComputerRx>(OnboardComputerRxIdentifier());
}

inline bool VerifySizePrefixedOnboardComputerRxBuffer(
    ::flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<fbs::OnboardComputerRx>(OnboardComputerRxIdentifier());
}

inline void FinishOnboardComputerRxBuffer(
    ::flatbuffers::FlatBufferBuilder &fbb,
    ::flatbuffers::Offset<fbs::OnboardComputerRx> root) {
  fbb.Finish(root, OnboardComputerRxIdentifier());
}

inline void FinishSizePrefixedOnboardComputerRxBuffer(
    ::flatbuffers::FlatBufferBuilder &fbb,
    ::flatbuffers::Offset<fbs::OnboardComputerRx> root) {
  fbb.FinishSizePrefixed(root, OnboardComputerRxIdentifier());
}

inline std::unique_ptr<fbs::OnboardComputerRxT> UnPackOnboardComputerRx(
    const void *buf,
    const ::flatbuffers::resolver_function_t *res = nullptr) {
  return std::unique_ptr<fbs::OnboardComputerRxT>(GetOnboardComputerRx(buf)->UnPack(res));
}

inline std::unique_ptr<fbs::OnboardComputerRxT> UnPackSizePrefixedOnboardComputerRx(
    const void *buf,
    const ::flatbuffers::resolver_function_t *res = nullptr) {
  return std::unique_ptr<fbs::OnboardComputerRxT>(GetSizePrefixedOnboardComputerRx(buf)->UnPack(res));
}

}  // namespace fbs

#endif  // FLATBUFFERS_GENERATED_ONBOARDCOMPUTERRXMESSAGE_FBS_H_
