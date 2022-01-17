#include "Effect.h"
WaveFile* Effect::fadeInFadeOut(int sampleNums) {
	WaveFile* res = new WaveFile(this->wavFile->header, this->wavFile->getData(), this->wavFile->getLength());
	
	for (int i = 0; i < this->wavFile->getLength(); i++) {
		if (i < 17) {
			res->setData(i, this->wavFile->getData()[i]);
		}
		else
			if (i - 17 < sampleNums) {
				float rate = ((float)i - 17 + 1) * 1.0 / sampleNums;
				res->setData(i,(short)(this->wavFile->getData()[i] * rate));
			}
			else
				if (i > this->wavFile->getLength() - sampleNums - 1) {
					float rate = ((float)wavFile->getLength() - (float)i - 1) * 1.0 / sampleNums;
					res->setData(i, (short)(this->wavFile->getData()[i] * rate));
				}
				else res->setData(i, this->wavFile->getData()[i]);
	}
	return res;
}

WaveFile* Effect::mergeFile(const char* filename)
{
	return nullptr;
}
