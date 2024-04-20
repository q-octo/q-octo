// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_OBCTXMESSAGE_FBS_H_
#define FLATBUFFERS_GENERATED_OBCTXMESSAGE_FBS_H_

#include "flatbuffers/flatbuffers.h"

// Ensure the included flatbuffers.h is the same version as when this file was
// generated, otherwise it may not be compatible.
static_assert(FLATBUFFERS_VERSION_MAJOR == 24 &&
              FLATBUFFERS_VERSION_MINOR == 3 &&
              FLATBUFFERS_VERSION_REVISION == 25,
             "Non-compatible flatbuffers version included");

namespace fbs {

struct TxMessage;
struct TxMessageBuilder;

struct TxMessage FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef TxMessageBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_CRSF_FRAME_TYPE = 4,
    VT_PAYLOAD_LENGTH = 6,
    VT_PAYLOAD_DATA = 8
  };
  uint8_t crsf_frame_type() const {
    return GetField<uint8_t>(VT_CRSF_FRAME_TYPE, 0);
  }
  /// excludes CRC byte
  uint8_t payload_length() const {
    return GetField<uint8_t>(VT_PAYLOAD_LENGTH, 0);
  }
  const ::flatbuffers::Vector<uint8_t> *payload_data() const {
    return GetPointer<const ::flatbuffers::Vector<uint8_t> *>(VT_PAYLOAD_DATA);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint8_t>(verifier, VT_CRSF_FRAME_TYPE, 1) &&
           VerifyField<uint8_t>(verifier, VT_PAYLOAD_LENGTH, 1) &&
           VerifyOffset(verifier, VT_PAYLOAD_DATA) &&
           verifier.VerifyVector(payload_data()) &&
           verifier.EndTable();
  }
};

struct TxMessageBuilder {
  typedef TxMessage Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_crsf_frame_type(uint8_t crsf_frame_type) {
    fbb_.AddElement<uint8_t>(TxMessage::VT_CRSF_FRAME_TYPE, crsf_frame_type, 0);
  }
  void add_payload_length(uint8_t payload_length) {
    fbb_.AddElement<uint8_t>(TxMessage::VT_PAYLOAD_LENGTH, payload_length, 0);
  }
  void add_payload_data(::flatbuffers::Offset<::flatbuffers::Vector<uint8_t>> payload_data) {
    fbb_.AddOffset(TxMessage::VT_PAYLOAD_DATA, payload_data);
  }
  explicit TxMessageBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<TxMessage> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<TxMessage>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<TxMessage> CreateTxMessage(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    uint8_t crsf_frame_type = 0,
    uint8_t payload_length = 0,
    ::flatbuffers::Offset<::flatbuffers::Vector<uint8_t>> payload_data = 0) {
  TxMessageBuilder builder_(_fbb);
  builder_.add_payload_data(payload_data);
  builder_.add_payload_length(payload_length);
  builder_.add_crsf_frame_type(crsf_frame_type);
  return builder_.Finish();
}

inline ::flatbuffers::Offset<TxMessage> CreateTxMessageDirect(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    uint8_t crsf_frame_type = 0,
    uint8_t payload_length = 0,
    const std::vector<uint8_t> *payload_data = nullptr) {
  auto payload_data__ = payload_data ? _fbb.CreateVector<uint8_t>(*payload_data) : 0;
  return fbs::CreateTxMessage(
      _fbb,
      crsf_frame_type,
      payload_length,
      payload_data__);
}

inline const fbs::TxMessage *GetTxMessage(const void *buf) {
  return ::flatbuffers::GetRoot<fbs::TxMessage>(buf);
}

inline const fbs::TxMessage *GetSizePrefixedTxMessage(const void *buf) {
  return ::flatbuffers::GetSizePrefixedRoot<fbs::TxMessage>(buf);
}

inline const char *TxMessageIdentifier() {
  return "OCTX";
}

inline bool TxMessageBufferHasIdentifier(const void *buf) {
  return ::flatbuffers::BufferHasIdentifier(
      buf, TxMessageIdentifier());
}

inline bool SizePrefixedTxMessageBufferHasIdentifier(const void *buf) {
  return ::flatbuffers::BufferHasIdentifier(
      buf, TxMessageIdentifier(), true);
}

inline bool VerifyTxMessageBuffer(
    ::flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<fbs::TxMessage>(TxMessageIdentifier());
}

inline bool VerifySizePrefixedTxMessageBuffer(
    ::flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<fbs::TxMessage>(TxMessageIdentifier());
}

inline void FinishTxMessageBuffer(
    ::flatbuffers::FlatBufferBuilder &fbb,
    ::flatbuffers::Offset<fbs::TxMessage> root) {
  fbb.Finish(root, TxMessageIdentifier());
}

inline void FinishSizePrefixedTxMessageBuffer(
    ::flatbuffers::FlatBufferBuilder &fbb,
    ::flatbuffers::Offset<fbs::TxMessage> root) {
  fbb.FinishSizePrefixed(root, TxMessageIdentifier());
}

}  // namespace fbs

#endif  // FLATBUFFERS_GENERATED_OBCTXMESSAGE_FBS_H_
