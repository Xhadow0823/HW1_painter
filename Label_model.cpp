//Label object model
#include <cstdio>
#include <cstdlib>

#include <iostream>
#include <string>

#include <GL/glut.h>

int window_width = 400,
    window_height = 400;

int rc = 0;
const char r[5] = "|/-\\";

class Label {
public:
    int x = -1,
        y = -1;
    int w = 8,
        h = 14;
    int size = 0;
    
    //left or center
    //fore_color = 
    //backgroud_color = 
    
    std::string content;
    float color[4] = {1.0, 1.0, 1.0, 1.0};
    void* fontnsize[2] = { GLUT_BITMAP_8_BY_13,  //const!!
                                 GLUT_BITMAP_9_BY_15};
    Label(){  x = 50; y = 50;  content = "Apple";  }
    Label(int x, int y, std::string str):x(x), y(y), content(str){}

    void update(int cursor_x, int cursor_y, int button, int state){
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
        glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(1.0f, 1.0f, 1.0f);

        int str_len = content.length();
        float str_width = glutBitmapLength(fontnsize[size], (const unsigned char*)content.c_str());
        //for(auto it = content.begin(); it != content.end(); it++){
        for(int i = 0; i < str_len; i++){
            glRasterPos2f(x + i*(8+size), y);
            glutBitmapCharacter(fontnsize[size], (int)content[i]);
        }

        glFinish();
    }
};

void reshape(int width, int height){
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, (double) width, 0.0, (double) height);
    glViewport(0,0,width, height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;

    glutPostRedisplay();
}

Label lb1;
void mouseFunc(int button, int state, int x, int y){
    lb1.update(x, y, button, state);
}

void display(void){
    lb1.render();  //for first
    

    glFlush();
    fprintf(stderr, "displayed \t%c\n", r[(++rc)%=4]);
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