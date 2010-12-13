 --  This is intended to be added to an individual project.
 --  it's also assumed that DXSDK directories and RELEASE_SYMBOLS
 --  have been defined.
 
configuration {}

libdirs { DXSDK_LIB_DIR }

links {
	"d3d10",
	"dxerr",
	"dxguid",
	"winmm",
	"comctl32"
}

configuration "Debug"
	links {
		"d3dx10d",
		"d3dx9d"
	}

configuration "Release"
	if RELEASE_SYMBOLS then
		links {
			"d3dx10d",
			"d3dx9d"
		}
	else
		links {
			"d3dx10",
			"d3dx9"
		}
	end


configuration {}

