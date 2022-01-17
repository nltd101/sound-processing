#pragma once
#include <iostream>
#include "WaveFile.h"
using namespace std;

class Effect
{
public:
	Effect(WaveFile* file){
		this->wavFile = file;		
	}
	WaveFile* fadeInFadeOut(int sampleNums);
	WaveFile* mergeFile(WaveFile*);
private:
	WaveFile* wavFile;
};

