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
    string input;
    filePath = "../../../data/16b_1c_16000.wav";
    destFilePath= "../../../data/dest/16b_1c_16000.wav";
   // filePath = "../../../data/test.txt";
  //  destFilePath = "../../../data/dest/test.txt";

    WaveFile* src=new WaveFile();
    src->load(filePath);
    
   // src->write(destFilePath);
    
    Effect e(src);
    WaveFile* des = e.fadeInFadeOut(1000);
    des->write(destFilePath);
    delete src;
   
   
      
      
     
      
  
//    return 0;
}
