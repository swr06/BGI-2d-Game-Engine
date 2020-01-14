#include "Functions.h"

/*
  WM_MOUSEMOVE
...detect a mouse movement
  WM_LBUTTONDBLCLK
...detect when the left mouse button is double clicked
  WM_LBUTTONDOWN
...detect when the left mouse button is clicked down
  WM_LBUTTONUP
...detect when the left mouse button is released up
  WM_MBUTTONDBLCLK
...detect when the middle mouse button is double clicked
  WM_MBUTTONDOWN
...detect when the middle mouse button is clicked down
  WM_MBUTTONUP
...detect when the middle mouse button is released up
  WM_RBUTTONDBLCLK
...detect when the right mouse button is double clicked
  WM_RBUTTONDOWN
...detect when the right mouse button is clicked down
  WM_RBUTTONUP
...detect when the right mouse button is released up
*/

char *prepend(char* prepend_string, char* _string)
{
    char *temp = (char*) prepend_string ;

    strcat(prepend_string,_string) ;
    return _string ;
}

bool _GetPJGSize_(const char *fn, bool val)
{
    unsigned int *x = new unsigned int ;
    unsigned int *y = new unsigned int ;

    FILE *f=fopen(fn,"rb");
    fseek(f,0,SEEK_END); long len=ftell(f); fseek(f,0,SEEK_SET);
    if (len<24) {fclose(f);}

    // Strategy:
    // reading GIF dimensions requires the first 10 bytes of the file
    // reading PNG dimensions requires the first 24 bytes of the file
    // reading JPEG dimensions requires scanning through jpeg chunks
    // In all formats, the file is at least 24 bytes big, so we'll read that always

    unsigned char buf[24]; fread(buf,1,24,f);

    // For JPEGs, we need to read the first 12 bytes of each chunk.
    // We'll read those 12 bytes at buf+2...buf+14, i.e. overwriting the existing buf.
    if (buf[0]==0xFF && buf[1]==0xD8 && buf[2]==0xFF && buf[3]==0xE0 && buf[6]=='J' && buf[7]=='F' && buf[8]=='I' && buf[9]=='F')
    { long pos=2;
    while (buf[2]==0xFF)
    { if (buf[3]==0xC0 || buf[3]==0xC1 || buf[3]==0xC2 || buf[3]==0xC3 || buf[3]==0xC9 || buf[3]==0xCA || buf[3]==0xCB) break;
    pos += 2+(buf[4]<<8)+buf[5];
    if (pos+12>len) break;
    fseek(f,pos,SEEK_SET); fread(buf+2,1,12,f);
    }
    }

    fclose(f);

    // JPEG: (first two bytes of buf are first two bytes of the jpeg file; rest of buf is the DCT frame
    if (buf[0]==0xFF && buf[1]==0xD8 && buf[2]==0xFF)
    { *y = (buf[7]<<8) + buf[8];
    *x = (buf[9]<<8) + buf[10];

    }

    // GIF: first three bytes say "GIF", next three give version number. Then dimensions
    if (buf[0]=='G' && buf[1]=='I' && buf[2]=='F')
    { *x = buf[6] + (buf[7]<<8);
    *y = buf[8] + (buf[9]<<8);

    }

    // PNG: the first frame is by definition an IHDR frame, which gives dimensions
    if ( buf[0]==0x89 && buf[1]=='P' && buf[2]=='N' && buf[3]=='G' && buf[4]==0x0D && buf[5]==0x0A && buf[6]==0x1A && buf[7]==0x0A
    && buf[12]=='I' && buf[13]=='H' && buf[14]=='D' && buf[15]=='R')
    { *x = (buf[16]<<24) + (buf[17]<<16) + (buf[18]<<8) + (buf[19]<<0);
    *y = (buf[20]<<24) + (buf[21]<<16) + (buf[22]<<8) + (buf[23]<<0);
    }

    if (val == true)
    {
        return *x ;
    }

    else
    {
        return *y ;
    }
}

bool _GetPJGSize(char *fn, int *x,int *y) // credit : codeproject.org
{ FILE *f=fopen(fn,"rb"); if (f==0) return false;
    fseek(f,0,SEEK_END); long len=ftell(f); fseek(f,0,SEEK_SET);
    if (len<24) {fclose(f); return false;}

    // Strategy:
    // reading GIF dimensions requires the first 10 bytes of the file
    // reading PNG dimensions requires the first 24 bytes of the file
    // reading JPEG dimensions requires scanning through jpeg chunks
    // In all formats, the file is at least 24 bytes big, so we'll read that always
    unsigned char buf[24]; fread(buf,1,24,f);

    // For JPEGs, we need to read the first 12 bytes of each chunk.
    // We'll read those 12 bytes at buf+2...buf+14, i.e. overwriting the existing buf.
    if (buf[0]==0xFF && buf[1]==0xD8 && buf[2]==0xFF && buf[3]==0xE0 && buf[6]=='J' && buf[7]=='F' && buf[8]=='I' && buf[9]=='F')
    { long pos=2;
    while (buf[2]==0xFF)
    { if (buf[3]==0xC0 || buf[3]==0xC1 || buf[3]==0xC2 || buf[3]==0xC3 || buf[3]==0xC9 || buf[3]==0xCA || buf[3]==0xCB) break;
    pos += 2+(buf[4]<<8)+buf[5];
    if (pos+12>len) break;
    fseek(f,pos,SEEK_SET); fread(buf+2,1,12,f);
    }
    }

    fclose(f);

    // JPEG: (first two bytes of buf are first two bytes of the jpeg file; rest of buf is the DCT frame
    if (buf[0]==0xFF && buf[1]==0xD8 && buf[2]==0xFF)
    {
        *y = (buf[7]<<8) + buf[8];
        *x = (buf[9]<<8) + buf[10];
        return true;
    }

    // GIF: first three bytes say "GIF", next three give version number. Then dimensions
    if (buf[0]=='G' && buf[1]=='I' && buf[2]=='F')
    {
        *x = buf[6] + (buf[7]<<8);
        *y = buf[8] + (buf[9]<<8);
        return true;
    }

    // PNG: the first frame is by definition an IHDR frame, which gives dimensions
    if ( buf[0]==0x89 && buf[1]=='P' && buf[2]=='N' && buf[3]=='G' && buf[4]==0x0D && buf[5]==0x0A && buf[6]==0x1A && buf[7]==0x0A
    && buf[12]=='I' && buf[13]=='H' && buf[14]=='D' && buf[15]=='R')
    { *x = (buf[16]<<24) + (buf[17]<<16) + (buf[18]<<8) + (buf[19]<<0);
    *y = (buf[20]<<24) + (buf[21]<<16) + (buf[22]<<8) + (buf[23]<<0);
    return true;
    }

    return false;
}

int _GetBmpSize(char* pth, int *x, int *y)
{
    FILE *bmp ;

    if ((bmp = fopen(pth, (char*) "rb")) == NULL)
    {
        // throw_error
    }

    else
    {
        rewind(bmp);
        *x = (int)_Extract(bmp,18,4) ;
        *y = (int)_Extract(bmp,22,4) ;
    }
}

long _Extract(FILE *fp1,long offset,int size_)
{
    unsigned char *ptr ;
    unsigned char temp = '0' ;
    long value = 0 ;

    ptr = &temp ;
    fseek(fp1,offset,SEEK_SET) ;

    for(int i = 1 ; i <= size_ ; i++)
    {
        fread(ptr,sizeof(char),1,fp1) ;
        value = (long)(value+(*ptr)*(pow(256,(i-1)))) ;   //combining the values one after another in a single variable
    }

    return value ;
}

int GetImageType(const char *pth) // png, jpeg, gif, bmp
{
    if (strstr(pth,(char*) ".png"))
    {
        return t_png ;
    }

    else if (strstr(pth,(char*) ".gif"))
    {
        return t_gif ;
    }

    else if (strstr(pth,(char*) ".bmp"))
    {
        return t_bmp ;
    }
}

void GetImageSize(string path, int *x, int *y)
{
    int img_type = GetImageType((char*) path.c_str()) ;

    if (IsFileExist(path.c_str()))
    {
        if (img_type == t_png || img_type == t_gif)
        {
            _GetPJGSize((char*) (path.c_str()),x,y) ;
        }

        else
        {
            _GetBmpSize((char*) (path.c_str()),x,y) ;
        }
    }
    else
    {
        // throw error
    }
}

int GetImageSize_(string path, bool return_val) // 0 = x, 1 = y
{
    int img_type = GetImageType((char*) path.c_str()) ;
    int x_ = 0 ;
    int y_ = 0 ;

    if (IsFileExist(path.c_str()) == true)
    {
        _getch() ;

        if (img_type == t_png || img_type == t_gif)
        {
            _GetPJGSize((char*) (path.c_str()),&x_,&y_) ;
        }

        else
        {
            _GetBmpSize((char*) (path.c_str()),&x_,&y_) ;
        }

        if (return_val == 0)
        {
            return_val = x_ ;
        }

        else
        {
            return_val = y_ ;
        }
    }

    return return_val ;
}


bool IsFileExist(const char *fname)
{
    ifstream infile(fname) ;
    return infile.good() ;
}

bool AnyMouseClick()
{
    bool retval = false ;

    if (ismouseclick(WM_LBUTTONDBLCLK) == true)
    {
        //getmouseclick(WM_LBUTTONDBLCLK, mouse_clk_x, mouse_clk_y) ;
        clearmouseclick(WM_LBUTTONDBLCLK) ;
        mouse_clk = WM_LBUTTONDBLCLK ;
        mouse_clk_x = mousex() ;
        mouse_clk_y = mousey() ;
        retval = true ;
    }

    else if (ismouseclick(WM_LBUTTONDOWN) == true)
    {
        //getmouseclick(WM_LBUTTONDOWN, mouse_clk_x, mouse_clk_y) ;
        clearmouseclick(WM_LBUTTONDOWN) ;
        mouse_clk = WM_LBUTTONDOWN ;
        mouse_clk_x = mousex() ;
        mouse_clk_y = mousey() ;
        retval = true ;
    }

    else if (ismouseclick(WM_LBUTTONUP) == true)
    {
        //getmouseclick(WM_LBUTTONUP, mouse_clk_x, mouse_clk_y) ;
        clearmouseclick(WM_LBUTTONUP) ;
        mouse_clk = WM_LBUTTONUP ;
        mouse_clk_x = mousex() ;
        mouse_clk_y = mousey() ;
        retval = true ;
    }

    else if (ismouseclick(WM_MBUTTONDBLCLK) == true)
    {
        //getmouseclick(WM_MBUTTONDBLCLK, mouse_clk_x, mouse_clk_y) ;
        clearmouseclick(WM_MBUTTONDBLCLK) ;
        mouse_clk = WM_MBUTTONDBLCLK ;
        mouse_clk_x = mousex() ;
        mouse_clk_y = mousey() ;
        retval = true ;
    }

    else if (ismouseclick(WM_MBUTTONDOWN) == true)
    {
        //getmouseclick(WM_MBUTTONDOWN, mouse_clk_x, mouse_clk_y) ;
        clearmouseclick(WM_MBUTTONDOWN) ;
        mouse_clk = WM_MBUTTONDOWN ;
        mouse_clk_x = mousex() ;
        mouse_clk_y = mousey() ;
        retval = true ;
    }

    else if (ismouseclick(WM_MBUTTONUP) == true)
    {
        //getmouseclick(WM_MBUTTONUP, mouse_clk_x, mouse_clk_y) ;
        clearmouseclick(WM_MBUTTONUP) ;
        mouse_clk = WM_MBUTTONUP ;
        mouse_clk_x = mousex() ;
        mouse_clk_y = mousey() ;
        retval = true ;
    }

    else if (ismouseclick(WM_RBUTTONDBLCLK) == true)
    {
        //getmouseclick(WM_RBUTTONDBLCLK, mouse_clk_x, mouse_clk_y) ;
        clearmouseclick(WM_RBUTTONDBLCLK) ;
        mouse_clk = WM_RBUTTONDBLCLK ;
        mouse_clk_x = mousex() ;
        mouse_clk_y = mousey() ;
        retval = true ;
    }

    else if (ismouseclick(WM_RBUTTONDOWN) == true)
    {
        //getmouseclick(WM_RBUTTONDOWN, mouse_clk_x, mouse_clk_y) ;
        clearmouseclick(WM_RBUTTONDOWN) ;
        mouse_clk = WM_RBUTTONDOWN ;
        mouse_clk_x = mousex() ;
        mouse_clk_y = mousey() ;
        retval = true ;
    }

    else if (ismouseclick(WM_RBUTTONUP) == true)
    {
        //getmouseclick(WM_RBUTTONUP, mouse_clk_x, mouse_clk_y) ;
        clearmouseclick(WM_RBUTTONUP) ;
        mouse_clk = WM_RBUTTONUP ;
        mouse_clk_x = mousex() ;
        mouse_clk_y = mousey() ;
        retval = true ;
    }

    _ClearAllMouseEvents() ;

    return retval ;
}

void _ClearAllMouseEvents()
{
    clearmouseclick(WM_MOUSEMOVE) ;
    clearmouseclick(WM_RBUTTONUP) ;
    clearmouseclick(WM_LBUTTONDBLCLK) ;
    clearmouseclick(WM_LBUTTONDOWN) ;
    clearmouseclick(WM_LBUTTONUP) ;
    clearmouseclick(WM_MBUTTONDBLCLK) ;
    clearmouseclick(WM_MBUTTONDOWN) ;
    clearmouseclick(WM_MBUTTONUP) ;
    clearmouseclick(WM_RBUTTONDBLCLK) ;
    clearmouseclick(WM_RBUTTONDOWN) ;
    clearmouseclick(WM_RBUTTONUP) ;
}

int GetRecentMouseClick(int &kind, int &x, int &y)
{
    kind = mouse_clk ;
    x = mouse_clk_x ;
    y = mouse_clk_y ;
}

void HideConsole()
{
    HWND console = GetConsoleWindow() ;
    ShowWindow(console, SW_HIDE);
}

void ShowConsole()
{
    HWND console = GetConsoleWindow() ;
    ShowWindow(console, SW_SHOW);
}
