// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_INDEX_INDEX_H_
#define FLATBUFFERS_GENERATED_INDEX_INDEX_H_

#include "flatbuffers/flatbuffers.h"

// Ensure the included flatbuffers.h is the same version as when this file was
// generated, otherwise it may not be compatible.
static_assert(FLATBUFFERS_VERSION_MAJOR == 24 &&
              FLATBUFFERS_VERSION_MINOR == 3 &&
              FLATBUFFERS_VERSION_REVISION == 25,
             "Non-compatible flatbuffers version included");

namespace Index {

struct File;
struct FileBuilder;

struct Node;
struct NodeBuilder;

struct FilesIndex;
struct FilesIndexBuilder;

struct File FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef FileBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_ID = 4,
    VT_ROWS = 6
  };
  uint64_t id() const {
    return GetField<uint64_t>(VT_ID, 0);
  }
  bool KeyCompareLessThan(const File * const o) const {
    return id() < o->id();
  }
  int KeyCompareWithValue(uint64_t _id) const {
    return static_cast<int>(id() > _id) - static_cast<int>(id() < _id);
  }
  const ::flatbuffers::Vector<uint64_t> *rows() const {
    return GetPointer<const ::flatbuffers::Vector<uint64_t> *>(VT_ROWS);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint64_t>(verifier, VT_ID, 8) &&
           VerifyOffset(verifier, VT_ROWS) &&
           verifier.VerifyVector(rows()) &&
           verifier.EndTable();
  }
};

struct FileBuilder {
  typedef File Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_id(uint64_t id) {
    fbb_.AddElement<uint64_t>(File::VT_ID, id, 0);
  }
  void add_rows(::flatbuffers::Offset<::flatbuffers::Vector<uint64_t>> rows) {
    fbb_.AddOffset(File::VT_ROWS, rows);
  }
  explicit FileBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<File> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<File>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<File> CreateFile(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    uint64_t id = 0,
    ::flatbuffers::Offset<::flatbuffers::Vector<uint64_t>> rows = 0) {
  FileBuilder builder_(_fbb);
  builder_.add_id(id);
  builder_.add_rows(rows);
  return builder_.Finish();
}

inline ::flatbuffers::Offset<File> CreateFileDirect(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    uint64_t id = 0,
    const std::vector<uint64_t> *rows = nullptr) {
  auto rows__ = rows ? _fbb.CreateVector<uint64_t>(*rows) : 0;
  return Index::CreateFile(
      _fbb,
      id,
      rows__);
}

struct Node FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef NodeBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_WORD = 4,
    VT_FILES = 6
  };
  const ::flatbuffers::String *word() const {
    return GetPointer<const ::flatbuffers::String *>(VT_WORD);
  }
  bool KeyCompareLessThan(const Node * const o) const {
    return *word() < *o->word();
  }
  int KeyCompareWithValue(const char *_word) const {
    return strcmp(word()->c_str(), _word);
  }
  template<typename StringType>
  int KeyCompareWithValue(const StringType& _word) const {
    if (word()->c_str() < _word) return -1;
    if (_word < word()->c_str()) return 1;
    return 0;
  }
  const ::flatbuffers::Vector<::flatbuffers::Offset<Index::File>> *files() const {
    return GetPointer<const ::flatbuffers::Vector<::flatbuffers::Offset<Index::File>> *>(VT_FILES);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffsetRequired(verifier, VT_WORD) &&
           verifier.VerifyString(word()) &&
           VerifyOffset(verifier, VT_FILES) &&
           verifier.VerifyVector(files()) &&
           verifier.VerifyVectorOfTables(files()) &&
           verifier.EndTable();
  }
};

struct NodeBuilder {
  typedef Node Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_word(::flatbuffers::Offset<::flatbuffers::String> word) {
    fbb_.AddOffset(Node::VT_WORD, word);
  }
  void add_files(::flatbuffers::Offset<::flatbuffers::Vector<::flatbuffers::Offset<Index::File>>> files) {
    fbb_.AddOffset(Node::VT_FILES, files);
  }
  explicit NodeBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<Node> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<Node>(end);
    fbb_.Required(o, Node::VT_WORD);
    return o;
  }
};

inline ::flatbuffers::Offset<Node> CreateNode(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    ::flatbuffers::Offset<::flatbuffers::String> word = 0,
    ::flatbuffers::Offset<::flatbuffers::Vector<::flatbuffers::Offset<Index::File>>> files = 0) {
  NodeBuilder builder_(_fbb);
  builder_.add_files(files);
  builder_.add_word(word);
  return builder_.Finish();
}

inline ::flatbuffers::Offset<Node> CreateNodeDirect(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    const char *word = nullptr,
    std::vector<::flatbuffers::Offset<Index::File>> *files = nullptr) {
  auto word__ = word ? _fbb.CreateString(word) : 0;
  auto files__ = files ? _fbb.CreateVectorOfSortedTables<Index::File>(files) : 0;
  return Index::CreateNode(
      _fbb,
      word__,
      files__);
}

struct FilesIndex FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef FilesIndexBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_WORDS = 4
  };
  const ::flatbuffers::Vector<::flatbuffers::Offset<Index::Node>> *words() const {
    return GetPointer<const ::flatbuffers::Vector<::flatbuffers::Offset<Index::Node>> *>(VT_WORDS);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_WORDS) &&
           verifier.VerifyVector(words()) &&
           verifier.VerifyVectorOfTables(words()) &&
           verifier.EndTable();
  }
};

struct FilesIndexBuilder {
  typedef FilesIndex Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_words(::flatbuffers::Offset<::flatbuffers::Vector<::flatbuffers::Offset<Index::Node>>> words) {
    fbb_.AddOffset(FilesIndex::VT_WORDS, words);
  }
  explicit FilesIndexBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<FilesIndex> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<FilesIndex>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<FilesIndex> CreateFilesIndex(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    ::flatbuffers::Offset<::flatbuffers::Vector<::flatbuffers::Offset<Index::Node>>> words = 0) {
  FilesIndexBuilder builder_(_fbb);
  builder_.add_words(words);
  return builder_.Finish();
}

inline ::flatbuffers::Offset<FilesIndex> CreateFilesIndexDirect(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    std::vector<::flatbuffers::Offset<Index::Node>> *words = nullptr) {
  auto words__ = words ? _fbb.CreateVectorOfSortedTables<Index::Node>(words) : 0;
  return Index::CreateFilesIndex(
      _fbb,
      words__);
}

inline const Index::FilesIndex *GetFilesIndex(const void *buf) {
  return ::flatbuffers::GetRoot<Index::FilesIndex>(buf);
}

inline const Index::FilesIndex *GetSizePrefixedFilesIndex(const void *buf) {
  return ::flatbuffers::GetSizePrefixedRoot<Index::FilesIndex>(buf);
}

inline bool VerifyFilesIndexBuffer(
    ::flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<Index::FilesIndex>(nullptr);
}

inline bool VerifySizePrefixedFilesIndexBuffer(
    ::flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<Index::FilesIndex>(nullptr);
}

inline void FinishFilesIndexBuffer(
    ::flatbuffers::FlatBufferBuilder &fbb,
    ::flatbuffers::Offset<Index::FilesIndex> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixedFilesIndexBuffer(
    ::flatbuffers::FlatBufferBuilder &fbb,
    ::flatbuffers::Offset<Index::FilesIndex> root) {
  fbb.FinishSizePrefixed(root);
}

}  // namespace Index

#endif  // FLATBUFFERS_GENERATED_INDEX_INDEX_H_