#include "pch.h"
#include "DirectXLibrary.h"
#define M_PI 3.14159265358979323846264
#define DIRECT_SOUND_CREATE(name)HRESULT WINAPI name(LPCGUID pcGuidDevice, LPDIRECTSOUND *ppDS, LPUNKNOWN pUnkOuter)
typedef DIRECT_SOUND_CREATE(direct_sound_create);

using std::fstream;

void Win32InitDSound(HWND Window, INT32 SamplesPerSecond, INT32 bufferSize) {
	BufferSize = bufferSize;
	//Load the library
	std::cout << "attempting to load library" << std::endl;
	HMODULE DSoundLibrary = LoadLibraryA("dsound.dll");

	if (DSoundLibrary) {
		std::cout << "lib loaded" << std::endl;
		//Get DirectSound object
		direct_sound_create* DirectSoundCreate = (direct_sound_create*)GetProcAddress(DSoundLibrary, "DirectSoundCreate");
		LPDIRECTSOUND DirectSound;

		HRESULT res = DirectSoundCreate(0, &DirectSound, 0);
		if (DirectSoundCreate && SUCCEEDED(res)) {
			std::cout << "created sound object" << std::endl;
			WAVEFORMATEX WaveFormat = {};
			WaveFormat.wFormatTag = WAVE_FORMAT_PCM;
			WaveFormat.nChannels = 2;
			WaveFormat.nSamplesPerSec = SamplesPerSecond;
			WaveFormat.wBitsPerSample = 16;
			WaveFormat.cbSize = 0;
			WaveFormat.nBlockAlign = (WaveFormat.nChannels * WaveFormat.wBitsPerSample) / 8;
			WaveFormat.nAvgBytesPerSec = WaveFormat.nSamplesPerSec * WaveFormat.nBlockAlign;

			if (SUCCEEDED(DirectSound->SetCooperativeLevel(Window, DSSCL_PRIORITY)))
			{
				std::cout << "set cooperativeLevel Window" << std::endl;
				DSBUFFERDESC BufferDescription = {0};
				BufferDescription.dwSize = sizeof(BufferDescription);
				BufferDescription.dwFlags = DSBCAPS_PRIMARYBUFFER;

				std::cout << "attempting creation of primary buffer" << std::endl;
				LPDIRECTSOUNDBUFFER PrimaryBuffer;
				if (SUCCEEDED(DirectSound->CreateSoundBuffer(&BufferDescription, &PrimaryBuffer, 0))) {
					std::cout << "Primary buffer created" << std::endl;
					unsigned short code = PrimaryBuffer->SetFormat(&WaveFormat);
					if (SUCCEEDED(code)) {
						OutputDebugStringA("Primary buffer format set");
						std::cout << "Primary buffer format set" << std::endl;
					}
					else
					{
						std::cout << "Primary buffer format set failed with error: " << code << std::endl;
					}
				}

				//Create the secondary buffer
				std::cout << "attempting creation of secondary buffer" << std::endl;
				DSBUFFERDESC SecondaryBufferDescription = {0};
				SecondaryBufferDescription.dwSize = sizeof(SecondaryBufferDescription);
				SecondaryBufferDescription.dwBufferBytes = BufferSize;
				SecondaryBufferDescription.lpwfxFormat = &WaveFormat;
				SecondaryBufferDescription.dwFlags = DSBCAPS_GETCURRENTPOSITION2;

				unsigned int code = DirectSound->CreateSoundBuffer(&SecondaryBufferDescription, &SecondaryBuffer, 0);
				if (SUCCEEDED(code)) {
					OutputDebugStringA("Secondary buffer was created");
					std::cout << "Secondary buffer was created" << std::endl;
				}
				else
				{
					std::cout << "Secondary buffer failed with error: " << code << std::endl;
				}
				HRESULT error = SecondaryBuffer->Play(0, 0, DSBPLAY_LOOPING);
				std::cout << "play error: " << error << std::endl;
			}

			
		}

	}

}

void SineWave(int volume, float frequency) {
	//Testing sound
	DWORD readPos;
	DWORD writePos;
	SecondaryBuffer->GetCurrentPosition(&readPos, &writePos);
	std::cout << "PlayPos: " << readPos << " WritePos: " << writePos << std::endl;

	//write
	LPVOID bufferPointer1;
	DWORD bufferSize1;
	LPVOID bufferPointer2;
	DWORD bufferSize2;
	unsigned int error = SecondaryBuffer->Lock(writePos, BufferSize, &bufferPointer1, &bufferSize1, &bufferPointer2, &bufferSize2, DSBLOCK_FROMWRITECURSOR);

	std::cout << "lock error: " << error << std::endl;
	std::cout << "address of first buffer: " << bufferPointer1 << " address of second buffer: " << bufferPointer2 << std::endl;
	std::cout << "Size of first buffer: " << bufferSize1 << " Size of second buffer: " << bufferSize2 << std::endl;

	//SIINGEL SUIOULJGTIOKN
	INT16* p1 = (INT16*)bufferPointer1;
	INT16* p2 = (INT16*)bufferPointer2;
	for (int i = 0; i < (bufferSize1 + bufferSize2) / 4; i++) {
		float wave_period = 48000.0f / 800.0f;
		float volume = 30000;
		short sine = sin(i * (M_PI * 2.0f / wave_period)) * volume;
		if (i < bufferSize1 / 4) {
			//left
			*p1 = sine;
			p1 += 1;
			//right
			*p1 = sine;
			p1 += 1;
		}
		else {
			//left
			*p2 = sine;
			p2 += 1;
			//right
			*p2 = sine;
			p2 += 1;
		}
	}

	error = SecondaryBuffer->Unlock(bufferPointer1, bufferSize1, bufferPointer2, bufferSize2);
	std::cout << "unlock error: " << error << std::endl;
}

void CombineWave(int volume, float frequency, int volume2, float frequency2) {
	//Testing sound
	DWORD readPos;
	DWORD writePos;
	SecondaryBuffer->GetCurrentPosition(&readPos, &writePos);
	std::cout << "PlayPos: " << readPos << " WritePos: " << writePos << std::endl;

	//write
	LPVOID bufferPointer1;
	DWORD bufferSize1;
	LPVOID bufferPointer2;
	DWORD bufferSize2;
	unsigned int error = SecondaryBuffer->Lock(writePos, BufferSize, &bufferPointer1, &bufferSize1, &bufferPointer2, &bufferSize2, DSBLOCK_FROMWRITECURSOR);

	std::cout << "lock error: " << error << std::endl;
	std::cout << "address of first buffer: " << bufferPointer1 << " address of second buffer: " << bufferPointer2 << std::endl;
	std::cout << "Size of first buffer: " << bufferSize1 << " Size of second buffer: " << bufferSize2 << std::endl;

	//SIINGEL SUIOULJGTIOKN
	INT16* p1 = (INT16*)bufferPointer1;
	INT16* p2 = (INT16*)bufferPointer2;
	for (int i = 0; i < (bufferSize1 + bufferSize2) / 4; i++) {
		float wave_period = 48000.0f / frequency;
		float wave_period2 = 48000.0f / frequency2;
		short sine = sin(i * (M_PI * 2.0f / wave_period)) * volume;
		short sine2 = sin(i * (M_PI * 2.0f / wave_period2)) * volume2;
		if (i < bufferSize1 / 4) {
			//left
			*p1 = sine + sine2;
			p1 += 1;
			//right
			*p1 = sine + sine2;
			p1 += 1;
		}
		else {
			//left
			*p2 = sine + sine2;
			p2 += 1;
			//right
			*p2 = sine + sine2;
			p2 += 1;
		}
	}

	error = SecondaryBuffer->Unlock(bufferPointer1, bufferSize1, bufferPointer2, bufferSize2);
	std::cout << "unlock error: " << error << std::endl;
}

void SquareWave(int volume, float frequency) {
	//Testing sound
	DWORD readPos;
	DWORD writePos;
	SecondaryBuffer->GetCurrentPosition(&readPos, &writePos);
	std::cout << "PlayPos: " << readPos << " WritePos: " << writePos << std::endl;

	//write
	LPVOID bufferPointer1;
	DWORD bufferSize1;
	LPVOID bufferPointer2;
	DWORD bufferSize2;
	unsigned int error = SecondaryBuffer->Lock(writePos, BufferSize, &bufferPointer1, &bufferSize1, &bufferPointer2, &bufferSize2, DSBLOCK_FROMWRITECURSOR);

	std::cout << "lock error: " << error << std::endl;
	std::cout << "address of first buffer: " << bufferPointer1 << " address of second buffer: " << bufferPointer2 << std::endl;
	std::cout << "Size of first buffer: " << bufferSize1 << " Size of second buffer: " << bufferSize2 << std::endl;

	//SIINGEL SUIOULJGTIOKN
	INT16* p1 = (INT16*)bufferPointer1;
	INT16* p2 = (INT16*)bufferPointer2;
	for (int i = 0; i < (bufferSize1 + bufferSize2) / 4; i++) {
		float wave_period = 48000.0f / frequency;
		//short sine = sin(i * (M_PI * 2.0f / wave_period)) * volume;
		//short square = sine == 0 ? 0 : (sine / abs(sine)) * volume;
		short square = (i%(int)wave_period < wave_period/2) ? volume : -volume;
		if (i < bufferSize1 / 4) {
			//left
			*p1 = square;
			p1 += 1;
			//right
			*p1 = square;
			p1 += 1;
		}
		else {
			//left
			*p2 = square;
			p2 += 1;
			//right
			*p2 = square;
			p2 += 1;
		}
	}

	error = SecondaryBuffer->Unlock(bufferPointer1, bufferSize1, bufferPointer2, bufferSize2);
	std::cout << "unlock error: " << error << std::endl;

}

void WriteBuffer(INT16* inputBuffer) {
	//Testing sound
	DWORD readPos;
	DWORD writePos;
	SecondaryBuffer->GetCurrentPosition(&readPos, &writePos);

	//write
	LPVOID bufferPointer1;
	DWORD bufferSize1;
	LPVOID bufferPointer2;
	DWORD bufferSize2;
	unsigned int error = SecondaryBuffer->Lock(writePos, BufferSize, &bufferPointer1, &bufferSize1, &bufferPointer2, &bufferSize2, DSBLOCK_FROMWRITECURSOR);

	//SIINGEL SUIOULJGTIOKN
	INT16* p1 = (INT16*)bufferPointer1;
	INT16* p2 = (INT16*)bufferPointer2;
	for (int i = 0; i < (bufferSize1 + bufferSize2) / 2; i++) {
		if (i < bufferSize1 / 2) {
			*p1 = *inputBuffer;
			p1 += 1;
			inputBuffer++;
		}
		else {
			*p2 = *inputBuffer;
			p2 += 1;
			inputBuffer++;
		}
	}

	error = SecondaryBuffer->Unlock(bufferPointer1, bufferSize1, bufferPointer2, bufferSize2);
	std::cout << "unlock error: " << error << std::endl;
}

//WAV SHIT

WAV_FILE LoadWavHeader(std::string path) {
	//Header
	WAV_FILE file = {};
	FILE* inFile;
	errno_t err = fopen_s(&inFile, path.c_str(), "r");
		
	int headerSize = sizeof(WAV_HEADER);
	size_t bytesRead = fread(&file.wavHeader, 1, headerSize, inFile);

	//Data
	if (bytesRead > 0)
	{
		//Read the data 
		uint16_t bytesPerSample = file.wavHeader.bitsPerSample / 8; //Number of bytes per sample 
		uint64_t numSamples = file.wavHeader.ChunkSize / bytesPerSample; //How many samples are in the wav file? static const 
		file.data = new int16_t[file.wavHeader.ChunkSize];
		bytesRead = fread(file.data, sizeof file.data[0], file.wavHeader.ChunkSize / (sizeof file.data[0]), inFile);
		
		//while ((bytesRead = fread(buffer, sizeof buffer[0], BUFFER_SIZE / (sizeof buffer[0]), inFile)) > 0) 
	}
	fclose(inFile);

	return file;
}

void Win32InitDSound() {
	Win32InitDSound(GetConsoleWindow(), 48000, 48000 * 4);
}