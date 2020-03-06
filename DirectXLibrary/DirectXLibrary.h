#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
#include <dsound.h>
#include <stdint.h>
#pragma once

#define DIRECTXLIBRARY_API __declspec(dllexport)

LPDIRECTSOUNDBUFFER SecondaryBuffer;
INT32 BufferSize;

void Win32InitDSound(HWND Window, INT32 SamplesPerSecond, INT32 BufferSize);

extern "C" DIRECTXLIBRARY_API void SineWave(int volume, float frequency);

extern "C" DIRECTXLIBRARY_API void SquareWave(int volume, float frequency);

extern "C" DIRECTXLIBRARY_API void CombineWave(int volume, float frequency, int volume2, float frequency2);

extern "C" DIRECTXLIBRARY_API void WriteBuffer(INT16*);

extern "C" DIRECTXLIBRARY_API void Win32InitDSound();

