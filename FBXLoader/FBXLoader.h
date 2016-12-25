// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the FBXLOADER_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// FBXLOADER_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef FBXLOADER_EXPORTS
#define FBXLOADER_API __declspec(dllexport)
#else
#define FBXLOADER_API __declspec(dllimport)
#endif

// This class is exported from the FBXLoader.dll
class FBXLOADER_API CFBXLoader {
public:
	CFBXLoader(void);
	// TODO: add your methods here.
};

extern FBXLOADER_API int nFBXLoader;

FBXLOADER_API int fnFBXLoader(void);
