//===- Buffer.cpp ---------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Buffer.h"
#include "llvm-objcopy.h"
#include "llvm/Support/FileOutputBuffer.h"
#include "llvm/Support/MemoryBuffer.h"
#include <memory>

namespace llvm {
namespace objcopy {

Buffer::~Buffer() {}

void FileBuffer::allocate(size_t Size) {
  Expected<std::unique_ptr<FileOutputBuffer>> BufferOrErr =
      FileOutputBuffer::create(getName(), Size, FileOutputBuffer::F_executable);
  handleAllErrors(BufferOrErr.takeError(), [this](const ErrorInfoBase &E) {
    error("failed to open " + getName() + ": " + E.message());
  });
  Buf = std::move(*BufferOrErr);
}

Error FileBuffer::commit() { return Buf->commit(); }

uint8_t *FileBuffer::getBufferStart() {
  return reinterpret_cast<uint8_t *>(Buf->getBufferStart());
}

void MemBuffer::allocate(size_t Size) {
  Buf = WritableMemoryBuffer::getNewMemBuffer(Size, getName());
}

Error MemBuffer::commit() { return Error::success(); }

uint8_t *MemBuffer::getBufferStart() {
  return reinterpret_cast<uint8_t *>(Buf->getBufferStart());
}

std::unique_ptr<WritableMemoryBuffer> MemBuffer::releaseMemoryBuffer() {
  return std::move(Buf);
}

} // end namespace objcopy
} // end namespace llvm
