#define _CRT_SECURE_NO_WARNINGS
#include "WaveFile.h"
#define BUFFER_SIZE 4096
WaveFile::WaveFile(WAV_HEADER header, short*data, int length)
{
	this->header = header;
	this->data = new short[length];
	this->length = length;
	memcpy(this->data, data, (sizeof data[0]) * length);
}
WaveFile::WaveFile()
{
	
}
void WaveFile::load(const char* filepath)
{
	FILE* wavFile = fopen(filepath, "rb");
	if (wavFile == nullptr)
	{
		fprintf(stderr, "Unable to open wave file: %s\n", filepath);
		return;
	}
	//Read the header
	size_t bytesRead = fread(&this->header, 1,sizeof this->header, wavFile);
	if (bytesRead > 0)
	{
		//Read the data
		uint16_t bytesPerSample = this->header.bitsPerSample / 8;      //Number     of bytes per sample
		uint64_t numSamples = this->header.ChunkSize / bytesPerSample; //How many samples are in the wav file?
	
		int8_t* buffer = new int8_t[this->header.ChunkSize];

		bytesRead = 0;
		int length = 0;
		while ((bytesRead = fread(buffer + length, sizeof buffer[0], BUFFER_SIZE / (sizeof buffer[0]), wavFile)) > 0)
		{
			//  cout << "Read " << bytesRead << " bytes." << endl; 
			length += bytesRead;
		}
		this->data = (short*)buffer;
		this->length = length;
	}
	fclose(wavFile);
}
void WaveFile::write(const char* filepath)
{
	FILE* wavFile = fopen(filepath, "wb");

	fwrite(&this->header, 1, sizeof this->header, wavFile);
	int i = 0;
	
	uint8_t* ptr = (uint8_t*)this->data;
	while (i < this->length) {
		fwrite(ptr + i, sizeof ptr[0], std::min(BUFFER_SIZE, length - i) / (sizeof ptr[0]), wavFile);
		i += BUFFER_SIZE;
	}

	fclose(wavFile);
}
int WaveFile::getLength()
{
	return this->length;
}
short* WaveFile::getData()
{
	return this->data;
}
void WaveFile::setData(int index, short value)
{
	this->data[index] = value;
}
WaveFile::~WaveFile() {
	delete[] this->data;
}