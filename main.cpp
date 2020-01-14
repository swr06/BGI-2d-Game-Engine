/* TODO

- Debug System
- Transparent Bitmaps
- Room Scrolling
- Sound System

*/

#include "Core/App/Game.h"
#include "Core/Audio/Audio.h"
#include "Core/Graphics/winbgim/winbgim.h"
#include <irrKlang.h>

using namespace irrklang ;
using namespace std ;

int main()
{
    int amt = 5 ;
    Texture &tex1 = *(new Texture) ;
    Texture &tex2 = *(new Texture) ;
    Object &obj1 = *(new Object) ;
    Object &obj2 = *(new Object) ;
    Sprite &spr1 = *(new Sprite) ;
    Sprite &spr2 = *(new Sprite) ;
    Scene &sce1 = *(new Scene) ;
    Scene &sce2 = *(new Scene) ;

    InitGame() ;

    tex1.InitTexture({"Assets/Sprites/frame.bmp","Assets/Sprites/frame2.bmp"}) ;
    tex2.InitTexture({"Assets/Sprites/frame3.bmp","Assets/Sprites/frame4.bmp"}) ;

    // Initializing some stuff
    spr1.InitSprite(tex1,30) ;
    spr2.InitSprite(tex2,20) ;
    obj1.SetObjectVisible(true) ;
    obj1.AddObjectSprite(spr1) ;
    obj2.SetObjectVisible(true) ;
    obj2.AddObjectSprite(spr2) ;

    // Placing Objects in the scene

    sce1.PlaceObject(obj1,100,50,1) ;
    sce1.PlaceObject(obj2,50,50,1) ;

    // Initializing Events

    obj1.InitKeyboardEvent(KB_RIGHT,[&obj1,&amt](){IncrementObjectX(obj1,amt);}) ;
    obj1.InitKeyboardEvent(KB_LEFT,[&obj1,&amt](){IncrementObjectX(obj1,0 - amt);}) ;
    obj1.InitKeyboardEvent(KB_DOWN,[&obj1,&amt](){IncrementObjectY(obj1,amt);}) ;
    obj1.InitKeyboardEvent(KB_UP,[&obj1,&amt](){IncrementObjectY(obj1,0 - amt);}) ;
    obj1.InitKeyboardEvent(KB_K,[&sce2](){SetCurrentScene(sce2,1);}) ;
    obj1.InitKeyboardEvent(KB_P,[&obj1,&spr2](){obj1.AddObjectSprite(spr2) ;}) ;

    // Setting up

    SetCurrentScene(sce1,1) ;
    StartGame() ;
    getch() ;
}
/*
#include <iostream>
#include <windows.h>
#include <conio.h>
#include "Core/Graphics/winbgim/winbgitypes.h"
#include "Core/Graphics/winbgim/graphics.h"
#include "Core/Graphics/png32/png32_helper.h"

int main()
{
    HDC hdc ;

    initwindow(400,400) ;

    for (int i = 0 ; i < 5 ; i++)
    {
        hdc = BGI__GetWinbgiDC() ;
        Png32_Show(hdc,-2,-2,31,31,Png32_Load("testt.png"),0,0) ;
        //readimagefile("testt.png",0,0,32,32) ;
        BGI__ReleaseWinbgiDC() ;
    }

    getch() ;
}

*/
