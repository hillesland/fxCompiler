#ifndef KEH_FX_COMPILER_H
#define KEH_FX_COMPILER_H

#include "kehCommon/kehStdInt.h"

#define KEH_STDCALL __stdcall

#ifdef FX_COMPILER_DLL
#  define KEH_EXPORT extern "C" __declspec(dllexport)
#else
#  define KEH_EXPORT
#endif

namespace keh
{
	// Semicolon delimited list.
	KEH_EXPORT void KEH_STDCALL fxSetIncludePath(const char * includePath);

	const uint32_t FX_IS_CHILD_MASK = 1;
	const uint32_t FX_DEBUG_MASK =    1 << 1;
	const uint32_t FX_NO_OPT_MASK =   1 << 2;
	KEH_EXPORT bool KEH_STDCALL fxCompile(const char * inFile, const char * outFile, uint32_t flags, char * errorString, uint32_t maxErrorLength);
}

#endif