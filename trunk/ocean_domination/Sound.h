#ifndef SOUND_H
#define SOUND_H

#include<fstream>
#include <string>
#include <openAL/al.h>
#include <openAL/alut.h>

using namespace std;

#define NUMBER_OF_SOURCES 5

static class Sound {
public:
	Sound();
	~Sound();
	int initialize();
	void playSound(string file_name);
	void pauseSound(string file_name);
	void stopSound(string file_name);
	void stopAllSounds();
	void pauseAllSounds();
	void playAllSounds();
	void increaseSoundVolume(string sound_name, float volume);
	void setSoundSourcePosition(float x, float y, float z);
	void setSoundSourceVelocity(float x, float y, float z);

private:
	int loadSounds();

	string fileNames[NUMBER_OF_SOURCES];

	ALuint	buffer[NUMBER_OF_SOURCES];
	ALuint	source[NUMBER_OF_SOURCES];
	bool soundPlayed[NUMBER_OF_SOURCES];

	ALfloat listenerPos[3];
	ALfloat listenerVel[3];
	ALfloat	listenerOri[6];

	ALsizei size, freq;
	ALenum format;
	ALvoid *data;
};

#endif