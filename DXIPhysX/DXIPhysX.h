// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the DXILUADLL_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// DXILUADLL_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef DXIPHYSX_EXPORTS
#define DXIPHYSX_API __declspec(dllexport)
#else
#define DXIPHYSX_API __declspec(dllimport)
#endif

#include <dxi/core/Game.h>

extern "C" DXIPHYSX_API bool DXILoader( dxi::core::Game * game, const qxml::Document * doc );