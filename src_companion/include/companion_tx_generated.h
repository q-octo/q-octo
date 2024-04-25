// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_COMPANIONTX_FBS_H_
#define FLATBUFFERS_GENERATED_COMPANIONTX_FBS_H_

#include "flatbuffers/flatbuffers.h"

// Ensure the included flatbuffers.h is the same version as when this file was
// generated, otherwise it may not be compatible.
static_assert(FLATBUFFERS_VERSION_MAJOR == 24 &&
              FLATBUFFERS_VERSION_MINOR == 3 &&
              FLATBUFFERS_VERSION_REVISION == 25,
             "Non-compatible flatbuffers version included");

namespace fbs {

struct ButtonPressed;
struct ButtonPressedBuilder;
struct ButtonPressedT;

struct CompanionTx;
struct CompanionTxBuilder;
struct CompanionTxT;

enum Button : int8_t {
  Button_A = 0,
  Button_B = 1,
  Button_X = 2,
  Button_MIN = Button_A,
  Button_MAX = Button_X
};

inline const Button (&EnumValuesButton())[3] {
  static const Button values[] = {
    Button_A,
    Button_B,
    Button_X
  };
  return values;
}

inline const char * const *EnumNamesButton() {
  static const char * const names[4] = {
    "A",
    "B",
    "X",
    nullptr
  };
  return names;
}

inline const char *EnumNameButton(Button e) {
  if (::flatbuffers::IsOutRange(e, Button_A, Button_X)) return "";
  const size_t index = static_cast<size_t>(e);
  return EnumNamesButton()[index];
}

enum CompanionTxUnion : uint8_t {
  CompanionTxUnion_NONE = 0,
  CompanionTxUnion_ButtonPressed = 1,
  CompanionTxUnion_MIN = CompanionTxUnion_NONE,
  CompanionTxUnion_MAX = CompanionTxUnion_ButtonPressed
};

inline const CompanionTxUnion (&EnumValuesCompanionTxUnion())[2] {
  static const CompanionTxUnion values[] = {
    CompanionTxUnion_NONE,
    CompanionTxUnion_ButtonPressed
  };
  return values;
}

inline const char * const *EnumNamesCompanionTxUnion() {
  static const char * const names[3] = {
    "NONE",
    "ButtonPressed",
    nullptr
  };
  return names;
}

inline const char *EnumNameCompanionTxUnion(CompanionTxUnion e) {
  if (::flatbuffers::IsOutRange(e, CompanionTxUnion_NONE, CompanionTxUnion_ButtonPressed)) return "";
  const size_t index = static_cast<size_t>(e);
  return EnumNamesCompanionTxUnion()[index];
}

template<typename T> struct CompanionTxUnionTraits {
  static const CompanionTxUnion enum_value = CompanionTxUnion_NONE;
};

template<> struct CompanionTxUnionTraits<fbs::ButtonPressed> {
  static const CompanionTxUnion enum_value = CompanionTxUnion_ButtonPressed;
};

template<typename T> struct CompanionTxUnionUnionTraits {
  static const CompanionTxUnion enum_value = CompanionTxUnion_NONE;
};

template<> struct CompanionTxUnionUnionTraits<fbs::ButtonPressedT> {
  static const CompanionTxUnion enum_value = CompanionTxUnion_ButtonPressed;
};

struct CompanionTxUnionUnion {
  CompanionTxUnion type;
  void *value;

  CompanionTxUnionUnion() : type(CompanionTxUnion_NONE), value(nullptr) {}
  CompanionTxUnionUnion(CompanionTxUnionUnion&& u) FLATBUFFERS_NOEXCEPT :
    type(CompanionTxUnion_NONE), value(nullptr)
    { std::swap(type, u.type); std::swap(value, u.value); }
  CompanionTxUnionUnion(const CompanionTxUnionUnion &);
  CompanionTxUnionUnion &operator=(const CompanionTxUnionUnion &u)
    { CompanionTxUnionUnion t(u); std::swap(type, t.type); std::swap(value, t.value); return *this; }
  CompanionTxUnionUnion &operator=(CompanionTxUnionUnion &&u) FLATBUFFERS_NOEXCEPT
    { std::swap(type, u.type); std::swap(value, u.value); return *this; }
  ~CompanionTxUnionUnion() { Reset(); }

  void Reset();

  template <typename T>
  void Set(T&& val) {
    typedef typename std::remove_reference<T>::type RT;
    Reset();
    type = CompanionTxUnionUnionTraits<RT>::enum_value;
    if (type != CompanionTxUnion_NONE) {
      value = new RT(std::forward<T>(val));
    }
  }

  static void *UnPack(const void *obj, CompanionTxUnion type, const ::flatbuffers::resolver_function_t *resolver);
  ::flatbuffers::Offset<void> Pack(::flatbuffers::FlatBufferBuilder &_fbb, const ::flatbuffers::rehasher_function_t *_rehasher = nullptr) const;

  fbs::ButtonPressedT *AsButtonPressed() {
    return type == CompanionTxUnion_ButtonPressed ?
      reinterpret_cast<fbs::ButtonPressedT *>(value) : nullptr;
  }
  const fbs::ButtonPressedT *AsButtonPressed() const {
    return type == CompanionTxUnion_ButtonPressed ?
      reinterpret_cast<const fbs::ButtonPressedT *>(value) : nullptr;
  }
};

bool VerifyCompanionTxUnion(::flatbuffers::Verifier &verifier, const void *obj, CompanionTxUnion type);
bool VerifyCompanionTxUnionVector(::flatbuffers::Verifier &verifier, const ::flatbuffers::Vector<::flatbuffers::Offset<void>> *values, const ::flatbuffers::Vector<uint8_t> *types);

struct ButtonPressedT : public ::flatbuffers::NativeTable {
  typedef ButtonPressed TableType;
  fbs::Button button = fbs::Button_A;
};

struct ButtonPressed FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef ButtonPressedT NativeTableType;
  typedef ButtonPressedBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_BUTTON = 4
  };
  fbs::Button button() const {
    return static_cast<fbs::Button>(GetField<int8_t>(VT_BUTTON, 0));
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<int8_t>(verifier, VT_BUTTON, 1) &&
           verifier.EndTable();
  }
  ButtonPressedT *UnPack(const ::flatbuffers::resolver_function_t *_resolver = nullptr) const;
  void UnPackTo(ButtonPressedT *_o, const ::flatbuffers::resolver_function_t *_resolver = nullptr) const;
  static ::flatbuffers::Offset<ButtonPressed> Pack(::flatbuffers::FlatBufferBuilder &_fbb, const ButtonPressedT* _o, const ::flatbuffers::rehasher_function_t *_rehasher = nullptr);
};

struct ButtonPressedBuilder {
  typedef ButtonPressed Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_button(fbs::Button button) {
    fbb_.AddElement<int8_t>(ButtonPressed::VT_BUTTON, static_cast<int8_t>(button), 0);
  }
  explicit ButtonPressedBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<ButtonPressed> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<ButtonPressed>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<ButtonPressed> CreateButtonPressed(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    fbs::Button button = fbs::Button_A) {
  ButtonPressedBuilder builder_(_fbb);
  builder_.add_button(button);
  return builder_.Finish();
}

::flatbuffers::Offset<ButtonPressed> CreateButtonPressed(::flatbuffers::FlatBufferBuilder &_fbb, const ButtonPressedT *_o, const ::flatbuffers::rehasher_function_t *_rehasher = nullptr);

struct CompanionTxT : public ::flatbuffers::NativeTable {
  typedef CompanionTx TableType;
  fbs::CompanionTxUnionUnion message{};
};

struct CompanionTx FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef CompanionTxT NativeTableType;
  typedef CompanionTxBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_MESSAGE_TYPE = 4,
    VT_MESSAGE = 6
  };
  fbs::CompanionTxUnion message_type() const {
    return static_cast<fbs::CompanionTxUnion>(GetField<uint8_t>(VT_MESSAGE_TYPE, 0));
  }
  const void *message() const {
    return GetPointer<const void *>(VT_MESSAGE);
  }
  template<typename T> const T *message_as() const;
  const fbs::ButtonPressed *message_as_ButtonPressed() const {
    return message_type() == fbs::CompanionTxUnion_ButtonPressed ? static_cast<const fbs::ButtonPressed *>(message()) : nullptr;
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint8_t>(verifier, VT_MESSAGE_TYPE, 1) &&
           VerifyOffset(verifier, VT_MESSAGE) &&
           VerifyCompanionTxUnion(verifier, message(), message_type()) &&
           verifier.EndTable();
  }
  CompanionTxT *UnPack(const ::flatbuffers::resolver_function_t *_resolver = nullptr) const;
  void UnPackTo(CompanionTxT *_o, const ::flatbuffers::resolver_function_t *_resolver = nullptr) const;
  static ::flatbuffers::Offset<CompanionTx> Pack(::flatbuffers::FlatBufferBuilder &_fbb, const CompanionTxT* _o, const ::flatbuffers::rehasher_function_t *_rehasher = nullptr);
};

template<> inline const fbs::ButtonPressed *CompanionTx::message_as<fbs::ButtonPressed>() const {
  return message_as_ButtonPressed();
}

struct CompanionTxBuilder {
  typedef CompanionTx Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_message_type(fbs::CompanionTxUnion message_type) {
    fbb_.AddElement<uint8_t>(CompanionTx::VT_MESSAGE_TYPE, static_cast<uint8_t>(message_type), 0);
  }
  void add_message(::flatbuffers::Offset<void> message) {
    fbb_.AddOffset(CompanionTx::VT_MESSAGE, message);
  }
  explicit CompanionTxBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<CompanionTx> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<CompanionTx>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<CompanionTx> CreateCompanionTx(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    fbs::CompanionTxUnion message_type = fbs::CompanionTxUnion_NONE,
    ::flatbuffers::Offset<void> message = 0) {
  CompanionTxBuilder builder_(_fbb);
  builder_.add_message(message);
  builder_.add_message_type(message_type);
  return builder_.Finish();
}

::flatbuffers::Offset<CompanionTx> CreateCompanionTx(::flatbuffers::FlatBufferBuilder &_fbb, const CompanionTxT *_o, const ::flatbuffers::rehasher_function_t *_rehasher = nullptr);

inline ButtonPressedT *ButtonPressed::UnPack(const ::flatbuffers::resolver_function_t *_resolver) const {
  auto _o = std::unique_ptr<ButtonPressedT>(new ButtonPressedT());
  UnPackTo(_o.get(), _resolver);
  return _o.release();
}

inline void ButtonPressed::UnPackTo(ButtonPressedT *_o, const ::flatbuffers::resolver_function_t *_resolver) const {
  (void)_o;
  (void)_resolver;
  { auto _e = button(); _o->button = _e; }
}

inline ::flatbuffers::Offset<ButtonPressed> ButtonPressed::Pack(::flatbuffers::FlatBufferBuilder &_fbb, const ButtonPressedT* _o, const ::flatbuffers::rehasher_function_t *_rehasher) {
  return CreateButtonPressed(_fbb, _o, _rehasher);
}

inline ::flatbuffers::Offset<ButtonPressed> CreateButtonPressed(::flatbuffers::FlatBufferBuilder &_fbb, const ButtonPressedT *_o, const ::flatbuffers::rehasher_function_t *_rehasher) {
  (void)_rehasher;
  (void)_o;
  struct _VectorArgs { ::flatbuffers::FlatBufferBuilder *__fbb; const ButtonPressedT* __o; const ::flatbuffers::rehasher_function_t *__rehasher; } _va = { &_fbb, _o, _rehasher}; (void)_va;
  auto _button = _o->button;
  return fbs::CreateButtonPressed(
      _fbb,
      _button);
}

inline CompanionTxT *CompanionTx::UnPack(const ::flatbuffers::resolver_function_t *_resolver) const {
  auto _o = std::unique_ptr<CompanionTxT>(new CompanionTxT());
  UnPackTo(_o.get(), _resolver);
  return _o.release();
}

inline void CompanionTx::UnPackTo(CompanionTxT *_o, const ::flatbuffers::resolver_function_t *_resolver) const {
  (void)_o;
  (void)_resolver;
  { auto _e = message_type(); _o->message.type = _e; }
  { auto _e = message(); if (_e) _o->message.value = fbs::CompanionTxUnionUnion::UnPack(_e, message_type(), _resolver); }
}

inline ::flatbuffers::Offset<CompanionTx> CompanionTx::Pack(::flatbuffers::FlatBufferBuilder &_fbb, const CompanionTxT* _o, const ::flatbuffers::rehasher_function_t *_rehasher) {
  return CreateCompanionTx(_fbb, _o, _rehasher);
}

inline ::flatbuffers::Offset<CompanionTx> CreateCompanionTx(::flatbuffers::FlatBufferBuilder &_fbb, const CompanionTxT *_o, const ::flatbuffers::rehasher_function_t *_rehasher) {
  (void)_rehasher;
  (void)_o;
  struct _VectorArgs { ::flatbuffers::FlatBufferBuilder *__fbb; const CompanionTxT* __o; const ::flatbuffers::rehasher_function_t *__rehasher; } _va = { &_fbb, _o, _rehasher}; (void)_va;
  auto _message_type = _o->message.type;
  auto _message = _o->message.Pack(_fbb);
  return fbs::CreateCompanionTx(
      _fbb,
      _message_type,
      _message);
}

inline bool VerifyCompanionTxUnion(::flatbuffers::Verifier &verifier, const void *obj, CompanionTxUnion type) {
  switch (type) {
    case CompanionTxUnion_NONE: {
      return true;
    }
    case CompanionTxUnion_ButtonPressed: {
      auto ptr = reinterpret_cast<const fbs::ButtonPressed *>(obj);
      return verifier.VerifyTable(ptr);
    }
    default: return true;
  }
}

inline bool VerifyCompanionTxUnionVector(::flatbuffers::Verifier &verifier, const ::flatbuffers::Vector<::flatbuffers::Offset<void>> *values, const ::flatbuffers::Vector<uint8_t> *types) {
  if (!values || !types) return !values && !types;
  if (values->size() != types->size()) return false;
  for (::flatbuffers::uoffset_t i = 0; i < values->size(); ++i) {
    if (!VerifyCompanionTxUnion(
        verifier,  values->Get(i), types->GetEnum<CompanionTxUnion>(i))) {
      return false;
    }
  }
  return true;
}

inline void *CompanionTxUnionUnion::UnPack(const void *obj, CompanionTxUnion type, const ::flatbuffers::resolver_function_t *resolver) {
  (void)resolver;
  switch (type) {
    case CompanionTxUnion_ButtonPressed: {
      auto ptr = reinterpret_cast<const fbs::ButtonPressed *>(obj);
      return ptr->UnPack(resolver);
    }
    default: return nullptr;
  }
}

inline ::flatbuffers::Offset<void> CompanionTxUnionUnion::Pack(::flatbuffers::FlatBufferBuilder &_fbb, const ::flatbuffers::rehasher_function_t *_rehasher) const {
  (void)_rehasher;
  switch (type) {
    case CompanionTxUnion_ButtonPressed: {
      auto ptr = reinterpret_cast<const fbs::ButtonPressedT *>(value);
      return CreateButtonPressed(_fbb, ptr, _rehasher).Union();
    }
    default: return 0;
  }
}

inline CompanionTxUnionUnion::CompanionTxUnionUnion(const CompanionTxUnionUnion &u) : type(u.type), value(nullptr) {
  switch (type) {
    case CompanionTxUnion_ButtonPressed: {
      value = new fbs::ButtonPressedT(*reinterpret_cast<fbs::ButtonPressedT *>(u.value));
      break;
    }
    default:
      break;
  }
}

inline void CompanionTxUnionUnion::Reset() {
  switch (type) {
    case CompanionTxUnion_ButtonPressed: {
      auto ptr = reinterpret_cast<fbs::ButtonPressedT *>(value);
      delete ptr;
      break;
    }
    default: break;
  }
  value = nullptr;
  type = CompanionTxUnion_NONE;
}

inline const fbs::CompanionTx *GetCompanionTx(const void *buf) {
  return ::flatbuffers::GetRoot<fbs::CompanionTx>(buf);
}

inline const fbs::CompanionTx *GetSizePrefixedCompanionTx(const void *buf) {
  return ::flatbuffers::GetSizePrefixedRoot<fbs::CompanionTx>(buf);
}

inline const char *CompanionTxIdentifier() {
  return "CNTX";
}

inline bool CompanionTxBufferHasIdentifier(const void *buf) {
  return ::flatbuffers::BufferHasIdentifier(
      buf, CompanionTxIdentifier());
}

inline bool SizePrefixedCompanionTxBufferHasIdentifier(const void *buf) {
  return ::flatbuffers::BufferHasIdentifier(
      buf, CompanionTxIdentifier(), true);
}

inline bool VerifyCompanionTxBuffer(
    ::flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<fbs::CompanionTx>(CompanionTxIdentifier());
}

inline bool VerifySizePrefixedCompanionTxBuffer(
    ::flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<fbs::CompanionTx>(CompanionTxIdentifier());
}

inline void FinishCompanionTxBuffer(
    ::flatbuffers::FlatBufferBuilder &fbb,
    ::flatbuffers::Offset<fbs::CompanionTx> root) {
  fbb.Finish(root, CompanionTxIdentifier());
}

inline void FinishSizePrefixedCompanionTxBuffer(
    ::flatbuffers::FlatBufferBuilder &fbb,
    ::flatbuffers::Offset<fbs::CompanionTx> root) {
  fbb.FinishSizePrefixed(root, CompanionTxIdentifier());
}

inline std::unique_ptr<fbs::CompanionTxT> UnPackCompanionTx(
    const void *buf,
    const ::flatbuffers::resolver_function_t *res = nullptr) {
  return std::unique_ptr<fbs::CompanionTxT>(GetCompanionTx(buf)->UnPack(res));
}

inline std::unique_ptr<fbs::CompanionTxT> UnPackSizePrefixedCompanionTx(
    const void *buf,
    const ::flatbuffers::resolver_function_t *res = nullptr) {
  return std::unique_ptr<fbs::CompanionTxT>(GetSizePrefixedCompanionTx(buf)->UnPack(res));
}

}  // namespace fbs

#endif  // FLATBUFFERS_GENERATED_COMPANIONTX_FBS_H_