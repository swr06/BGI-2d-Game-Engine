#ifndef GAME_SETTINGS_H
#define GAME_SETTINGS_H

#include <string>

using namespace std ;

enum game_priorities
{
    medium,
    high,
    realtime
};

// Game Information

const string game_title = "My Game" ;
const string game_author = "Your Name" ; // :)
const string game_version = "1.0" ;
const string game_company = "Your Company" ;
const string game_product = "" ;

// Game Settings

const string game_window_title = "My Game Title" ;
const unsigned int game_window_x = 400 ;
const unsigned int game_window_y = 400 ;
const bool game_show_debug = true ;
const bool game_multi_threaded_mode = false ;
const bool game_show_border = true ;
const int game_max_fps = 30 ;
const bool game_show_fps = false ;
const int game_show_fps_x = 0 ; // ignore if game_show_fps = false
const int game_show_fps_y = 0 ;
const signed short game_priority = medium; //medium, high, Realtime(very high)

// Touches

const string game_icon = "icon.ico" ;
const string game_icon_small = "icon.ico" ;
const string game_cursor = "icon.ico" ;

// Advanced

const bool writedebug = true ;
const string debugfilen = "debugmsg.dmsg" ;

#endif // GAME_SETTINGS_H
