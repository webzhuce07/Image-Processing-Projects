#pragma once

#ifdef PEAR
#define PEAR_API __declspec(dllexport)
#else
#define PEAR_API __declspec(dllimport)
#endif

PEAR_API void pear();