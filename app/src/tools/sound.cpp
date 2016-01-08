#include "tools/sound.hpp"

Uint8* Sound::audio_pos = 0; // global pointer to the audio buffer to be played
int Sound::audio_len = 0; // remaining length of the sample we have to play

void Sound::my_audio_callback(void *userdata, Uint8 *stream, int len) {

    if (audio_len ==0)
        return;
    
    len = ( len > audio_len ? audio_len : len );
    SDL_memcpy (stream, audio_pos, len);                  // simply copy from one buffer into the other
    SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);// mix from one buffer into another
    
    audio_pos += len;
    audio_len -= len;
}

void Sound::loadSound(const char* music_path){
	static Uint32 wav_length; // length of our sample
    static Uint8 *wav_buffer; // buffer containing our audio file
    static SDL_AudioSpec wav_spec; // the specs of our piece of music

    if (SDL_Init(SDL_INIT_AUDIO) < 0)
    	std::cout << "Error init audio" << std::endl;

    /* Load the WAV */
    // the specs, length and buffer of our wav are filled
    if( SDL_LoadWAV(music_path, &wav_spec, &wav_buffer, &wav_length) == NULL ){
    	std::cout << "Error load music" << std::endl;
    }
    // set the callback function
    wav_spec.callback = my_audio_callback;
    wav_spec.userdata = NULL;
    // set our global static variables
    audio_pos = wav_buffer; // copy sound buffer
    audio_len = wav_length; // copy file length
    
    /* Open the audio device */
    if ( SDL_OpenAudio(&wav_spec, NULL) < 0 ){
      fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
      exit(-1);
    }
    /* Start playing */
    SDL_PauseAudio(0);
}

// static void closeSound() {
// 	// shut everything down
//     SDL_CloseAudio();
//     SDL_FreeWAV(wav_buffer);
// }