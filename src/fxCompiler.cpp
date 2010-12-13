#include "fxCompiler/fxCompiler.h"

#include <d3dx10.h>
#undef min
#undef max

#include <string>
#include <fstream>
#include <vector>
#include <assert.h>


using namespace std;

namespace keh
{

	// Yucky staticness.
	static std::vector<std::string> sIncludeDirs;

	KEH_EXPORT void KEH_STDCALL fxSetIncludePath(const char * includePath)
	{
		std::string s(includePath);

		//size_t p = 0;
		//size_t next = 0;
		//while(next = s.find_first_of(';',p) != std::string::npos)
		//for(size_t p = 0; p = s.find_first_of(';',p); p!=std::string::npos)
		for(
			size_t p = 0, next = s.find_first_of(';'); 
			p < s.size(); 
			next = s.find_first_of(';',p)
		)
		{
			if(next == std::string::npos)
				next = s.size();
			sIncludeDirs.push_back(std::string(s,p,next-p));
			p = next+1;
		}
	}

	static bool CompileShader(const char* filename, uint32_t flags, ID3D10Blob** error, ID3D10Blob** out);

	KEH_EXPORT bool KEH_STDCALL fxCompile(const char * inFile, const char * outFile, uint32_t flags, char * errorString, uint32_t maxErrorLength)
	{

		ID3D10Blob* pErrorBlob;
		ID3D10Blob* pShaderBlob;

		if(false == CompileShader(inFile, flags, &pErrorBlob, &pShaderBlob))
		{
			strncpy(errorString, (const char *) pErrorBlob->GetBufferPointer(), std::min<size_t>(maxErrorLength-1, pErrorBlob->GetBufferSize()));
			errorString[maxErrorLength-1] = 0;
			return false;
		}
		else
		{
			ofstream os(outFile);
			os.write( (char *) pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize());
			os.close();
			return ! os.fail();
		}
	}


bool FindInclude(D3D10_INCLUDE_TYPE /*includeType*/, const char * base, ifstream& is)
{
	is.open(base);
	if(is.good())
		return true;

	size_t n = sIncludeDirs.size();
	for(size_t i=0; i<n; ++i)
	{
		is.clear();
		std::string path(sIncludeDirs[i]);
		path += '\\';
		path += base;

		is.open(path.c_str());
		if(is.good())
			return true;
	}
	return false;

}

class Include : public ID3DInclude
{
public:
	typedef CHAR StreamCharType;
	STDMETHOD(Open)(D3D10_INCLUDE_TYPE includeType,
		LPCSTR pFileName,
		LPCVOID /*pParentData*/,
		LPCVOID *ppData,
		UINT *pBytes
	)
	{
		//assert(IncludeType == D3D10_INCLUDE_LOCAL);


		//std::string filename(pFileName);
		//basic_ifstream<StreamCharType> is(filename.c_str());

		basic_ifstream<StreamCharType> is;
		if(! FindInclude(includeType, pFileName, is))
		{
			//Make a message about not finding an included file.
			return E_FAIL;
		}

		is.seekg(0,ios_base::end);
		streampos pos = is.tellg();
		UINT size = pos;
		is.seekg(0,ios_base::beg);
		StreamCharType * effectBuffer = new StreamCharType[size];
		is.read(effectBuffer,size);

		//Clean out bad characters.
		for(UINT i=0; i<size; ++i)
			if(effectBuffer[i] < 0 || effectBuffer[i] > 127) // Only works with CHAR, obviously.
				effectBuffer[i] = ' ';

		*ppData = (const void*) effectBuffer;
		*pBytes = size;

		return S_OK;
	}
	STDMETHOD(Close)(LPCVOID pData)
	{
		delete [] (StreamCharType*)pData;

		return S_OK;
	}


};

#if defined( DEBUG ) || defined( _DEBUG)
	const DWORD COMPILER_FLAGS = D3D10_SHADER_ENABLE_STRICTNESS | D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION;
#else
	const DWORD COMPILER_FLAGS = D3D10_SHADER_ENABLE_STRICTNESS;
#endif

static bool CompileShader(const char* filename, uint32_t flags, ID3D10Blob** error, ID3D10Blob** out)
{
	HRESULT hr;

	Include include;

	bool isChild = (flags & FX_IS_CHILD_MASK) != 0;

	UINT compilerFlags = D3D10_SHADER_ENABLE_STRICTNESS;
	if(flags & FX_DEBUG_MASK)
		compilerFlags |= D3D10_SHADER_DEBUG;
	if(flags & FX_NO_OPT_MASK)
		compilerFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;

	hr = D3DX10CompileFromFileA(
		filename, //__in   LPCTSTR pSrcFile
		NULL, //__in   const D3D10_SHADER_MACRO *pDefines
		&include, //__in   LPD3D10INCLUDE pInclude
		NULL, // __in   LPCSTR pFunctionName,
		"fx_4_0", //__in   LPCSTR pProfile,
		COMPILER_FLAGS, // __in   UINT Flags1,
		isChild ? D3D10_EFFECT_COMPILE_CHILD_EFFECT : 0, //__in   UINT Flags2,
		NULL, // __in   ID3DX10ThreadPump *pPump,
		out, //__out  ID3D10Blob **ppShader,
		error, //__out  ID3D10Blob **ppErrorMsgs,
		NULL //__out  HRESULT *pHResult
		);

	if(FAILED(hr))
	{
		return false;
	}
	else
		return true;
}

}


