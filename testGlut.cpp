// testGlut.cpp
// 2020/09/29

#include <cstdio>
#include <cstdlib>
#include <cmath>

#include <GL/glut.h>

#define SIZEX 1000
#define SIZEY 1000

enum COLOR {
    white,
    red,
    green,
    blue
};

int width = 800,
    height = 600;

unsigned char image[SIZEX*SIZEY][4];

int cursor_x = -1,
    cursor_Y = -1;

float current_color[3] = {0.0, 1.0, 0.0};

float paint_size = 1.0;

static void resize(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, (double) width, 0.0, (double) height);
    glViewport(0,0,width, height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
int x = 0, y = 0;
void display_func(void){
    glClear(GL_COLOR_BUFFER_BIT);  //make glClearColor() works
    glColor3f(0.0f, 1.0f, 0.0f);
    glPointSize(5.0f);
    //glRecti(50, 50, 190, 190);
  glBegin(GL_POLYGON);
    /*glVertex2f(-1.0f, -1.0f);
    glVertex2f( 0.5f, -0.5f);
    glVertex2f( 0.5f,  0.5f);
    glVertex2f(-0.5f,  0.5f);*/
    glVertex3f(x+50, y+50, 0);
    glVertex3f(x+100, y+50, 0);
    glVertex3f(x+100, y+100, 0);
    glVertex3f(x+50, y+100, 0);
  glEnd();
    x++, y++;
    glFlush();
}




/* 
 * Main ^_^
 */
int main (int argc, char **argv){
    glutInit(&argc, argv);  //connect with server

    glutInitWindowPosition(50, 50);  //window position
    glutInitWindowSize(width, height);  //window size
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGBA);

//---init_func segment---
    glReadBuffer(GL_FRONT);  //?????
    glDrawBuffer(GL_FRONT);  //?????
    glPixelStorei(GL_PACK_ALIGNMENT, 1);  //?????
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);  //?????
//---END init_func segment---


    glutCreateWindow("I M Title");  //?????

    glClearColor(1.0F, 1.0F,1.0F, 0);  //set the background color

    glutReshapeFunc(resize);
    glutDisplayFunc(display_func);



//---event loop
    glutMainLoop();
//---END event loop

    return 0;
}


