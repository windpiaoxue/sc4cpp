// Copyright (c) 2021 smh <windpiaoxue@foxmail.com>
// All rights reserved
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
#pragma once

#include <type_traits>
#include <Windows.h>
// #include <intrin.h>
#include <winternl.h>

#ifdef _DEBUG
#define SC_DEBUG
#endif  // #ifdef _DEBUG

#ifdef _WIN64
#define SC_WIN64
#endif

#define SC_CONSTEXPR   constexpr
#define SC_NOINLINE    __declspec(noinline)
#define SC_FORCEINLINE __forceinline

#define SC_NAKEDFUNC __declspec(naked)
#define SC_ASM       __asm
#define SC_EMIT(c)   __asm _emit(c)

#define SC_DLL_IMPORT __declspec(dllimport)
#define SC_DLL_EXPORT __declspec(dllexport)

#define SC_EXTERN_C_BEGIN extern "C" {
#define SC_EXTERN_C_END   }

namespace SC {
// https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
template <typename Converter>
SC_FORCEINLINE SC_CONSTEXPR DWORD Hash(PCSTR lpName) {
    DWORD dwHash = 2166136261u;
    for (; *lpName != '\0'; ++lpName) {
        dwHash = (dwHash ^ (BYTE)Converter()(*lpName)) * 16777619ull;
    }
    return dwHash;
}

SC_FORCEINLINE SC_CONSTEXPR DWORD Hash(PCSTR lpName) {
    struct Converter {
        SC_CONSTEXPR Converter() {}
        SC_CONSTEXPR CHAR operator()(CHAR c) const { return c; }
    };
    return Hash<Converter>(lpName);
}

SC_FORCEINLINE SC_CONSTEXPR DWORD HashI(PCSTR lpName) {
    struct Converter {
        SC_CONSTEXPR Converter() {}
        SC_CONSTEXPR CHAR operator()(CHAR c) const {
            return c >= 'A' && c <= 'Z' ? c + ('a' - 'A') : c;
        }
    };
    return Hash<Converter>(lpName);
}

SC_FORCEINLINE PPEB GetPEB() {
#ifdef _WIN64
    return (PPEB)__readgsqword(offsetof(TEB, ProcessEnvironmentBlock));
#else
    return (PPEB)__readfsdword(offsetof(TEB, ProcessEnvironmentBlock));
#endif  // _WIN64
}

SC_FORCEINLINE PIMAGE_NT_HEADERS GetNTHeaders(PVOID lpDLLBase) {
    PIMAGE_DOS_HEADER lpDOSHeader = (PIMAGE_DOS_HEADER)lpDLLBase;
    return (PIMAGE_NT_HEADERS)((PBYTE)lpDLLBase + lpDOSHeader->e_lfanew);
}

SC_FORCEINLINE PLDR_DATA_TABLE_ENTRY GetDataTableEntry(PLIST_ENTRY lpList) {
    SIZE_T zuEntryOffset = offsetof(LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);
    return (PLDR_DATA_TABLE_ENTRY)((LPBYTE)lpList - zuEntryOffset);
}

SC_FORCEINLINE PVOID GetProcAddressByHash(DWORD dwDLLHash, DWORD dwProcHash) {
    PLIST_ENTRY lpSentryNode = GetPEB()->Ldr->InMemoryOrderModuleList.Flink;
    PLIST_ENTRY lpIterNode = lpSentryNode;
    do {
        PLDR_DATA_TABLE_ENTRY lpDLLEntry = GetDataTableEntry(lpIterNode);
        lpIterNode = lpIterNode->Flink;
        LPSTR lpDLLBase = (LPSTR)lpDLLEntry->DllBase;
        if (lpDLLBase == NULL) {
            continue;
        }
        PIMAGE_NT_HEADERS lpNTHeaders = GetNTHeaders(lpDLLBase);
        DWORD dwExportDirectoryRAV =
            lpNTHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
        if (dwExportDirectoryRAV == 0) {
            continue;
        }
        PIMAGE_EXPORT_DIRECTORY lpExportDirectory =
            (PIMAGE_EXPORT_DIRECTORY)(lpDLLBase + dwExportDirectoryRAV);
        if (HashI(lpDLLBase + lpExportDirectory->Name) != dwDLLHash) {
            continue;
        }
        PDWORD lpNameRAVs = (PDWORD)(lpDLLBase + lpExportDirectory->AddressOfNames);
        PWORD lpOrdinals = (PWORD)(lpDLLBase + lpExportDirectory->AddressOfNameOrdinals);
        PDWORD lpProcRAVs = (PDWORD)(lpDLLBase + lpExportDirectory->AddressOfFunctions);
        for (DWORD dwIdx = 0; dwIdx < lpExportDirectory->NumberOfNames; ++dwIdx) {
            if (Hash(lpDLLBase + lpNameRAVs[dwIdx]) == dwProcHash) {
                // FIXME: DLL Function Forwarding
                return lpDLLBase + lpProcRAVs[lpOrdinals[dwIdx]];
            }
        }
    } while (lpIterNode != lpSentryNode);
    __debugbreak();
    return NULL;  // No return
}

// For Compile-time calculation
template <DWORD dwDLLHash, DWORD dwProcHash>
SC_FORCEINLINE PVOID GetProcAddressByHash() {
    return GetProcAddressByHash(dwDLLHash, dwProcHash);
}

// Position Independent String
template <typename CharType, typename Indices>
struct PIString;

template <typename CharType, size_t... Indices>
struct PIString<CharType, std::index_sequence<Indices...>> {
    CharType szBuffer_[sizeof...(Indices)];
    SC_FORCEINLINE SC_CONSTEXPR explicit PIString(const CharType (&szLiteral)[sizeof...(Indices)])
        : szBuffer_{(szLiteral[Indices])...} {}
};
}  // namespace SC

#ifdef SC_WIN64
#define SC_BEGIN_CODE                                                                              \
    SC_DLL_EXPORT VOID SCBegin() { SCMain(NULL); }
#else
// clang-format off
#define SC_BEGIN_CODE                                                                              \
    SC_DLL_EXPORT SC_NAKEDFUNC VOID SCBegin() {                                                    \
        /* CALL $+5 */                                                                             \
        SC_EMIT(0xE8) SC_EMIT(0x00) SC_EMIT(0x00) SC_EMIT(0x00) SC_EMIT(0x00)                      \
        SC_ASM POP EAX                                                                             \
        SC_ASM LEA EAX, [EAX - 5]                                                                  \
        SC_ASM LEA ECX, [SCBegin]                                                                  \
        SC_ASM NEG ECX                                                                             \
        SC_ASM LEA EAX, [EAX + ECX + SCMain]                                                       \
        SC_ASM PUSH EAX                                                                            \
        SC_ASM CALL EAX                                                                            \
        SC_ASM RET                                                                                 \
    }
// clang-format on
#endif  // SC_WIN64

#define SC_MAIN_BEGIN()                                                                            \
    SC_EXTERN_C_BEGIN                                                                              \
    SC_DLL_EXPORT VOID WINAPI SCMain(ULONG_PTR SCMainVA);                                          \
    SC_BEGIN_CODE                                                                                  \
    SC_DLL_EXPORT VOID WINAPI SCMain(ULONG_PTR SCMainVA)

#define SC_MAIN_END()                                                                              \
    SC_DLL_EXPORT VOID SCEnd() { __debugbreak(); }                                                 \
    SC_EXTERN_C_END

#define SC_PISTRINGA(szLiteralA)                                                                   \
    (::SC::PIString<CHAR, std::make_index_sequence<_countof(szLiteralA)>>(szLiteralA).szBuffer_)
#define SC_PISTRINGW(szLiteralW)                                                                   \
    (::SC::PIString<WCHAR, std::make_index_sequence<_countof(szLiteralW)>>(szLiteralW).szBuffer_)

#ifdef SC_WIN64
#define SC_PIFUNCTION(fn) (fn)
#else
// Must be invoked in SCMain.
#define SC_PIFUNCTION(fn) ((decltype(fn)*)(((ULONG_PTR)(fn) - (ULONG_PTR)SCMain) + SCMainVA))
#endif  // SC_WIN64

#define SC_GET_API_ADDRESS(szDLLName, szAPIName)                                                   \
    (::SC::GetProcAddressByHash<::SC::HashI(szDLLName), ::SC::Hash(szAPIName)>())

#define SC_IMPORT_API(szDLLName, fnAPIName)                                                        \
    auto fnAPIName = (decltype(::fnAPIName)*)SC_GET_API_ADDRESS(szDLLName, #fnAPIName)

#define SC_IMPORT_API_BATCH_BEGIN()                                                                \
    SC_IMPORT_API("Kernel32.dll", LoadLibraryA);                                                   \
    SC_IMPORT_API("Kernel32.dll", GetProcAddress)

#define SC_IMPORT_API_BATCH(szDLLName, fnAPIName)                                                  \
    auto fnAPIName = (decltype(::fnAPIName)*)(GetProcAddress(                                      \
        LoadLibraryA(SC_PISTRINGA(szDLLName)), SC_PISTRINGA(#fnAPIName)))

#define SC_IMPORT_API_BATCH_END()
