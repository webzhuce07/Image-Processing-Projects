#pragma once

#ifdef APPLE
#define APPLE_API __declspec(dllexport)
#else
#define APPLE_API __declspec(dllimport)
#endif

APPLE_API void apple();