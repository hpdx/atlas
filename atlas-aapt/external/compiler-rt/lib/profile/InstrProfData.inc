/*===-- InstrProfData.inc - instr profiling runtime structures -----------=== *\
|*
|*                     The LLVM Compiler Infrastructure
|*
|* This file is distributed under the University of Illinois Open Source
|* License. See LICENSE.TXT for details.
|*
\*===----------------------------------------------------------------------===*/
/*
 * This is the master file that defines all the data structure, signature,
 * constant literals that are shared across profiling runtime library,
 * compiler (instrumentation), and host tools (reader/writer). The entities
 * defined in this file affect the profile runtime ABI, the raw profile format,
 * or both.
 *
 * The file has two identical copies. The master copy lives in LLVM and
 * the other one  sits in compiler-rt/lib/profile directory. To make changes
 * in this file, first modify the master copy and copy it over to compiler-rt.
 * Testing of any change in this file can start only after the two copies are
 * synced up.
 *
 * The first part of the file includes macros that defines types, names, and
 * initializers for the member fields of the core data structures. The field
 * declarations for one structure is enabled by defining the field activation
 * macro associated with that structure. Only one field activation record
 * can be defined at one time and the rest definitions will be filtered out by
 * the preprocessor.
 *
 * Examples of how the template is used to instantiate structure definition:
 * 1. To declare a structure:
 * 
 * struct ProfData {
 * #define INSTR_PROF_DATA(Type, LLVMType, Name, Initializer) \
 *    Type Name;
 * #include "llvm/ProfileData/InstrProfData.inc"
 * };
 *
 * 2. To construct LLVM type arrays for the struct type:
 *
 * Type *DataTypes[] = {
 * #define INSTR_PROF_DATA(Type, LLVMType, Name, Initializer) \
 *   LLVMType,
 * #include "llvm/ProfileData/InstrProfData.inc"
 * };
 *
 * 4. To construct constant array for the initializers:
 * #define INSTR_PROF_DATA(Type, LLVMType, Name, Initializer) \
 *   Initializer,
 * Constant *ConstantVals[] = {
 * #include "llvm/ProfileData/InstrProfData.inc"
 * };
 *
 *
 * The second part of the file includes definitions all other entities that
 * are related to runtime ABI and format. When no field activation macro is
 * defined, this file can be included to introduce the definitions.
 *
\*===----------------------------------------------------------------------===*/

/* INSTR_PROF_DATA start. */
/* Definition of member fields of the per-function control structure. */
#ifndef INSTR_PROF_DATA
#define INSTR_PROF_DATA(Type, LLVMType, Name, Initializer)
#else
#define INSTR_PROF_DATA_DEFINED
#endif

INSTR_PROF_DATA(const uint32_t, llvm::Type::getInt32Ty(Ctx), NameSize, \
                ConstantInt::get(llvm::Type::getInt32Ty(Ctx), \
                NamePtr->getType()->getPointerElementType()->getArrayNumElements()))
INSTR_PROF_DATA(const uint32_t, llvm::Type::getInt32Ty(Ctx), NumCounters, \
                ConstantInt::get(llvm::Type::getInt32Ty(Ctx), NumCounters))
INSTR_PROF_DATA(const uint64_t, llvm::Type::getInt64Ty(Ctx), FuncHash, \
                ConstantInt::get(llvm::Type::getInt64Ty(Ctx), \
                Inc->getHash()->getZExtValue()))
INSTR_PROF_DATA(const IntPtrT, llvm::Type::getInt8PtrTy(Ctx), NamePtr, \
                ConstantExpr::getBitCast(NamePtr, llvm::Type::getInt8PtrTy(Ctx)))
INSTR_PROF_DATA(const IntPtrT, llvm::Type::getInt64PtrTy(Ctx), CounterPtr, \
                ConstantExpr::getBitCast(CounterPtr, \
                llvm::Type::getInt64PtrTy(Ctx)))
INSTR_PROF_DATA(const IntPtrT, llvm::Type::getInt8PtrTy(Ctx), FunctionPointer, \
                FunctionAddr)
INSTR_PROF_DATA(IntPtrT, llvm::Type::getInt8PtrTy(Ctx), Values, \
                ConstantPointerNull::get(Int8PtrTy))
INSTR_PROF_DATA(const uint16_t, Int16ArrayTy, NumValueSites[IPVK_Last+1], \
                ConstantArray::get(Int16ArrayTy, Int16ArrayVals))
#undef INSTR_PROF_DATA
/* INSTR_PROF_DATA end. */

/* INSTR_PROF_RAW_HEADER  start */
/* Definition of member fields of the raw profile header data structure. */
#ifndef INSTR_PROF_RAW_HEADER
#define INSTR_PROF_RAW_HEADER(Type, Name, Initializer)
#else
#define INSTR_PROF_DATA_DEFINED
#endif
INSTR_PROF_RAW_HEADER(uint64_t, Magic, __llvm_profile_get_magic())
INSTR_PROF_RAW_HEADER(uint64_t, Version, __llvm_profile_get_version())
INSTR_PROF_RAW_HEADER(uint64_t, DataSize, DataSize)
INSTR_PROF_RAW_HEADER(uint64_t, CountersSize, CountersSize)
INSTR_PROF_RAW_HEADER(uint64_t, NamesSize,  NamesSize)
INSTR_PROF_RAW_HEADER(uint64_t, CountersDelta, (uintptr_t)CountersBegin)
INSTR_PROF_RAW_HEADER(uint64_t, NamesDelta, (uintptr_t)NamesBegin)
INSTR_PROF_RAW_HEADER(uint64_t, ValueKindLast, IPVK_Last)
INSTR_PROF_RAW_HEADER(uint64_t, ValueDataSize, ValueDataSize)
INSTR_PROF_RAW_HEADER(uint64_t, ValueDataDelta, (uintptr_t)ValueDataBegin)
#undef INSTR_PROF_RAW_HEADER
/* INSTR_PROF_RAW_HEADER  end */

/* VALUE_PROF_FUNC_PARAM start */
/* Definition of parameter types of the runtime API used to do value profiling
 * for a given value site.
 */
#ifndef VALUE_PROF_FUNC_PARAM
#define VALUE_PROF_FUNC_PARAM(ArgType, ArgName, ArgLLVMType)
#define INSTR_PROF_COMMA
#else
#define INSTR_PROF_DATA_DEFINED
#define INSTR_PROF_COMMA ,
#endif
VALUE_PROF_FUNC_PARAM(uint64_t, TargetValue, Type::getInt64Ty(Ctx)) \
                      INSTR_PROF_COMMA
VALUE_PROF_FUNC_PARAM(void *, Data, Type::getInt8PtrTy(Ctx)) INSTR_PROF_COMMA
VALUE_PROF_FUNC_PARAM(uint32_t, CounterIndex, Type::getInt32Ty(Ctx))
#undef VALUE_PROF_FUNC_PARAM
#undef INSTR_PROF_COMMA
/* VALUE_PROF_FUNC_PARAM end */

/* VALUE_PROF_KIND start */
#ifndef VALUE_PROF_KIND
#define VALUE_PROF_KIND(Enumerator, Value)
#else
#define INSTR_PROF_DATA_DEFINED
#endif
VALUE_PROF_KIND(IPVK_IndirectCallTarget, 0)
/* These two kinds must be the last to be
 * declared. This is to make sure the string
 * array created with the template can be
 * indexed with the kind value.
 */
VALUE_PROF_KIND(IPVK_First, IPVK_IndirectCallTarget)
VALUE_PROF_KIND(IPVK_Last, IPVK_IndirectCallTarget)

#undef VALUE_PROF_KIND
/* VALUE_PROF_KIND end */

/* COVMAP_FUNC_RECORD start */
/* Definition of member fields of the function record structure in coverage
 * map.
 */
#ifndef COVMAP_FUNC_RECORD
#define COVMAP_FUNC_RECORD(Type, LLVMType, Name, Initializer)
#else
#define INSTR_PROF_DATA_DEFINED
#endif
COVMAP_FUNC_RECORD(const IntPtrT, llvm::Type::getInt8PtrTy(Ctx), \
                   NamePtr, llvm::ConstantExpr::getBitCast(NamePtr, \
                   llvm::Type::getInt8PtrTy(Ctx))) 
COVMAP_FUNC_RECORD(const uint32_t, llvm::Type::getInt32Ty(Ctx), NameSize, \
                   llvm::ConstantInt::get(llvm::Type::getInt32Ty(Ctx),\
                   NameValue.size()))
COVMAP_FUNC_RECORD(const uint32_t, llvm::Type::getInt32Ty(Ctx), DataSize, \
                   llvm::ConstantInt::get(llvm::Type::getInt32Ty(Ctx),\
                   CoverageMapping.size()))
COVMAP_FUNC_RECORD(const uint64_t, llvm::Type::getInt64Ty(Ctx), FuncHash, \
                   llvm::ConstantInt::get(llvm::Type::getInt64Ty(Ctx), FuncHash))
#undef COVMAP_FUNC_RECORD
/* COVMAP_FUNC_RECORD end.  */


#ifdef INSTR_PROF_VALUE_PROF_DATA
#define INSTR_PROF_DATA_DEFINED

/*! 
 * This is the header of the data structure that defines the on-disk
 * layout of the value profile data of a particular kind for one function.
 */
typedef struct ValueProfRecord {
  /* The kind of the value profile record. */
  uint32_t Kind;
  /*
   * The number of value profile sites. It is guaranteed to be non-zero;
   * otherwise the record for this kind won't be emitted.
   */
  uint32_t NumValueSites;
  /* 
   * The first element of the array that stores the number of profiled
   * values for each value site. The size of the array is NumValueSites.
   * Since NumValueSites is greater than zero, there is at least one
   * element in the array.
   */
  uint8_t SiteCountArray[1];

  /*
   * The fake declaration is for documentation purpose only.
   * Align the start of next field to be on 8 byte boundaries.
  uint8_t Padding[X];
   */

  /* The array of value profile data. The size of the array is the sum
   * of all elements in SiteCountArray[].
  InstrProfValueData ValueData[];
   */

#ifdef __cplusplus
  /*!
   * \brief Return the number of value sites.
   */
  uint32_t getNumValueSites() const { return NumValueSites; }
  /*! 
   * \brief Read data from this record and save it to Record.
   */
  void deserializeTo(InstrProfRecord &Record,
                     InstrProfRecord::ValueMapType *VMap);
  /*
   * In-place byte swap:
   * Do byte swap for this instance. \c Old is the original order before
   * the swap, and \c New is the New byte order.
   */
  void swapBytes(support::endianness Old, support::endianness New);
#endif
} ValueProfRecord;

/*!
 * Per-function header/control data structure for value profiling
 * data in indexed format.
 */
typedef struct ValueProfData {
  /*
   * Total size in bytes including this field. It must be a multiple
   * of sizeof(uint64_t). 
   */
  uint32_t TotalSize;
  /* 
   *The number of value profile kinds that has value profile data.
   * In this implementation, a value profile kind is considered to
   * have profile data if the number of value profile sites for the
   * kind is not zero. More aggressively, the implementation can
   * choose to check the actual data value: if none of the value sites
   * has any profiled values, the kind can be skipped.
   */
  uint32_t NumValueKinds;

  /* 
   * Following are a sequence of variable length records. The prefix/header
   * of each record is defined by ValueProfRecord type. The number of
   * records is NumValueKinds.
   * ValueProfRecord Record_1;
   * ValueProfRecord Record_N;
   */

#if __cplusplus
  /*!
   * Return the total size in bytes of the on-disk value profile data
   * given the data stored in Record.
   */
  static uint32_t getSize(const InstrProfRecord &Record);
  /*!
   * Return a pointer to \c ValueProfData instance ready to be streamed.
   */
  static std::unique_ptr<ValueProfData>
  serializeFrom(const InstrProfRecord &Record);
  /*!
   * Check the integrity of the record. Return the error code when
   * an error is detected, otherwise return instrprof_error::success.
   */
  instrprof_error checkIntegrity();
  /*!
   * Return a pointer to \c ValueProfileData instance ready to be read.
   * All data in the instance are properly byte swapped. The input
   * data is assumed to be in little endian order.
   */
  static ErrorOr<std::unique_ptr<ValueProfData>>
  getValueProfData(const unsigned char *SrcBuffer,
                   const unsigned char *const SrcBufferEnd,
                   support::endianness SrcDataEndianness);
  /*!
   * Swap byte order from \c Endianness order to host byte order.
   */
  void swapBytesToHost(support::endianness Endianness);
  /*!
   * Swap byte order from host byte order to \c Endianness order.
   */
  void swapBytesFromHost(support::endianness Endianness);
  /*!
   * Return the total size of \c ValueProfileData.
   */
  uint32_t getSize() const { return TotalSize; }
  /*!
   * Read data from this data and save it to \c Record.
   */
  void deserializeTo(InstrProfRecord &Record,
                     InstrProfRecord::ValueMapType *VMap);
  void operator delete(void *ptr) { ::operator delete(ptr); }
#endif
} ValueProfData;

/* 
 * The closure is designed to abstact away two types of value profile data:
 * - InstrProfRecord which is the primary data structure used to
 *   represent profile data in host tools (reader, writer, and profile-use)
 * - value profile runtime data structure suitable to be used by C
 *   runtime library.
 *
 * Both sources of data need to serialize to disk/memory-buffer in common
 * format: ValueProfData. The abstraction allows compiler-rt's raw profiler
 * writer to share the same format and code with indexed profile writer.
 *
 * For documentation of the member methods below, refer to corresponding methods
 * in class InstrProfRecord.
 */
typedef struct ValueProfRecordClosure {
  const void *Record;
  uint32_t (*GetNumValueKinds)(const void *Record);
  uint32_t (*GetNumValueSites)(const void *Record, uint32_t VKind);
  uint32_t (*GetNumValueData)(const void *Record, uint32_t VKind);
  uint32_t (*GetNumValueDataForSite)(const void *R, uint32_t VK, uint32_t S);

  /* 
   * After extracting the value profile data from the value profile record,
   * this method is used to map the in-memory value to on-disk value. If
   * the method is null, value will be written out untranslated.
   */
  uint64_t (*RemapValueData)(uint32_t, uint64_t Value);
  void (*GetValueForSite)(const void *R, InstrProfValueData *Dst, uint32_t K,
                          uint32_t S, uint64_t (*Mapper)(uint32_t, uint64_t));
  ValueProfData *(*AllocValueProfData)(size_t TotalSizeInBytes);
} ValueProfRecordClosure;

/* 
 * A wrapper struct that represents value profile runtime data.
 * Like InstrProfRecord class which is used by profiling host tools,
 * ValueProfRuntimeRecord also implements the abstract intefaces defined in
 * ValueProfRecordClosure so that the runtime data can be serialized using
 * shared C implementation. In this structure, NumValueSites and Nodes
 * members are the primary fields while other fields hold the derived
 * information for fast implementation of closure interfaces.
 */
typedef struct ValueProfRuntimeRecord {
  /* Number of sites for each value profile kind.  */
  const uint16_t *NumValueSites;
  /* An array of linked-list headers. The size of of the array is the
   * total number of value profile sites : sum(NumValueSites[*])). Each
   * linked-list stores the values profiled for a value profile site. */
  ValueProfNode **Nodes;

  /* Total number of value profile kinds which have at least one
   *  value profile sites. */
  uint32_t NumValueKinds;
  /* An array recording the number of values tracked at each site.
   * The size of the array is TotalNumValueSites. */
  uint8_t *SiteCountArray[IPVK_Last + 1];
  ValueProfNode **NodesKind[IPVK_Last + 1];
} ValueProfRuntimeRecord;

/* Forward declarations of C interfaces.  */
int initializeValueProfRuntimeRecord(ValueProfRuntimeRecord *RuntimeRecord,
                                     const uint16_t *NumValueSites,
                                     ValueProfNode **Nodes);
void finalizeValueProfRuntimeRecord(ValueProfRuntimeRecord *RuntimeRecord);
uint32_t getValueProfDataSizeRT(const ValueProfRuntimeRecord *Record);
ValueProfData *
serializeValueProfDataFromRT(const ValueProfRuntimeRecord *Record,
                             ValueProfData *Dst);
uint32_t getNumValueKindsRT(const void *R);

#undef INSTR_PROF_VALUE_PROF_DATA
#endif  /* INSTR_PROF_VALUE_PROF_DATA */ 


#ifdef INSTR_PROF_COMMON_API_IMPL
#define INSTR_PROF_DATA_DEFINED
#ifdef __cplusplus
#define INSTR_PROF_INLINE inline
#else
#define INSTR_PROF_INLINE
#endif

#ifndef offsetof
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif

/*!
 * \brief Return the \c ValueProfRecord header size including the
 * padding bytes.
 */
INSTR_PROF_INLINE
uint32_t getValueProfRecordHeaderSize(uint32_t NumValueSites) {
  uint32_t Size = offsetof(ValueProfRecord, SiteCountArray) +
                  sizeof(uint8_t) * NumValueSites;
  /* Round the size to multiple of 8 bytes. */
  Size = (Size + 7) & ~7;
  return Size;
}

/*! 
 * \brief Return the total size of the value profile record including the
 * header and the value data.
 */
INSTR_PROF_INLINE
uint32_t getValueProfRecordSize(uint32_t NumValueSites,
                                uint32_t NumValueData) {
  return getValueProfRecordHeaderSize(NumValueSites) +
         sizeof(InstrProfValueData) * NumValueData;
}

/*!
 * \brief Return the pointer to the start of value data array.
 */
INSTR_PROF_INLINE
InstrProfValueData *getValueProfRecordValueData(ValueProfRecord *This) {
  return (InstrProfValueData *)((char *)This + getValueProfRecordHeaderSize(
                                                   This->NumValueSites));
}

/*! 
 * \brief Return the total number of value data for \c This record.
 */
INSTR_PROF_INLINE
uint32_t getValueProfRecordNumValueData(ValueProfRecord *This) {
  uint32_t NumValueData = 0;
  uint32_t I;
  for (I = 0; I < This->NumValueSites; I++)
    NumValueData += This->SiteCountArray[I];
  return NumValueData;
}

/*! 
 * \brief Use this method to advance to the next \c This \c ValueProfRecord.
 */
INSTR_PROF_INLINE
ValueProfRecord *getValueProfRecordNext(ValueProfRecord *This) {
  uint32_t NumValueData = getValueProfRecordNumValueData(This);
  return (ValueProfRecord *)((char *)This +
                             getValueProfRecordSize(This->NumValueSites,
                                                    NumValueData));
}

/*!
 * \brief Return the first \c ValueProfRecord instance.
 */
INSTR_PROF_INLINE
ValueProfRecord *getFirstValueProfRecord(ValueProfData *This) {
  return (ValueProfRecord *)((char *)This + sizeof(ValueProfData));
}

/* Closure based interfaces.  */

/*! 
 * Return the total size in bytes of the on-disk value profile data
 * given the data stored in Record.
 */
uint32_t getValueProfDataSize(ValueProfRecordClosure *Closure) {
  uint32_t Kind;
  uint32_t TotalSize = sizeof(ValueProfData);
  const void *Record = Closure->Record;
  uint32_t NumValueKinds = Closure->GetNumValueKinds(Record);
  if (NumValueKinds == 0)
    return TotalSize;

  for (Kind = IPVK_First; Kind <= IPVK_Last; Kind++) {
    uint32_t NumValueSites = Closure->GetNumValueSites(Record, Kind);
    if (!NumValueSites)
      continue;
    TotalSize += getValueProfRecordSize(NumValueSites,
                                        Closure->GetNumValueData(Record, Kind));
  }
  return TotalSize;
}

/*!
 * Extract value profile data of a function for the profile kind \c ValueKind
 * from the \c Closure and serialize the data into \c This record instance.
 */
void serializeValueProfRecordFrom(ValueProfRecord *This,
                                  ValueProfRecordClosure *Closure,
                                  uint32_t ValueKind, uint32_t NumValueSites) {
  uint32_t S;
  const void *Record = Closure->Record;
  This->Kind = ValueKind;
  This->NumValueSites = NumValueSites;
  InstrProfValueData *DstVD = getValueProfRecordValueData(This);

  for (S = 0; S < NumValueSites; S++) {
    uint32_t ND = Closure->GetNumValueDataForSite(Record, ValueKind, S);
    This->SiteCountArray[S] = ND;
    Closure->GetValueForSite(Record, DstVD, ValueKind, S,
                             Closure->RemapValueData);
    DstVD += ND;
  }
}

/*!
 * Extract value profile data of a function  from the \c Closure
 * and serialize the data into \c DstData if it is not NULL or heap
 * memory allocated by the \c Closure's allocator method.
 */
ValueProfData *serializeValueProfDataFrom(ValueProfRecordClosure *Closure,
                                          ValueProfData *DstData) {
  uint32_t Kind;
  uint32_t TotalSize = getValueProfDataSize(Closure);

  ValueProfData *VPD =
      DstData ? DstData : Closure->AllocValueProfData(TotalSize);

  VPD->TotalSize = TotalSize;
  VPD->NumValueKinds = Closure->GetNumValueKinds(Closure->Record);
  ValueProfRecord *VR = getFirstValueProfRecord(VPD);
  for (Kind = IPVK_First; Kind <= IPVK_Last; Kind++) {
    uint32_t NumValueSites = Closure->GetNumValueSites(Closure->Record, Kind);
    if (!NumValueSites)
      continue;
    serializeValueProfRecordFrom(VR, Closure, Kind, NumValueSites);
    VR = getValueProfRecordNext(VR);
  }
  return VPD;
}

/* 
 * The value profiler runtime library stores the value profile data
 * for a given function in \c NumValueSites and \c Nodes structures.
 * \c ValueProfRuntimeRecord class is used to encapsulate the runtime
 * profile data and provides fast interfaces to retrieve the profile
 * information. This interface is used to initialize the runtime record
 * and pre-compute the information needed for efficient implementation
 * of callbacks required by ValueProfRecordClosure class.
 */
int initializeValueProfRuntimeRecord(ValueProfRuntimeRecord *RuntimeRecord,
                                     const uint16_t *NumValueSites,
                                     ValueProfNode **Nodes) {
  unsigned I, J, S = 0, NumValueKinds = 0;
  RuntimeRecord->NumValueSites = NumValueSites;
  RuntimeRecord->Nodes = Nodes;
  for (I = 0; I <= IPVK_Last; I++) {
    uint16_t N = NumValueSites[I];
    if (!N) {
      RuntimeRecord->SiteCountArray[I] = 0;
      continue;
    }
    NumValueKinds++;
    RuntimeRecord->SiteCountArray[I] = (uint8_t *)calloc(N, 1);
    if (!RuntimeRecord->SiteCountArray[I])
      return 1;
    RuntimeRecord->NodesKind[I] = Nodes ? &Nodes[S] : NULL;
    for (J = 0; J < N; J++) {
      /* Compute value count for each site. */
      uint32_t C = 0;
      ValueProfNode *Site = Nodes ? RuntimeRecord->NodesKind[I][J] : NULL;
      while (Site) {
        C++;
        Site = Site->Next;
      }
      if (C > UCHAR_MAX)
        C = UCHAR_MAX;
      RuntimeRecord->SiteCountArray[I][J] = C;
    }
    S += N;
  }
  RuntimeRecord->NumValueKinds = NumValueKinds;
  return 0;
}

void finalizeValueProfRuntimeRecord(ValueProfRuntimeRecord *RuntimeRecord) {
  unsigned I;
  for (I = 0; I <= IPVK_Last; I++) {
    if (RuntimeRecord->SiteCountArray[I])
      free(RuntimeRecord->SiteCountArray[I]);
  }
}

/* ValueProfRecordClosure Interface implementation for
 * ValueProfDataRuntimeRecord.  */
uint32_t getNumValueKindsRT(const void *R) {
  return ((const ValueProfRuntimeRecord *)R)->NumValueKinds;
}

uint32_t getNumValueSitesRT(const void *R, uint32_t VK) {
  return ((const ValueProfRuntimeRecord *)R)->NumValueSites[VK];
}

uint32_t getNumValueDataForSiteRT(const void *R, uint32_t VK, uint32_t S) {
  const ValueProfRuntimeRecord *Record = (const ValueProfRuntimeRecord *)R;
  return Record->SiteCountArray[VK][S];
}

uint32_t getNumValueDataRT(const void *R, uint32_t VK) {
  unsigned I, S = 0;
  const ValueProfRuntimeRecord *Record = (const ValueProfRuntimeRecord *)R;
  if (Record->SiteCountArray[VK] == 0)
    return 0;
  for (I = 0; I < Record->NumValueSites[VK]; I++)
    S += Record->SiteCountArray[VK][I];
  return S;
}

void getValueForSiteRT(const void *R, InstrProfValueData *Dst, uint32_t VK,
                       uint32_t S, uint64_t (*Mapper)(uint32_t, uint64_t)) {
  unsigned I, N = 0;
  const ValueProfRuntimeRecord *Record = (const ValueProfRuntimeRecord *)R;
  N = getNumValueDataForSiteRT(R, VK, S);
  if (N == 0)
    return;
  ValueProfNode *VNode = Record->NodesKind[VK][S];
  for (I = 0; I < N; I++) {
    Dst[I] = VNode->VData;
    VNode = VNode->Next;
  }
}

ValueProfData *allocValueProfDataRT(size_t TotalSizeInBytes) {
  return (ValueProfData *)calloc(TotalSizeInBytes, 1);
}

static ValueProfRecordClosure RTRecordClosure = {0,
                                                 getNumValueKindsRT,
                                                 getNumValueSitesRT,
                                                 getNumValueDataRT,
                                                 getNumValueDataForSiteRT,
                                                 0,
                                                 getValueForSiteRT,
                                                 allocValueProfDataRT};

/* 
 * Return the size of ValueProfData structure to store data
 * recorded in the runtime record.
 */
uint32_t getValueProfDataSizeRT(const ValueProfRuntimeRecord *Record) {
  RTRecordClosure.Record = Record;
  return getValueProfDataSize(&RTRecordClosure);
}

/* 
 * Return a ValueProfData instance that stores the data collected
 * from runtime. If \c DstData is provided by the caller, the value
 * profile data will be store in *DstData and DstData is returned,
 * otherwise the method will allocate space for the value data and
 * return pointer to the newly allocated space.
 */
ValueProfData *
serializeValueProfDataFromRT(const ValueProfRuntimeRecord *Record,
                             ValueProfData *DstData) {
  RTRecordClosure.Record = Record;
  return serializeValueProfDataFrom(&RTRecordClosure, DstData);
}


#undef INSTR_PROF_COMMON_API_IMPL
#endif /* INSTR_PROF_COMMON_API_IMPL */

/*============================================================================*/


#ifndef INSTR_PROF_DATA_DEFINED

#ifndef INSTR_PROF_DATA_INC_
#define INSTR_PROF_DATA_INC_

/* Helper macros.  */
#define INSTR_PROF_SIMPLE_QUOTE(x) #x
#define INSTR_PROF_QUOTE(x) INSTR_PROF_SIMPLE_QUOTE(x)
#define INSTR_PROF_SIMPLE_CONCAT(x,y) x ## y
#define INSTR_PROF_CONCAT(x,y) INSTR_PROF_SIMPLE_CONCAT(x,y)

/* Magic number to detect file format and endianness.
 * Use 255 at one end, since no UTF-8 file can use that character.  Avoid 0,
 * so that utilities, like strings, don't grab it as a string.  129 is also
 * invalid UTF-8, and high enough to be interesting.
 * Use "lprofr" in the centre to stand for "LLVM Profile Raw", or "lprofR"
 * for 32-bit platforms.
 */
#define INSTR_PROF_RAW_MAGIC_64 (uint64_t)255 << 56 | (uint64_t)'l' << 48 | \
       (uint64_t)'p' << 40 | (uint64_t)'r' << 32 | (uint64_t)'o' << 24 |  \
        (uint64_t)'f' << 16 | (uint64_t)'r' << 8 | (uint64_t)129
#define INSTR_PROF_RAW_MAGIC_32 (uint64_t)255 << 56 | (uint64_t)'l' << 48 | \
       (uint64_t)'p' << 40 | (uint64_t)'r' << 32 | (uint64_t)'o' << 24 |  \
        (uint64_t)'f' << 16 | (uint64_t)'R' << 8 | (uint64_t)129

/* Raw profile format version. */
#define INSTR_PROF_RAW_VERSION 2

/* Runtime section names and name strings.  */
#define INSTR_PROF_DATA_SECT_NAME __llvm_prf_data
#define INSTR_PROF_NAME_SECT_NAME __llvm_prf_names
#define INSTR_PROF_CNTS_SECT_NAME __llvm_prf_cnts

#define INSTR_PROF_DATA_SECT_NAME_STR \
        INSTR_PROF_QUOTE(INSTR_PROF_DATA_SECT_NAME)
#define INSTR_PROF_NAME_SECT_NAME_STR \
        INSTR_PROF_QUOTE(INSTR_PROF_NAME_SECT_NAME)
#define INSTR_PROF_CNTS_SECT_NAME_STR \
        INSTR_PROF_QUOTE(INSTR_PROF_CNTS_SECT_NAME)

/* Macros to define start/stop section symbol for a given
 * section on Linux. For instance
 * INSTR_PROF_SECT_START(INSTR_PROF_DATA_SECT_NAME) will
 * expand to __start___llvm_prof_data
 */
#define INSTR_PROF_SECT_START(Sect) \
        INSTR_PROF_CONCAT(__start_,Sect)
#define INSTR_PROF_SECT_STOP(Sect) \
        INSTR_PROF_CONCAT(__stop_,Sect)

/* Value Profiling API linkage name.  */
#define INSTR_PROF_VALUE_PROF_FUNC __llvm_profile_instrument_target
#define INSTR_PROF_VALUE_PROF_FUNC_STR \
        INSTR_PROF_QUOTE(INSTR_PROF_VALUE_PROF_FUNC)

/* InstrProfile per-function control data alignment.  */
#define INSTR_PROF_DATA_ALIGNMENT 8

/* The data structure that represents a tracked value by the
 * value profiler.
 */
typedef struct InstrProfValueData {
  /* Profiled value. */
  uint64_t Value;
  /* Number of times the value appears in the training run. */
  uint64_t Count;
} InstrProfValueData;

/* This is an internal data structure used by value profiler. It
 * is defined here to allow serialization code sharing by LLVM
 * to be used in unit test.
 */
typedef struct ValueProfNode {
  InstrProfValueData VData;
  struct ValueProfNode *Next;
} ValueProfNode;

#endif /* INSTR_PROF_DATA_INC_ */

#else
#undef INSTR_PROF_DATA_DEFINED
#endif

