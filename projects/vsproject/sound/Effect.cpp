#include "Effect.h"
WaveFile* Effect::fadeInFadeOut(int sampleNums) {
	WaveFile* res = new WaveFile(this->wavFile->header, this->wavFile->data,this->wavFile->header.ChunkSize);
	for (int i = 0; i < this->wavFile->length/2; i++) {
		if (i < 100) {
			res->setData(i, this->wavFile->data[i]);
		}
		else
			if (i - 100 < sampleNums) {
				float rate =  ((float)i - 17 + 1) * 1.0 / sampleNums;
				res->setData(i,(short)(this->wavFile->data[i] * rate));
			}
			else
				if (i > this->wavFile->length - sampleNums - 1) {
					float rate = ((float)wavFile->length - (float)i - 1) * 1.0 / sampleNums;
					res->setData(i, (short)(this->wavFile->data[i] * rate));
				}
				else res->setData(i, this->wavFile->data[i]);
	}
	return res;
}

WaveFile* Effect::mergeFile(WaveFile *waveFile)
{
	WaveFile* res = new WaveFile(this->wavFile->header, this->wavFile->data, this->wavFile->header.ChunkSize);
	for (int i = 0; i < this->wavFile->length/2; i++) {
		if (i < 17) {
			res->setData(i, this->wavFile->data[i]);
		}
		else
			//if (i < res->length/2)
				res->setData(i, max((int)this->wavFile->data[i], (int)waveFile->data[i]));
			//else res->setData(i, this->wavFile->data[i]);
	}
	return res;
}
WaveFile* Effect::echo(int delay)
{
	WaveFile* res = new WaveFile(this->wavFile->header, this->wavFile->data, this->wavFile->length);
	for (int i = 0; i < this->wavFile->length/2; i++) {
		if (i < 17) {
			res->setData(i, this->wavFile->data[i]);
		}
		if (i+delay<this->wavFile->length/2)
			res->setData(i, max((int)this->wavFile->data[i], (int)res->data[i + delay]));
		else
			res->setData(i, this->wavFile->data[i]);
	}
	return res;
}
#define AMOUNTSAMPLETOAPPEND 6 
WaveFile* Effect::roboticVoice() {
    WaveFile* res = new WaveFile(this->wavFile->header, this->wavFile->data, this->wavFile->length);
    vector<int>* iCircle = new vector<int>();
    for (int rIndex = 22; rIndex < this->wavFile->length - 1; rIndex++) {
        if (this->wavFile->data[rIndex] >= 0 && this->wavFile->data[rIndex + 1] < 0) {// && abs(this->rawData[rIndex + 1] - this->rawData[rIndex]) < 500) {
            iCircle->push_back(rIndex);
        }

    }
    for (int rCicle = 0; rCicle < iCircle->size() / AMOUNTSAMPLETOAPPEND - 1; rCicle++)
    {
        removeCircle(res, iCircle->at(rCicle * AMOUNTSAMPLETOAPPEND), iCircle->at((rCicle + 1) * AMOUNTSAMPLETOAPPEND) - 1, rCicle * AMOUNTSAMPLETOAPPEND);
    }
    removeCircle(res, iCircle->at((iCircle->size() / AMOUNTSAMPLETOAPPEND - 1) * AMOUNTSAMPLETOAPPEND), length - 1, ((iCircle->size() / AMOUNTSAMPLETOAPPEND - 1) * AMOUNTSAMPLETOAPPEND));

    return res;
}

short* SoundPCM::decreasePitch() {
    short* res = new short[length];
    std::copy(this->rawData, this->rawData + length, res);
    for (int rCicle = 0; rCicle < iCircle->size() / AMOUNTSAMPLETOAPPEND - 1; rCicle++)
    {
        removeCircle(res, iCircle->at(rCicle * AMOUNTSAMPLETOAPPEND), iCircle->at((rCicle + 1) * AMOUNTSAMPLETOAPPEND) - 1, rCicle * AMOUNTSAMPLETOAPPEND);
    }
    removeCircle(res, iCircle->at((iCircle->size() / AMOUNTSAMPLETOAPPEND - 1) * AMOUNTSAMPLETOAPPEND), length - 1, ((iCircle->size() / AMOUNTSAMPLETOAPPEND - 1) * AMOUNTSAMPLETOAPPEND));
    return res;
};
short* SoundPCM::increasePitch() {
    short* res = new short[length];
    if (iCircle->size() < AMOUNTSAMPLETOAPPEND) return NULL;
    std::copy(this->rawData, this->rawData + length, res);
    for (int rCicle = 0; rCicle < iCircle->size() / AMOUNTSAMPLETOAPPEND - 1; rCicle++)
    {
        appendCircle(res, iCircle->at(rCicle * AMOUNTSAMPLETOAPPEND), iCircle->at((rCicle + 1) * AMOUNTSAMPLETOAPPEND) - 1, (rCicle * AMOUNTSAMPLETOAPPEND + AMOUNTSAMPLETOAPPEND / 2));
    }

    appendCircle(res, iCircle->at((iCircle->size() / AMOUNTSAMPLETOAPPEND - 1) * AMOUNTSAMPLETOAPPEND), length - 1, ((iCircle->size() / AMOUNTSAMPLETOAPPEND - 1) * AMOUNTSAMPLETOAPPEND + AMOUNTSAMPLETOAPPEND / 2));
    return res;
};
void removeCircle(short* res, int startIndex, int endIndex, int iCircleAppend, vector<int>* iCircle, short* source) {
    int length = endIndex - startIndex + 1;
    int lengthOfRemoved = iCircle->at(iCircleAppend + 1) - iCircle->at(iCircleAppend);
    int count = lengthOfRemoved;
    for (int i = 0; i < length; i++) {
        if (i % (length / lengthOfRemoved) == 0 && count > 0) {//&& startIndex + length + count > endIndex) {
            if (i + startIndex > 0 && i + startIndex < this->length - 1) {
                res[i + startIndex] = (res[i + startIndex - 1 + count] + res[i + startIndex + 1 + count]) / 2;
            }
            else {
                res[startIndex + i] = res[startIndex + i + count];
            }
            count--;
            continue;
        }
        res[startIndex + i] = res[startIndex + i + count];
    }

}
void appendCircle(short* res, int startIndex, int endIndex, int iCircleAppend, vector<int>* iCircle,short* source) {
    int length = endIndex - startIndex + 1;
    int lengthOfNew = iCircle->at(iCircleAppend + 1) - iCircle->at(iCircleAppend);
    short* appendedData = new short[length + lengthOfNew];
    for (int i = 0; i < length + lengthOfNew; i++) {
        if (i <= iCircle->at(iCircleAppend + 1) - 1 - startIndex) {
            appendedData[i] = source[i + startIndex];
        }
        else {
            appendedData[i] =source[i - lengthOfNew + startIndex];
        }
    }

    int count = 0;

    for (int i = 0; i < lengthOfNew + length; i++) {
        if (i % ((lengthOfNew + length) / lengthOfNew) == 0 && startIndex + lengthOfNew + length - count > endIndex) {
            count++;
            continue;
        }
        if (startIndex + i - count <= endIndex)
            res[startIndex + i - count] = appendedData[i];
    }
    delete appendedData;
}