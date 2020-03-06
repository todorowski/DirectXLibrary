#include <iostream>
#include "DirectXLibrary.h"
#include <iostream>
#include <string>

#define M_PI 3.14159265358979323846264

int main()
{
    INT16 sineBuffer[48000 * 2];

    for (int i = 0; i < 48000; i++) {
        float wave_period = 48000.0f / 220.0f;
        float volume = 30000;
        INT16 sine = sin(i * (M_PI * 2.0f / wave_period)) * volume;
        sineBuffer[i * 2] = sine;
        sineBuffer[(i * 2) + 1] = sine;
    }

    //Win32InitDSound(GetDesktopWindow(), 48000, 48000*4);
    Win32InitDSound();
    std::string hej = "";
    std::cin >> hej;
    std::cout << "AHHHHHHHHHHHHH!\n";

    WriteBuffer(sineBuffer);
    std::cin >> hej;
    CombineWave(3000, 440, 3000, 659);
    std::cin >> hej;
    SquareWave(3000, 400);
    std::cin >> hej;

}