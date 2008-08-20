/*
 * dispcurs.h - <disp.h> driver for the MyMan video game
 * Copyright 2007-2008, Benjamin C. Wiley Sittler <bsittler@gmail.com>
 *
 *  Permission is hereby granted, free of charge, to any person
 *  obtaining a copy of this software and associated documentation
 *  files (the "Software"), to deal in the Software without
 *  restriction, including without limitation the rights to use, copy,
 *  modify, merge, publish, distribute, sublicense, and/or sell copies
 *  of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be
 *  included in all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *  NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *  HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 *  DEALINGS IN THE SOFTWARE.
 */

/* work-arounds for <disp.h> (Digital Mars/Symantec/Zortech C/C++) */
#include <conio.h>
#include <disp.h>
#include <sound.h>

#if defined(WIN32)
#include <windows.h>
#endif

#if defined(__MSDOS__)
#include <dos.h>
#endif

#undef timeval
#define timeval dispcurses_timeval
struct timeval
{
    long tv_sec;
    long tv_usec;
};

#undef gettimeofday
#define gettimeofday dispcurses_gettimeofday
static int gettimeofday(struct timeval *tv, void *tz)
{

#if defined(WIN32)

/* originally from http://curl.haxx.se/mail/lib-2005-01/0089.html by Gisle Vanem */
    union {
        long long ns100;
        FILETIME ft;
    } now;
    GetSystemTimeAsFileTime(&now.ft);
    tv->tv_usec = (long) ((now.ns100 / 10LL) % 1000000LL);
    tv->tv_sec = (long) ((now.ns100 - 116444736000000000LL) / 10000000LL);

#else /* ! defined(WIN32) */

#if defined(__MSDOS__)

/* HACK: this is wildly, wildly wrong for anything but tick counting! */

/* originally from http://www.lightner.net/lightner/bruce/photopc/msdos/patch/usleep.c */

    static unsigned long far *p;
    unsigned long ticks;

    if (!p) p = MK_FP(0, 0x46c);
    ticks = *p;
    tv->tv_usec = ((ticks & 0xff) * 55000L) % 1000000L;
    tv->tv_sec = (ticks & 0xff) * 55L / 1000L;

#else /* ! defined(__MSDOS__) */

#error no gettimeofday(2) implementation for your platform, sorry
    return 1;

#endif /* ! defined(__MSDOS__) */

#endif /* ! defined(WIN32) */

    return 0;
}

#ifdef WIN32

#undef usleep

/* originally from http://wyw.dcweb.cn/sleep.h.txt by Wu Yongwei */

#define usleep(t) Sleep((t) / 1000)

#else /* ! defined(WIN32) */

#undef usleep
#define usleep dispcurses_usleep
static int usleep(unsigned long usecs)
{
    while (usecs)
    {
        struct timeval tv0, tv1;

        if (gettimeofday(&tv0, NULL)) break;
        sleep(0);
        if (gettimeofday(&tv1, NULL)) break;
        if (tv1.tv_sec < tv0.tv_sec) break;
        if ((tv1.tv_sec == tv0.tv_sec)
            &&
            (tv1.tv_usec <= tv0.tv_usec))
        {
            break;
        }
        if ((tv1.tv_sec - tv0.tv_sec) > (usecs / 1000000L))
        {
            break;
        }
        if (((tv1.tv_sec - tv0.tv_sec) * 1000000L
             +
             (tv1.tv_usec - tv0.tv_usec))
            >=
            usecs)
        {
            break;
        }
        usecs -= (tv1.tv_sec - tv0.tv_sec) * 1000000L;
        usecs -= tv1.tv_usec - tv0.tv_usec;
    }
    return 0;
}

#endif /* ! defined(WIN32) */

#ifndef USE_ATTR
#define USE_ATTR 1
#endif

#ifndef USE_UNDERLINE
#define USE_UNDERLINE 1
#endif

#ifndef SWAPDOTS
#define SWAPDOTS 0
#endif

#ifndef USE_ACS
#define USE_ACS 1
#endif

#ifndef USE_RAW_UCS
#define USE_RAW_UCS 0
#endif

#ifndef USE_SIGWINCH
#define USE_SIGWINCH 0
#endif

#ifndef USE_IOCTL
#define USE_IOCTL 0
#endif

#ifndef USE_WCWIDTH
#define USE_WCWIDTH 1
#endif

#ifndef USE_ICONV
#define USE_ICONV 0
#endif

#ifndef OK
#define OK (0)
#endif

#ifndef ERR
#define ERR (-1)
#endif

#ifndef FALSE
#define FALSE (0)
#endif

#ifndef TRUE
#define TRUE (! FALSE)
#endif

#undef wcwidth
#define wcwidth(ch) dispcurses_wcwidth(ch)

static int dispcurses_ready = 0;

typedef unsigned long int dispcurses_chtype;

#undef chtype
#define chtype dispcurses_chtype

typedef dispcurses_chtype dispcurses_attr_t;

#undef attr_t
#define attr_t dispcurses_attr_t

static unsigned long dispcurses_w = 0, dispcurses_h = 0;

static unsigned long dispcurses_x = 0, dispcurses_y = 0;

static dispcurses_attr_t dispcurses_attr = 0;

#define clear erase

#define cbreak()

#define noecho()

#define nonl()

#define nodelay(stdscr, x)

#define intrflush(stdscr, x)

#define immedok(stdscr, x)

#define idlok(stdscr, x)

#define clearok(stdscr, x)

#define leaveok(stdscr, x)

#define getyx(stdscr, y, x) ((y) = dispcurses_y), ((x) = dispcurses_x)

#define wrefresh(stdscr) refresh()

#define beep() dispcurses_addch('\a')

#define use_env(x)

#define echo()

#define can_change_color() (0)

#define keypad(stdscr, x)

#define LINES (dispcurses_h)

#define COLS (dispcurses_w)

#undef KEY_RESIZE
#define KEY_RESIZE 0xef00

#undef KEY_UP
#define KEY_UP 0x0f01

#undef KEY_DOWN
#define KEY_DOWN 0xef02

#undef KEY_LEFT
#define KEY_LEFT 0xef03

#undef KEY_RIGHT
#define KEY_RIGHT 0xef04

#define A_DIM 0x800000UL

#define A_UNDERLINE A_DIM

#define A_BOLD 0x400000UL

#define A_STANDOUT 0x200000UL

#define A_CHARTEXT 0x1fffffUL

#define A_REVERSE A_STANDOUT

#define DISPCURSES___A_COLOR 0xff000000UL

#define COLOR_PAIRS 256

#define COLORS 16

#define COLOR_BLACK 0

#define COLOR_BLUE 1

#define COLOR_GREEN 2

#define COLOR_CYAN (COLOR_BLUE | COLOR_GREEN)

#define COLOR_RED 4

#define COLOR_MAGENTA (COLOR_RED | COLOR_BLUE)

#define COLOR_YELLOW (COLOR_RED | COLOR_GREEN)

#define COLOR_WHITE (COLOR_RED | COLOR_GREEN | COLOR_BLUE)

#define PEN_BRIGHT 8

#define _PAIR_SHIFT 24

#define COLOR_PAIR(p) (((dispcurses_attr_t) p) << _PAIR_SHIFT)

#define PAIR_NUMBER(a) (((a) & DISPCURSES___A_COLOR) >> _PAIR_SHIFT)

#define ACS_LRCORNER 0xd9UL

#define ACS_URCORNER 0xbfUL

#define ACS_ULCORNER 0xdaUL

#define ACS_LLCORNER 0xc0UL

#define ACS_PLUS 0xc5UL

#define ACS_HLINE 0xc4UL

#define ACS_LTEE 0xc3UL

#define ACS_RTEE 0xb4UL

#define ACS_BTEE 0xc1UL

#define ACS_TTEE 0xc2UL

#define ACS_VLINE 0xb3UL

/* #define ACS_BULLET 0xf9UL */

#define ACS_BULLET 0xfeUL

#define ACS_BLOCK 0xdbUL

static struct { short fg, bg; } dispcurses_pairs[COLOR_PAIRS];

static int dispcurses_wcwidth(dispcurses_chtype ch)
{
    ch &= A_CHARTEXT;
    switch (ch)
    {
    case ACS_LRCORNER:
    case ACS_URCORNER:
    case ACS_ULCORNER:
    case ACS_LLCORNER:
    case ACS_PLUS:
    case ACS_HLINE:
    case ACS_LTEE:
    case ACS_RTEE:
    case ACS_BTEE:
    case ACS_TTEE:
    case ACS_VLINE:
    case ACS_BULLET:
    case ACS_BLOCK:
        return 1;
    }
    return ((((ch) >= ' ') && ((ch) == (dispcurses_chtype) (unsigned long) (unsigned char) (char) ch)) ? 1 : 0);
}

static int curs_set(x)
{
    if (! dispcurses_ready) return ERR;
    if (x)
        disp_showcursor();
    else
        disp_hidecursor();
    return OK;
}

static int attrset(dispcurses_attr_t a)
{
    if (! dispcurses_ready) return ERR;
    dispcurses_attr = a;
    return OK;
}

static int standout(void)
{
    return attrset(A_STANDOUT);
}

static int standend(void)
{
    return attrset(0);
}

static int has_colors(void)
{
    if (! dispcurses_ready) return ERR;
    return ! disp_mono;
}

static int start_color(void)
{
    if (! dispcurses_ready) return ERR;
    if (! has_colors()) return ERR;
    return OK;
}

static int init_pair(short i, short fg, short bg);

#define INITSCR_WITH_HINTS

static void initscrWithHints(int h, int w, const char *title, const char *shortname)
{
    int i;

    disp_open();

    if (getenv("DISPCURSES_USEBIOS") && *getenv("DISPCURSES_USEBIOS"))
    {
        if (strcmp(getenv("DISPCURSES_USEBIOS"), "0"))
        {
#ifndef WIN32
            disp_usebios();
#endif
        }
    }

    if (! w) w = 80;
    if (! h) h = 25;
    dispcurses_w = disp_numcols ? disp_numcols : w;
    dispcurses_h = disp_numrows ? disp_numrows : h;
    dispcurses_ready = 1;
    dispcurses_attr = -1;
    for (i = 0; i < COLOR_PAIRS; i ++)
    {
        dispcurses_pairs[i].fg = i ? (i % COLORS) : COLOR_WHITE;
        dispcurses_pairs[i].bg = i / COLORS;
    }
    disp_startstand();
    disp_endstand();
    attrset(0);
}

static void endwin(void)
{
    dispcurses_ready = 0;
    disp_close();
}

static int move(int y, int x);

static int dispcurses_addch(dispcurses_chtype ch);

static int erase(void)
{
    if (! dispcurses_ready) return ERR;
    attrset(0);
    move(0, 0);
    disp_move(dispcurses_y, dispcurses_x);
    disp_eeop();
    return OK;
}

static int resizeterm(int y, int x)
{
    if (! dispcurses_ready) return ERR;
    dispcurses_w = disp_numcols;
    dispcurses_h = disp_numrows;
    return OK;
}

static int dispcurses_getch(void) {
    int ret = ERR;
    int i, j;

    if (! dispcurses_ready) return (dispcurses_chtype) ERR;
    i = disp_numcols;
    j = disp_numrows;
    if ((i != dispcurses_w) || (j != dispcurses_h))
    {
        return KEY_RESIZE;
    }
    if (kbhit())
    {
        ret = getch();
        if (ret == 0)
        {
            ret = getch();
            if ((ret == 0)
                ||
                (ret == 0x03))
            {
                ret = 0;
            }
            else if (ret == 0x48)
            {
                return KEY_UP;
            }
            else if (ret == 0x4b)
            {
                return KEY_LEFT;
            }
            else if (ret == 0x4d)
            {
                return KEY_RIGHT;
            }
            else if (ret == 0x50)
            {
                return KEY_DOWN;
            }
            else
            {
                ret = ERR;
            }
        }
    }
    return ret;
}

#undef getch
#define getch dispcurses_getch

#undef addch
#define addch dispcurses_addch

static int refresh(void)
{
    if (! dispcurses_ready) return OK;
    disp_move(dispcurses_y, dispcurses_x);
    disp_flush();
    return OK;
}

static int move(int y, int x)
{
    if (! dispcurses_ready) return ERR;
    if (y < 0) return ERR;
    if (y > dispcurses_h) return ERR;
    if (x < 0) return ERR;
    if (x > dispcurses_w) return ERR;
    dispcurses_y = y;
    dispcurses_x = x;
    return OK;
}

static int insch(const dispcurses_chtype ch)
{
    return ERR;
}

static int init_pair(short i, short fg, short bg)
{
    if (! dispcurses_ready) return ERR;
    if (! has_colors()) return ERR;
    if ((i < 0) || (i > COLOR_PAIRS)) return ERR;
    if ((fg < 0) || (fg > COLORS)) return ERR;
    if ((bg < 0) || (bg > COLORS)) return ERR;
    dispcurses_pairs[i].fg = fg;
    dispcurses_pairs[i].bg = bg;
    return OK;
}

static int pair_content(short i, short *fg, short *bg)
{
    if (! dispcurses_ready) return ERR;
    if (! has_colors()) return ERR;
    if ((i < 0) || (i > COLOR_PAIRS)) return ERR;
    *fg = dispcurses_pairs[i].fg;
    *bg = dispcurses_pairs[i].bg;
    return OK;
}

static int init_color(short i, short r, short g, short b)
{
    return ERR;
}

static int color_content(short i, short *r, short *g, short *b)
{
    return ERR;
}

static int dispcurses_addch(dispcurses_chtype ch)
{
    short fg, bg;

    if (! dispcurses_ready) return ERR;
    if (ch & ~A_CHARTEXT)
    {
        int ret;
        dispcurses_attr_t a;

        a = dispcurses_attr;
        attrset((PAIR_NUMBER(ch & ~A_CHARTEXT) ? (a & ~COLOR_PAIR(PAIR_NUMBER(a))) : a) | (ch & ~A_CHARTEXT));
        ret = dispcurses_addch(ch & A_CHARTEXT);
        attrset(a);
        return ret;
    }
    if (ch == (dispcurses_chtype) (unsigned long) (unsigned char) '\a')
    {
        sound_beep(440);
        return OK;
    }
    else if (ch == (dispcurses_chtype) (unsigned long) (unsigned char) '\b')
    {
        if (dispcurses_y && ! dispcurses_x)
        {
            move(dispcurses_y - 1, dispcurses_w - 1);
        }
        else if (dispcurses_x)
        {
            move(dispcurses_y, dispcurses_x - 1);
        }
        return OK;
    }
    else if (ch == (dispcurses_chtype) (unsigned long) (unsigned char) '\n')
    {
        if ((dispcurses_y + 1) < dispcurses_h)
        {
            move(dispcurses_y + 1, 0);
        }
        else
        {
            move(dispcurses_y, 0);
        }
        return OK;
    }
    else if (ch == (dispcurses_chtype) (unsigned long) (unsigned char) '\t')
    {
        do
        {
            dispcurses_addch((dispcurses_chtype) (unsigned long) (unsigned char) ' ');
        }
        while (dispcurses_x % 8);
        return OK;
    }
    else if (wcwidth(ch) < 1)
    {
        return ERR;
    }
    fg = COLOR_WHITE;
    bg = COLOR_BLACK;
    if (has_colors())
    {
        pair_content(PAIR_NUMBER(dispcurses_attr), &fg, &bg);
    }
    if (dispcurses_attr & A_UNDERLINE)
    {
        fg = COLOR_BLUE;
        bg = COLOR_BLACK;
    }
    else if ((dispcurses_attr & A_STANDOUT) && has_colors())
    {
        fg = COLOR_MAGENTA;
        bg = COLOR_BLACK;
    }
    else if ((dispcurses_attr & A_BOLD) && has_colors())
    {
        fg = COLOR_YELLOW | PEN_BRIGHT;
        bg = COLOR_BLACK;
    }
    if (has_colors() && PAIR_NUMBER(dispcurses_attr))
    {
        pair_content(PAIR_NUMBER(dispcurses_attr), &fg, &bg);
    }
    if (dispcurses_attr & A_BOLD)
    {
        fg |= PEN_BRIGHT;
    }
    if (dispcurses_attr & (A_STANDOUT | A_REVERSE))
    {
        short tmp;

        tmp = bg;
        bg = fg;
        fg = tmp;
    }
    if (dispcurses_x < 0) dispcurses_x = 0;
    if (dispcurses_x >= dispcurses_w)
    {
        dispcurses_x = 0;
        dispcurses_y ++;
    }
    if (dispcurses_y < 0) dispcurses_y = 0;
    if (dispcurses_y >= dispcurses_h)
    {
        dispcurses_y = dispcurses_h - 1;
    }
    disp_pokew(dispcurses_y, dispcurses_x, (ch & 0xff) | ((bg & 0x7) << 12) | ((fg & 0xf) << 8));
    dispcurses_x ++;
    return OK;
}

static int clrtobot(void)
{
    if (! dispcurses_ready) return ERR;
    disp_move(dispcurses_y, dispcurses_x);
    disp_eeop();
    return OK;
}

static int clrtoeol(void)
{
    if (! dispcurses_ready) return ERR;
    disp_move(dispcurses_y, dispcurses_x);
    disp_eeol();
    return OK;
}

static int addstr(const char *s)
{
    if (! dispcurses_ready) return ERR;
    while (*s)
    {
        dispcurses_addch(*s++);
    }
    return OK;
}

static int mvprintw(int y, int x, const char *s)
{
    if (! dispcurses_ready) return ERR;
    move(y, x);
    return addstr(s);
}
