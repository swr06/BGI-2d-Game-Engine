#include "Game.h"
/* initwindow()
setactivepage()
setvisualpage
*/
void InitGame()
{
    InitDebugWindow() ;
    game_window = initwindow(game_window_x, game_window_y,game_window_title.c_str()) ;
    game_first_buffer = getactivepage() ;
    game_current_buffer = game_first_buffer ;
    game_second_buffer = game_current_buffer + 1 ;
}

void StartGame()
{
    if (game_current_scene == NULL)
    {
        DisplayDebugMessage("GAME CURRENT SCENE NOT SET!") ;
    }

    thread display_thread(_GameDisplayThread) ;
    _SendEventMass(e_game_start) ;
    _GameEventThread() ;

    display_thread.join() ;
}

void EndGame()
{
    _SendEventMass(e_game_end) ;
    exit(0) ;
}

void SetCurrentScene(Scene &scene, bool reset)
{
    game_current_scene = &scene ;
    scene.scene_reset = true ;
    _SendEventMass(e_game_start) ;
}

void IncrementObjectX(Object &obj, int increment_amt)
{
    int obj_ref ;

    for (int i = 0 ; i < game_current_scene -> scene_textures.size() ; i++)
    {
        if (game_current_scene -> scene_textures[i].objp != -1)
        {
            obj_ref = game_current_scene -> scene_textures[i].objp ;

            if (game_current_scene -> scene_objects[obj_ref] == &obj)
            {
                game_current_scene -> scene_textures[i].pixel_x = (game_current_scene -> scene_textures[i].pixel_x
                                                                   + increment_amt) ;
            }
        }
    }
}

void IncrementObjectY(Object &obj, int increment_amt)
{
    int obj_ref ;

    for (int i = 0 ; i < game_current_scene -> scene_textures.size() ; i++)
    {
        if (game_current_scene -> scene_textures[i].objp != -1)
        {
            obj_ref = game_current_scene -> scene_textures[i].objp ;

            if (game_current_scene -> scene_objects[obj_ref] == &obj)
            {
                game_current_scene -> scene_textures[i].pixel_y = (game_current_scene -> scene_textures[i].pixel_y
                                                                   + increment_amt) ;
            }
        }
    }
}

void SetObjectXY(Object &obj, int x, int y)
{
    int obj_ref ;

    for (int i = 0 ; i < game_current_scene -> scene_textures.size() ; i++)
    {
        if (game_current_scene -> scene_textures[i].objp != -1)
        {
            obj_ref = game_current_scene -> scene_textures[i].objp ;

            if (game_current_scene -> scene_objects[obj_ref] == &obj)
            {
                game_current_scene -> scene_textures[i].pixel_x = x ;
                game_current_scene -> scene_textures[i].pixel_y = y ;
            }
        }
    }
}

void TakeScreenShot(string filename)
{
    writeimagefile(filename.c_str(),0,0,game_window_x,game_window_y,true) ;
}

void AddGameLoopStart(void_function func)
{
    game_start_loop = func ;
}

void AddGameLoopEnd(void_function func)
{
    game_end_loop = func ;
}

void _GameDisplayThread()
{
    cout << "\nDISPLAY THREAD STARTING..\n" ;

    while (game_window != error_win)
    {
        while (stop_display_thread == false)
        {
            _UpdateGameWindow() ;
            SortInstancesDepth(game_current_scene) ;
            _UpdateObjectTextures() ;
            this_thread::sleep_for(chrono::milliseconds(display_thread_slp_time)) ;
        }
    }
}

void _GameEventThread()
{
    int scene_size = game_current_scene -> scene_objects.size() ;
    int kb_key ;
    int mb_key ;
    int mx ;
    int my ;
    int e_obj ;
    Event g_event ;

    while (game_window != error_win)
    {
        while (stop_event_thread == false)
        {
            game_start_loop() ;

            if (kbhit() == true)
            {
                kb_key = _RecieveKeyboardInput() ;

                for (int i = 0 ; i < game_current_scene -> scene_objects.size() ; i++)
                {
                    if ((kb_key >= 65 && kb_key <= 90))
                    {
                        g_event.event_type = e_keyboard ;
                        g_event.event_second_type = kb_key ;
                        game_current_scene -> scene_objects[i] -> __SendMsg(g_event) ;
                        g_event.event_type = e_keyboard ;
                        g_event.event_second_type = kb_key + 32 ;
                        game_current_scene -> scene_objects[i] -> __SendMsg(g_event) ;
                    }

                    else if (kb_key >= 97 && kb_key <= 122)
                    {
                        g_event.event_type = e_keyboard ;
                        g_event.event_second_type = kb_key ;
                        game_current_scene -> scene_objects[i] -> __SendMsg(g_event) ;
                        g_event.event_type = e_keyboard ;
                        g_event.event_second_type = abs(kb_key - 32) ;
                        game_current_scene -> scene_objects[i] -> __SendMsg(g_event) ;
                    }

                    else
                    {
                        g_event.event_type = e_keyboard ;
                        g_event.event_second_type = kb_key ;
                        game_current_scene -> scene_objects[i] -> __SendMsg(g_event) ;
                    }
                }
            }

            if (AnyMouseClick() == true)
            {
                GetRecentMouseClick(mb_key,mx,my) ;

                for (int i = 0 ; i < game_current_scene -> scene_objects.size() ; i++)
                {
                    g_event.event_type = e_mouse ;
                    g_event.event_second_type = mb_key ;
                    game_current_scene -> scene_objects[i] -> __SendMsg(g_event) ;
                }
            }

            _SendEventMass(e_loop) ;
            game_end_loop() ;
            this_thread::sleep_for(chrono::milliseconds(event_thread_slp_time)) ;
        }
    }
}

void _UpdateGameWindow()
{
    int tsize = game_current_scene -> scene_textures.size() ;
    int buff = _GetRequiredBuffer() ;
    int s_origin_x = 0 ;
    int s_origin_y = 0 ;
    char fps_s[48] ;

    setactivepage(buff) ;

    auto stime = chrono::steady_clock::now() ;

    cleardevice() ;

    for (int i = 0 ; i < tsize ; i++)
    {
        if (game_frame_count % *(game_current_scene -> scene_textures[i].spd) == 0)
        {
            if (*(game_current_scene -> scene_textures[i].o_visible) == true)
            {
                _ReadSceneTex(game_current_scene -> scene_textures[i],true) ;
            }
        }

        else
        {
            if (*(game_current_scene -> scene_textures[i].o_visible) == true)
            {
                _ReadSceneTex(game_current_scene -> scene_textures[i],false) ;
            }
        }
    }

    setvisualpage(buff) ;
    game_current_buffer = buff ;
    auto etime = chrono::steady_clock::now() ;
    double tfps = chrono::duration_cast<chrono::seconds>(etime - stime).count() ;

    if (game_frame_count > game_max_fps - 1)
    {
        game_frame_count = 0 ;
    }

    game_frame_count++ ;
    _CalculateFPS(tfps) ;

    if (game_show_fps == true)
    {
        sprintf(fps_s,"FPS : %f", game_fps) ;
        outtextxy(game_show_fps_x, game_show_fps_y, fps_s) ;
    }

    return ;
}

void _CalculateFPS(double time_fps)
{
    game_fps = (1000 - time_fps) / game_max_fps ;
}

void _ReadSceneTex(Instance &s, bool update)
{
    bool draw_pos = false ; // 1 = draw from top left 0 = draw from top right
                            // draw_pos = 1 is a little buggy but draw_pos = 0 is fine
    int w_x ; // x location
    int w_y ; // y location
    int size_x ;
    int size_y ;

    w_x = s.pixel_x ;
    w_y = s.pixel_y ;
    size_x = s._texture -> texture_x ;
    size_y = s._texture -> texture_y ;

    if (update == true)
    {
        s._UpdateFrame() ;
    }

    if (draw_pos == true)
    {
        RenderImageFile(s._texture, w_x, w_y, abs(size_x - w_x), abs(size_y - w_y), s.current_frame) ;
    }

    else if (draw_pos == false)
    {
        RenderImageFile(s._texture, w_x + size_x, w_y + size_y, w_x, w_y, s.current_frame) ;
    }
}

int _GetRequiredBuffer()
{
    if (game_current_buffer == game_first_buffer)
    {
        return game_second_buffer ;
    }

    else
    {
        return game_first_buffer ;
    }
}

int _RecieveKeyboardInput()
{
    char ric ; // recieved input character
    int ri1 ; // recieved input 1
    int ri2 ; // recieved input 1
    int return_val ;

    ri1 = getch() ;

    if (ri1 == 0) // I'm pretty sure when you press the arrow keys the FIRST getch() returns 224 but,
                    // it seems to return 0. So the condition is 'ri1 == 0' instead of 'ri1 == 224'
    {
        ri2 = getch() ;

        switch(ri2)
        {
            case 72 :
            {
                return_val = KB_UP ;
                break ;
            }

            case 75 :
            {
                return_val = KB_LEFT ;
                break ;
            }

            case 80 :
            {
                return_val = KB_DOWN ;
                break ;
            }

            case 77 :
            {
                return_val = KB_RIGHT ;
                break ;
            }
        }
    }

    else
    {
        return_val = ri1 ;
    }

    return return_val ;

}

bool _ObjectClicked(Instance &s, int mx, int my)
{
    int range_x ;
    int range_y ;
    int range_x2 ;
    int range_y2 ;

    range_x = abs(s.pixel_x - s._texture -> texture_x) ;
    range_x = abs(s.pixel_y - s._texture -> texture_y) ;
    range_x2 = s.pixel_x ;
    range_y2 = s.pixel_y ;

    if ((mx >= range_x && my >= range_y) && (mx <= range_x2 && my <= range_y2))
    {
        return true ;
    }

    else
    {
        return false ;
    }

    return false ;
}

bool _Collision(Instance &instance_1, Instance &instance_2)
{
    bool collision_type ;
    int size_x1 ;
    int size_y1 ;
    int size_x2 ;
    int size_y2 ;

    size_x1 = instance_1._texture -> texture_x ;
    size_y1 = instance_1._texture -> texture_y ;
    size_x2 = instance_2._texture -> texture_x ;
    size_y1 = instance_2._texture -> texture_y ;

    if (instance_1.pixel_x < instance_2.pixel_x + size_x2 &&
        instance_1.pixel_x + size_x1 > instance_2.pixel_x &&
        instance_1.pixel_y < instance_2.pixel_y + size_y2 &&
        instance_1.pixel_y + size_y1 > instance_2.pixel_y)
    {
        return true ;
    }

    else
    {
        return false ;
    }

    return false ;
}

void _SendEventMass(int etype)
{
    for (int i = 0 ; i < game_current_scene -> scene_objects.size() ; i++)
    {
        game_current_scene -> scene_objects[i] -> _SendMsg(etype) ;
    }
}

void _UpdateObjectTextures()
{
    for (int i = 0 ; i < game_current_scene -> scene_textures.size() ; i++)
    {
        if (game_current_scene -> scene_textures[i].objp != -1)
        {
            game_current_scene -> scene_textures[i]._texture = game_current_scene -> scene_objects
                                                               [game_current_scene -> scene_textures[i].objp]
                                                                -> obj_sprite -> sprite_texture ;
        }
    }
}

