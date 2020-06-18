#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
#include <dsound.h>
#include <stdint.h>
#include <string>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <thread>
#pragma once

#define DIRECTXLIBRARY_API __declspec(dllexport)

struct Sound{
	int16_t* soundBuffer;
	int soundBufferSize;
	int16_t* writePos;
	bool looping = false;
};

extern "C" DIRECTXLIBRARY_API struct WAV_HEADER {
	uint8_t         RIFF[4];        // RIFF Header Magic header
	uint32_t        ChunkSize;      // RIFF Chunk Size
	uint8_t         WAVE[4];        // WAVE Header
	/* "fmt" sub-chunk */
	uint8_t         fmt[4];         // FMT header
	uint32_t        Subchunk1Size;  // Size of the fmt chunk
	uint16_t        AudioFormat;    // Audio format 1=PCM,6=mulaw,7=alaw,     257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
	uint16_t        NumOfChan;      // Number of channels 1=Mono 2=Sterio
	uint32_t        SamplesPerSec;  // Sampling Frequency in Hz
	uint32_t        bytesPerSec;    // bytes per second
	uint16_t        blockAlign;     // 2=16-bit mono, 4=16-bit stereo
	uint16_t        bitsPerSample;  // Number of bits per sample
	/* "data" sub-chunk */
	uint8_t         Subchunk2ID[4]; // "data"  string
	uint32_t        Subchunk2Size;  // Sampled data length
};

extern "C" DIRECTXLIBRARY_API struct WAV_FILE {
	WAV_HEADER wavHeader;
	int16_t* data;
};

LPDIRECTSOUNDBUFFER SecondaryBuffer;
const INT32 SampleSize = 44100;
const INT32 BufferSizeBytes = SampleSize * 4;
std::vector<Sound> sounds;
std::thread soundManagerThread;

void StartBufferManagerThread();
void BufferManagerThread();
void Win32InitDSound(HWND Window);

extern "C" DIRECTXLIBRARY_API INT16* SineWave(int volume, float frequency);

extern "C" DIRECTXLIBRARY_API void SquareWave(int volume, float frequency);

extern "C" DIRECTXLIBRARY_API void CombineWave(int volume, float frequency, int volume2, float frequency2);

extern "C" DIRECTXLIBRARY_API void WriteBuffer(INT16* SoundData, DWORD writePos, INT32 writeLengthBytes);

extern "C" DIRECTXLIBRARY_API void AddSound(INT16* SoundData, uint32_t soundSize, bool looping);

extern "C" DIRECTXLIBRARY_API void Win32InitDSound();

extern "C" DIRECTXLIBRARY_API WAV_FILE LoadWavHeader(std::string file);

