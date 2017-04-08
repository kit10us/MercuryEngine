// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the MELUADLL_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// MELUADLL_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef MELUADLL_EXPORTS
#define MELUADLL_API __declspec(dllexport)
#else
#define MELUADLL_API __declspec(dllimport)
#endif

