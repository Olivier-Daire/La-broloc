#pragma once

#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>

class Sound
{
	public:
		static void my_audio_callback(void *userdata, Uint8 *stream, int len);
		static void loadSound(const char* music_path);
	private: 
		static Uint8 *audio_pos; // global pointer to the audio buffer to be played
		static int audio_len; // remaining length of the sample we have to play
};