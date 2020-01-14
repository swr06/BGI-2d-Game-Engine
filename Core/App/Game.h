#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <thread>
#include <chrono>
#include <unistd.h>
#include <windows.h>
#include <conio.h>
#include <functional>

#include "../FileDefines.h"
#include "../Graphics/winbgim/winbgim.h"

#include KB_MACROS
#include GLOBALS
#include CONSTANTS
#include FUNCTIONS
#include DEBUG_WINDOW
//#include GRAPHICS
#include GAME_SETTINGS
#include TEXTURE
#include SPRITE
#include OBJECT
#include SCENE
#include RENDER
#include SORT

using namespace std ;

enum mouse_precision_levels
{
    fast,
    precise
};

// file scope variables

static Scene* game_current_scene = NULL ;
static const signed short game_mchk_level = fast ; // fast or precise
static const signed short game_transparent_color = 15 ; // white
static int32_t game_frame_count = 0 ;
static bool caps_differ = false ;
static char* temp_title = (char*) "test" ; // temporary
static int event_thread_slp_time = 12 ;
static int display_thread_slp_time = 33 ;//(int) 1000 / game_max_fps ;
static int game_window = 0 ;
static unsigned long fcount = 0 ;
static int game_first_buffer = 0;
static int game_second_buffer = 1 ;
static int game_current_buffer = 0 ;
static double game_fps = 0 ;
static bool stop_event_thread = false ;
static bool stop_display_thread = false ;
static void_function game_start_loop = []{} ;
static void_function game_end_loop = []{} ;

// user functions

void InitGame() ;
void StartGame() ;
void EndGame() ;
void PauseGame(gtime time) ;
void PauseEventThread(gtime time) ;
void PauseDisplayThread(gtime time) ;
void SetCurrentScene(Scene &scene, bool reset) ; // reset is used to tell the engine whether it has to reset the objects or not
void IncrementObjectX(Object &obj, int increment_amt) ;
void IncrementObjectY(Object &obj, int increment_amt) ;
void SetObjectXY(Object &obj, int x, int y) ;
void TakeScreenShot(string filename) ;
void AddGameLoopStart(void_function func) ;
void AddGameLoopEnd(void_function func) ;

// user wrapper functions

void DrawTextXY() ;

// time functions

void GetDate() ;
void GetDay() ;
void GetTimeMinute() ;
void GetTimeSecond() ;
void GetTimeMillisecond() ;

// saving and loading

void SaveGame(string file_name) ;
void LoadGame() ;

// misc

void ChangeAllInstances() ;

// internal functions

void _UpdateGameWindow() ;
int _RecieveKeyboardInput() ; // NOT TO BE CONFUSED WITH 'getch()'
void _GameEventThread() ;
void _CalculateFPS(double time_fps) ;
void _ReadSceneTex(Instance &s, bool update) ;
int __GetNextFrame(Texture *t) ;
int _GetNextFrame(Texture &t) ;
int _GetRequiredBuffer() ;
bool _ObjectClicked(Instance &s, int mx, int my) ;
void _GameDisplayThread() ;
void _GameEventThread() ;
void _UpdateFrameCount(Instance &s_) ;
void _UpdateObjectMembers() ;
void _RestoreObjectMembers() ;
bool _ObjectClicked(Instance &s, int mx, int my) ;
bool _Collision(Instance &instance_1, Instance &instance_2) ;
void _SendEventMass(int etype) ;
void _UpdateObjectTextures() ;

// to-implement



// for testing purposes only
void print() ;

#endif // GAME_H
