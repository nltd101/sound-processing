#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include "WaveFile.h"
#include "Effect.h"
using std::cin;
using std::cout;
using std::endl;
using std::fstream;
using std::string;




int main(int argc, char* argv[])
{
   // wav_hdr wavHeader;
   // int headerSize = sizeof(wav_hdr), filelength = 0;

    const char* filePath;
    const char* destFilePath;
    const char* fileRain;
    string input;
    filePath = "../../../data/out.wav";
    destFilePath= "../../../data/dest/out.wav";
    fileRain = "../../../data/rain.wav";
   // filePath = "../../../data/test.txt";
  //  destFilePath = "../../../data/dest/test.txt";

    WaveFile* src=new WaveFile();
    src->load(filePath);
    
   // src->write(destFilePath);
    WaveFile* rain = new WaveFile();
    rain->load(fileRain);
    Effect e(src);
    WaveFile* des =  e.fadeInFadeOut(60000);
    des->write(destFilePath);
    delete src;
    delete rain;
}
