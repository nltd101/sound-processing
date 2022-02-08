#pragma once
#include <iostream>
#include "WaveFile.h"
#include <vector>
using namespace std;

class Effect
{
public:
	Effect(WaveFile* file){
		this->wavFile = file;		
	}
	WaveFile* fadeInFadeOut(int sampleNums);
	WaveFile* mergeFile(WaveFile*);
	WaveFile* echo(int delay);
	WaveFile* roboticVoice();
private:
	WaveFile* wavFile;
};

