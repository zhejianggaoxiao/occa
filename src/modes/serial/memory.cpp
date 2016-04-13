/* The MIT License (MIT)
 * 
 * Copyright (c) 2014 David Medina and Tim Warburton
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 */

#include "occa/modes/serial/memory.hpp"
#include "occa/device.hpp"

namespace occa {
  namespace serial {
    memory::memory() : occa::memory_v() {}

    memory::memory(const memory &m){
      *this = m;
    }

    memory& memory::operator = (const memory &m){
      initFrom(m);
      return *this;
    }

    memory::~memory(){}

    void* memory::getMemoryHandle(){
      return handle;
    }

    void memory::copyFrom(const void *src,
                          const uintptr_t bytes,
                          const uintptr_t offset,
                          const bool asycn){
      dHandle->finish();

      const uintptr_t bytes_ = (bytes == 0) ? size : bytes;

      OCCA_CHECK((bytes_ + offset) <= size,
                 "Memory has size [" << size << "],"
                 << "trying to access [ " << offset << " , " << (offset + bytes_) << " ]");

      void *destPtr      = ((char*) handle) + offset;
      const void *srcPtr = src;

      ::memcpy(destPtr, srcPtr, bytes_);
    }

    void memory::copyFrom(const memory_v *src,
                          const uintptr_t bytes,
                          const uintptr_t destOffset,
                          const uintptr_t srcOffset,
                          const bool asycn){
      dHandle->finish();

      const uintptr_t bytes_ = (bytes == 0) ? size : bytes;

      OCCA_CHECK((bytes_ + destOffset) <= size,
                 "Memory has size [" << size << "],"
                 << "trying to access [ " << destOffset << " , " << (destOffset + bytes_) << " ]");

      OCCA_CHECK((bytes_ + srcOffset) <= src->size,
                 "Source has size [" << src->size << "],"
                 << "trying to access [ " << srcOffset << " , " << (srcOffset + bytes_) << " ]");

      void *destPtr      = ((char*) handle)      + destOffset;
      const void *srcPtr = ((char*) src->handle) + srcOffset;

      ::memcpy(destPtr, srcPtr, bytes_);
    }

    void memory::copyTo(void *dest,
                        const uintptr_t bytes,
                        const uintptr_t offset,
                        const bool asycn){
      dHandle->finish();

      const uintptr_t bytes_ = (bytes == 0) ? size : bytes;

      OCCA_CHECK((bytes_ + offset) <= size,
                 "Memory has size [" << size << "],"
                 << "trying to access [ " << offset << " , " << (offset + bytes_) << " ]");

      void *destPtr      = dest;
      const void *srcPtr = ((char*) handle) + offset;

      ::memcpy(destPtr, srcPtr, bytes_);
    }

    void memory::copyTo(memory_v *dest,
                        const uintptr_t bytes,
                        const uintptr_t destOffset,
                        const uintptr_t srcOffset,
                        const bool asycn){
      dHandle->finish();

      const uintptr_t bytes_ = (bytes == 0) ? size : bytes;

      OCCA_CHECK((bytes_ + srcOffset) <= size,
                 "Memory has size [" << size << "],"
                 << "trying to access [ " << srcOffset << " , " << (srcOffset + bytes_) << " ]");

      OCCA_CHECK((bytes_ + destOffset) <= dest->size,
                 "Destination has size [" << dest->size << "],"
                 << "trying to access [ " << destOffset << " , " << (destOffset + bytes_) << " ]");

      void *destPtr      = ((char*) dest->handle) + destOffset;
      const void *srcPtr = ((char*) handle)       + srcOffset;

      ::memcpy(destPtr, srcPtr, bytes_);
    }

    void memory::free(){
      sys::free(handle);
      handle = NULL;
      mappedPtr = NULL;

      size = 0;
    }
  }
}
