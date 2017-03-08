#pragma once

#ifdef MEPHYSX_EXPORTS
#define MEPHYSX_API __declspec(dllexport)
#else
#define MEPHYSX_API __declspec(dllimport)
#endif


