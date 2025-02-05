// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.

#ifndef NANOARROW_BUILD_ID_H_INCLUDED
#define NANOARROW_BUILD_ID_H_INCLUDED

// #define NANOARROW_NAMESPACE YourNamespaceHere

#define NANOARROW_BUILD_ID "gha7325d629245c290bd96fb645a5d38e72bba2f8af"

#endif
// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.

#ifndef NANOARROW_NANOARROW_TYPES_H_INCLUDED
#define NANOARROW_NANOARROW_TYPES_H_INCLUDED

#include <stdint.h>
#include <string.h>



#ifdef __cplusplus
extern "C" {
#endif

/// \defgroup nanoarrow-inline-typedef Type definitions used in inlined implementations

// Extra guard for versions of Arrow without the canonical guard
#ifndef ARROW_FLAG_DICTIONARY_ORDERED

#ifndef ARROW_C_DATA_INTERFACE
#define ARROW_C_DATA_INTERFACE

#define ARROW_FLAG_DICTIONARY_ORDERED 1
#define ARROW_FLAG_NULLABLE 2
#define ARROW_FLAG_MAP_KEYS_SORTED 4

struct ArrowSchema {
  // Array type description
  const char* format;
  const char* name;
  const char* metadata;
  int64_t flags;
  int64_t n_children;
  struct ArrowSchema** children;
  struct ArrowSchema* dictionary;

  // Release callback
  void (*release)(struct ArrowSchema*);
  // Opaque producer-specific data
  void* private_data;
};

struct ArrowArray {
  // Array data description
  int64_t length;
  int64_t null_count;
  int64_t offset;
  int64_t n_buffers;
  int64_t n_children;
  const void** buffers;
  struct ArrowArray** children;
  struct ArrowArray* dictionary;

  // Release callback
  void (*release)(struct ArrowArray*);
  // Opaque producer-specific data
  void* private_data;
};

#endif  // ARROW_C_DATA_INTERFACE

#ifndef ARROW_C_STREAM_INTERFACE
#define ARROW_C_STREAM_INTERFACE

struct ArrowArrayStream {
  // Callback to get the stream type
  // (will be the same for all arrays in the stream).
  //
  // Return value: 0 if successful, an `errno`-compatible error code otherwise.
  //
  // If successful, the ArrowSchema must be released independently from the stream.
  int (*get_schema)(struct ArrowArrayStream*, struct ArrowSchema* out);

  // Callback to get the next array
  // (if no error and the array is released, the stream has ended)
  //
  // Return value: 0 if successful, an `errno`-compatible error code otherwise.
  //
  // If successful, the ArrowArray must be released independently from the stream.
  int (*get_next)(struct ArrowArrayStream*, struct ArrowArray* out);

  // Callback to get optional detailed error information.
  // This must only be called if the last stream operation failed
  // with a non-0 return code.
  //
  // Return value: pointer to a null-terminated character array describing
  // the last error, or NULL if no description is available.
  //
  // The returned pointer is only valid until the next operation on this stream
  // (including release).
  const char* (*get_last_error)(struct ArrowArrayStream*);

  // Release callback: release the stream's own resources.
  // Note that arrays returned by `get_next` must be individually released.
  void (*release)(struct ArrowArrayStream*);

  // Opaque producer-specific data
  void* private_data;
};

#endif  // ARROW_C_STREAM_INTERFACE
#endif  // ARROW_FLAG_DICTIONARY_ORDERED

/// \brief Return code for success.
#define NANOARROW_OK 0

/// \brief Represents an errno-compatible error code
typedef int ArrowErrorCode;

/// \brief Arrow type enumerator
///
/// These names are intended to map to the corresponding arrow::Type::type
/// enumerator; however, the numeric values are specifically not equal
/// (i.e., do not rely on numeric comparison).
enum ArrowType {
  NANOARROW_TYPE_UNINITIALIZED = 0,
  NANOARROW_TYPE_NA = 1,
  NANOARROW_TYPE_BOOL,
  NANOARROW_TYPE_UINT8,
  NANOARROW_TYPE_INT8,
  NANOARROW_TYPE_UINT16,
  NANOARROW_TYPE_INT16,
  NANOARROW_TYPE_UINT32,
  NANOARROW_TYPE_INT32,
  NANOARROW_TYPE_UINT64,
  NANOARROW_TYPE_INT64,
  NANOARROW_TYPE_HALF_FLOAT,
  NANOARROW_TYPE_FLOAT,
  NANOARROW_TYPE_DOUBLE,
  NANOARROW_TYPE_STRING,
  NANOARROW_TYPE_BINARY,
  NANOARROW_TYPE_FIXED_SIZE_BINARY,
  NANOARROW_TYPE_DATE32,
  NANOARROW_TYPE_DATE64,
  NANOARROW_TYPE_TIMESTAMP,
  NANOARROW_TYPE_TIME32,
  NANOARROW_TYPE_TIME64,
  NANOARROW_TYPE_INTERVAL_MONTHS,
  NANOARROW_TYPE_INTERVAL_DAY_TIME,
  NANOARROW_TYPE_DECIMAL128,
  NANOARROW_TYPE_DECIMAL256,
  NANOARROW_TYPE_LIST,
  NANOARROW_TYPE_STRUCT,
  NANOARROW_TYPE_SPARSE_UNION,
  NANOARROW_TYPE_DENSE_UNION,
  NANOARROW_TYPE_DICTIONARY,
  NANOARROW_TYPE_MAP,
  NANOARROW_TYPE_EXTENSION,
  NANOARROW_TYPE_FIXED_SIZE_LIST,
  NANOARROW_TYPE_DURATION,
  NANOARROW_TYPE_LARGE_STRING,
  NANOARROW_TYPE_LARGE_BINARY,
  NANOARROW_TYPE_LARGE_LIST,
  NANOARROW_TYPE_INTERVAL_MONTH_DAY_NANO
};

/// \brief Functional types of buffers as described in the Arrow Columnar Specification
enum ArrowBufferType {
  NANOARROW_BUFFER_TYPE_NONE,
  NANOARROW_BUFFER_TYPE_VALIDITY,
  NANOARROW_BUFFER_TYPE_TYPE_ID,
  NANOARROW_BUFFER_TYPE_UNION_OFFSET,
  NANOARROW_BUFFER_TYPE_DATA_OFFSET,
  NANOARROW_BUFFER_TYPE_DATA
};

#define _NANOARROW_CONCAT(x, y) x##y
#define _NANOARROW_MAKE_NAME(x, y) _NANOARROW_CONCAT(x, y)

#define _NANOARROW_RETURN_NOT_OK_IMPL(NAME, EXPR) \
  do {                                            \
    const int NAME = (EXPR);                      \
    if (NAME) return NAME;                        \
  } while (0)

#define NANOARROW_RETURN_NOT_OK(EXPR) \
  _NANOARROW_RETURN_NOT_OK_IMPL(_NANOARROW_MAKE_NAME(errno_status_, __COUNTER__), EXPR)

#define _NANOARROW_CHECK_RANGE(x_, min_, max_) \
  NANOARROW_RETURN_NOT_OK((x_ >= min_ && x_ <= max_) ? NANOARROW_OK : EINVAL)

/// \brief A description of an arrangement of buffers
///
/// Contains the minimum amount of information required to
/// calculate the size of each buffer in an ArrowArray knowing only
/// the length and offset of the array.
struct ArrowLayout {
  /// \brief The function of each buffer
  enum ArrowBufferType buffer_type[3];

  /// \brief The size of an element each buffer or 0 if this size is variable or unknown
  int64_t element_size_bits[3];

  /// \brief The number of elements in the child array per element in this array for a
  /// fixed-size list
  int64_t child_size_elements;
};

/// \brief An non-owning view of a string
struct ArrowStringView {
  /// \brief A pointer to the start of the string
  ///
  /// If n_bytes is 0, this value may be NULL.
  const char* data;

  /// \brief The size of the string in bytes,
  ///
  /// (Not including the null terminator.)
  int64_t n_bytes;
};

static inline struct ArrowStringView ArrowCharView(const char* value) {
  struct ArrowStringView out;

  out.data = value;
  if (value) {
    out.n_bytes = (int64_t)strlen(value);
  } else {
    out.n_bytes = 0;
  }

  return out;
}

/// \brief An non-owning view of a buffer
struct ArrowBufferView {
  /// \brief A pointer to the start of the buffer
  ///
  /// If n_bytes is 0, this value may be NULL.
  union {
    const void* data;
    const int8_t* as_int8;
    const uint8_t* as_uint8;
    const int16_t* as_int16;
    const uint16_t* as_uint16;
    const int32_t* as_int32;
    const uint32_t* as_uint32;
    const int64_t* as_int64;
    const uint64_t* as_uint64;
    const double* as_double;
    const float* as_float;
    const char* as_char;
  } data;

  /// \brief The size of the buffer in bytes
  int64_t n_bytes;
};

/// \brief Array buffer allocation and deallocation
///
/// Container for allocate, reallocate, and free methods that can be used
/// to customize allocation and deallocation of buffers when constructing
/// an ArrowArray.
struct ArrowBufferAllocator {
  /// \brief Reallocate a buffer or return NULL if it cannot be reallocated
  uint8_t* (*reallocate)(struct ArrowBufferAllocator* allocator, uint8_t* ptr,
                         int64_t old_size, int64_t new_size);

  /// \brief Deallocate a buffer allocated by this allocator
  void (*free)(struct ArrowBufferAllocator* allocator, uint8_t* ptr, int64_t size);

  /// \brief Opaque data specific to the allocator
  void* private_data;
};

/// \brief An owning mutable view of a buffer
struct ArrowBuffer {
  /// \brief A pointer to the start of the buffer
  ///
  /// If capacity_bytes is 0, this value may be NULL.
  uint8_t* data;

  /// \brief The size of the buffer in bytes
  int64_t size_bytes;

  /// \brief The capacity of the buffer in bytes
  int64_t capacity_bytes;

  /// \brief The allocator that will be used to reallocate and/or free the buffer
  struct ArrowBufferAllocator allocator;
};

/// \brief An owning mutable view of a bitmap
struct ArrowBitmap {
  /// \brief An ArrowBuffer to hold the allocated memory
  struct ArrowBuffer buffer;

  /// \brief The number of bits that have been appended to the bitmap
  int64_t size_bits;
};

// Used as the private data member for ArrowArrays allocated here and accessed
// internally within inline ArrowArray* helpers.
struct ArrowArrayPrivateData {
  // Holder for the validity buffer (or first buffer for union types, which are
  // the only type whose first buffer is not a valdiity buffer)
  struct ArrowBitmap bitmap;

  // Holder for additional buffers as required
  struct ArrowBuffer buffers[2];

  // The array of pointers to buffers. This must be updated after a sequence
  // of appends to synchronize its values with the actual buffer addresses
  // (which may have ben reallocated uring that time)
  const void* buffer_data[3];

  // The storage data type, or NANOARROW_TYPE_UNINITIALIZED if unknown
  enum ArrowType storage_type;

  // The buffer arrangement for the storage type
  struct ArrowLayout layout;
};

struct ArrowArrayView {
  struct ArrowArray* array;
  enum ArrowType storage_type;
  struct ArrowLayout layout;
  struct ArrowBufferView buffer_views[3];
  int64_t n_children;
  struct ArrowArrayView** children;
};

/// }@

#ifdef __cplusplus
}
#endif

#endif
// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.

#ifndef NANOARROW_H_INCLUDED
#define NANOARROW_H_INCLUDED

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>



// If using CMake, optionally pass -DNANOARROW_NAMESPACE=MyNamespace which will set this
// define in build_id.h. If not, you can optionally #define NANOARROW_NAMESPACE
// MyNamespace here.

// This section remaps the non-prefixed symbols to the prefixed symbols so that
// code written against this build can be used independent of the value of
// NANOARROW_NAMESPACE.
#ifdef NANOARROW_NAMESPACE
#define NANOARROW_CAT(A, B) A##B
#define NANOARROW_SYMBOL(A, B) NANOARROW_CAT(A, B)

#define ArrowNanoarrowBuildId NANOARROW_SYMBOL(NANOARROW_NAMESPACE, ArrowNanoarrowBuildId)
#define ArrowErrorMessage NANOARROW_SYMBOL(NANOARROW_NAMESPACE, ArrowErrorMessage)
#define ArrowMalloc NANOARROW_SYMBOL(NANOARROW_NAMESPACE, ArrowMalloc)
#define ArrowRealloc NANOARROW_SYMBOL(NANOARROW_NAMESPACE, ArrowRealloc)
#define ArrowFree NANOARROW_SYMBOL(NANOARROW_NAMESPACE, ArrowFree)
#define ArrowBufferAllocatorDefault \
  NANOARROW_SYMBOL(NANOARROW_NAMESPACE, ArrowBufferAllocatorDefault)
#define ArrowBufferDeallocator \
  NANOARROW_SYMBOL(NANOARROW_NAMESPACE, ArrowBufferDeallocator)
#define ArrowErrorSet NANOARROW_SYMBOL(NANOARROW_NAMESPACE, ArrowErrorSet)
#define ArrowLayoutInit NANOARROW_SYMBOL(NANOARROW_NAMESPACE, ArrowLayoutInit)
#define ArrowSchemaInit NANOARROW_SYMBOL(NANOARROW_NAMESPACE, ArrowSchemaInit)
#define ArrowSchemaInitFixedSize \
  NANOARROW_SYMBOL(NANOARROW_NAMESPACE, ArrowSchemaInitFixedSize)
#define ArrowSchemaInitDecimal \
  NANOARROW_SYMBOL(NANOARROW_NAMESPACE, ArrowSchemaInitDecimal)
#define ArrowSchemaInitDateTime \
  NANOARROW_SYMBOL(NANOARROW_NAMESPACE, ArrowSchemaInitDateTime)
#define ArrowSchemaDeepCopy NANOARROW_SYMBOL(NANOARROW_NAMESPACE, ArrowSchemaDeepCopy)
#define ArrowSchemaSetFormat NANOARROW_SYMBOL(NANOARROW_NAMESPACE, ArrowSchemaSetFormat)
#define ArrowSchemaSetName NANOARROW_SYMBOL(NANOARROW_NAMESPACE, ArrowSchemaSetName)
#define ArrowSchemaSetMetadata \
  NANOARROW_SYMBOL(NANOARROW_NAMESPACE, ArrowSchemaSetMetadata)
#define ArrowSchemaAllocateChildren \
  NANOARROW_SYMBOL(NANOARROW_NAMESPACE, ArrowSchemaAllocateChildren)
#define ArrowSchemaAllocateDictionary \
  NANOARROW_SYMBOL(NANOARROW_NAMESPACE, ArrowSchemaAllocateDictionary)
#define ArrowMetadataReaderInit \
  NANOARROW_SYMBOL(NANOARROW_NAMESPACE, ArrowMetadataReaderInit)
#define ArrowMetadataReaderRead \
  NANOARROW_SYMBOL(NANOARROW_NAMESPACE, ArrowMetadataReaderRead)
#define ArrowMetadataSizeOf NANOARROW_SYMBOL(NANOARROW_NAMESPACE, ArrowMetadataSizeOf)
#define ArrowMetadataHasKey NANOARROW_SYMBOL(NANOARROW_NAMESPACE, ArrowMetadataHasKey)
#define ArrowMetadataGetValue NANOARROW_SYMBOL(NANOARROW_NAMESPACE, ArrowMetadataGetValue)
#define ArrowMetadataBuilderInit \
  NANOARROW_SYMBOL(NANOARROW_NAMESPACE, ArrowMetadataBuilderInit)
#define ArrowMetadataBuilderAppend \
  NANOARROW_SYMBOL(NANOARROW_NAMESPACE, ArrowMetadataBuilderAppend)
#define ArrowMetadataBuilderSet \
  NANOARROW_SYMBOL(NANOARROW_NAMESPACE, ArrowMetadataBuilderSet)
#define ArrowMetadataBuilderRemove \
  NANOARROW_SYMBOL(NANOARROW_NAMESPACE, ArrowMetadataBuilderRemove)
#define ArrowSchemaViewInit NANOARROW_SYMBOL(NANOARROW_NAMESPACE, ArrowSchemaViewInit)
#define ArrowArrayInit NANOARROW_SYMBOL(NANOARROW_NAMESPACE, ArrowArrayInit)
#define ArrowArrayInitFromSchema \
  NANOARROW_SYMBOL(NANOARROW_NAMESPACE, ArrowArrayInitFromSchema)
#define ArrowArrayAllocateChildren \
  NANOARROW_SYMBOL(NANOARROW_NAMESPACE, ArrowArrayAllocateChildren)
#define ArrowArrayAllocateDictionary \
  NANOARROW_SYMBOL(NANOARROW_NAMESPACE, ArrowArrayAllocateDictionary)
#define ArrowArraySetValidityBitmap \
  NANOARROW_SYMBOL(NANOARROW_NAMESPACE, ArrowArraySetValidityBitmap)
#define ArrowArraySetBuffer NANOARROW_SYMBOL(NANOARROW_NAMESPACE, ArrowArraySetBuffer)
#define ArrowArrayReserve NANOARROW_SYMBOL(NANOARROW_NAMESPACE, ArrowArrayReserve)
#define ArrowArrayFinishBuilding \
  NANOARROW_SYMBOL(NANOARROW_NAMESPACE, ArrowArrayFinishBuilding)
#define ArrowArrayViewInit NANOARROW_SYMBOL(NANOARROW_NAMESPACE, ArrowArrayViewInit)
#define ArrowArrayViewInitFromSchema \
  NANOARROW_SYMBOL(NANOARROW_NAMESPACE, ArrowArrayViewInitFromSchema)
#define ArrowArrayViewAllocateChildren \
  NANOARROW_SYMBOL(NANOARROW_NAMESPACE, ArrowArrayViewAllocateChildren)
#define ArrowArrayViewSetLength \
  NANOARROW_SYMBOL(NANOARROW_NAMESPACE, ArrowArrayViewSetLength)
#define ArrowArrayViewSetArray \
  NANOARROW_SYMBOL(NANOARROW_NAMESPACE, ArrowArrayViewSetArray)
#define ArrowArrayViewReset NANOARROW_SYMBOL(NANOARROW_NAMESPACE, ArrowArrayViewReset)

#endif

#ifdef __cplusplus
extern "C" {
#endif

/// \file Arrow C Implementation
///
/// EXPERIMENTAL. Interface subject to change.

/// \page object-model Object Model
///
/// Except where noted, objects are not thread-safe and clients should
/// take care to serialize accesses to methods.
///
/// Because this library is intended to be vendored, it provides full type
/// definitions and encourages clients to stack or statically allocate
/// where convenient.

/// \defgroup nanoarrow-malloc Memory management
///
/// Non-buffer members of a struct ArrowSchema and struct ArrowArray
/// must be allocated using ArrowMalloc() or ArrowRealloc() and freed
/// using ArrowFree for schemas and arrays allocated here. Buffer members
/// are allocated using an ArrowBufferAllocator.

/// \brief Allocate like malloc()
void* ArrowMalloc(int64_t size);

/// \brief Reallocate like realloc()
void* ArrowRealloc(void* ptr, int64_t size);

/// \brief Free a pointer allocated using ArrowMalloc() or ArrowRealloc().
void ArrowFree(void* ptr);

/// \brief Return the default allocator
///
/// The default allocator uses ArrowMalloc(), ArrowRealloc(), and
/// ArrowFree().
struct ArrowBufferAllocator ArrowBufferAllocatorDefault();

/// \brief Create a custom deallocator
///
/// Creates a buffer allocator with only a free method that can be used to
/// attach a custom deallocator to an ArrowBuffer. This may be used to
/// avoid copying an existing buffer that was not allocated using the
/// infrastructure provided here (e.g., by an R or Python object).
struct ArrowBufferAllocator ArrowBufferDeallocator(
    void (*custom_free)(struct ArrowBufferAllocator* allocator, uint8_t* ptr,
                        int64_t size),
    void* private_data);

/// }@

/// \defgroup nanoarrow-errors Error handling primitives
/// Functions generally return an errno-compatible error code; functions that
/// need to communicate more verbose error information accept a pointer
/// to an ArrowError. This can be stack or statically allocated. The
/// content of the message is undefined unless an error code has been
/// returned.

/// \brief Error type containing a UTF-8 encoded message.
struct ArrowError {
  char message[1024];
};

/// \brief Set the contents of an error using printf syntax
ArrowErrorCode ArrowErrorSet(struct ArrowError* error, const char* fmt, ...);

/// \brief Get the contents of an error
const char* ArrowErrorMessage(struct ArrowError* error);

/// }@

/// \defgroup nanoarrow-utils Utility data structures

/// \brief Return the build id against which the library was compiled
const char* ArrowNanoarrowBuildId();

/// \brief Initialize a description of buffer arrangements from a storage type
void ArrowLayoutInit(struct ArrowLayout* layout, enum ArrowType storage_type);

/// \brief Create a string view from a null-terminated string
static inline struct ArrowStringView ArrowCharView(const char* value);

/// \brief Arrow time unit enumerator
///
/// These names and values map to the corresponding arrow::TimeUnit::type
/// enumerator.
enum ArrowTimeUnit {
  NANOARROW_TIME_UNIT_SECOND = 0,
  NANOARROW_TIME_UNIT_MILLI = 1,
  NANOARROW_TIME_UNIT_MICRO = 2,
  NANOARROW_TIME_UNIT_NANO = 3
};

/// }@

/// \defgroup nanoarrow-schema Schema producer helpers
/// These functions allocate, copy, and destroy ArrowSchema structures

/// \brief Initialize the fields of a schema
///
/// Initializes the fields and release callback of schema_out. Caller
/// is responsible for calling the schema->release callback if
/// NANOARROW_OK is returned.
ArrowErrorCode ArrowSchemaInit(struct ArrowSchema* schema, enum ArrowType type);

/// \brief Initialize the fields of a fixed-size schema
///
/// Returns EINVAL for fixed_size <= 0 or for data_type that is not
/// NANOARROW_TYPE_FIXED_SIZE_BINARY or NANOARROW_TYPE_FIXED_SIZE_LIST.
ArrowErrorCode ArrowSchemaInitFixedSize(struct ArrowSchema* schema,
                                        enum ArrowType data_type, int32_t fixed_size);

/// \brief Initialize the fields of a decimal schema
///
/// Returns EINVAL for scale <= 0 or for data_type that is not
/// NANOARROW_TYPE_DECIMAL128 or NANOARROW_TYPE_DECIMAL256.
ArrowErrorCode ArrowSchemaInitDecimal(struct ArrowSchema* schema,
                                      enum ArrowType data_type, int32_t decimal_precision,
                                      int32_t decimal_scale);

/// \brief Initialize the fields of a time, timestamp, or duration schema
///
/// Returns EINVAL for data_type that is not
/// NANOARROW_TYPE_TIME32, NANOARROW_TYPE_TIME64,
/// NANOARROW_TYPE_TIMESTAMP, or NANOARROW_TYPE_DURATION. The
/// timezone parameter must be NULL for a non-timestamp data_type.
ArrowErrorCode ArrowSchemaInitDateTime(struct ArrowSchema* schema,
                                       enum ArrowType data_type,
                                       enum ArrowTimeUnit time_unit,
                                       const char* timezone);

/// \brief Make a (recursive) copy of a schema
///
/// Allocates and copies fields of schema into schema_out.
ArrowErrorCode ArrowSchemaDeepCopy(struct ArrowSchema* schema,
                                   struct ArrowSchema* schema_out);

/// \brief Copy format into schema->format
///
/// schema must have been allocated using ArrowSchemaInit or
/// ArrowSchemaDeepCopy.
ArrowErrorCode ArrowSchemaSetFormat(struct ArrowSchema* schema, const char* format);

/// \brief Copy name into schema->name
///
/// schema must have been allocated using ArrowSchemaInit or
/// ArrowSchemaDeepCopy.
ArrowErrorCode ArrowSchemaSetName(struct ArrowSchema* schema, const char* name);

/// \brief Copy metadata into schema->metadata
///
/// schema must have been allocated using ArrowSchemaInit or
/// ArrowSchemaDeepCopy.
ArrowErrorCode ArrowSchemaSetMetadata(struct ArrowSchema* schema, const char* metadata);

/// \brief Allocate the schema->children array
///
/// Includes the memory for each child struct ArrowSchema.
/// schema must have been allocated using ArrowSchemaInit or
/// ArrowSchemaDeepCopy.
ArrowErrorCode ArrowSchemaAllocateChildren(struct ArrowSchema* schema,
                                           int64_t n_children);

/// \brief Allocate the schema->dictionary member
///
/// schema must have been allocated using ArrowSchemaInit or
/// ArrowSchemaDeepCopy.
ArrowErrorCode ArrowSchemaAllocateDictionary(struct ArrowSchema* schema);

/// \brief Reader for key/value pairs in schema metadata
struct ArrowMetadataReader {
  const char* metadata;
  int64_t offset;
  int32_t remaining_keys;
};

/// \brief Initialize an ArrowMetadataReader
ArrowErrorCode ArrowMetadataReaderInit(struct ArrowMetadataReader* reader,
                                       const char* metadata);

/// \brief Read the next key/value pair from an ArrowMetadataReader
ArrowErrorCode ArrowMetadataReaderRead(struct ArrowMetadataReader* reader,
                                       struct ArrowStringView* key_out,
                                       struct ArrowStringView* value_out);

/// \brief The number of bytes in in a key/value metadata string
int64_t ArrowMetadataSizeOf(const char* metadata);

/// \brief Check for a key in schema metadata
char ArrowMetadataHasKey(const char* metadata, struct ArrowStringView key);

/// \brief Extract a value from schema metadata
///
/// If key does not exist in metadata, value_out is unmodified
ArrowErrorCode ArrowMetadataGetValue(const char* metadata, struct ArrowStringView key,
                                     struct ArrowStringView* value_out);

/// \brief Initialize a builder for schema metadata from key/value pairs
///
/// metadata can be an existing metadata string or NULL to initialize
/// an empty metadata string.
ArrowErrorCode ArrowMetadataBuilderInit(struct ArrowBuffer* buffer, const char* metadata);

/// \brief Append a key/value pair to a buffer containing serialized metadata
ArrowErrorCode ArrowMetadataBuilderAppend(struct ArrowBuffer* buffer,
                                          struct ArrowStringView key,
                                          struct ArrowStringView value);

/// \brief Set a key/value pair to a buffer containing serialized metadata
///
/// Ensures that the only entry for key in the metadata is set to value.
/// This function maintains the existing position of (the first instance of)
/// key if present in the data.
ArrowErrorCode ArrowMetadataBuilderSet(struct ArrowBuffer* buffer,
                                       struct ArrowStringView key,
                                       struct ArrowStringView value);

/// \brief Remove a key from a buffer containing serialized metadata
ArrowErrorCode ArrowMetadataBuilderRemove(struct ArrowBuffer* buffer,
                                          struct ArrowStringView key);

/// }@

/// \defgroup nanoarrow-schema-view Schema consumer helpers

/// \brief A non-owning view of a parsed ArrowSchema
///
/// Contains more readily extractable values than a raw ArrowSchema.
/// Clients can stack or statically allocate this structure but are
/// encouraged to use the provided getters to ensure forward
/// compatiblity.
struct ArrowSchemaView {
  /// \brief A pointer to the schema represented by this view
  struct ArrowSchema* schema;

  /// \brief The data type represented by the schema
  ///
  /// This value may be NANOARROW_TYPE_DICTIONARY if the schema has a
  /// non-null dictionary member; datetime types are valid values.
  /// This value will never be NANOARROW_TYPE_EXTENSION (see
  /// extension_name and/or extension_metadata to check for
  /// an extension type).
  enum ArrowType data_type;

  /// \brief The storage data type represented by the schema
  ///
  /// This value will never be NANOARROW_TYPE_DICTIONARY, NANOARROW_TYPE_EXTENSION
  /// or any datetime type. This value represents only the type required to
  /// interpret the buffers in the array.
  enum ArrowType storage_data_type;

  /// \brief The storage layout represented by the schema
  struct ArrowLayout layout;

  /// \brief The extension type name if it exists
  ///
  /// If the ARROW:extension:name key is present in schema.metadata,
  /// extension_name.data will be non-NULL.
  struct ArrowStringView extension_name;

  /// \brief The extension type metadata if it exists
  ///
  /// If the ARROW:extension:metadata key is present in schema.metadata,
  /// extension_metadata.data will be non-NULL.
  struct ArrowStringView extension_metadata;

  /// \brief Format fixed size parameter
  ///
  /// This value is set when parsing a fixed-size binary or fixed-size
  /// list schema; this value is undefined for other types. For a
  /// fixed-size binary schema this value is in bytes; for a fixed-size
  /// list schema this value refers to the number of child elements for
  /// each element of the parent.
  int32_t fixed_size;

  /// \brief Decimal bitwidth
  ///
  /// This value is set when parsing a decimal type schema;
  /// this value is undefined for other types.
  int32_t decimal_bitwidth;

  /// \brief Decimal precision
  ///
  /// This value is set when parsing a decimal type schema;
  /// this value is undefined for other types.
  int32_t decimal_precision;

  /// \brief Decimal scale
  ///
  /// This value is set when parsing a decimal type schema;
  /// this value is undefined for other types.
  int32_t decimal_scale;

  /// \brief Format time unit parameter
  ///
  /// This value is set when parsing a date/time type. The value is
  /// undefined for other types.
  enum ArrowTimeUnit time_unit;

  /// \brief Format timezone parameter
  ///
  /// This value is set when parsing a timestamp type and represents
  /// the timezone format parameter. The ArrowStrintgView points to
  /// data within the schema and the value is undefined for other types.
  struct ArrowStringView timezone;

  /// \brief Union type ids parameter
  ///
  /// This value is set when parsing a union type and represents
  /// type ids parameter. The ArrowStringView points to
  /// data within the schema and the value is undefined for other types.
  struct ArrowStringView union_type_ids;
};

/// \brief Initialize an ArrowSchemaView
ArrowErrorCode ArrowSchemaViewInit(struct ArrowSchemaView* schema_view,
                                   struct ArrowSchema* schema, struct ArrowError* error);

/// }@

/// \defgroup nanoarrow-buffer Owning, growable buffers

/// \brief Initialize an ArrowBuffer
///
/// Initialize a buffer with a NULL, zero-size buffer using the default
/// buffer allocator.
static inline void ArrowBufferInit(struct ArrowBuffer* buffer);

/// \brief Set a newly-initialized buffer's allocator
///
/// Returns EINVAL if the buffer has already been allocated.
static inline ArrowErrorCode ArrowBufferSetAllocator(
    struct ArrowBuffer* buffer, struct ArrowBufferAllocator allocator);

/// \brief Reset an ArrowBuffer
///
/// Releases the buffer using the allocator's free method if
/// the buffer's data member is non-null, sets the data member
/// to NULL, and sets the buffer's size and capacity to 0.
static inline void ArrowBufferReset(struct ArrowBuffer* buffer);

/// \brief Move an ArrowBuffer
///
/// Transfers the buffer data and lifecycle management to another
/// address and resets buffer.
static inline void ArrowBufferMove(struct ArrowBuffer* buffer,
                                   struct ArrowBuffer* buffer_out);

/// \brief Grow or shrink a buffer to a given capacity
///
/// When shrinking the capacity of the buffer, the buffer is only reallocated
/// if shrink_to_fit is non-zero. Calling ArrowBufferResize() does not
/// adjust the buffer's size member except to ensure that the invariant
/// capacity >= size remains true.
static inline ArrowErrorCode ArrowBufferResize(struct ArrowBuffer* buffer,
                                               int64_t new_capacity_bytes,
                                               char shrink_to_fit);

/// \brief Ensure a buffer has at least a given additional capacity
///
/// Ensures that the buffer has space to append at least
/// additional_size_bytes, overallocating when required.
static inline ArrowErrorCode ArrowBufferReserve(struct ArrowBuffer* buffer,
                                                int64_t additional_size_bytes);

/// \brief Write data to buffer and increment the buffer size
///
/// This function does not check that buffer has the required capacity
static inline void ArrowBufferAppendUnsafe(struct ArrowBuffer* buffer, const void* data,
                                           int64_t size_bytes);

/// \brief Write data to buffer and increment the buffer size
///
/// This function writes and ensures that the buffer has the required capacity,
/// possibly by reallocating the buffer. Like ArrowBufferReserve, this will
/// overallocate when reallocation is required.
static inline ArrowErrorCode ArrowBufferAppend(struct ArrowBuffer* buffer,
                                               const void* data, int64_t size_bytes);

/// \brief Write fill to buffer and increment the buffer size
///
/// This function writes the specified number of fill bytes and
/// ensures that the buffer has the required capacity,
static inline ArrowErrorCode ArrowBufferAppendFill(struct ArrowBuffer* buffer,
                                                   uint8_t value, int64_t size_bytes);

/// \brief Write an 8-bit integer to a buffer
static inline ArrowErrorCode ArrowBufferAppendInt8(struct ArrowBuffer* buffer,
                                                   int8_t value);

/// \brief Write an unsigned 8-bit integer to a buffer
static inline ArrowErrorCode ArrowBufferAppendUInt8(struct ArrowBuffer* buffer,
                                                    uint8_t value);

/// \brief Write a 16-bit integer to a buffer
static inline ArrowErrorCode ArrowBufferAppendInt16(struct ArrowBuffer* buffer,
                                                    int16_t value);

/// \brief Write an unsigned 16-bit integer to a buffer
static inline ArrowErrorCode ArrowBufferAppendUInt16(struct ArrowBuffer* buffer,
                                                     uint16_t value);

/// \brief Write a 32-bit integer to a buffer
static inline ArrowErrorCode ArrowBufferAppendInt32(struct ArrowBuffer* buffer,
                                                    int32_t value);

/// \brief Write an unsigned 32-bit integer to a buffer
static inline ArrowErrorCode ArrowBufferAppendUInt32(struct ArrowBuffer* buffer,
                                                     uint32_t value);

/// \brief Write a 64-bit integer to a buffer
static inline ArrowErrorCode ArrowBufferAppendInt64(struct ArrowBuffer* buffer,
                                                    int64_t value);

/// \brief Write an unsigned 64-bit integer to a buffer
static inline ArrowErrorCode ArrowBufferAppendUInt64(struct ArrowBuffer* buffer,
                                                     uint64_t value);

/// \brief Write a double to a buffer
static inline ArrowErrorCode ArrowBufferAppendDouble(struct ArrowBuffer* buffer,
                                                     double value);

/// \brief Write a float to a buffer
static inline ArrowErrorCode ArrowBufferAppendFloat(struct ArrowBuffer* buffer,
                                                    float value);

/// }@

/// \defgroup nanoarrow-bitmap Bitmap utilities

/// \brief Extract a boolean value from a bitmap
static inline int8_t ArrowBitGet(const uint8_t* bits, int64_t i);

/// \brief Set a boolean value to a bitmap to true
static inline void ArrowBitSet(uint8_t* bits, int64_t i);

/// \brief Set a boolean value to a bitmap to false
static inline void ArrowBitClear(uint8_t* bits, int64_t i);

/// \brief Set a boolean value to a bitmap
static inline void ArrowBitSetTo(uint8_t* bits, int64_t i, uint8_t value);

/// \brief Set a boolean value to a range in a bitmap
static inline void ArrowBitsSetTo(uint8_t* bits, int64_t start_offset, int64_t length,
                                  uint8_t bits_are_set);

/// \brief Count true values in a bitmap
static inline int64_t ArrowBitCountSet(const uint8_t* bits, int64_t i_from, int64_t i_to);

/// \brief Initialize an ArrowBitmap
///
/// Initialize the builder's buffer, empty its cache, and reset the size to zero
static inline void ArrowBitmapInit(struct ArrowBitmap* bitmap);

/// \brief Ensure a bitmap builder has at least a given additional capacity
///
/// Ensures that the buffer has space to append at least
/// additional_size_bits, overallocating when required.
static inline ArrowErrorCode ArrowBitmapReserve(struct ArrowBitmap* bitmap,
                                                int64_t additional_size_bits);

/// \brief Grow or shrink a bitmap to a given capacity
///
/// When shrinking the capacity of the bitmap, the bitmap is only reallocated
/// if shrink_to_fit is non-zero. Calling ArrowBitmapResize() does not
/// adjust the buffer's size member except when shrinking new_capacity_bits
/// to a value less than the current number of bits in the bitmap.
static inline ArrowErrorCode ArrowBitmapResize(struct ArrowBitmap* bitmap,
                                               int64_t new_capacity_bits,
                                               char shrink_to_fit);

/// \brief Reserve space for and append zero or more of the same boolean value to a bitmap
static inline ArrowErrorCode ArrowBitmapAppend(struct ArrowBitmap* bitmap,
                                               uint8_t bits_are_set, int64_t length);

/// \brief Append zero or more of the same boolean value to a bitmap
static inline void ArrowBitmapAppendUnsafe(struct ArrowBitmap* bitmap,
                                           uint8_t bits_are_set, int64_t length);

/// \brief Append boolean values encoded as int8_t to a bitmap
///
/// The values must all be 0 or 1.
static inline void ArrowBitmapAppendInt8Unsafe(struct ArrowBitmap* bitmap,
                                               const int8_t* values, int64_t n_values);

/// \brief Append boolean values encoded as int32_t to a bitmap
///
/// The values must all be 0 or 1.
static inline void ArrowBitmapAppendInt32Unsafe(struct ArrowBitmap* bitmap,
                                                const int32_t* values, int64_t n_values);

/// \brief Reset a bitmap builder
///
/// Releases any memory held by buffer, empties the cache, and resets the size to zero
static inline void ArrowBitmapReset(struct ArrowBitmap* bitmap);

/// }@

/// \defgroup nanoarrow-array Array producer helpers
/// These functions allocate, copy, and destroy ArrowArray structures

/// \brief Initialize the fields of an array
///
/// Initializes the fields and release callback of array. Caller
/// is responsible for calling the array->release callback if
/// NANOARROW_OK is returned.
ArrowErrorCode ArrowArrayInit(struct ArrowArray* array, enum ArrowType storage_type);

/// \brief Initialize the contents of an ArrowArray from an ArrowSchema
///
/// Caller is responsible for calling the array->release callback if
/// NANOARROW_OK is returned.
ArrowErrorCode ArrowArrayInitFromSchema(struct ArrowArray* array,
                                        struct ArrowSchema* schema,
                                        struct ArrowError* error);

/// \brief Allocate the array->children array
///
/// Includes the memory for each child struct ArrowArray,
/// whose members are marked as released and may be subsequently initialized
/// with ArrowArrayInit or moved from an existing ArrowArray.
/// schema must have been allocated using ArrowArrayInit.
ArrowErrorCode ArrowArrayAllocateChildren(struct ArrowArray* array, int64_t n_children);

/// \brief Allocate the array->dictionary member
///
/// Includes the memory for the struct ArrowArray, whose contents
/// is marked as released and may be subsequently initialized
/// with ArrowArrayInit or moved from an existing ArrowArray.
/// array must have been allocated using ArrowArrayInit
ArrowErrorCode ArrowArrayAllocateDictionary(struct ArrowArray* array);

/// \brief Set the validity bitmap of an ArrowArray
///
/// array must have been allocated using ArrowArrayInit
void ArrowArraySetValidityBitmap(struct ArrowArray* array, struct ArrowBitmap* bitmap);

/// \brief Set a buffer of an ArrowArray
///
/// array must have been allocated using ArrowArrayInit
ArrowErrorCode ArrowArraySetBuffer(struct ArrowArray* array, int64_t i,
                                   struct ArrowBuffer* buffer);

/// \brief Get the validity bitmap of an ArrowArray
///
/// array must have been allocated using ArrowArrayInit
static inline struct ArrowBitmap* ArrowArrayValidityBitmap(struct ArrowArray* array);

/// \brief Get a buffer of an ArrowArray
///
/// array must have been allocated using ArrowArrayInit
static inline struct ArrowBuffer* ArrowArrayBuffer(struct ArrowArray* array, int64_t i);

/// \brief Start element-wise appending to an ArrowArray
///
/// Initializes any values needed to use ArrowArrayAppend*() functions.
/// All element-wise appenders append by value and return EINVAL if the exact value
/// cannot be represented by the underlying storage type.
/// array must have been allocated using ArrowArrayInit
static inline ArrowErrorCode ArrowArrayStartAppending(struct ArrowArray* array);

/// \brief Reserve space for future appends
///
/// For buffer sizes that can be calculated (i.e., not string data buffers or
/// child array sizes for non-fixed-size arrays), recursively reserve space for
/// additional elements. This is useful for reducing the number of reallocations
/// that occur using the item-wise appenders.
ArrowErrorCode ArrowArrayReserve(struct ArrowArray* array,
                                 int64_t additional_size_elements);

/// \brief Append a null value to an array
static inline ArrowErrorCode ArrowArrayAppendNull(struct ArrowArray* array, int64_t n);

/// \brief Append a signed integer value to an array
///
/// Returns NANOARROW_OK if value can be exactly represented by
/// the underlying storage type or EINVAL otherwise (e.g., value
/// is outside the valid array range).
static inline ArrowErrorCode ArrowArrayAppendInt(struct ArrowArray* array, int64_t value);

/// \brief Append an unsigned integer value to an array
///
/// Returns NANOARROW_OK if value can be exactly represented by
/// the underlying storage type or EINVAL otherwise (e.g., value
/// is outside the valid array range).
static inline ArrowErrorCode ArrowArrayAppendUInt(struct ArrowArray* array,
                                                  uint64_t value);

/// \brief Append a double value to an array
///
/// Returns NANOARROW_OK if value can be exactly represented by
/// the underlying storage type or EINVAL otherwise (e.g., value
/// is outside the valid array range or there is an attempt to append
/// a non-integer to an array with an integer storage type).
static inline ArrowErrorCode ArrowArrayAppendDouble(struct ArrowArray* array,
                                                    double value);

/// \brief Append a string of bytes to an array
///
/// Returns NANOARROW_OK if value can be exactly represented by
/// the underlying storage type or EINVAL otherwise (e.g.,
/// the underlying array is not a binary, string, large binary, large string,
/// or fixed-size binary array, or value is the wrong size for a fixed-size
/// binary array).
static inline ArrowErrorCode ArrowArrayAppendBytes(struct ArrowArray* array,
                                                   struct ArrowBufferView value);

/// \brief Append a string value to an array
/// Returns NANOARROW_OK if value can be exactly represented by
/// the underlying storage type or EINVAL otherwise (e.g.,
/// the underlying array is not a string or large string array).
static inline ArrowErrorCode ArrowArrayAppendString(struct ArrowArray* array,
                                                    struct ArrowStringView value);

/// \brief Finish a nested array element
///
/// Appends a non-null element to the array based on the first child's current
/// length. Returns NANOARROW_OK if the item was successfully added or EINVAL
/// if the underlying storage type is not a struct, list, large list, or fixed-size
/// list, or if there was an attempt to add a struct or fixed-size list element where the
/// length of the child array(s) did not match the expected length.
static inline ArrowErrorCode ArrowArrayFinishElement(struct ArrowArray* array);

/// \brief Shrink buffer capacity to the size required
///
/// Also applies shrinking to any child arrays. array must have been allocated using
/// ArrowArrayInit
static inline ArrowErrorCode ArrowArrayShrinkToFit(struct ArrowArray* array);

/// \brief Finish building an ArrowArray
///
/// Flushes any pointers from internal buffers that may have been reallocated
/// into the array->buffers array and checks the actual size of the buffers
/// against the expected size based on the final length.
/// array must have been allocated using ArrowArrayInit
ArrowErrorCode ArrowArrayFinishBuilding(struct ArrowArray* array,
                                        struct ArrowError* error);

/// }@

/// \defgroup nanoarrow-array Array consumer helpers
/// These functions read and validate the contents ArrowArray structures

/// \brief Initialize the contents of an ArrowArrayView
void ArrowArrayViewInit(struct ArrowArrayView* array_view, enum ArrowType storage_type);

/// \brief Initialize the contents of an ArrowArrayView from an ArrowSchema
ArrowErrorCode ArrowArrayViewInitFromSchema(struct ArrowArrayView* array_view,
                                            struct ArrowSchema* schema,
                                            struct ArrowError* error);

/// \brief Allocate the schema_view->children array
///
/// Includes the memory for each child struct ArrowArrayView
ArrowErrorCode ArrowArrayViewAllocateChildren(struct ArrowArrayView* array_view,
                                              int64_t n_children);

/// \brief Set data-independent buffer sizes from length
void ArrowArrayViewSetLength(struct ArrowArrayView* array_view, int64_t length);

/// \brief Set buffer sizes and data pointers from an ArrowArray
ArrowErrorCode ArrowArrayViewSetArray(struct ArrowArrayView* array_view,
                                      struct ArrowArray* array, struct ArrowError* error);

/// \brief Reset the contents of an ArrowArrayView and frees resources
void ArrowArrayViewReset(struct ArrowArrayView* array_view);

/// \brief Check for a null element in an ArrowArrayView
static inline int8_t ArrowArrayViewIsNull(struct ArrowArrayView* array_view, int64_t i);

/// \brief Get an element in an ArrowArrayView as an integer
///
/// This function does not check for null values, that values are actually integers, or
/// that values are within a valid range for an int64.
static inline int64_t ArrowArrayViewGetIntUnsafe(struct ArrowArrayView* array_view,
                                                 int64_t i);

/// \brief Get an element in an ArrowArrayView as an unsigned integer
///
/// This function does not check for null values, that values are actually integers, or
/// that values are within a valid range for a uint64.
static inline uint64_t ArrowArrayViewGetUIntUnsafe(struct ArrowArrayView* array_view,
                                                   int64_t i);

/// \brief Get an element in an ArrowArrayView as a double
///
/// This function does not check for null values, or
/// that values are within a valid range for a double.
static inline double ArrowArrayViewGetDoubleUnsafe(struct ArrowArrayView* array_view,
                                                   int64_t i);

/// \brief Get an element in an ArrowArrayView as an ArrowStringView
///
/// This function does not check for null values.
static inline struct ArrowStringView ArrowArrayViewGetStringUnsafe(
    struct ArrowArrayView* array_view, int64_t i);

/// \brief Get an element in an ArrowArrayView as an ArrowBufferView
///
/// This function does not check for null values.
static inline struct ArrowBufferView ArrowArrayViewGetBytesUnsafe(
    struct ArrowArrayView* array_view, int64_t i);

/// }@

// Inline function definitions



#ifdef __cplusplus
}
#endif

#endif
// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.

#ifndef NANOARROW_BUFFER_INLINE_H_INCLUDED
#define NANOARROW_BUFFER_INLINE_H_INCLUDED

#include <errno.h>
#include <stdint.h>
#include <string.h>



#ifdef __cplusplus
extern "C" {
#endif

static inline int64_t _ArrowGrowByFactor(int64_t current_capacity, int64_t new_capacity) {
  int64_t doubled_capacity = current_capacity * 2;
  if (doubled_capacity > new_capacity) {
    return doubled_capacity;
  } else {
    return new_capacity;
  }
}

static inline void ArrowBufferInit(struct ArrowBuffer* buffer) {
  buffer->data = NULL;
  buffer->size_bytes = 0;
  buffer->capacity_bytes = 0;
  buffer->allocator = ArrowBufferAllocatorDefault();
}

static inline ArrowErrorCode ArrowBufferSetAllocator(
    struct ArrowBuffer* buffer, struct ArrowBufferAllocator allocator) {
  if (buffer->data == NULL) {
    buffer->allocator = allocator;
    return NANOARROW_OK;
  } else {
    return EINVAL;
  }
}

static inline void ArrowBufferReset(struct ArrowBuffer* buffer) {
  if (buffer->data != NULL) {
    buffer->allocator.free(&buffer->allocator, (uint8_t*)buffer->data,
                           buffer->capacity_bytes);
    buffer->data = NULL;
  }

  buffer->capacity_bytes = 0;
  buffer->size_bytes = 0;
}

static inline void ArrowBufferMove(struct ArrowBuffer* buffer,
                                   struct ArrowBuffer* buffer_out) {
  memcpy(buffer_out, buffer, sizeof(struct ArrowBuffer));
  buffer->data = NULL;
  ArrowBufferReset(buffer);
}

static inline ArrowErrorCode ArrowBufferResize(struct ArrowBuffer* buffer,
                                               int64_t new_capacity_bytes,
                                               char shrink_to_fit) {
  if (new_capacity_bytes < 0) {
    return EINVAL;
  }

  if (new_capacity_bytes > buffer->capacity_bytes || shrink_to_fit) {
    buffer->data = buffer->allocator.reallocate(
        &buffer->allocator, buffer->data, buffer->capacity_bytes, new_capacity_bytes);
    if (buffer->data == NULL && new_capacity_bytes > 0) {
      buffer->capacity_bytes = 0;
      buffer->size_bytes = 0;
      return ENOMEM;
    }

    buffer->capacity_bytes = new_capacity_bytes;
  }

  // Ensures that when shrinking that size <= capacity
  if (new_capacity_bytes < buffer->size_bytes) {
    buffer->size_bytes = new_capacity_bytes;
  }

  return NANOARROW_OK;
}

static inline ArrowErrorCode ArrowBufferReserve(struct ArrowBuffer* buffer,
                                                int64_t additional_size_bytes) {
  int64_t min_capacity_bytes = buffer->size_bytes + additional_size_bytes;
  if (min_capacity_bytes <= buffer->capacity_bytes) {
    return NANOARROW_OK;
  }

  return ArrowBufferResize(
      buffer, _ArrowGrowByFactor(buffer->capacity_bytes, min_capacity_bytes), 0);
}

static inline void ArrowBufferAppendUnsafe(struct ArrowBuffer* buffer, const void* data,
                                           int64_t size_bytes) {
  if (size_bytes > 0) {
    memcpy(buffer->data + buffer->size_bytes, data, size_bytes);
    buffer->size_bytes += size_bytes;
  }
}

static inline ArrowErrorCode ArrowBufferAppend(struct ArrowBuffer* buffer,
                                               const void* data, int64_t size_bytes) {
  NANOARROW_RETURN_NOT_OK(ArrowBufferReserve(buffer, size_bytes));

  ArrowBufferAppendUnsafe(buffer, data, size_bytes);
  return NANOARROW_OK;
}

static inline ArrowErrorCode ArrowBufferAppendInt8(struct ArrowBuffer* buffer,
                                                   int8_t value) {
  return ArrowBufferAppend(buffer, &value, sizeof(int8_t));
}

static inline ArrowErrorCode ArrowBufferAppendUInt8(struct ArrowBuffer* buffer,
                                                    uint8_t value) {
  return ArrowBufferAppend(buffer, &value, sizeof(uint8_t));
}

static inline ArrowErrorCode ArrowBufferAppendInt16(struct ArrowBuffer* buffer,
                                                    int16_t value) {
  return ArrowBufferAppend(buffer, &value, sizeof(int16_t));
}

static inline ArrowErrorCode ArrowBufferAppendUInt16(struct ArrowBuffer* buffer,
                                                     uint16_t value) {
  return ArrowBufferAppend(buffer, &value, sizeof(uint16_t));
}

static inline ArrowErrorCode ArrowBufferAppendInt32(struct ArrowBuffer* buffer,
                                                    int32_t value) {
  return ArrowBufferAppend(buffer, &value, sizeof(int32_t));
}

static inline ArrowErrorCode ArrowBufferAppendUInt32(struct ArrowBuffer* buffer,
                                                     uint32_t value) {
  return ArrowBufferAppend(buffer, &value, sizeof(uint32_t));
}

static inline ArrowErrorCode ArrowBufferAppendInt64(struct ArrowBuffer* buffer,
                                                    int64_t value) {
  return ArrowBufferAppend(buffer, &value, sizeof(int64_t));
}

static inline ArrowErrorCode ArrowBufferAppendUInt64(struct ArrowBuffer* buffer,
                                                     uint64_t value) {
  return ArrowBufferAppend(buffer, &value, sizeof(uint64_t));
}

static inline ArrowErrorCode ArrowBufferAppendDouble(struct ArrowBuffer* buffer,
                                                     double value) {
  return ArrowBufferAppend(buffer, &value, sizeof(double));
}

static inline ArrowErrorCode ArrowBufferAppendFloat(struct ArrowBuffer* buffer,
                                                    float value) {
  return ArrowBufferAppend(buffer, &value, sizeof(float));
}

static inline ArrowErrorCode ArrowBufferAppendFill(struct ArrowBuffer* buffer,
                                                   uint8_t value, int64_t size_bytes) {
  NANOARROW_RETURN_NOT_OK(ArrowBufferReserve(buffer, size_bytes));

  memset(buffer->data + buffer->size_bytes, value, size_bytes);
  buffer->size_bytes += size_bytes;
  return NANOARROW_OK;
}

static const uint8_t _ArrowkBitmask[] = {1, 2, 4, 8, 16, 32, 64, 128};
static const uint8_t _ArrowkFlippedBitmask[] = {254, 253, 251, 247, 239, 223, 191, 127};
static const uint8_t _ArrowkPrecedingBitmask[] = {0, 1, 3, 7, 15, 31, 63, 127};
static const uint8_t _ArrowkTrailingBitmask[] = {255, 254, 252, 248, 240, 224, 192, 128};

static const uint8_t _ArrowkBytePopcount[] = {
    0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3,
    4, 4, 5, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4,
    4, 5, 4, 5, 5, 6, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4,
    5, 3, 4, 4, 5, 4, 5, 5, 6, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5,
    4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2,
    3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5,
    5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4,
    5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 3, 4, 4, 5, 4, 5, 5, 6,
    4, 5, 5, 6, 5, 6, 6, 7, 4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8};

static inline int64_t _ArrowRoundUpToMultipleOf8(int64_t value) {
  return (value + 7) & ~((int64_t)7);
}

static inline int64_t _ArrowRoundDownToMultipleOf8(int64_t value) {
  return (value / 8) * 8;
}

static inline int64_t _ArrowBytesForBits(int64_t bits) {
  return (bits >> 3) + ((bits & 7) != 0);
}

static inline void _ArrowBitmapPackInt8(const int8_t* values, uint8_t* out) {
  *out = (values[0] | values[1] << 1 | values[2] << 2 | values[3] << 3 | values[4] << 4 |
          values[5] << 5 | values[6] << 6 | values[7] << 7);
}

static inline void _ArrowBitmapPackInt32(const int32_t* values, uint8_t* out) {
  *out = (values[0] | values[1] << 1 | values[2] << 2 | values[3] << 3 | values[4] << 4 |
          values[5] << 5 | values[6] << 6 | values[7] << 7);
}

static inline int8_t ArrowBitGet(const uint8_t* bits, int64_t i) {
  return (bits[i >> 3] >> (i & 0x07)) & 1;
}

static inline void ArrowBitSet(uint8_t* bits, int64_t i) {
  bits[i / 8] |= _ArrowkBitmask[i % 8];
}

static inline void ArrowBitClear(uint8_t* bits, int64_t i) {
  bits[i / 8] &= _ArrowkFlippedBitmask[i % 8];
}

static inline void ArrowBitSetTo(uint8_t* bits, int64_t i, uint8_t bit_is_set) {
  bits[i / 8] ^=
      ((uint8_t)(-((uint8_t)(bit_is_set != 0)) ^ bits[i / 8])) & _ArrowkBitmask[i % 8];
}

static inline void ArrowBitsSetTo(uint8_t* bits, int64_t start_offset, int64_t length,
                                  uint8_t bits_are_set) {
  const int64_t i_begin = start_offset;
  const int64_t i_end = start_offset + length;
  const uint8_t fill_byte = (uint8_t)(-bits_are_set);

  const int64_t bytes_begin = i_begin / 8;
  const int64_t bytes_end = i_end / 8 + 1;

  const uint8_t first_byte_mask = _ArrowkPrecedingBitmask[i_begin % 8];
  const uint8_t last_byte_mask = _ArrowkTrailingBitmask[i_end % 8];

  if (bytes_end == bytes_begin + 1) {
    // set bits within a single byte
    const uint8_t only_byte_mask =
        i_end % 8 == 0 ? first_byte_mask : (uint8_t)(first_byte_mask | last_byte_mask);
    bits[bytes_begin] &= only_byte_mask;
    bits[bytes_begin] |= (uint8_t)(fill_byte & ~only_byte_mask);
    return;
  }

  // set/clear trailing bits of first byte
  bits[bytes_begin] &= first_byte_mask;
  bits[bytes_begin] |= (uint8_t)(fill_byte & ~first_byte_mask);

  if (bytes_end - bytes_begin > 2) {
    // set/clear whole bytes
    memset(bits + bytes_begin + 1, fill_byte, (size_t)(bytes_end - bytes_begin - 2));
  }

  if (i_end % 8 == 0) {
    return;
  }

  // set/clear leading bits of last byte
  bits[bytes_end - 1] &= last_byte_mask;
  bits[bytes_end - 1] |= (uint8_t)(fill_byte & ~last_byte_mask);
}

static inline int64_t ArrowBitCountSet(const uint8_t* bits, int64_t start_offset,
                                       int64_t length) {
  if (length == 0) {
    return 0;
  }

  const int64_t i_begin = start_offset;
  const int64_t i_end = start_offset + length;

  const int64_t bytes_begin = i_begin / 8;
  const int64_t bytes_end = i_end / 8 + 1;

  const uint8_t first_byte_mask = _ArrowkPrecedingBitmask[i_begin % 8];
  const uint8_t last_byte_mask = _ArrowkTrailingBitmask[i_end % 8];

  if (bytes_end == bytes_begin + 1) {
    // count bits within a single byte
    const uint8_t only_byte_mask =
        i_end % 8 == 0 ? first_byte_mask : (uint8_t)(first_byte_mask | last_byte_mask);
    const uint8_t byte_masked = bits[bytes_begin] & only_byte_mask;
    return _ArrowkBytePopcount[byte_masked];
  }

  int64_t count = 0;

  // first byte
  count += _ArrowkBytePopcount[bits[bytes_begin] & ~first_byte_mask];

  // middle bytes
  for (int64_t i = bytes_begin + 1; i < (bytes_end - 1); i++) {
    count += _ArrowkBytePopcount[bits[i]];
  }

  // last byte
  count += _ArrowkBytePopcount[bits[bytes_end - 1] & ~last_byte_mask];

  return count;
}

static inline void ArrowBitmapInit(struct ArrowBitmap* bitmap) {
  ArrowBufferInit(&bitmap->buffer);
  bitmap->size_bits = 0;
}

static inline ArrowErrorCode ArrowBitmapReserve(struct ArrowBitmap* bitmap,
                                                int64_t additional_size_bits) {
  int64_t min_capacity_bits = bitmap->size_bits + additional_size_bits;
  if (min_capacity_bits <= (bitmap->buffer.capacity_bytes * 8)) {
    return NANOARROW_OK;
  }

  NANOARROW_RETURN_NOT_OK(
      ArrowBufferReserve(&bitmap->buffer, _ArrowBytesForBits(additional_size_bits)));

  bitmap->buffer.data[bitmap->buffer.capacity_bytes - 1] = 0;
  return NANOARROW_OK;
}

static inline ArrowErrorCode ArrowBitmapResize(struct ArrowBitmap* bitmap,
                                               int64_t new_capacity_bits,
                                               char shrink_to_fit) {
  if (new_capacity_bits < 0) {
    return EINVAL;
  }

  int64_t new_capacity_bytes = _ArrowBytesForBits(new_capacity_bits);
  NANOARROW_RETURN_NOT_OK(
      ArrowBufferResize(&bitmap->buffer, new_capacity_bytes, shrink_to_fit));

  if (new_capacity_bits < bitmap->size_bits) {
    bitmap->size_bits = new_capacity_bits;
  }

  return NANOARROW_OK;
}

static inline ArrowErrorCode ArrowBitmapAppend(struct ArrowBitmap* bitmap,
                                               uint8_t bits_are_set, int64_t length) {
  NANOARROW_RETURN_NOT_OK(ArrowBitmapReserve(bitmap, length));

  ArrowBitmapAppendUnsafe(bitmap, bits_are_set, length);
  return NANOARROW_OK;
}

static inline void ArrowBitmapAppendUnsafe(struct ArrowBitmap* bitmap,
                                           uint8_t bits_are_set, int64_t length) {
  ArrowBitsSetTo(bitmap->buffer.data, bitmap->size_bits, length, bits_are_set);
  bitmap->size_bits += length;
  bitmap->buffer.size_bytes = _ArrowBytesForBits(bitmap->size_bits);
}

static inline void ArrowBitmapAppendInt8Unsafe(struct ArrowBitmap* bitmap,
                                               const int8_t* values, int64_t n_values) {
  if (n_values == 0) {
    return;
  }

  const int8_t* values_cursor = values;
  int64_t n_remaining = n_values;
  int64_t out_i_cursor = bitmap->size_bits;
  uint8_t* out_cursor = bitmap->buffer.data + bitmap->size_bits / 8;

  // First byte
  if ((out_i_cursor % 8) != 0) {
    int64_t n_partial_bits = _ArrowRoundUpToMultipleOf8(out_i_cursor) - out_i_cursor;
    for (int i = 0; i < n_partial_bits; i++) {
      ArrowBitSetTo(bitmap->buffer.data, out_i_cursor++, values[i]);
    }

    out_cursor++;
    values_cursor += n_partial_bits;
    n_remaining -= n_partial_bits;
  }

  // Middle bytes
  int64_t n_full_bytes = n_remaining / 8;
  for (int64_t i = 0; i < n_full_bytes; i++) {
    _ArrowBitmapPackInt8(values_cursor, out_cursor);
    values_cursor += 8;
    out_cursor++;
  }

  // Last byte
  out_i_cursor += n_full_bytes * 8;
  n_remaining -= n_full_bytes * 8;
  if (n_remaining > 0) {
    // Zero out the last byte
    *out_cursor = 0x00;
    for (int i = 0; i < n_remaining; i++) {
      ArrowBitSetTo(bitmap->buffer.data, out_i_cursor++, values_cursor[i]);
    }
    out_cursor++;
  }

  bitmap->size_bits += n_values;
  bitmap->buffer.size_bytes = out_cursor - bitmap->buffer.data;
}

static inline void ArrowBitmapAppendInt32Unsafe(struct ArrowBitmap* bitmap,
                                                const int32_t* values, int64_t n_values) {
  if (n_values == 0) {
    return;
  }

  const int32_t* values_cursor = values;
  int64_t n_remaining = n_values;
  int64_t out_i_cursor = bitmap->size_bits;
  uint8_t* out_cursor = bitmap->buffer.data + bitmap->size_bits / 8;

  // First byte
  if ((out_i_cursor % 8) != 0) {
    int64_t n_partial_bits = _ArrowRoundUpToMultipleOf8(out_i_cursor) - out_i_cursor;
    for (int i = 0; i < n_partial_bits; i++) {
      ArrowBitSetTo(bitmap->buffer.data, out_i_cursor++, values[i]);
    }

    out_cursor++;
    values_cursor += n_partial_bits;
    n_remaining -= n_partial_bits;
  }

  // Middle bytes
  int64_t n_full_bytes = n_remaining / 8;
  for (int64_t i = 0; i < n_full_bytes; i++) {
    _ArrowBitmapPackInt32(values_cursor, out_cursor);
    values_cursor += 8;
    out_cursor++;
  }

  // Last byte
  out_i_cursor += n_full_bytes * 8;
  n_remaining -= n_full_bytes * 8;
  if (n_remaining > 0) {
    // Zero out the last byte
    *out_cursor = 0x00;
    for (int i = 0; i < n_remaining; i++) {
      ArrowBitSetTo(bitmap->buffer.data, out_i_cursor++, values_cursor[i]);
    }
    out_cursor++;
  }

  bitmap->size_bits += n_values;
  bitmap->buffer.size_bytes = out_cursor - bitmap->buffer.data;
}

static inline void ArrowBitmapReset(struct ArrowBitmap* bitmap) {
  ArrowBufferReset(&bitmap->buffer);
  bitmap->size_bits = 0;
}

#ifdef __cplusplus
}
#endif

#endif
// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.

#ifndef NANOARROW_ARRAY_INLINE_H_INCLUDED
#define NANOARROW_ARRAY_INLINE_H_INCLUDED

#include <errno.h>
#include <float.h>
#include <limits.h>
#include <stdint.h>
#include <string.h>




#ifdef __cplusplus
extern "C" {
#endif

static inline struct ArrowBitmap* ArrowArrayValidityBitmap(struct ArrowArray* array) {
  struct ArrowArrayPrivateData* private_data =
      (struct ArrowArrayPrivateData*)array->private_data;
  return &private_data->bitmap;
}

static inline struct ArrowBuffer* ArrowArrayBuffer(struct ArrowArray* array, int64_t i) {
  struct ArrowArrayPrivateData* private_data =
      (struct ArrowArrayPrivateData*)array->private_data;
  switch (i) {
    case 0:
      return &private_data->bitmap.buffer;
    default:
      return private_data->buffers + i - 1;
  }
}

static inline ArrowErrorCode ArrowArrayStartAppending(struct ArrowArray* array) {
  struct ArrowArrayPrivateData* private_data =
      (struct ArrowArrayPrivateData*)array->private_data;

  if (private_data->storage_type == NANOARROW_TYPE_UNINITIALIZED) {
    return EINVAL;
  }

  // Initialize any data offset buffer with a single zero
  for (int i = 0; i < 3; i++) {
    if (private_data->layout.buffer_type[i] == NANOARROW_BUFFER_TYPE_DATA_OFFSET &&
        private_data->layout.element_size_bits[i] == 64) {
      NANOARROW_RETURN_NOT_OK(ArrowBufferAppendInt64(ArrowArrayBuffer(array, i), 0));
    } else if (private_data->layout.buffer_type[i] == NANOARROW_BUFFER_TYPE_DATA_OFFSET &&
               private_data->layout.element_size_bits[i] == 32) {
      NANOARROW_RETURN_NOT_OK(ArrowBufferAppendInt32(ArrowArrayBuffer(array, i), 0));
    }
  }

  // Start building any child arrays
  for (int64_t i = 0; i < array->n_children; i++) {
    NANOARROW_RETURN_NOT_OK(ArrowArrayStartAppending(array->children[i]));
  }

  return NANOARROW_OK;
}

static inline ArrowErrorCode ArrowArrayShrinkToFit(struct ArrowArray* array) {
  for (int64_t i = 0; i < 3; i++) {
    struct ArrowBuffer* buffer = ArrowArrayBuffer(array, i);
    NANOARROW_RETURN_NOT_OK(ArrowBufferResize(buffer, buffer->size_bytes, 1));
  }

  for (int64_t i = 0; i < array->n_children; i++) {
    NANOARROW_RETURN_NOT_OK(ArrowArrayShrinkToFit(array->children[i]));
  }

  return NANOARROW_OK;
}

static inline ArrowErrorCode _ArrowArrayAppendBits(struct ArrowArray* array,
                                                   int64_t buffer_i, uint8_t value,
                                                   int64_t n) {
  struct ArrowArrayPrivateData* private_data =
      (struct ArrowArrayPrivateData*)array->private_data;
  struct ArrowBuffer* buffer = ArrowArrayBuffer(array, buffer_i);
  int64_t bytes_required =
      _ArrowRoundUpToMultipleOf8(private_data->layout.element_size_bits[buffer_i] *
                                 (array->length + 1)) /
      8;
  if (bytes_required > buffer->size_bytes) {
    NANOARROW_RETURN_NOT_OK(
        ArrowBufferAppendFill(buffer, 0, bytes_required - buffer->size_bytes));
  }

  ArrowBitsSetTo(buffer->data, array->length, n, value);
  return NANOARROW_OK;
}

static inline ArrowErrorCode ArrowArrayAppendNull(struct ArrowArray* array, int64_t n) {
  struct ArrowArrayPrivateData* private_data =
      (struct ArrowArrayPrivateData*)array->private_data;

  if (n == 0) {
    return NANOARROW_OK;
  }

  if (private_data->storage_type == NANOARROW_TYPE_NA) {
    array->null_count += n;
    array->length += n;
    return NANOARROW_OK;
  }

  // Append n 0 bits to the validity bitmap. If we haven't allocated a bitmap yet, do it
  // now
  if (private_data->bitmap.buffer.data == NULL) {
    NANOARROW_RETURN_NOT_OK(ArrowBitmapReserve(&private_data->bitmap, array->length + n));
    ArrowBitmapAppendUnsafe(&private_data->bitmap, 1, array->length);
    ArrowBitmapAppendUnsafe(&private_data->bitmap, 0, n);
  } else {
    NANOARROW_RETURN_NOT_OK(ArrowBitmapReserve(&private_data->bitmap, n));
    ArrowBitmapAppendUnsafe(&private_data->bitmap, 0, n);
  }

  // Add appropriate buffer fill
  struct ArrowBuffer* buffer;
  int64_t size_bytes;

  for (int i = 0; i < 3; i++) {
    buffer = ArrowArrayBuffer(array, i);
    size_bytes = private_data->layout.element_size_bits[i] / 8;

    switch (private_data->layout.buffer_type[i]) {
      case NANOARROW_BUFFER_TYPE_NONE:
      case NANOARROW_BUFFER_TYPE_VALIDITY:
        continue;
      case NANOARROW_BUFFER_TYPE_DATA_OFFSET:
        // Append the current value at the end of the offset buffer for each element
        NANOARROW_RETURN_NOT_OK(ArrowBufferReserve(buffer, size_bytes * n));

        for (int64_t j = 0; j < n; j++) {
          ArrowBufferAppendUnsafe(buffer, buffer->data + size_bytes * (array->length + j),
                                  size_bytes);
        }

        // Skip the data buffer
        i++;
        continue;
      case NANOARROW_BUFFER_TYPE_DATA:
        // Zero out the next bit of memory
        if (private_data->layout.element_size_bits[i] % 8 == 0) {
          NANOARROW_RETURN_NOT_OK(ArrowBufferAppendFill(buffer, 0, size_bytes * n));
        } else {
          NANOARROW_RETURN_NOT_OK(_ArrowArrayAppendBits(array, i, 0, n));
        }
        continue;

      case NANOARROW_BUFFER_TYPE_TYPE_ID:
      case NANOARROW_BUFFER_TYPE_UNION_OFFSET:
        // Not supported
        return EINVAL;
    }
  }

  // For fixed-size list and struct we need to append some nulls to
  // children for the lengths to line up properly
  switch (private_data->storage_type) {
    case NANOARROW_TYPE_FIXED_SIZE_LIST:
      NANOARROW_RETURN_NOT_OK(ArrowArrayAppendNull(
          array->children[0], n * private_data->layout.child_size_elements));
      break;
    case NANOARROW_TYPE_STRUCT:
      for (int64_t i = 0; i < array->n_children; i++) {
        NANOARROW_RETURN_NOT_OK(ArrowArrayAppendNull(array->children[i], n));
      }
    default:
      break;
  }

  array->length += n;
  array->null_count += n;
  return NANOARROW_OK;
}

static inline ArrowErrorCode ArrowArrayAppendInt(struct ArrowArray* array,
                                                 int64_t value) {
  struct ArrowArrayPrivateData* private_data =
      (struct ArrowArrayPrivateData*)array->private_data;

  struct ArrowBuffer* data_buffer = ArrowArrayBuffer(array, 1);

  switch (private_data->storage_type) {
    case NANOARROW_TYPE_INT64:
      NANOARROW_RETURN_NOT_OK(ArrowBufferAppend(data_buffer, &value, sizeof(int64_t)));
      break;
    case NANOARROW_TYPE_INT32:
      _NANOARROW_CHECK_RANGE(value, INT32_MIN, INT32_MAX);
      NANOARROW_RETURN_NOT_OK(ArrowBufferAppendInt32(data_buffer, value));
      break;
    case NANOARROW_TYPE_INT16:
      _NANOARROW_CHECK_RANGE(value, INT16_MIN, INT16_MAX);
      NANOARROW_RETURN_NOT_OK(ArrowBufferAppendInt16(data_buffer, value));
      break;
    case NANOARROW_TYPE_INT8:
      _NANOARROW_CHECK_RANGE(value, INT8_MIN, INT8_MAX);
      NANOARROW_RETURN_NOT_OK(ArrowBufferAppendInt8(data_buffer, value));
      break;
    case NANOARROW_TYPE_UINT64:
    case NANOARROW_TYPE_UINT32:
    case NANOARROW_TYPE_UINT16:
    case NANOARROW_TYPE_UINT8:
      _NANOARROW_CHECK_RANGE(value, 0, INT64_MAX);
      return ArrowArrayAppendUInt(array, value);
    case NANOARROW_TYPE_DOUBLE:
      NANOARROW_RETURN_NOT_OK(ArrowBufferAppendDouble(data_buffer, value));
      break;
    case NANOARROW_TYPE_FLOAT:
      NANOARROW_RETURN_NOT_OK(ArrowBufferAppendFloat(data_buffer, value));
      break;
    case NANOARROW_TYPE_BOOL:
      NANOARROW_RETURN_NOT_OK(_ArrowArrayAppendBits(array, 1, value != 0, 1));
      break;
    default:
      return EINVAL;
  }

  if (private_data->bitmap.buffer.data != NULL) {
    NANOARROW_RETURN_NOT_OK(ArrowBitmapAppend(ArrowArrayValidityBitmap(array), 1, 1));
  }

  array->length++;
  return NANOARROW_OK;
}

static inline ArrowErrorCode ArrowArrayAppendUInt(struct ArrowArray* array,
                                                  uint64_t value) {
  struct ArrowArrayPrivateData* private_data =
      (struct ArrowArrayPrivateData*)array->private_data;

  struct ArrowBuffer* data_buffer = ArrowArrayBuffer(array, 1);

  switch (private_data->storage_type) {
    case NANOARROW_TYPE_UINT64:
      NANOARROW_RETURN_NOT_OK(ArrowBufferAppend(data_buffer, &value, sizeof(uint64_t)));
      break;
    case NANOARROW_TYPE_UINT32:
      _NANOARROW_CHECK_RANGE(value, 0, UINT32_MAX);
      NANOARROW_RETURN_NOT_OK(ArrowBufferAppendUInt32(data_buffer, value));
      break;
    case NANOARROW_TYPE_UINT16:
      _NANOARROW_CHECK_RANGE(value, 0, UINT16_MAX);
      NANOARROW_RETURN_NOT_OK(ArrowBufferAppendUInt16(data_buffer, value));
      break;
    case NANOARROW_TYPE_UINT8:
      _NANOARROW_CHECK_RANGE(value, 0, UINT8_MAX);
      NANOARROW_RETURN_NOT_OK(ArrowBufferAppendUInt8(data_buffer, value));
      break;
    case NANOARROW_TYPE_INT64:
    case NANOARROW_TYPE_INT32:
    case NANOARROW_TYPE_INT16:
    case NANOARROW_TYPE_INT8:
      _NANOARROW_CHECK_RANGE(value, 0, INT64_MAX);
      return ArrowArrayAppendInt(array, value);
    case NANOARROW_TYPE_DOUBLE:
      NANOARROW_RETURN_NOT_OK(ArrowBufferAppendDouble(data_buffer, value));
      break;
    case NANOARROW_TYPE_FLOAT:
      NANOARROW_RETURN_NOT_OK(ArrowBufferAppendFloat(data_buffer, value));
      break;
    case NANOARROW_TYPE_BOOL:
      NANOARROW_RETURN_NOT_OK(_ArrowArrayAppendBits(array, 1, value != 0, 1));
      break;
    default:
      return EINVAL;
  }

  if (private_data->bitmap.buffer.data != NULL) {
    NANOARROW_RETURN_NOT_OK(ArrowBitmapAppend(ArrowArrayValidityBitmap(array), 1, 1));
  }

  array->length++;
  return NANOARROW_OK;
}

static inline ArrowErrorCode ArrowArrayAppendDouble(struct ArrowArray* array,
                                                    double value) {
  struct ArrowArrayPrivateData* private_data =
      (struct ArrowArrayPrivateData*)array->private_data;

  struct ArrowBuffer* data_buffer = ArrowArrayBuffer(array, 1);

  switch (private_data->storage_type) {
    case NANOARROW_TYPE_DOUBLE:
      NANOARROW_RETURN_NOT_OK(ArrowBufferAppend(data_buffer, &value, sizeof(double)));
      break;
    case NANOARROW_TYPE_FLOAT:
      _NANOARROW_CHECK_RANGE(value, FLT_MIN, FLT_MAX);
      NANOARROW_RETURN_NOT_OK(ArrowBufferAppendFloat(data_buffer, value));
      break;
    default:
      return EINVAL;
  }

  if (private_data->bitmap.buffer.data != NULL) {
    NANOARROW_RETURN_NOT_OK(ArrowBitmapAppend(ArrowArrayValidityBitmap(array), 1, 1));
  }

  array->length++;
  return NANOARROW_OK;
}

static inline ArrowErrorCode ArrowArrayAppendBytes(struct ArrowArray* array,
                                                   struct ArrowBufferView value) {
  struct ArrowArrayPrivateData* private_data =
      (struct ArrowArrayPrivateData*)array->private_data;

  struct ArrowBuffer* offset_buffer = ArrowArrayBuffer(array, 1);
  struct ArrowBuffer* data_buffer = ArrowArrayBuffer(
      array, 1 + (private_data->storage_type != NANOARROW_TYPE_FIXED_SIZE_BINARY));
  int32_t offset;
  int64_t large_offset;
  int64_t fixed_size_bytes = private_data->layout.element_size_bits[1] / 8;

  switch (private_data->storage_type) {
    case NANOARROW_TYPE_STRING:
    case NANOARROW_TYPE_BINARY:
      offset = ((int32_t*)offset_buffer->data)[array->length];
      if ((offset + value.n_bytes) > INT32_MAX) {
        return EINVAL;
      }

      offset += value.n_bytes;
      NANOARROW_RETURN_NOT_OK(ArrowBufferAppend(offset_buffer, &offset, sizeof(int32_t)));
      NANOARROW_RETURN_NOT_OK(
          ArrowBufferAppend(data_buffer, value.data.data, value.n_bytes));
      break;

    case NANOARROW_TYPE_LARGE_STRING:
    case NANOARROW_TYPE_LARGE_BINARY:
      large_offset = ((int64_t*)offset_buffer->data)[array->length];
      large_offset += value.n_bytes;
      NANOARROW_RETURN_NOT_OK(
          ArrowBufferAppend(offset_buffer, &large_offset, sizeof(int64_t)));
      NANOARROW_RETURN_NOT_OK(
          ArrowBufferAppend(data_buffer, value.data.data, value.n_bytes));
      break;

    case NANOARROW_TYPE_FIXED_SIZE_BINARY:
      if (value.n_bytes != fixed_size_bytes) {
        return EINVAL;
      }

      NANOARROW_RETURN_NOT_OK(
          ArrowBufferAppend(data_buffer, value.data.data, value.n_bytes));
      break;
    default:
      return EINVAL;
  }

  if (private_data->bitmap.buffer.data != NULL) {
    NANOARROW_RETURN_NOT_OK(ArrowBitmapAppend(ArrowArrayValidityBitmap(array), 1, 1));
  }

  array->length++;
  return NANOARROW_OK;
}

static inline ArrowErrorCode ArrowArrayAppendString(struct ArrowArray* array,
                                                    struct ArrowStringView value) {
  struct ArrowArrayPrivateData* private_data =
      (struct ArrowArrayPrivateData*)array->private_data;

  struct ArrowBufferView buffer_view;
  buffer_view.data.data = value.data;
  buffer_view.n_bytes = value.n_bytes;

  switch (private_data->storage_type) {
    case NANOARROW_TYPE_STRING:
    case NANOARROW_TYPE_LARGE_STRING:
      return ArrowArrayAppendBytes(array, buffer_view);
    default:
      return EINVAL;
  }
}

static inline ArrowErrorCode ArrowArrayFinishElement(struct ArrowArray* array) {
  struct ArrowArrayPrivateData* private_data =
      (struct ArrowArrayPrivateData*)array->private_data;

  int64_t child_length;

  switch (private_data->storage_type) {
    case NANOARROW_TYPE_LIST:
      child_length = array->children[0]->length;
      if (child_length > INT32_MAX) {
        return EINVAL;
      }
      NANOARROW_RETURN_NOT_OK(
          ArrowBufferAppendInt32(ArrowArrayBuffer(array, 1), child_length));
      break;
    case NANOARROW_TYPE_LARGE_LIST:
      child_length = array->children[0]->length;
      NANOARROW_RETURN_NOT_OK(
          ArrowBufferAppendInt64(ArrowArrayBuffer(array, 1), child_length));
      break;
    case NANOARROW_TYPE_FIXED_SIZE_LIST:
      child_length = array->children[0]->length;
      if (child_length !=
          ((array->length + 1) * private_data->layout.child_size_elements)) {
        return EINVAL;
      }
      break;
    case NANOARROW_TYPE_STRUCT:
      for (int64_t i = 0; i < array->n_children; i++) {
        child_length = array->children[i]->length;
        if (child_length != (array->length + 1)) {
          return EINVAL;
        }
      }
      break;
    default:
      return EINVAL;
  }

  if (private_data->bitmap.buffer.data != NULL) {
    NANOARROW_RETURN_NOT_OK(ArrowBitmapAppend(ArrowArrayValidityBitmap(array), 1, 1));
  }

  array->length++;
  return NANOARROW_OK;
}

static inline int8_t ArrowArrayViewIsNull(struct ArrowArrayView* array_view, int64_t i) {
  const uint8_t* validity_buffer = array_view->buffer_views[0].data.as_uint8;
  i += array_view->array->offset;
  switch (array_view->storage_type) {
    case NANOARROW_TYPE_NA:
      return 0x01;
    case NANOARROW_TYPE_DENSE_UNION:
    case NANOARROW_TYPE_SPARSE_UNION:
      // Not supported yet
      return 0xff;
    default:
      return validity_buffer != NULL && !ArrowBitGet(validity_buffer, i);
  }
}

static inline int64_t ArrowArrayViewGetIntUnsafe(struct ArrowArrayView* array_view,
                                                 int64_t i) {
  struct ArrowBufferView* data_view = &array_view->buffer_views[1];
  i += array_view->array->offset;
  switch (array_view->storage_type) {
    case NANOARROW_TYPE_INT64:
      return data_view->data.as_int64[i];
    case NANOARROW_TYPE_UINT64:
      return data_view->data.as_uint64[i];
    case NANOARROW_TYPE_INT32:
      return data_view->data.as_int32[i];
    case NANOARROW_TYPE_UINT32:
      return data_view->data.as_uint32[i];
    case NANOARROW_TYPE_INT16:
      return data_view->data.as_int16[i];
    case NANOARROW_TYPE_UINT16:
      return data_view->data.as_uint16[i];
    case NANOARROW_TYPE_INT8:
      return data_view->data.as_int8[i];
    case NANOARROW_TYPE_UINT8:
      return data_view->data.as_uint8[i];
    case NANOARROW_TYPE_DOUBLE:
      return data_view->data.as_double[i];
    case NANOARROW_TYPE_FLOAT:
      return data_view->data.as_float[i];
    case NANOARROW_TYPE_BOOL:
      return ArrowBitGet(data_view->data.as_uint8, i);
    default:
      return INT64_MAX;
  }
}

static inline uint64_t ArrowArrayViewGetUIntUnsafe(struct ArrowArrayView* array_view,
                                                   int64_t i) {
  i += array_view->array->offset;
  struct ArrowBufferView* data_view = &array_view->buffer_views[1];
  switch (array_view->storage_type) {
    case NANOARROW_TYPE_INT64:
      return data_view->data.as_int64[i];
    case NANOARROW_TYPE_UINT64:
      return data_view->data.as_uint64[i];
    case NANOARROW_TYPE_INT32:
      return data_view->data.as_int32[i];
    case NANOARROW_TYPE_UINT32:
      return data_view->data.as_uint32[i];
    case NANOARROW_TYPE_INT16:
      return data_view->data.as_int16[i];
    case NANOARROW_TYPE_UINT16:
      return data_view->data.as_uint16[i];
    case NANOARROW_TYPE_INT8:
      return data_view->data.as_int8[i];
    case NANOARROW_TYPE_UINT8:
      return data_view->data.as_uint8[i];
    case NANOARROW_TYPE_DOUBLE:
      return data_view->data.as_double[i];
    case NANOARROW_TYPE_FLOAT:
      return data_view->data.as_float[i];
    case NANOARROW_TYPE_BOOL:
      return ArrowBitGet(data_view->data.as_uint8, i);
    default:
      return UINT64_MAX;
  }
}

static inline double ArrowArrayViewGetDoubleUnsafe(struct ArrowArrayView* array_view,
                                                   int64_t i) {
  i += array_view->array->offset;
  struct ArrowBufferView* data_view = &array_view->buffer_views[1];
  switch (array_view->storage_type) {
    case NANOARROW_TYPE_INT64:
      return data_view->data.as_int64[i];
    case NANOARROW_TYPE_UINT64:
      return data_view->data.as_uint64[i];
    case NANOARROW_TYPE_INT32:
      return data_view->data.as_int32[i];
    case NANOARROW_TYPE_UINT32:
      return data_view->data.as_uint32[i];
    case NANOARROW_TYPE_INT16:
      return data_view->data.as_int16[i];
    case NANOARROW_TYPE_UINT16:
      return data_view->data.as_uint16[i];
    case NANOARROW_TYPE_INT8:
      return data_view->data.as_int8[i];
    case NANOARROW_TYPE_UINT8:
      return data_view->data.as_uint8[i];
    case NANOARROW_TYPE_DOUBLE:
      return data_view->data.as_double[i];
    case NANOARROW_TYPE_FLOAT:
      return data_view->data.as_float[i];
    case NANOARROW_TYPE_BOOL:
      return ArrowBitGet(data_view->data.as_uint8, i);
    default:
      return DBL_MAX;
  }
}

static inline struct ArrowStringView ArrowArrayViewGetStringUnsafe(
    struct ArrowArrayView* array_view, int64_t i) {
  i += array_view->array->offset;
  struct ArrowBufferView* offsets_view = &array_view->buffer_views[1];
  const char* data_view = array_view->buffer_views[2].data.as_char;

  struct ArrowStringView view;
  switch (array_view->storage_type) {
    case NANOARROW_TYPE_STRING:
    case NANOARROW_TYPE_BINARY:
      view.data = data_view + offsets_view->data.as_int32[i];
      view.n_bytes = offsets_view->data.as_int32[i + 1] - offsets_view->data.as_int32[i];
      break;
    case NANOARROW_TYPE_LARGE_STRING:
    case NANOARROW_TYPE_LARGE_BINARY:
      view.data = data_view + offsets_view->data.as_int64[i];
      view.n_bytes = offsets_view->data.as_int64[i + 1] - offsets_view->data.as_int64[i];
      break;
    case NANOARROW_TYPE_FIXED_SIZE_BINARY:
      view.n_bytes = array_view->layout.element_size_bits[1] / 8;
      view.data = array_view->buffer_views[1].data.as_char + (i * view.n_bytes);
      break;
    default:
      view.data = NULL;
      view.n_bytes = 0;
      break;
  }

  return view;
}

static inline struct ArrowBufferView ArrowArrayViewGetBytesUnsafe(
    struct ArrowArrayView* array_view, int64_t i) {
  i += array_view->array->offset;
  struct ArrowBufferView* offsets_view = &array_view->buffer_views[1];
  const uint8_t* data_view = array_view->buffer_views[2].data.as_uint8;

  struct ArrowBufferView view;
  switch (array_view->storage_type) {
    case NANOARROW_TYPE_STRING:
    case NANOARROW_TYPE_BINARY:
      view.n_bytes = offsets_view->data.as_int32[i + 1] - offsets_view->data.as_int32[i];
      view.data.as_uint8 = data_view + offsets_view->data.as_int32[i];
      break;
    case NANOARROW_TYPE_LARGE_STRING:
    case NANOARROW_TYPE_LARGE_BINARY:
      view.n_bytes = offsets_view->data.as_int64[i + 1] - offsets_view->data.as_int64[i];
      view.data.as_uint8 = data_view + offsets_view->data.as_int64[i];
      break;
    case NANOARROW_TYPE_FIXED_SIZE_BINARY:
      view.n_bytes = array_view->layout.element_size_bits[1] / 8;
      view.data.as_uint8 = array_view->buffer_views[1].data.as_uint8 + (i * view.n_bytes);
      break;
    default:
      view.data.data = NULL;
      view.n_bytes = 0;
      break;
  }

  return view;
}

#ifdef __cplusplus
}
#endif

#endif
