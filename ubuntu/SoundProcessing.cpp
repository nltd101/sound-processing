#include "SoundProcessing.h"


short* SoundPCM::increaseVolumn() {

    short* res = new short[length];

    for (int i = 0; i < length; i++) {
        if (i < 22) {
            res[i] = rawData[i];
            continue;
        }
        res[i] = (rawData[i] * 1.3 > 32767) ? 32767 : rawData[i] * 1.3;
    }
    return res;

};

short* SoundPCM::decreaseVolumn() {
    short* res = new short[length];

    for (int i = 0; i < length; i++) {
        if (i < 22) {
            res[i] = rawData[i];
            continue;
        }
        res[i] = rawData[i] * 0.5;
    }
    return res;
};
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
void SoundPCM::removeCircle(short* res, int startIndex, int endIndex, int iCircleAppend) {
    int length = endIndex - startIndex + 1;
    int lengthOfRemoved = this->iCircle->at(iCircleAppend + 1) - this->iCircle->at(iCircleAppend);
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
void SoundPCM::appendCircle(short* res, int startIndex, int endIndex, int iCircleAppend) {
    int length = endIndex - startIndex + 1;
    int lengthOfNew = this->iCircle->at(iCircleAppend + 1) - this->iCircle->at(iCircleAppend);
    short* appendedData = new short[length + lengthOfNew];
    for (int i = 0; i < length + lengthOfNew; i++) {
        if (i <= this->iCircle->at(iCircleAppend + 1) - 1 - startIndex) {
            appendedData[i] = this->rawData[i + startIndex];
        }
        else {
            appendedData[i] = this->rawData[i - lengthOfNew + startIndex];
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
SoundPCM::SoundPCM(short* rawdata, int length) {

    this->rawData = rawdata;
    this->length = length;
    this->iCircle = new vector<int>();
    for (int rIndex = 22; rIndex < length - 1; rIndex++) {
        if (this->rawData[rIndex] >= 0 && this->rawData[rIndex + 1] < 0) {// && abs(this->rawData[rIndex + 1] - this->rawData[rIndex]) < 500) {
            iCircle->push_back(rIndex);
        }

    }
}
SoundPCM::~SoundPCM() {
    delete iCircle;
}
// int main() {
//     short raw[] = { 523,-445,-485,252,1016,1292,2248,3574,4668,4958,4584,4699,5713,5596,3491,2789,2486,765,-1411,-3976,-5690,-7237,-9586,-12041,-10759,-11029,-11940,-11949,-10269,-7960,-6681,-6355,-3127,-544,1229,2654,3938,5936,7034,7651,7760,7339,7481,7016,5433,4472,3629,2493,1765,604,121,188,663,1074,1501,2466,3676,4084,4064,4384,4661,5014,4554,3308,3213,2551,29,-1936,-2691,-4898,-8105,-10129,-9212,-10462,-11745,-12005,-10369,-9277,-8565,-7753,-5878,-3433,-1118,291,1868,3818,5676,6899,7149,7541,8106,8022,6878,6037,5527,4685,3383,2075,1660,1320,1365,972,1236,1778,2729,2912,2755,2911,3602,3967,3867,3082,2680,2575,734,-1147,-2106,-3599,-6203,-8846,-9508,-8639,-10063,-11851,-10810,-8891,-9180,-8960,-6766,-4460,-2902,-1398,243,2625,4429,6165,6419,6937,7960,8426,7680,6434,5725,5802,4811,2851,2042,2092,1908,1228,1013,1788,2543,2627,2578,2429,2960,3905,3521,2698,2622,2813,1876,-601,-1652,-2066,-4583,-7644,-8533,-7754,-9154,-11121,-11045,-9372,-8570,-9415,-7929,-6075,-3484,-2330,-1522,790,2760,4886,5814,5247,7209,8602,7729,6677,6273,6369,5557,3859,3052,2848,2418,1956,1671,1978,2383,2723,2559,2377,2476,3495,3305,2637,2258,2470,2074,288,-1452,-1820,-3463,-6936,-8211,-6843,-8623 };
//     short* ptr = raw;
//     SoundPCM* t = new SoundPCM(ptr, 215);
//     short* x = t->increasePitch();
//     if (x != NULL)
//         for (int i = 0; i < 215; i++) {
//             cout << x[i] << "," << endl;
//         }
//     delete x;
//     return 0;
// }