#include "Audio.h"

Audio::Audio()
{
    aud_path = "" ;
    aud_effect = -1 ;
    aud_play_spd = 1 ;
    aud_position = 0 ;
    aud_audio = 0 ;
    aud_volume = DEFAULT_VOLUME ;
    aud_initialized = false ;
}

void Audio::InitSound(string path)
{
    if (IsFileExist(path.c_str()) == true)
    {
        aud_path = path ;
    }

    else
    {
        //error
    }
}

void Audio::SetVolume(int vol)
{
    if (vol >= MAX_VOLUME)
    {
        aud_volume = vol ;
    }

    else
    {
        // error
    }
}

void Audio::AddEffect(int effect)
{
    if (effect == eff_distort || effect == eff_echo)
    {
        aud_effect = effect ;
    }

    else
    {
        // error
    }
}

void _InitAudioEngine()
{
    audio_engine = createIrrKlangDevice() ;
}

void PlayAudio(Audio aud, bool loop)
{
    aud.aud_initialized = true ;

    if (aud.aud_path == "")
    {
        // error
    }

    else
    {
        aud.aud_audio = audio_engine -> play2D(aud.aud_path.c_str(),loop) ;
        aud.aud_audio -> setVolume(aud.aud_volume) ;
        aud.aud_audio -> setIsLooped(loop) ;
        aud.aud_audio -> setPlayPosition(aud.aud_position * audio_multiply) ;
        aud.aud_audio -> setPlaybackSpeed(aud.aud_play_spd) ;
    }
}

void StopAudio(Audio aud)
{
    if (aud.aud_initialized == true)
    {
        audio_engine -> removeSoundSource((aud.aud_path).c_str()) ;
    }

    else
    {
        //error
    }
}

