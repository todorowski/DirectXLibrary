#include <iostream>
#include "DirectXLibrary.h"
#include <iostream>
#include <string>

#define M_PI 3.14159265358979323846264
using namespace std;

int main()
{
    Win32InitDSound();
	WAV_FILE file = LoadWavHeader("F:\Fagert.wav");
	cout << "SamplesPerSec:" << file.wavHeader.bitsPerSample;

	//Pause
	string hej = "";    
	cin >> hej;
	cout << "Playing Fagert1";
	AddSound(file.data, file.wavHeader.ChunkSize, true);

	cin >> hej;
	cout << "Playing Fagert2";
	AddSound(file.data, file.wavHeader.ChunkSize, true);
	
	cin >> hej;
	cout << "Playing Fagert3";
	AddSound(file.data, file.wavHeader.ChunkSize, true);

	cin >> hej;
}