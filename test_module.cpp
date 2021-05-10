// test_module.cpp
#define _main_
#include <cstdio>
#include <cstdlib>

#include <GL/glut.h>

#define winWH
const int   window_width = 600,
            window_height = 600;

#include "canvas.cpp"
#include "button.cpp"
#include "label.cpp"



            

Canvas cvs1(100, 50, 450, 500);
Button btn1(20, 50, 60, 60);
Button btn2(20, 120, 60, 60);

// ===============================

void reshape(int width, int height){
    cvs1.init();
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, (double) width, 0.0, (double) height);
    glViewport(0,0,width, height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;

    glutPostRedisplay();
}

void mouseFunc(int button, int state, int x, int y){
    cvs1.update(x, y, button, state);
    btn1.update(x, y, button, state);
    btn2.update(x, y, button, state);
}
void motionFunc(int x, int y){
    cvs1.update(x, y, GLUT_LEFT_BUTTON, GLUT_DOWN);
}
void passiveMotionFunc(int x, int y){
    cvs1.update(x, y);
    btn1.update(x, y);
    btn2.update(x, y);

}

void display(void){
    btn1.render();
    btn2.render();
    cvs1.render();
    

    glFlush();
}

void setLine(){
    cvs1.setState(Canvas::State::LINE);
}
void setCircle(){
    cvs1.setState(Canvas::State::CIRCLE);
}

int main(int argc, char **argv){
    glutInit(&argc, argv);
    glutInitWindowSize(window_width, window_height);

    btn1.upF = setLine;
    btn2.upF = []{ cvs1.setState(Canvas::State::ERASE); };
    glutCreateWindow("Models' Demo");

    glutReshapeFunc(reshape);
    glutMotionFunc(motionFunc);
    glutPassiveMotionFunc(passiveMotionFunc);
    glutMouseFunc(mouseFunc);
    glutDisplayFunc(display);
    

    glutMainLoop();

    return 0;
}