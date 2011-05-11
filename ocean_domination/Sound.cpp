#include "Sound.h"

Sound::Sound() {
	listenerPos[0] = 0.0;
	listenerPos[1] = 0.0; 
	listenerPos[2] = 0.0;
	
	listenerVel[0] = 0.0;
	listenerVel[1] = 0.0; 
	listenerVel[2] = 0.0;
	
	listenerOri[0] = 0.0;
	listenerOri[1] = 0.0; 
	listenerOri[2] = -1.0;
	listenerOri[3] = 0.0;
	listenerOri[4] = 1.0;
	listenerOri[5] = 0.0;
}

Sound::~Sound() {
	alutExit();
}

int Sound::initialize() {
	alListenerfv(AL_POSITION,listenerPos);
	alListenerfv(AL_VELOCITY,listenerVel);
	alListenerfv(AL_ORIENTATION,listenerOri);

	alGetError(); /* clear error */
	
	// Generate buffers, or no sound will be produced
	alGenBuffers(NUMBER_OF_SOURCES, buffer);
	
	if(alGetError() != AL_NO_ERROR) {
		printf("- Error creating buffers !!\n");
		exit(1);
	}
	
	loadSounds();

	alGetError(); /* clear error */
	alGenSources(NUMBER_OF_SOURCES, source);

	if(alGetError() != AL_NO_ERROR) {
		printf("- Error creating sources !!\n");
		exit(2);
	}

	for(int i = 0; i < NUMBER_OF_SOURCES; i++) {
		alSourcef(source[i], AL_PITCH, 1.0f);
		alSourcef(source[i], AL_GAIN, 1.0f);
		alSourcei(source[i], AL_BUFFER, buffer[i]);
		alSourcei(source[i], AL_LOOPING, AL_FALSE);
	}

	return 0;
}

int Sound::loadSounds() {
	char al_bool;
	char* file_name;

	string sound_file_name, sound_location = "bin/sounds/", sounds_file_name = "bin/sounds/sounds.txt";
	fstream read_sounds;

	read_sounds.open(sounds_file_name, ios::in);
	
	if(!read_sounds) {
		return 1;
	}

	int i = 0;
	while(!read_sounds.eof()) {
		read_sounds >> sound_file_name;
		
		fileNames[i] = sound_location + sound_file_name;

		i++;
	}

	read_sounds.close();

	for(int i = 0; i < NUMBER_OF_SOURCES; i++) {
		file_name = new char[fileNames[i].length() + 1];
		strcpy(file_name, fileNames[i].c_str());

		alutLoadWAVFile(file_name, &format, &data, &size, &freq, &al_bool);
		alBufferData(buffer[i], format, data, size, freq);
		alutUnloadWAV(format, data, size, freq);

		delete [] file_name;
	}

	return 0;
}

void Sound::playSound(string file_name) {
	int sound_id = 0;
	string sound_location = "bin/sounds/";

	file_name = sound_location + file_name;

	for(int i = 0; i < NUMBER_OF_SOURCES; i++) {
		if(file_name.compare(fileNames[i]) == 0)
			sound_id = i;
	}

	alSourcePlay(source[sound_id]);
	soundPlayed[sound_id] = true;
}

void Sound::pauseSound(string file_name) {
	int sound_id = 0;
	string sound_location = "bin/sounds/";

	file_name = sound_location + file_name;

	for(int i = 0; i < NUMBER_OF_SOURCES; i++) {
		if(file_name.compare(fileNames[i]) == 0)
			sound_id = i;
	}

	alSourcePause(source[sound_id]);
}

void Sound::stopSound(string file_name) {
	int sound_id = 0;
	string sound_location = "bin/sounds/";

	file_name = sound_location + file_name;

	for(int i = 0; i < NUMBER_OF_SOURCES; i++) {
		if(file_name.compare(fileNames[i]) == 0)
			sound_id = i;
	}

	alSourceStop(source[sound_id]);
	soundPlayed[sound_id] = false;
}

void Sound::pauseAllSounds() {
	for(int i = 0; i < NUMBER_OF_SOURCES; i++) {
		alSourcePause(source[i]);
	}
}

void Sound::playAllSounds() {
	for(int i = 0; i < NUMBER_OF_SOURCES; i++) {
		if(soundPlayed[i] == true) {
			alSourcePlay(source[i]);
		}
	}
}

void Sound::stopAllSounds() {
	for(int i = 0; i < NUMBER_OF_SOURCES; i++) {
		alSourceStop(source[i]);
		soundPlayed[i] = false;
	}
}

void Sound::increaseSoundVolume(string sound_name, float volume) {
	int sound_id = 0;
	string sound_location = "bin/sounds/";

	sound_name = sound_location + sound_name;

	for(int i = 0; i < NUMBER_OF_SOURCES; i++) {
		if(sound_name.compare(fileNames[i]) == 0)
			sound_id = i;
	}

	alSourcef (source[sound_id], AL_GAIN, volume);
}

void Sound::setSoundSourcePosition(float x, float y, float z) {
	ALfloat* position = new ALfloat[3];
	position[0] = x;
	position[1] = y;
	position[2] = z;

	alListenerfv(AL_POSITION, position);
}

void setSoundSourceVelocity(float x, float y, float z) {
	ALfloat* velocity = new ALfloat[3];
	velocity[0] = x;
	velocity[1] = y;
	velocity[2] = z;

	alListenerfv(AL_VELOCITY, velocity);
}