//
// Copyright (C) 2010-: Madhav P. Desai
// All Rights Reserved.
//  
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the
// "Software"), to deal with the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
// 
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimers.
//  * Redistributions in binary form must reproduce the above
//    copyright notice, this list of conditions and the following
//    disclaimers in the documentation and/or other materials provided
//    with the distribution.
//  * Neither the names of the AHIR Team, the Indian Institute of
//    Technology Bombay, nor the names of its contributors may be used
//    to endorse or promote products derived from this Software
//    without specific prior written permission.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE CONTRIBUTORS OR COPYRIGHT HOLDERS BE LIABLE FOR
// ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS WITH THE SOFTWARE.
#ifndef __Pipes_h__
#define __Pipes_h__

#include <stdlib.h>
#include <stdint.h>

uint64_t read_uint64(const char *id);
void write_uint64(const char *id, uint64_t data);
void read_uint64_n(const char *id, uint64_t* buf, int buf_len);
void write_uint64_n(const char *id, uint64_t* buf, int buf_len);

uint32_t read_uint32(const char *id);
void write_uint32(const char *id, uint32_t data);
void read_uint32_n(const char *id, uint32_t* buf, int buf_len);
void write_uint32_n(const char *id, uint32_t* buf, int buf_len);

uint16_t read_uint16(const char *id);
void write_uint16(const char *id, uint16_t data);
void read_uint16_n(const char *id, uint16_t* buf, int buf_len);
void write_uint16_n(const char *id, uint16_t* buf, int buf_len);


uint8_t read_uint8(const char *id);
void write_uint8(const char *id, uint8_t data);
void read_uint8_n(const char *id, uint8_t* buf, int buf_len);
void write_uint8_n(const char *id, uint8_t* buf, int buf_len);

uint32_t* read_uintptr(const char *id);
void write_uintptr(const char *id, uint32_t* data);

void* read_pointer(const char *id);
void write_pointer(const char *id, void* data);

float read_float32(const char *id);
void write_float32(const char *id, float data);
void read_float32_n(const char *id, float* buf, int buf_len);
void write_float32_n(const char *id, float* buf, int buf_len);


double read_float64(const char *id);
void write_float64(const char *id, double data);
void read_float64_n(const char *id, double* buf, int buf_len);
void write_float64_n(const char *id, double* buf, int buf_len);


#endif
