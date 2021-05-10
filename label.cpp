// label.cpp
// 2020/10/12 18:44
// by Eric W. 00757036

#include <cstdio>
#include <cstdlib>
#include <string>

#include <GL/glut.h>

#include "color.h"

#ifndef winWH
    #define winWH
    const int window_width = 400,
              window_height = 400;
#endif

class Label {
public:
    int x = -1,
        y = -1;
    int w = 8,
        h = 14;
    int size = 1;
    
    //left or center
    
    bool background = false;
    std::string content;
    Color background_color{0.0, 0.0, 0.0};  //black
    Color foreground_color{1.0, 1.0, 1.0};  //white


    void* fontnsize[2] = { GLUT_BITMAP_8_BY_13,  //const!!
                           GLUT_BITMAP_9_BY_15};
    Label(){  x = 50; y = 50;  content = "Apple";  }
    Label(int x, int y, std::string str):x(x), y(y), content(str){}

    void update(){
        /*
        if(state==GLUT_DOWN && button==GLUT_LEFT_BUTTON){
            size = 1;
            fprintf(stderr, "clicked \t%c\n", r[(++rc)%=4]);
        }else{
            size = 0;
        }
        */  
        render();
    }

    void render(){
        //glClear(GL_COLOR_BUFFER_BIT);
        int str_len = content.length();
        float str_width = glutBitmapLength(fontnsize[size], (const unsigned char*)content.c_str());

        if(background){
            glColor3f(background_color.r,
                      background_color.g,
                      background_color.b);
            glBegin(GL_POLYGON);
                glVertex2f(x, y);
                glVertex2f(x+str_width, y);
                glVertex2f(x+str_width, y+14);
                glVertex2f(x, y+14);
            glEnd();
        }
        glColor3f(foreground_color.r,
                  foreground_color.g,
                  foreground_color.b);
        /*glBegin(GL_LINES);
            glVertex2f(x, y);
            glVertex2f(x+str_width, y);
        glEnd();*/
        for(int i = 0; i < str_len; i++){
            glRasterPos2f(x + i*(8+size), y);
            glutBitmapCharacter(fontnsize[size], (int)content[i]);
        }

        glFinish();
    }
};

// test ---------------------------------
#ifndef _main_
Label lb1;
void reshape(int width, int height){
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, (double) width, 0.0, (double) height);
    glViewport(0,0,width, height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;

    glutPostRedisplay();
}

void mouseFunc(int button, int state, int x, int y){
    lb1.update();
}

void display(void){
    lb1.render();  //for first
    
    glFlush();
}

int main(int argc, char **argv){
    glutInit(&argc, argv);
    glutInitWindowSize(window_width, window_height);

    glutCreateWindow("Label Model Demo");

    glutReshapeFunc(reshape);
    glutMouseFunc(mouseFunc);
    glutDisplayFunc(display);

    glutMainLoop();

    return 0;
}
#endif