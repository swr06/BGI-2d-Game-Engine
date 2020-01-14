/*
Author : Samuel Wesley Rasquinha
Title : Audio.h
Description : Audio File for BGI Game Engine
*/

#ifndef AUDIO_H
#define AUDIO_H

#include <irrKlang.h>
#include <string>
#include <iostream>
#include "../FileDefines.h"
#include CONSTANTS
#include FUNCTIONS

using namespace std ;
using namespace irrklang ;

enum
{
    eff_distort = 1,
    eff_echo
};
class Audio ;

static const signed short int audio_multiply = 100 ; // for playback
static int audio_master_volume = DEFAULT_MASTER_VOLUME ;
static int audio_master_effect = 0 ;
static bool audio_master_mute = false ;
static bool audio_master_pause = false ;
static ISoundEngine *audio_engine ;

void _InitAudioEngine() ;
void SetMasterVolume(int vol) ;
void AddMasterEffect(int effect) ;
void PlayAudio(Audio snd, bool loop) ;
void StopAudio(Audio snd) ;
void StopAllAudio() ;

class Audio
{
    public :

    Audio() ;
    void InitSound(string path) ;
    void AddEffect(int effect) ;
    void SetVolume(int vol) ;

    ISound *aud_audio ;
    string aud_path ;
    int aud_volume ;
    int aud_effect ;

    int aud_position ;
    int aud_play_spd ;

    bool aud_initialized ;
};

#endif //AUDIO_H
