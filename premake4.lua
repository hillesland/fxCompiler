DXSDK_DIR = "../DXJune2010"
DXSDK_INC_DIR = DXSDK_DIR .. "/Include"
DXSDK_LIB_DIR = DXSDK_DIR .. "/Lib/x86"

RELEASE_SYMBOLS = true

solution "fxCompiler"
	configurations { "Debug", "Release" }

	includedirs { "include" }

	configuration "vs*"
	   defines {"WIN32", "_WINDOWS", "_CRT_SECURE_NO_WARNINGS"}
	   flags {"NoMinimalRebuild", "Unicode"}
	   flags {"EnableSSE2", "FloatFast"}

	configuration "Debug"
		targetdir "bin/Debug"
		defines {"DEBUG"}
		flags {"Symbols"}

	configuration "Release"
		targetdir "bin/Release"
		defines {"NDEBUG"}
		flags {"OptimizeSpeed"}
		if RELEASE_SYMBOLS then
		   flags {"Symbols"}
		end

	project "fxCompilerLib"
		kind "StaticLib"
		language "C++"
		dofile "DXCompile.lua"

		files { "src/*.cpp", "include/fxCompiler/*.h" }

	project "fxCompilerDll"
		kind "SharedLib"
		language "C++"
		dofile "DXCompile.lua"
		dofile "DXLink.lua"
		defines { "FX_COMPILER_DLL" }

		files { "src/*.cpp", "include/fxCompiler/*.h" }

	project "fxCompilerCmd"
		kind "ConsoleApp"
		targetname "fxCompiler"
		language "C++"
		dofile "DXLink.lua"

		-- For DLL testing.
		-- defines { "FX_COMPILER_DLL" }
		-- links {"fxCompilerDll"}

		links {"fxCompilerLib"}

		files { "cmd/*.h", "cmd/*.cpp" }




		


