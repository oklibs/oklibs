// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKBASE_ARCHITECTURE_DEFINES_HPP
#define OKBASE_ARCHITECTURE_DEFINES_HPP

#include "okbase/core_defines.hpp"

#if defined(__ARM_ARCH) || defined(__TARGET_ARCH_ARM) || defined(__TARGET_ARCH_THUMB) || defined(_M_ARM) || \
    defined(__arm__) || defined(__arm64) || defined(__thumb__) || defined(_M_ARM64) || defined(__aarch64__) || \
    defined(__AARCH64EL__) || defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) || \
    defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6Z__) || defined(__ARM_ARCH_6KZ__) || \
    defined(__ARM_ARCH_6T2__) || defined(__ARM_ARCH_5TE__) || defined(__ARM_ARCH_5TEJ__) || \
    defined(__ARM_ARCH_4T__) || defined(__ARM_ARCH_4__)
	#define OKL_ARCH_ARM_AVAILABLE 1
	#if defined(__ARM_ARCH)
		#define OKL_ARCH_ARM_VERSION OKL_ENCODE_VERSION(__ARM_ARCH, 0, 0)
	#elif defined(__TARGET_ARCH_ARM)
		#define OKL_ARCH_ARM_VERSION OKL_ENCODE_VERSION(__TARGET_ARCH_ARM, 0, 0)
	#elif defined(__TARGET_ARCH_THUMB)
		#define OKL_ARCH_ARM_VERSION OKL_ENCODE_VERSION(__TARGET_ARCH_THUMB, 0, 0)
	#elif defined(_M_ARM)
		#define OKL_ARCH_ARM_VERSION OKL_ENCODE_VERSION(_M_ARM, 0, 0)
	#elif defined(__arm64) || defined(_M_ARM64) || defined(__aarch64__) || defined(__AARCH64EL__)
		#define OKL_ARCH_ARM_VERSION OKL_ENCODE_VERSION(8, 0, 0)
	#elif defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__)
		#define OKL_ARCH_ARM_VERSION OKL_ENCODE_VERSION(7, 0, 0)
	#elif defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6Z__) || defined(__ARM_ARCH_6KZ__) || defined(__ARM_ARCH_6T2__)
		#define OKL_ARCH_ARM_VERSION OKL_ENCODE_VERSION(6, 0, 0)
	#elif defined(__ARM_ARCH_5TE__) || defined(__ARM_ARCH_5TEJ__)
		#define OKL_ARCH_ARM_VERSION OKL_ENCODE_VERSION(5, 0, 0)
	#elif defined(__ARM_ARCH_4T__) || defined(__ARM_ARCH_4__)
		#define OKL_ARCH_ARM_VERSION OKL_ENCODE_VERSION(4, 0, 0)
	#else
		#define OKL_ARCH_ARM_VERSION (1)
	#endif
#else
	#define OKL_ARCH_ARM_AVAILABLE 0
	#define OKL_ARCH_ARM_VERSION (0)
#endif

#if defined(__riscv)
	#define OKL_ARCH_RISCV_AVAILABLE 1
	#define OKL_ARCH_RISCV_VERSION (1)
#else
	#define OKL_ARCH_RISCV_AVAILABLE 0
	#define OKL_ARCH_RISCV_VERSION (0)
#endif

#if defined(__wasm__)
	#define OKL_ARCH_WASM_AVAILABLE 1
	#define OKL_ARCH_WASM_VERSION (1)
#else
	#define OKL_ARCH_WASM_AVAILABLE 0
	#define OKL_ARCH_WASM_VERSION (0)
#endif

#if defined(i386) || defined(__i386__) || defined(__i486__) || defined(__i586__) || defined(__i686__) || \
    defined(__i386) || defined(_M_IX86) || defined(_X86_) || defined(__THW_INTEL__) || defined(__I86__) || \
    defined(__INTEL__) || defined(__x86_64) || defined(__x86_64__) || defined(__amd64__) || defined(__amd64) || \
    defined(_M_X64)
	#define OKL_ARCH_X86_AVAILABLE 1
	#if defined(__I86__)
		#define OKL_ARCH_X86_VERSION OKL_ENCODE_VERSION(__I86__, 0, 0)
	#elif defined(_M_IX86)
		#define OKL_ARCH_X86_VERSION OKL_ENCODE_VERSION((_M_IX86) / 100 % 100, 0, 0)
	#elif defined(__i686__)
		#define OKL_ARCH_X86_VERSION OKL_ENCODE_VERSION(6, 0, 0)
	#elif defined(__i586__)
		#define OKL_ARCH_X86_VERSION OKL_ENCODE_VERSION(5, 0, 0)
	#elif defined(__i486__)
		#define OKL_ARCH_X86_VERSION OKL_ENCODE_VERSION(4, 0, 0)
	#elif defined(__i386__)
		#define OKL_ARCH_X86_VERSION OKL_ENCODE_VERSION(3, 0, 0)
	#else
		#define OKL_ARCH_X86_VERSION (1)
	#endif
#else
	#define OKL_ARCH_X86_AVAILABLE 0
	#define OKL_ARCH_X86_VERSION (0)
#endif

#if (OKL_ARCH_ARM_AVAILABLE + OKL_ARCH_X86_AVAILABLE + OKL_ARCH_RISCV_AVAILABLE + OKL_ARCH_WASM_AVAILABLE) <= 0
	#error "okbase: No architecture detected."
#endif

// Detect the main architecture.
#if OKL_ARCH_ARM_AVAILABLE
	#define OKL_ARCH_ARM 1
#elif OKL_ARCH_X86_AVAILABLE
	#define OKL_ARCH_X86 1
#elif OKL_ARCH_RISCV_AVAILABLE
	#define OKL_ARCH_RISCV 1
#elif OKL_ARCH_WASM_AVAILABLE
	#define OKL_ARCH_WASM 1
#endif

#if !defined(OKL_ARCH_ARM)
	#define OKL_ARCH_ARM 0
#endif
#if !defined(OKL_ARCH_RISCV)
	#define OKL_ARCH_RISCV 0
#endif
#if !defined(OKL_ARCH_WASM)
	#define OKL_ARCH_WASM 0
#endif
#if !defined(OKL_ARCH_X86)
	#define OKL_ARCH_X86 0
#endif

#if (OKL_ARCH_ARM + OKL_ARCH_X86 + OKL_ARCH_RISCV + OKL_ARCH_WASM) != 1
	#error "Ok: Exactly one main architecture must be defined."
#endif

#endif
