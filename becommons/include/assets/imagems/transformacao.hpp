/** @copyright 
MIT License
Copyright (c) 2025 Daniel Oliveira

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE. 
*/
/**
 * @file transformacao.hpp
 */

inline unsigned char transformacao_png[] = {
  0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 0x00, 0x00, 0x00, 0x0d,
  0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x30,
  0x08, 0x06, 0x00, 0x00, 0x00, 0x57, 0x02, 0xf9, 0x87, 0x00, 0x00, 0x00,
  0x09, 0x70, 0x48, 0x59, 0x73, 0x00, 0x00, 0x0b, 0x13, 0x00, 0x00, 0x0b,
  0x13, 0x01, 0x00, 0x9a, 0x9c, 0x18, 0x00, 0x00, 0x01, 0xc3, 0x49, 0x44,
  0x41, 0x54, 0x78, 0x9c, 0xed, 0xd9, 0xbd, 0x4a, 0xc3, 0x50, 0x1c, 0x86,
  0xf1, 0xa7, 0x82, 0x20, 0xe8, 0x22, 0x5d, 0x1c, 0xd4, 0xc1, 0xc1, 0x0f,
  0x04, 0x3f, 0x6a, 0x67, 0x07, 0x71, 0xf0, 0x02, 0xf4, 0x16, 0x1c, 0x1c,
  0xd4, 0x49, 0x47, 0xdd, 0x1c, 0x3a, 0x89, 0x57, 0xa0, 0xa3, 0xe0, 0x24,
  0x78, 0x03, 0x3a, 0xeb, 0xa8, 0x37, 0x60, 0x1d, 0x54, 0x04, 0x07, 0x07,
  0x29, 0x7d, 0xa5, 0x90, 0x21, 0x48, 0x92, 0x93, 0x9c, 0xa4, 0x39, 0x27,
  0xd0, 0xc0, 0x7f, 0x6a, 0x93, 0x3e, 0xbf, 0x7c, 0x35, 0xa5, 0x08, 0x46,
  0x05, 0x9b, 0x11, 0x53, 0xa7, 0x0a, 0x8b, 0x60, 0x5e, 0xa0, 0x88, 0xd9,
  0xa2, 0x0a, 0x8b, 0x2a, 0x04, 0x10, 0x4c, 0x09, 0x26, 0x2a, 0x05, 0x10,
  0xcc, 0x08, 0x0e, 0x04, 0x0f, 0x82, 0xae, 0x60, 0xd6, 0x6b, 0x80, 0x60,
  0x48, 0xb0, 0x26, 0x38, 0x15, 0x3c, 0xff, 0x6b, 0xfa, 0x14, 0xd4, 0xbc,
  0x03, 0x08, 0x46, 0x82, 0x1b, 0xc7, 0xb9, 0xa0, 0x1d, 0xd3, 0xd3, 0x9b,
  0xbb, 0xa8, 0x95, 0x9d, 0x00, 0x04, 0xe3, 0x82, 0x1d, 0xc1, 0x95, 0xe0,
  0x3b, 0x21, 0x3a, 0x3c, 0x27, 0x4e, 0x01, 0x82, 0x69, 0xc1, 0xae, 0xe0,
  0x56, 0xf0, 0x9b, 0x32, 0x3a, 0xb9, 0xa9, 0xdf, 0x00, 0xc1, 0xa2, 0xe0,
  0x38, 0x74, 0x11, 0xca, 0x72, 0xba, 0x91, 0xdf, 0x4d, 0xfd, 0x04, 0xf4,
  0x2e, 0x38, 0xc1, 0x45, 0x8e, 0x68, 0x85, 0xe6, 0x25, 0xee, 0x43, 0xfa,
  0x7d, 0x04, 0x8a, 0x42, 0x5c, 0x3a, 0x01, 0x14, 0x88, 0xd8, 0x73, 0x06,
  0x28, 0x08, 0xd1, 0x70, 0x0a, 0xc8, 0x89, 0xf8, 0x11, 0x0c, 0x3b, 0x07,
  0x84, 0x10, 0xd7, 0x19, 0x01, 0xf7, 0x49, 0x1b, 0x2c, 0x1b, 0xd0, 0x08,
  0x1e, 0x09, 0xb2, 0x00, 0x5a, 0x5e, 0x00, 0x2c, 0xe3, 0x25, 0xd8, 0x76,
  0x0e, 0x10, 0xac, 0x5a, 0xc6, 0x4b, 0x30, 0xe9, 0x14, 0x20, 0x68, 0x0a,
  0xbe, 0x12, 0x02, 0x9f, 0x12, 0x5e, 0x7b, 0x35, 0x6d, 0xbc, 0xdf, 0x5f,
  0x64, 0xa6, 0x3d, 0xdf, 0x32, 0xdc, 0x9d, 0x6e, 0x9c, 0x01, 0x04, 0x2b,
  0x82, 0x8f, 0x34, 0x17, 0xa7, 0xe2, 0x11, 0x47, 0x4e, 0x00, 0x59, 0xe2,
  0x0d, 0x88, 0x75, 0xca, 0x06, 0xd8, 0xc4, 0xc7, 0x20, 0x3a, 0x82, 0x31,
  0xca, 0x04, 0xe4, 0x89, 0x8f, 0x40, 0x3c, 0x62, 0x5a, 0x8a, 0x04, 0xa4,
  0x88, 0x3f, 0xcb, 0xb0, 0xad, 0x9a, 0x60, 0xa3, 0x34, 0x80, 0x60, 0x59,
  0xf0, 0x9e, 0x67, 0xcf, 0x5b, 0x2d, 0x45, 0x00, 0x9c, 0xc5, 0x17, 0x01,
  0x10, 0x2c, 0x39, 0x8b, 0xcf, 0x0b, 0x70, 0x1e, 0x9f, 0x07, 0x20, 0x58,
  0x10, 0xbc, 0x39, 0x8d, 0xb7, 0x05, 0x04, 0xeb, 0xb8, 0x8f, 0xb7, 0x01,
  0x78, 0x15, 0x9f, 0x15, 0xe0, 0x5d, 0x7c, 0x16, 0x40, 0xf0, 0xbe, 0xb6,
  0x57, 0xf1, 0x69, 0x01, 0x82, 0x39, 0x2f, 0xe3, 0xd3, 0x00, 0xbc, 0x8e,
  0x37, 0x01, 0xbc, 0x8f, 0x37, 0x00, 0xf6, 0x7b, 0x3f, 0xe7, 0xbc, 0x8e,
  0x37, 0x00, 0x3a, 0x45, 0x3c, 0x55, 0xba, 0x04, 0xc8, 0xeb, 0x3d, 0x6f,
  0x09, 0xf0, 0x2b, 0x3e, 0x23, 0xc0, 0xbf, 0x78, 0xcb, 0x53, 0xc8, 0xa7,
  0x39, 0x1c, 0x00, 0x34, 0x38, 0x02, 0x0c, 0x4e, 0xa1, 0x7a, 0xf0, 0x37,
  0x68, 0x15, 0xa7, 0xf9, 0x07, 0xa6, 0x49, 0x7b, 0x31, 0xef, 0xa0, 0xbc,
  0xa6, 0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4e, 0x44, 0xae, 0x42, 0x60,
  0x82
};
unsigned int transformacao_png_len = 529;
