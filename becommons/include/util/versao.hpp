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
 * @file vercao.hpp
 */

/** Definição da verção */
#ifndef BUBBLE_VERSAO
#define BUBBLE_VERSAO

#define VERSAO_STATUS_ALPHA   0x01
#define VERSAO_STATUS_BETA    0x02
#define VERSAO_STATUS_RC      0x03
#define VERSAO_STATUS_STABLE  0xFF

#define BUBBLE_VERSION_MAJOR_STR   "1"
#define BUBBLE_VERSION_MINOR_STR   "1"
#define BUBBLE_VERSION_PATCH_STR   "0"
#define BUBBLE_STATUS               VERSAO_STATUS_BETA

#if BUBBLE_STATUS == VERSAO_STATUS_ALPHA
    #define BUBBLE_VERSION_STATUS_STR "alpha"
#elif BUBBLE_STATUS == VERSAO_STATUS_BETA
    #define BUBBLE_VERSION_STATUS_STR "beta"
#elif BUBBLE_STATUS == VERSAO_STATUS_RC
    #define BUBBLE_VERSION_STATUS_STR "rc"
#elif BUBBLE_STATUS == VERSAO_STATUS_STABLE
    #define BUBBLE_VERSION_STATUS_STR "stable"
#else
    #define BUBBLE_VERSION_STATUS_STR "desconhecido"
#endif

#define BUBBLE_VERSAO_COMPLETA_STR \
    BUBBLE_VERSION_MAJOR_STR "." BUBBLE_VERSION_MINOR_STR "." BUBBLE_VERSION_PATCH_STR "-" BUBBLE_VERSION_STATUS_STR
#endif
