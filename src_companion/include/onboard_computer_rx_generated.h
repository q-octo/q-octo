// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_ONBOARDCOMPUTERRX_FBS_H_
#define FLATBUFFERS_GENERATED_ONBOARDCOMPUTERRX_FBS_H_

#include "flatbuffers/flatbuffers.h"

// Ensure the included flatbuffers.h is the same version as when this file was
// generated, otherwise it may not be compatible.
static_assert(FLATBUFFERS_VERSION_MAJOR == 24 &&
              FLATBUFFERS_VERSION_MINOR == 3 &&
              FLATBUFFERS_VERSION_REVISION == 25,
             "Non-compatible flatbuffers version included");

#include "companion_tx_generated.h"
#include "robot_state_generated.h"

namespace fbs {

struct OnboardComputerRx;
struct OnboardComputerRxBuilder;
struct OnboardComputerRxT;

enum OnboardComputerRxUnion : uint8_t {
  OnboardComputerRxUnion_NONE = 0,
  OnboardComputerRxUnion_Robot = 1,
  OnboardComputerRxUnion_ButtonPressed = 2,
  OnboardComputerRxUnion_MIN = OnboardComputerRxUnion_NONE,
  OnboardComputerRxUnion_MAX = OnboardComputerRxUnion_ButtonPressed
};

inline const OnboardComputerRxUnion (&EnumValuesOnboardComputerRxUnion())[3] {
  static const OnboardComputerRxUnion values[] = {
    OnboardComputerRxUnion_NONE,
    OnboardComputerRxUnion_Robot,
    OnboardComputerRxUnion_ButtonPressed
  };
  return values;
}

inline const char * const *EnumNamesOnboardComputerRxUnion() {
  static const char * const names[4] = {
    "NONE",
    "Robot",
    "ButtonPressed",
    nullptr
  };
  return names;
}

inline const char *EnumNameOnboardComputerRxUnion(OnboardComputerRxUnion e) {
  if (::flatbuffers::IsOutRange(e, OnboardComputerRxUnion_NONE, OnboardComputerRxUnion_ButtonPressed)) return "";
  const size_t index = static_cast<size_t>(e);
  return EnumNamesOnboardComputerRxUnion()[index];
}

template<typename T> struct OnboardComputerRxUnionTraits {
  static const OnboardComputerRxUnion enum_value = OnboardComputerRxUnion_NONE;
};

template<> struct OnboardComputerRxUnionTraits<fbs::Robot> {
  static const OnboardComputerRxUnion enum_value = OnboardComputerRxUnion_Robot;
};

template<> struct OnboardComputerRxUnionTraits<fbs::ButtonPressed> {
  static const OnboardComputerRxUnion enum_value = OnboardComputerRxUnion_ButtonPressed;
};

template<typename T> struct OnboardComputerRxUnionUnionTraits {
  static const OnboardComputerRxUnion enum_value = OnboardComputerRxUnion_NONE;
};

template<> struct OnboardComputerRxUnionUnionTraits<fbs::RobotT> {
  static const OnboardComputerRxUnion enum_value = OnboardComputerRxUnion_Robot;
};

template<> struct OnboardComputerRxUnionUnionTraits<fbs::ButtonPressedT> {
  static const OnboardComputerRxUnion enum_value = OnboardComputerRxUnion_ButtonPressed;
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

  fbs::RobotT *AsRobot() {
    return type == OnboardComputerRxUnion_Robot ?
      reinterpret_cast<fbs::RobotT *>(value) : nullptr;
  }
  const fbs::RobotT *AsRobot() const {
    return type == OnboardComputerRxUnion_Robot ?
      reinterpret_cast<const fbs::RobotT *>(value) : nullptr;
  }
  fbs::ButtonPressedT *AsButtonPressed() {
    return type == OnboardComputerRxUnion_ButtonPressed ?
      reinterpret_cast<fbs::ButtonPressedT *>(value) : nullptr;
  }
  const fbs::ButtonPressedT *AsButtonPressed() const {
    return type == OnboardComputerRxUnion_ButtonPressed ?
      reinterpret_cast<const fbs::ButtonPressedT *>(value) : nullptr;
  }
};

bool VerifyOnboardComputerRxUnion(::flatbuffers::Verifier &verifier, const void *obj, OnboardComputerRxUnion type);
bool VerifyOnboardComputerRxUnionVector(::flatbuffers::Verifier &verifier, const ::flatbuffers::Vector<::flatbuffers::Offset<void>> *values, const ::flatbuffers::Vector<uint8_t> *types);

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
  const fbs::Robot *message_as_Robot() const {
    return message_type() == fbs::OnboardComputerRxUnion_Robot ? static_cast<const fbs::Robot *>(message()) : nullptr;
  }
  const fbs::ButtonPressed *message_as_ButtonPressed() const {
    return message_type() == fbs::OnboardComputerRxUnion_ButtonPressed ? static_cast<const fbs::ButtonPressed *>(message()) : nullptr;
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

template<> inline const fbs::Robot *OnboardComputerRx::message_as<fbs::Robot>() const {
  return message_as_Robot();
}

template<> inline const fbs::ButtonPressed *OnboardComputerRx::message_as<fbs::ButtonPressed>() const {
  return message_as_ButtonPressed();
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
    case OnboardComputerRxUnion_Robot: {
      auto ptr = reinterpret_cast<const fbs::Robot *>(obj);
      return verifier.VerifyTable(ptr);
    }
    case OnboardComputerRxUnion_ButtonPressed: {
      auto ptr = reinterpret_cast<const fbs::ButtonPressed *>(obj);
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
    case OnboardComputerRxUnion_Robot: {
      auto ptr = reinterpret_cast<const fbs::Robot *>(obj);
      return ptr->UnPack(resolver);
    }
    case OnboardComputerRxUnion_ButtonPressed: {
      auto ptr = reinterpret_cast<const fbs::ButtonPressed *>(obj);
      return ptr->UnPack(resolver);
    }
    default: return nullptr;
  }
}

inline ::flatbuffers::Offset<void> OnboardComputerRxUnionUnion::Pack(::flatbuffers::FlatBufferBuilder &_fbb, const ::flatbuffers::rehasher_function_t *_rehasher) const {
  (void)_rehasher;
  switch (type) {
    case OnboardComputerRxUnion_Robot: {
      auto ptr = reinterpret_cast<const fbs::RobotT *>(value);
      return CreateRobot(_fbb, ptr, _rehasher).Union();
    }
    case OnboardComputerRxUnion_ButtonPressed: {
      auto ptr = reinterpret_cast<const fbs::ButtonPressedT *>(value);
      return CreateButtonPressed(_fbb, ptr, _rehasher).Union();
    }
    default: return 0;
  }
}

inline OnboardComputerRxUnionUnion::OnboardComputerRxUnionUnion(const OnboardComputerRxUnionUnion &u) : type(u.type), value(nullptr) {
  switch (type) {
    case OnboardComputerRxUnion_Robot: {
      value = new fbs::RobotT(*reinterpret_cast<fbs::RobotT *>(u.value));
      break;
    }
    case OnboardComputerRxUnion_ButtonPressed: {
      value = new fbs::ButtonPressedT(*reinterpret_cast<fbs::ButtonPressedT *>(u.value));
      break;
    }
    default:
      break;
  }
}

inline void OnboardComputerRxUnionUnion::Reset() {
  switch (type) {
    case OnboardComputerRxUnion_Robot: {
      auto ptr = reinterpret_cast<fbs::RobotT *>(value);
      delete ptr;
      break;
    }
    case OnboardComputerRxUnion_ButtonPressed: {
      auto ptr = reinterpret_cast<fbs::ButtonPressedT *>(value);
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

#endif  // FLATBUFFERS_GENERATED_ONBOARDCOMPUTERRX_FBS_H_
