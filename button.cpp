// button.h
// 2020/10/12 18:44
// by Eric W. 00757036

#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

#include <GL/glut.h>

#include "color.h"

#ifndef winWH
    #define winWH
    const int window_width  = 400,
              window_height = 400;
#endif

class Button {
public:

    int x = -1,
        y = -1;
    int w = 10,
        h = 10;
    int click_depth = 3;
    bool is_pressed = false,
         is_hover = false;
    
    int fontSize = 0;  //0, 1
    void* fontnsize[2] = { GLUT_BITMAP_8_BY_13,  //const!!
                           GLUT_BITMAP_9_BY_15};

    std::string text = "Button";

    Color background_color{217.0/255, 217.0/255, 217.0/255};
    Color foreground_color{0.0, 0.0, 0.0};  //black

    void (*downF)(void) = []{ return; };
    void (*upF)(void)   = []{ return; };
    //const unsigned char *text = "Button";
    //Button(){}
    Button(int x, int y, int width, int height):x(x), y(y), w(width), h(height){};
    Button(int x, int y, int width, int height, std::string str):x(x), y(y), w(width), h(height), text(str){};
    void update(float cursor_x, float cursor_y, int button=-1, int state=-1){
        is_pressed = false;
        is_hover = false;
        if(cursor_x > x && cursor_x-x < w){
            if((window_height-cursor_y) > y && (window_height-cursor_y)-y < h){
                // in button area
                if(button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN){
                    is_pressed = true;
                    downF();
                    //fprintf(stderr, "mouse down\n");
                }else if(button==GLUT_LEFT_BUTTON&&state==GLUT_UP){
                    upF();
                    //fprintf(stderr, "mouse up\n");
                }else if(button==-1 && state==-1){
                    is_hover = true;
                    //fprintf(stderr, "mouse hover\n");
                }
            }
        }
        render();
    }
    void render(){
        //glClear(GL_COLOR_BUFFER_BIT);
        glBegin(GL_POLYGON);  // upper triangle
            if(is_pressed)
                glColor3f(94.0/255, 94.0/255, 94.0/255);
            else
                glColor3f(138.0/255, 138.0/255, 141.0/255);
            glVertex2f(x, y);
            glVertex2f(x+w, y+h);
            glVertex2f(x, y+h);
        glEnd();
        glBegin(GL_POLYGON);  // lower triangle
            if(is_pressed)
                glColor3f(138.0/255, 138.0/255, 141.0/255);
            else
                glColor3f(94.0/255, 94.0/255, 94.0/255);
            glVertex2i(x, y);
            glVertex2i(x+w, y);
            glVertex2i(x+w, y+h);
        glEnd();
        glBegin(GL_POLYGON);  // inner rect
            if(is_hover){
                glColor3f(138.0/255, 138.0/255, 141.0/255);
            }else if(is_pressed){
                //fprintf(stderr, "change color\n");
                glColor3f(38.0/255, 38.0/255, 141.0/255);
            }else{
                glColor3f(background_color.r,
                        background_color.g,
                        background_color.b);
            }
            glVertex2i(x+click_depth, y+click_depth);
            glVertex2i(x+click_depth, y+h-click_depth);
            glVertex2i(x+w-click_depth, y+h-click_depth);
            glVertex2i(x+w-click_depth, y+click_depth);
        glEnd();
    // text part-----
        if(is_pressed)
            glColor3f(1.0f, 1.0f, 1.0f);
        else
            glColor3f(foreground_color.r,
                      foreground_color.g,
                      foreground_color.b);
        float str_len = text.length();
        float str_width = glutBitmapLength(fontnsize[fontSize], (const unsigned char*)text.c_str());
        /*
        glBegin(GL_LINES);
            glVertex2f(x, y+ h/2.0);
            glVertex2f(x+str_width, y+ h/2.0);
        glEnd();
        */
        for(int i = 0; i < str_len; i++){
            glRasterPos2f(x+ w/2.0 - str_width/2.0 + i*(7+fontSize)+2, y + h/2.0 - 4);
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13, (int)text[i]);
        }
        
        glFlush();
    }
};

class ButtonGroup {
public:
    int x_min = INT_MAX,
        y_min = INT_MAX;
    int x_max = INT_MIN,
        y_max = INT_MIN;
    int padding = 3;
    bool outline = true;

    std::vector<Button*> buttonVec;
    void addButton(Button* btn){
        buttonVec.push_back(btn);
        if(btn->x < x_min)
            x_min = btn->x - padding;
        if(btn->x+btn->w > x_max)
            x_max = btn->x+btn->w + padding;
        if(btn->y < y_min)
            y_min = btn->y - padding;
        if(btn->y+btn->h > y_max)
            y_max = btn->y+btn->h + padding;        
    }
    void moveTo(int x_new, int y_new){
        int delta_x = x_new - x_min,
            delta_y = y_new - y_min;
        for(auto it=buttonVec.begin(); it!=buttonVec.end(); it++){
            (*it)->x+=delta_x;  (*it)->y+=delta_y;
        }
        x_min+=delta_x;
        y_min+=delta_y;
        x_max+=delta_x;
        y_max+=delta_y;
    }


    void update(float cursor_x, float cursor_y, int button=-1, int state=-1){
        if(outline){
            glColor3f(1.0, 1.0, 1.0);
            glLineWidth(1.5);
            glBegin(GL_LINES);
                glVertex2f(x_min, y_min);  glVertex2f(x_max, y_min);
                glVertex2f(x_max, y_min);  glVertex2f(x_max, y_max);
                glVertex2f(x_max, y_max);  glVertex2f(x_min, y_max);
                glVertex2f(x_min, y_max);  glVertex2f(x_min, y_min);
            glEnd();
        }
        for(auto it=buttonVec.begin(); it!=buttonVec.end(); it++){
            (*it)->update(cursor_x, cursor_y, button, state);
        }
    }
    void render(){
        //glClear(GL_COLOR_BUFFER_BIT);
        if(outline){
            glColor3f(1.0, 1.0, 1.0);
            glLineWidth(1.5);
            glBegin(GL_LINES);
                glVertex2f(x_min, y_min);  glVertex2f(x_max, y_min);
                glVertex2f(x_max, y_min);  glVertex2f(x_max, y_max);
                glVertex2f(x_max, y_max);  glVertex2f(x_min, y_max);
                glVertex2f(x_min, y_max);  glVertex2f(x_min, y_min);
            glEnd();
        }
        for(auto it=buttonVec.begin(); it!=buttonVec.end(); it++){
            (*it)->render();
        }
    }
};

// test----------------------
#ifndef _main_
Button btn1(50, 50, 50, 50, "Line");
Button btn2(102, 50, 50, 50, "Circle");
Button btn3(102, 102, 50, 50, "A");
Button btn4(50, 102, 50, 50);

ButtonGroup bg1;

static void reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, (double) width, 0.0, (double) height);
    glViewport(0,0,width, height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;

    glutPostRedisplay();
}

void hello(void){
    fprintf(stderr, "Hello\n");
}

void mouseFunc(int button, int state, int x, int y){
    //btn1.update(x, y, button, state);
    bg1.update(x, y, button, state);
}

void passiveMotionFunc(int x, int y){
    //btn1.update(x, y);
    bg1.update(x, y);
}

void display(void){
    //btn1.render();  //for first rander
    bg1.render();

// text test
    const unsigned char str[] = "Macbook Pro";
    int str_w = glutBitmapLength(GLUT_BITMAP_8_BY_13, str);
    glColor3f(1., 1., 1.);
    int len = strlen((const char *)str);
    for(int i = 0; i < len; i++){
        glRasterPos2f(window_width/2 - str_w/2 + 9*i, window_height/2);
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, (int)str[i]);
    }
// END text test

    glFlush();

    fprintf(stderr, "displayed\n");
}

int main(int argc, char ** argv){
    glutInit(&argc, argv);
    // default window size
    glutInitWindowSize(window_width, window_height);

    //btn1.clickF = hello;
    bg1.addButton(&btn1);
    bg1.addButton(&btn2);
    bg1.addButton(&btn3);
    bg1.addButton(&btn4);
    glutCreateWindow("Button Model Demo");

    // default ClearColor  glClear(GL_COLOR_BUFFER_BIT);

    glutReshapeFunc(reshape);
    glutMouseFunc(mouseFunc);
    glutPassiveMotionFunc(passiveMotionFunc);
    glutDisplayFunc(display);

    glutMainLoop();

    return 0;
}
#endif