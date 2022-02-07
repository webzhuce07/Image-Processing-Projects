#pragma once

#ifdef ORANGE
#define ORANGE_API __declspec(dllexport)
#else
#define ORANGE_API __declspec(dllimport)
#endif

ORANGE_API void orange();