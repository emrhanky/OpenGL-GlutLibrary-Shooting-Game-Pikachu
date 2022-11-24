//Emirhan KAYA

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 500

#define TIMER_PERIOD  20 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false, fire = false, end = false, pause = true;
int  winWidth, winHeight; // current Window width and height
float pika_y = 0;
float pokeball_x = -210;
float pokeball_y = 0;
float timeforgame = 400;
int score = 0;

typedef struct ghost {
    float x = rand() % 150;
    float y = -200;
    bool appear = false;
    float R = rand() % 255,G = rand() % 255, B = rand() % 255;//for random colors
}ghost_t;
ghost_t ghosts[5];
//
// to draw circle, center at (x,y)
// radius r
//
void circle(int x, int y, int r)
{
#define PI 3.1415
    float angle;
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}
void pie_filled(float x, float y, float r, float start, float end) {
    glBegin(GL_POLYGON);
    glVertex2f(x, y);
    for (float angle = start; angle < end; angle += 10) {
        glVertex2f(r * cos(angle * D2R) + x, r * sin(angle * D2R) + y);
    }
    glVertex2f(r * cos(end * D2R) + x, r * sin(end * D2R) + y);
    glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
    float angle;

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void print(int x, int y, const char* string, void* font)
{
    int len, i;

    glRasterPos2f(x, y);
    len = (int)strlen(string);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, string[i]);
    }
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void* font, const char* string, ...)
{
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);

    int len, i;
    glRasterPos2f(x, y);
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, str[i]);
    }
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, const char* string, ...) {
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(size, size, 1);

    int len, i;
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
    }
    glPopMatrix();
}

//
// To display onto window using OpenGL commands
//

void drawpikachu()
{
    int x = -350;// i draw the pikachu (0,0) thats why i use x to swipe left
    glColor3ub(0, 0, 0);
    glRectf(0 + x, pika_y + 0, 8 + x, pika_y + -16);
    glRectf(8 + x, pika_y + -4, 12 + x, pika_y + -24);
    glRectf(4 + x, pika_y + -16, 8 + x, pika_y + -24);
    glRectf(12 + x, pika_y + -8, 24 + x, pika_y + -12);
    glRectf(20 + x, pika_y + -12, 24 + x, pika_y + -16);
    //right ear
    glRectf(108 + x, pika_y + -8, 104 + x, pika_y + -32);
    glRectf(104 + x, pika_y + -12, 100 + x, pika_y + -40);
    glRectf(100 + x, pika_y + -16, 96 + x, pika_y + -20);
    glRectf(100 + x, pika_y + -40, 96 + x, pika_y + -48);
    glRectf(92 + x, pika_y + -20, 96 + x, pika_y + -24);
    //right eye
    glRectf(96 + x, pika_y + -48, 84 + x, pika_y + -52);
    glRectf(84 + x, pika_y + -52, 88 + x, pika_y + -60);
    glRectf(92 + x, pika_y + -52, 96 + x, pika_y + -60);
    glRectf(88 + x, pika_y + -56, 92 + x, pika_y + -64);
    //right/left eye
    glColor3ub(255, 255, 255);
    glRectf(88 + x, pika_y + -52, 92 + x, pika_y + -56);
    glRectf(48 + x, pika_y + -48, 52 + x, pika_y + -52);
    //noise
    glColor3ub(0, 0, 0);
    glRectf(76 + x, pika_y + -56, 80 + x, pika_y + -60);
    //left eye
    glRectf(44 + x, pika_y + -44, 52 + x, pika_y + -48);
    glRectf(40 + x, pika_y + -48, 48 + x, pika_y + -56);
    glRectf(44 + x, pika_y + -52, 52 + x, pika_y + -60);
    glRectf(52 + x, pika_y + -48, 56 + x, pika_y + -56);
    //mouth
    glRectf(72 + x, pika_y + -64, 80 + x, pika_y + -68);
    glRectf(76 + x, pika_y + -68, 84 + x, pika_y + -72);
    glRectf(64 + x, pika_y + -68, 72 + x, pika_y + -72);
    glRectf(60 + x, pika_y + -64, 64 + x, pika_y + -68);
    glColor3ub(255, 255, 0);
    glRectf(84 + x, pika_y + -68, 88 + x, pika_y + -72);
    glRectf(76 + x, pika_y + -68, 72 + x, pika_y + -72);
    //red part
    glColor3ub(255, 0, 0);
    glRectf(32 + x, pika_y + -60, 40 + x, pika_y + -76);
    glRectf(28 + x, pika_y + -64, 44 + x, pika_y + -72);
    glRectf(88 + x, pika_y + -68, 92 + x, pika_y + -76);
    glRectf(84 + x, pika_y + -72, 88 + x, pika_y + -80);
    //yellow part
    glColor3ub(255, 255, 0);
    glRectf(12 + x, pika_y + -12, 20 + x, pika_y + -28);
    glRectf(20 + x, pika_y + -16, 28 + x, pika_y + -40);
    glRectf(28 + x, pika_y + -20, 32 + x, pika_y + -66);
    glRectf(24 + x, pika_y + -40, 28 + x, pika_y + -96);
    glRectf(32 + x, pika_y + -24, 40 + x, pika_y + -60);
    glRectf(40 + x, pika_y + -32, 64 + x, pika_y + -44);
    glRectf(64 + x, pika_y + -32, 72 + x, pika_y + -68);
    glRectf(72 + x, pika_y + -32, 76 + x, pika_y + -64);
    glRectf(76 + x, pika_y + -36, 84 + x, pika_y + -56);
    glRectf(84 + x, pika_y + -32, 96 + x, pika_y + -48);
    glRectf(96 + x, pika_y + -20, 100 + x, pika_y + -40);
    glRectf(92 + x, pika_y + -24, 96 + x, pika_y + -32);
    glRectf(88 + x, pika_y + -28, 92 + x, pika_y + -32);
    glRectf(40 + x, pika_y + -44, 44 + x, pika_y + -48);
    glRectf(52 + x, pika_y + -44, 64 + x, pika_y + -48);
    glRectf(56 + x, pika_y + -48, 64 + x, pika_y + -64);
    glRectf(52 + x, pika_y + -136, 56 + x, pika_y + -56);
    glRectf(56 + x, pika_y + -64, 60 + x, pika_y + -136);
    glRectf(60 + x, pika_y + -68, 64 + x, pika_y + -136);
    glRectf(64 + x, pika_y + -72, 68 + x, pika_y + -100);
    glRectf(68 + x, pika_y + -72, 76 + x, pika_y + -96);
    glRectf(76 + x, pika_y + -72, 84 + x, pika_y + -92);
    glRectf(80 + x, pika_y + -92, 84 + x, pika_y + -112);
    glRectf(68 + x, pika_y + -100, 88 + x, pika_y + -108);
    glRectf(76 + x, pika_y + -96, 88 + x, pika_y + -100);
    glRectf(72 + x, pika_y + -104, 84 + x, pika_y + -112);
    glRectf(40 + x, pika_y + -56, 44 + x, pika_y + -64);
    glRectf(44 + x, pika_y + -60, 52 + x, pika_y + -88);
    glRectf(20 + x, pika_y + -52, 24 + x, pika_y + -140);
    glRectf(16 + x, pika_y + -60, 20 + x, pika_y + -140);
    glRectf(12 + x, pika_y + -68, 16 + x, pika_y + -140);
    glRectf(8 + x, pika_y + -76, 12 + x, pika_y + -136);
    glRectf(4 + x, pika_y + -84, 8 + x, pika_y + -128);
    glRectf(80 + x, pika_y + -56, 84 + x, pika_y + -68);
    glRectf(76 + x, pika_y + -60, 88 + x, pika_y + -64);
    glRectf(84 + x, pika_y + -64, 92 + x, pika_y + -68);
    glRectf(0 + x, pika_y + -96, 4 + x, pika_y + -120);
    glRectf(24 + x, pika_y + -100, 28 + x, pika_y + -140);
    glRectf(28 + x, pika_y + -100, 32 + x, pika_y + -132);
    glRectf(28 + x, pika_y + -84, 36 + x, pika_y + -96);
    glRectf(28 + x, pika_y + -72, 32 + x, pika_y + -80);
    glRectf(32 + x, pika_y + -76, 40 + x, pika_y + -80);
    glRectf(40 + x, pika_y + -72, 44 + x, pika_y + -84);
    glRectf(40 + x, pika_y + -28, 64 + x, pika_y + -32);
    glRectf(68 + x, pika_y + -92, 76 + x, pika_y + -100);
    glRectf(36 + x, pika_y + -84, 40 + x, pika_y + -80);
    glRectf(48 + x, pika_y + -88, 52 + x, pika_y + -92);
    glRectf(32 + x, pika_y + -96, 44 + x, pika_y + -100);
    glRectf(36 + x, pika_y + -92, 48 + x, pika_y + -96);
    glRectf(36 + x, pika_y + -88, 44 + x, pika_y + -92);
    glRectf(32 + x, pika_y + -104, 52 + x, pika_y + -124);
    glRectf(36 + x, pika_y + -124, 52 + x, pika_y + -136);
    glRectf(32 + x, pika_y + -136, 36 + x, pika_y + -132);
    glRectf(48 + x, pika_y + -96, 52 + x, pika_y + -100);
    glRectf(44 + x, pika_y + -100, 52 + x, pika_y + -104);
    glRectf(64 + x, pika_y + -104, 68 + x, pika_y + -140);
    glRectf(68 + x, pika_y + -108, 72 + x, pika_y + -140);
    glRectf(72 + x, pika_y + -112, 84 + x, pika_y + -124);
    glRectf(72 + x, pika_y + -124, 80 + x, pika_y + -128);
    glRectf(72 + x, pika_y + -128, 76 + x, pika_y + -132);
    glRectf(72 + x, pika_y + -136, 76 + x, pika_y + -140);
    //tail
    glRectf(-20 + x, pika_y + -40, -28 + x, pika_y + -36);
    glRectf(-28 + x, pika_y + -40, -16 + x, pika_y + -44);
    glRectf(-32 + x, pika_y + -44, -12 + x, pika_y + -52);
    glRectf(-36 + x, pika_y + -52, -4 + x, pika_y + -56);
    glRectf(-36 + x, pika_y + -56, 0 + x, pika_y + -60);
    glRectf(-32 + x, pika_y + -60, 0 + x, pika_y + -64);
    glRectf(-28 + x, pika_y + -64, 0 + x, pika_y + -68);
    glRectf(-24 + x, pika_y + -68, -4 + x, pika_y + -72);
    glRectf(-20 + x, pika_y + -72, -8 + x, pika_y + -76);
    glRectf(-16 + x, pika_y + -76, -8 + x, pika_y + -80);
    glRectf(-20 + x, pika_y + -80, -8 + x, pika_y + -84);
    glRectf(-20 + x, pika_y + -84, -12 + x, pika_y + -88);
    glRectf(-24 + x, pika_y + -88, -4 + x, pika_y + -92);
    glRectf(-24 + x, pika_y + -92, 0 + x, pika_y + -96);
    glRectf(-16 + x, pika_y + -96, -4 + x, pika_y + -100);
    glRectf(-4 + x, pika_y + -100, -8 + x, pika_y + -116);
    glRectf(-8 + x, pika_y + -100, -12 + x, pika_y + -104);
    glRectf(-8 + x, pika_y + -112, -12 + x, pika_y + -116);
    //brown part
    glColor3ub(153, 76, 0);
    glRectf(24 + x, pika_y + -12, 28 + x, pika_y + -16);
    glRectf(28 + x, pika_y + -16, 32 + x, pika_y + -20);
    glRectf(32 + x, pika_y + -20, 40 + x, pika_y + -24);
    glRectf(40 + x, pika_y + -24, 64 + x, pika_y + -28);
    glRectf(64 + x, pika_y + -28, 76 + x, pika_y + -32);
    glRectf(76 + x, pika_y + -32, 84 + x, pika_y + -36);
    glRectf(84 + x, pika_y + -28, 88 + x, pika_y + -32);
    glRectf(88 + x, pika_y + -24, 92 + x, pika_y + -28);
    glRectf(12 + x, pika_y + -24, 20 + x, pika_y + -36);
    glRectf(16 + x, pika_y + -36, 20 + x, pika_y + -40);
    glRectf(20 + x, pika_y + -40, 24 + x, pika_y + -52);
    glRectf(16 + x, pika_y + -52, 20 + x, pika_y + -60);
    glRectf(12 + x, pika_y + -60, 16 + x, pika_y + -68);
    glRectf(0 + x, pika_y + -56, 4 + x, pika_y + -68);
    glRectf(8 + x, pika_y + -68, 12 + x, pika_y + -76);
    glRectf(4 + x, pika_y + -76, 8 + x, pika_y + -84);
    glRectf(0 + x, pika_y + -84, 4 + x, pika_y + -96);
    glRectf(0 + x, pika_y + -120, 4 + x, pika_y + -128);
    glRectf(-4 + x, pika_y + -88, 0 + x, pika_y + -92);
    glRectf(0 + x, pika_y + -96, -4 + x, pika_y + -120);
    glRectf(-4 + x, pika_y + -116, -12 + x, pika_y + -120);
    glRectf(-12 + x, pika_y + -108, -16 + x, pika_y + -116);
    glRectf(-8 + x, pika_y + -104, -12 + x, pika_y + -112);
    glRectf(-12 + x, pika_y + -104, -16 + x, pika_y + -100);
    glRectf(0 + x, pika_y + -56, -4 + x, pika_y + -52);
    glRectf(-4 + x, pika_y + -48, -12 + x, pika_y + -52);
    glRectf(-12 + x, pika_y + -44, -8 + x, pika_y + -48);
    glRectf(-12 + x, pika_y + -40, -16 + x, pika_y + -44);
    glRectf(-16 + x, pika_y + -36, -20 + x, pika_y + -40);
    glRectf(-20 + x, pika_y + -32, -28 + x, pika_y + -36);
    glRectf(-28 + x, pika_y + -36, -32 + x, pika_y + -44);
    glRectf(-32 + x, pika_y + -44, -36 + x, pika_y + -52);
    glRectf(-36 + x, pika_y + -52, -40 + x, pika_y + -60);
    glRectf(-32 + x, pika_y + -60, -36 + x, pika_y + -64);
    glRectf(-28 + x, pika_y + -64, -32 + x, pika_y + -68);
    glRectf(-24 + x, pika_y + -68, -28 + x, pika_y + -72);
    glRectf(-20 + x, pika_y + -72, -24 + x, pika_y + -76);
    glRectf(-16 + x, pika_y + -76, -20 + x, pika_y + -80);
    glRectf(-20 + x, pika_y + -80, -24 + x, pika_y + -88);
    glRectf(-24 + x, pika_y + -88, -28 + x, pika_y + -96);
    glRectf(-24 + x, pika_y + -96, -16 + x, pika_y + -100);
    glRectf(0 + x, pika_y + -68, -4 + x, pika_y + -72);
    glRectf(-4 + x, pika_y + -72, -8 + x, pika_y + -88);
    glRectf(-8 + x, pika_y + -84, -12 + x, pika_y + -88);
    glRectf(4 + x, pika_y + -128, 8 + x, pika_y + -136);
    glRectf(8 + x, pika_y + -136, 12 + x, pika_y + -140);
    glRectf(12 + x, pika_y + -140, 28 + x, pika_y + -144);
    glRectf(28 + x, pika_y + -132, 32 + x, pika_y + -140);
    glRectf(32 + x, pika_y + -132, 36 + x, pika_y + -124);
    glRectf(32 + x, pika_y + -136, 64 + x, pika_y + -140);
    glRectf(64 + x, pika_y + -140, 76 + x, pika_y + -144);
    glRectf(76 + x, pika_y + -140, 80 + x, pika_y + -136);
    glRectf(72 + x, pika_y + -136, 76 + x, pika_y + -132);
    glRectf(76 + x, pika_y + -132, 80 + x, pika_y + -128);
    glRectf(80 + x, pika_y + -128, 84 + x, pika_y + -124);
    glRectf(84 + x, pika_y + -124, 88 + x, pika_y + -112);
    glRectf(84 + x, pika_y + -112, 72 + x, pika_y + -108);
    glRectf(84 + x, pika_y + -108, 88 + x, pika_y + -104);
    glRectf(72 + x, pika_y + -108, 68 + x, pika_y + -104);
    glRectf(68 + x, pika_y + -104, 64 + x, pika_y + -96);
    glRectf(88 + x, pika_y + -104, 92 + x, pika_y + -92);
    glRectf(68 + x, pika_y + -92, 80 + x, pika_y + -96);
    glRectf(84 + x, pika_y + -96, 88 + x, pika_y + -80);
    glRectf(92 + x, pika_y + -76, 96 + x, pika_y + -60);
    glRectf(28 + x, pika_y + -80, 36 + x, pika_y + -84);
    glRectf(36 + x, pika_y + -84, 44 + x, pika_y + -88);
    glRectf(44 + x, pika_y + -88, 48 + x, pika_y + -92);
    glRectf(48 + x, pika_y + -92, 52 + x, pika_y + -96);
    glRectf(48 + x, pika_y + -96, 44 + x, pika_y + -100);
    glRectf(44 + x, pika_y + -100, 32 + x, pika_y + -104);
    glRectf(32 + x, pika_y + -96, 24 + x, pika_y + -100);
}
void drawpokeball()
{
    glColor3ub(1, 1, 1);
    circle(pokeball_x + -30, pika_y + -100, 24);
    glColor3ub(255, 0, 0);
    pie_filled(pokeball_x + -30, pika_y + -100, 20, 0, 180);
    glColor3ub(255, 255, 255);
    pie_filled(pokeball_x + -30, pika_y + -100, 20, 180, 360);
    glColor3ub(1, 1, 1);
    glRectf(pokeball_x + -6, pika_y + -97, pokeball_x + -54, pika_y + -103);
    glColor3ub(1, 1, 1);
    circle(pokeball_x + -30, pika_y + -100, 8);
    glColor3ub(255, 255, 255);
    circle(pokeball_x + -30, pika_y + -100, 4);
}
void drawghost(ghost_t ghost)
{
    int g = 150;
    glColor3ub(1, 1, 1);
    circle(ghost.x + g + 50, ghost.y + 50, 43);
    glColor3ub(ghost.R+255, ghost.G, ghost.B);
    circle(ghost.x + g + 50, ghost.y + 50, 40);

    glColor3ub(255, 255, 255);
    circle(ghost.x + g + 57, ghost.y + 57, 12);

    glColor3ub(51, 153, 255);
    circle(ghost.x + g + 57, ghost.y + 57, 6);

    glColor3ub(255, 255, 255);
    circle(ghost.x + g + 28, ghost.y + 57, 12);

    glColor3ub(51, 153, 255);
    circle(ghost.x + g + 28, ghost.y + 57, 6);

}
void pausescreen()
{
    glColor3ub(1, 1, 1);
    glRectf(-400, -300, 400, 300);
    glColor3ub(255, 255, 255);
    glRectf(-40, 100, -80, -100);
    glRectf(80, 100, 40, -100);
    glColor3ub(255, 255, 255);
    vprint(-130, -130, GLUT_BITMAP_TIMES_ROMAN_24, "Do you want some break? :)");
    vprint(-116, 130, GLUT_BITMAP_TIMES_ROMAN_24, "The game has paused");
}
void endscreen()
{
    glColor3ub(160, 160, 160);
    glRectf(-400, -300, 400, 300);
    glColor3ub(255, 255, 255);
    vprint(-80, 20, GLUT_BITMAP_TIMES_ROMAN_24, "GAME OVER!");
    vprint(-150, -100, GLUT_BITMAP_TIMES_ROMAN_24, "PRESS F1 TO START AGAIN");
    vprint(-83, -20, GLUT_BITMAP_TIMES_ROMAN_24, "Your score is: %d",score);
    if(score==0)
        vprint(-120, -60, GLUT_BITMAP_TIMES_ROMAN_24, "Didn't you like the game ?");
    else if (score <= 10) 
        vprint(-78, -60, GLUT_BITMAP_TIMES_ROMAN_24, "Try a bit harder :|");
    else if(score<=20)
        vprint(-78, -60, GLUT_BITMAP_TIMES_ROMAN_24, "Well Done :D ");
    else
        vprint(-100, -60, GLUT_BITMAP_TIMES_ROMAN_24, "You are insane @_@");
    
}
void display() {
    //
    // clear window to black
    //

    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    //left ear
    glColor3ub(0, 210, 128);
    glRectf(-400, -300, 400, 300);
   

   
    drawpikachu();
    drawpokeball();

    for (int i = 0; i < 5; i++)
    {
        
        if (ghosts[i].appear == true)
        {
            drawghost(ghosts[i]);
          
                    
        }
    }
    
    glColor3ub(1, 1, 1);
    vprint(-370, 220, GLUT_BITMAP_8_BY_13, "Emirhan KAYA");
    vprint(-370, 205, GLUT_BITMAP_8_BY_13, "22002062");

    vprint(280, 220, GLUT_BITMAP_8_BY_13, "REMAINING TIME");
    vprint(280, 205, GLUT_BITMAP_8_BY_13, "%0.2f", timeforgame / 20);

    
    vprint(-370, -235, GLUT_BITMAP_8_BY_13, "Press <Spacebar> to throw the pokeball.");
    vprint(-370, -220, GLUT_BITMAP_8_BY_13, "Press <F1> to start/pause the game.");
    vprint(-370, -205, GLUT_BITMAP_8_BY_13, "Press up and down keys to relocate the Pikachu.");
    vprint(280, 190, GLUT_BITMAP_8_BY_13, "Score:%d", score);
    if (end == true & timeforgame == 400)
    {
        endscreen();
    }

    if (pause == true && timeforgame < 400)
    {
        pausescreen();
    }
    glutSwapBuffers();
   
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);
    if (key == ' ')
        fire = true;
    // to refresh the window it calls display() function
    glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP:
        up = true;
        if (pika_y < 250 && fire == false)
            pika_y += 10;
        break;
    case GLUT_KEY_DOWN:
        down = true;
        if (pika_y > -105 && fire == false)
            pika_y -= 10;
        break;

    case GLUT_KEY_F1:
        
        if (end)
        {
            score = 0;
            end = false;
            fire = false;
            pika_y = 0;
            pokeball_x = -210;
            timeforgame = 400;
           
            for (int i = 0; i < 5; i++)
            {
              
                ghost_t neww;
                ghosts[i] = neww;
                ghosts[i].y = -360 - 120 * i;
                ghosts[i].appear = true;
                
            }
            
        }
        else
            
            pause = !pause;
        
    case GLUT_KEY_LEFT: left = true; break;
    case GLUT_KEY_RIGHT: right = true; break;
        
    }

    // to refresh the window it calls display() function
    

    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = false; break;
    case GLUT_KEY_DOWN: down = false; break;
    case GLUT_KEY_LEFT: left = false; break;
    case GLUT_KEY_RIGHT: right = false; break;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
void onClick(int button, int stat, int x, int y)
{
    // Write your codes here.



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
    winWidth = w;
    winHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    display(); // refresh window.
}

void onMoveDown(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function   
    glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
    // Write your codes here.
    
    if (timeforgame <= 0)
    {
        end = true;
        timeforgame = 400;
        
        
    }
    else if (!end && !pause)
    {
        timeforgame -= 0.4;
        
    }
   

    if (pause == false && end == false)
    {
        
        for (int i = 0; i < 5; i++)
        {
            ghosts[i].y += 3;
            if (ghosts[i].appear == true)
            {
                if (fire)
                {
                    if (pokeball_x >= ghosts[i].x + 170 && pokeball_x <= ghosts[i].x + 250)
                    {
                        if (pika_y - 100 >= ghosts[i].y - 20 && pika_y - 100 <= ghosts[i].y + 100)
                        {
                            fire = false;
                            pokeball_x = -210;
                            ghosts[i].appear = false;
                            score += 1;
                        }
                    }

                    else if (pokeball_x > 450)
                    {
                        fire = false;
                        pokeball_x = -210;
                    }
                }
            }
            if (ghosts[i].y > 200)
            {
                ghosts[i].y = -400;
                ghosts[i].appear = true;
            }


        }
        if (fire && end == false)
            pokeball_x += 15;
    }


    // to refresh the window it calls display() function
    glutPostRedisplay(); // display()

}
#endif

void Init() {

    // Smoothing shapes
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    for (int i = 0; i < 5; i++)
    {
        ghosts[i].y = -360 - 120 * i;
        ghosts[i].appear = true;
    }

}

void main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Emirhan KAYA");
    glutDisplayFunc(display);
    glutReshapeFunc(onResize);
    
    //
    // keyboard registration
    //
    glutKeyboardFunc(onKeyDown);
    glutSpecialFunc(onSpecialKeyDown);

    glutKeyboardUpFunc(onKeyUp);
    glutSpecialUpFunc(onSpecialKeyUp);

    //
    // mouse registration
    //
    glutMouseFunc(onClick);
    glutMotionFunc(onMoveDown);
    glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
    // timer event
    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

    Init();

    glutMainLoop();
}
