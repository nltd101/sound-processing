#include <iostream>
using namespace std;
#include <memory>
#include <vector>
class SoundPCM {
public:
	SoundPCM(short* rawdata, int length);
	~SoundPCM();

	short* increasePitch();
	short* decreasePitch();
	short* increaseVolumn();
	short* decreaseVolumn();
	const int AMOUNTSAMPLETOAPPEND = 6;
private:
	short* rawData;
	int length;
	void appendCircle(short* res, int startIndex, int endIndex, int iCircleAppend);
	void removeCircle(short* res, int startIndex, int endIndex, int iCircleAppend);
	vector<int>* iCircle;
};