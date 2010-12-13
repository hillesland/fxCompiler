#include "fxCompiler/fxCompiler.h"
#include <iostream>

using namespace keh;

int main(int argc, char ** argv)
{
	static const size_t MAX_ERROR_STRING_SIZE = 1024;
	char errorString[MAX_ERROR_STRING_SIZE];

	if(argc < 2)
	{
		std::cout << "Usage: " << argv[0] << " " << "<options> <file>\n";
		std::cout << "  /I<include>        which may be semicolon delimited list\n";
		//std::cout << "  /T<profile>\n";
		std::cout << "  /Gch               compile as child effect.\n";
		std::cout << "  /Zi                enable debugging info.\n";
		std::cout << "  /Od                disable optimizations.\n";
		return 0;
	}

	std::string includePath;
	const char * inFile = NULL;
	uint32_t flags = 0;

	for(int i=1; i<argc; ++i)
	{
		const char * arg = argv[i];
		if(arg[0]== '/')
		{
			++arg;  // Eat the forward slash.
			switch (arg[0])
			{
			case 'I':
				{
					const char * start = arg+1;
					size_t n = strlen(start);
					if(n)
					{
						if(!includePath.empty())
							includePath += ';';
						includePath += std::string(start,n);
					}
				}
				break;
			case 'G':
				{
					if(arg[1]=='c' && arg[2]=='h' && strlen(arg)==3)
						flags += FX_IS_CHILD_MASK;
				}
				break;
			case 'Z':
				{
					if(arg[1]=='i' && strlen(arg)==2)
						flags += FX_DEBUG_MASK;
				}
				break;
			case 'O':
				{
					if(arg[1]=='d' && strlen(arg)==2)
						flags += FX_NO_OPT_MASK;
				}
				break;
			default:
				std::cout << "Ignoring unknown option " << arg <<"\n";
			}
		}
		else
			inFile = argv[i];
	}

	if(!includePath.empty())
	{
		//Not worrying about quoted or paths with spaces in them yet.

		fxSetIncludePath(includePath.c_str());
	}


	std::string outname(inFile);
	outname += "o";

	if(false == fxCompile(inFile, outname.c_str(), flags, errorString, MAX_ERROR_STRING_SIZE))
	{
		std::cout << "Error:\n";
		std::cout << errorString;
		std::cout << "\n";
	}

	return 0;
}