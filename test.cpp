#include <alsa/asoundlib.h>
#include <stdio.h>
#include <iostream>
#include <thread>
#include <fstream>
#include "SoundProcessing.h"
using namespace std;
#define PCM_DEVICE "default"
short convertChar2Short(char byte1, char byte2) {
	short res = (((short)byte1) << 8) | (0x00ff & byte2);
	return res;
}
void convertShort2Char(short short0, char& byte1, char& byte2) {
	byte1 = short0 >> 8;
	byte2 = short0 & 0xFF;
}

int main(int argc, char** argv) {
	int* amount = new int;
	*amount = 0;
	unsigned int pcm, tmp, dir;
	int rate, channels, seconds;
	snd_pcm_t* pcm_handle;
	snd_pcm_hw_params_t* params;
	snd_pcm_uframes_t frames;
	char* buff;
	int buff_size, loops;

	if (argc < 4) {
		printf("Usage: %s <sample_rate> <channels> <seconds>\n",
			argv[0]);
		return -1;
	}

	rate = atoi(argv[1]);
	channels = atoi(argv[2]);
	seconds = atoi(argv[3]);

	/* Open the PCM device in playback mode */
	if (pcm = snd_pcm_open(&pcm_handle, PCM_DEVICE,
		SND_PCM_STREAM_PLAYBACK, 0) < 0)
		printf("ERROR: Can't open \"%s\" PCM device. %s\n",
			PCM_DEVICE, snd_strerror(pcm));

	/* Allocate parameters object and fill it with default values*/
	snd_pcm_hw_params_alloca(&params);

	snd_pcm_hw_params_any(pcm_handle, params);

	/* Set parameters */
	if (pcm = snd_pcm_hw_params_set_access(pcm_handle, params,
		SND_PCM_ACCESS_RW_INTERLEAVED) < 0)
		printf("ERROR: Can't set interleaved mode. %s\n", snd_strerror(pcm));

	if (pcm = snd_pcm_hw_params_set_format(pcm_handle, params,
		SND_PCM_FORMAT_S16_LE) < 0)
		printf("ERROR: Can't set format. %s\n", snd_strerror(pcm));

	if (pcm = snd_pcm_hw_params_set_channels(pcm_handle, params, channels) < 0)
		printf("ERROR: Can't set channels number. %s\n", snd_strerror(pcm));

	if (pcm = snd_pcm_hw_params_set_rate_near(pcm_handle, params, (unsigned int*)&rate, 0) < 0)
		printf("ERROR: Can't set rate. %s\n", snd_strerror(pcm));

	/* Write parameters */
	if (pcm = snd_pcm_hw_params(pcm_handle, params) < 0)
		printf("ERROR: Can't set harware parameters. %s\n", snd_strerror(pcm));

	/* Resume information */
	printf("PCM name: '%s'\n", snd_pcm_name(pcm_handle));

	printf("PCM state: %s\n", snd_pcm_state_name(snd_pcm_state(pcm_handle)));

	snd_pcm_hw_params_get_channels(params, &tmp);
	printf("channels: %i ", tmp);

	if (tmp == 1)
		printf("(mono)\n");
	else if (tmp == 2)
		printf("(stereo)\n");

	snd_pcm_hw_params_get_rate(params, &tmp, 0);
	printf("rate: %d bps\n", tmp);

	printf("seconds: %d\n", seconds);

	/* Allocate buffer to hold single period */
	snd_pcm_hw_params_get_period_size(params, &frames, 0);

	buff_size = frames * channels * 2; /* 2 -> sample size */;
	buff = (char*)malloc(buff_size);

	snd_pcm_hw_params_get_period_time(params, &tmp, NULL);
	int count = 0;
	std::ofstream outfile;
	outfile.open("test.txt", std::ios::out);

	std::ofstream outfile2;
	outfile2.open("test2.txt", std::ios::out);

	short* originAudio = new short[seconds * 1000000 / tmp * buff_size / 2];
	for (loops = (seconds * 1000000) / tmp; loops > 0; loops--) {
		if (pcm = read(0, buff, buff_size) == 0) {
			printf("Early end of file.\n");
			return 0;
		}
		for (int i = 0; i < buff_size; i += 2) {
			short short0 = convertChar2Short(buff[i + 1], buff[i]);
			originAudio[count] = short0;
			count += 1;
		}
	}

	SoundPCM* soundPCM = new SoundPCM(originAudio, count);
	short* newAudio = soundPCM->increaseVolumn();

	*amount = 0;
	for (int i = 0; i < count; i++)
	{
		if (originAudio[i] >= 0 && originAudio[i + 1] < 0)
		{
			(*amount) = (*amount) + 1;

		}
		if (i < 25000 && i>21000)
		{
			outfile2 << newAudio[i] << endl;
			outfile << originAudio[i] << endl;
		}
		if (i % 1200 == 0) {
			cout << *amount / 0.05 << "Hz" << endl;
			*amount = 0;
		}

	}
	std::cout << *amount << endl;
	std::cout << "-------------------------------" << endl;
	*amount = 0;
	for (int i = 0; i < count; i++)
	{
		if (newAudio[i] >= 0 && newAudio[i + 1] < 0)
		{
			(*amount) = (*amount) + 1;

		}
		if (i % 1200 == 0) {
			cout << *amount / 0.05 << "Hz" << endl;
			*amount = 0;
		}
	}
	std::cout << *amount << endl;
	for (int i = 0; i < count; i++)
	{
		convertShort2Char(newAudio[i], buff[(2 * i + 1) % buff_size], buff[(2 * i) % buff_size]);
		if ((2 * i + 2) % buff_size == 0)
		{
			if (pcm = snd_pcm_writei(pcm_handle, buff, frames) == -EPIPE) {
				printf("XRUN.\n");
				snd_pcm_prepare(pcm_handle);
			}
			else if (pcm < 0) {
				printf("ERROR. Can't write to PCM device. %s\n", snd_strerror(pcm));
			}
		}
	}
	delete originAudio;
	delete newAudio;
	snd_pcm_drain(pcm_handle);
	snd_pcm_close(pcm_handle);
	free(buff);
	outfile.close();
	outfile2.close();
	return 0;
}