// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_FILESNAME_FILES_H_
#define FLATBUFFERS_GENERATED_FILESNAME_FILES_H_

#include "flatbuffers/flatbuffers.h"

// Ensure the included flatbuffers.h is the same version as when this file was
// generated, otherwise it may not be compatible.
static_assert(FLATBUFFERS_VERSION_MAJOR == 24 &&
              FLATBUFFERS_VERSION_MINOR == 3 &&
              FLATBUFFERS_VERSION_REVISION == 25,
             "Non-compatible flatbuffers version included");

namespace Files {

struct name_file;
struct name_fileBuilder;

struct id_files;
struct id_filesBuilder;

struct name_file FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef name_fileBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_NAME = 4,
    VT_ID = 6,
    VT_DL = 8
  };
  const ::flatbuffers::String *name() const {
    return GetPointer<const ::flatbuffers::String *>(VT_NAME);
  }
  uint64_t id() const {
    return GetField<uint64_t>(VT_ID, 0);
  }
  bool KeyCompareLessThan(const name_file * const o) const {
    return id() < o->id();
  }
  int KeyCompareWithValue(uint64_t _id) const {
    return static_cast<int>(id() > _id) - static_cast<int>(id() < _id);
  }
  uint64_t dl() const {
    return GetField<uint64_t>(VT_DL, 0);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_NAME) &&
           verifier.VerifyString(name()) &&
           VerifyField<uint64_t>(verifier, VT_ID, 8) &&
           VerifyField<uint64_t>(verifier, VT_DL, 8) &&
           verifier.EndTable();
  }
};

struct name_fileBuilder {
  typedef name_file Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_name(::flatbuffers::Offset<::flatbuffers::String> name) {
    fbb_.AddOffset(name_file::VT_NAME, name);
  }
  void add_id(uint64_t id) {
    fbb_.AddElement<uint64_t>(name_file::VT_ID, id, 0);
  }
  void add_dl(uint64_t dl) {
    fbb_.AddElement<uint64_t>(name_file::VT_DL, dl, 0);
  }
  explicit name_fileBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<name_file> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<name_file>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<name_file> Createname_file(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    ::flatbuffers::Offset<::flatbuffers::String> name = 0,
    uint64_t id = 0,
    uint64_t dl = 0) {
  name_fileBuilder builder_(_fbb);
  builder_.add_dl(dl);
  builder_.add_id(id);
  builder_.add_name(name);
  return builder_.Finish();
}

inline ::flatbuffers::Offset<name_file> Createname_fileDirect(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    const char *name = nullptr,
    uint64_t id = 0,
    uint64_t dl = 0) {
  auto name__ = name ? _fbb.CreateString(name) : 0;
  return Files::Createname_file(
      _fbb,
      name__,
      id,
      dl);
}

struct id_files FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef id_filesBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_FILES = 4,
    VT_D_AVG = 6
  };
  const ::flatbuffers::Vector<::flatbuffers::Offset<Files::name_file>> *files() const {
    return GetPointer<const ::flatbuffers::Vector<::flatbuffers::Offset<Files::name_file>> *>(VT_FILES);
  }
  uint64_t d_avg() const {
    return GetField<uint64_t>(VT_D_AVG, 0);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_FILES) &&
           verifier.VerifyVector(files()) &&
           verifier.VerifyVectorOfTables(files()) &&
           VerifyField<uint64_t>(verifier, VT_D_AVG, 8) &&
           verifier.EndTable();
  }
};

struct id_filesBuilder {
  typedef id_files Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_files(::flatbuffers::Offset<::flatbuffers::Vector<::flatbuffers::Offset<Files::name_file>>> files) {
    fbb_.AddOffset(id_files::VT_FILES, files);
  }
  void add_d_avg(uint64_t d_avg) {
    fbb_.AddElement<uint64_t>(id_files::VT_D_AVG, d_avg, 0);
  }
  explicit id_filesBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<id_files> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<id_files>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<id_files> Createid_files(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    ::flatbuffers::Offset<::flatbuffers::Vector<::flatbuffers::Offset<Files::name_file>>> files = 0,
    uint64_t d_avg = 0) {
  id_filesBuilder builder_(_fbb);
  builder_.add_d_avg(d_avg);
  builder_.add_files(files);
  return builder_.Finish();
}

inline ::flatbuffers::Offset<id_files> Createid_filesDirect(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    std::vector<::flatbuffers::Offset<Files::name_file>> *files = nullptr,
    uint64_t d_avg = 0) {
  auto files__ = files ? _fbb.CreateVectorOfSortedTables<Files::name_file>(files) : 0;
  return Files::Createid_files(
      _fbb,
      files__,
      d_avg);
}

inline const Files::id_files *Getid_files(const void *buf) {
  return ::flatbuffers::GetRoot<Files::id_files>(buf);
}

inline const Files::id_files *GetSizePrefixedid_files(const void *buf) {
  return ::flatbuffers::GetSizePrefixedRoot<Files::id_files>(buf);
}

inline bool Verifyid_filesBuffer(
    ::flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<Files::id_files>(nullptr);
}

inline bool VerifySizePrefixedid_filesBuffer(
    ::flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<Files::id_files>(nullptr);
}

inline void Finishid_filesBuffer(
    ::flatbuffers::FlatBufferBuilder &fbb,
    ::flatbuffers::Offset<Files::id_files> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixedid_filesBuffer(
    ::flatbuffers::FlatBufferBuilder &fbb,
    ::flatbuffers::Offset<Files::id_files> root) {
  fbb.FinishSizePrefixed(root);
}

}  // namespace Files

#endif  // FLATBUFFERS_GENERATED_FILESNAME_FILES_H_