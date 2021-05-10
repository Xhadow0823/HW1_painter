//button object model
#include <cstdio>
#include <cstdlib>
#include <string>
#include <GL/glut.h>

int window_width = 400,
    window_height = 400;


class Button {
public:
    int x = -1,
        y = -1;
    int w = 10,
        h = 10;
    int click_depth = 3;
    bool is_pressed = false,
         is_hover = false;
    
    std::string text = "Button";

    void (*downF)(void) = []{ return; };
    void (*upF)(void)   = []{ return; };
    
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
                    fprintf(stderr, "mouse down\n");
                }else if(button==GLUT_LEFT_BUTTON&&state==GLUT_UP){
                    upF();
                    fprintf(stderr, "mouse up\n");
                }else if(button==-1 && state==-1){
                    is_hover = true;
                    fprintf(stderr, "mouse hover\n");
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
            glColor3f(217.0/255, 217.0/255, 217.0/255);
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
            glColor3f(0.0f, 0.0f, 0.0f);
        int str_len = text.length();
        float str_width = glutBitmapLength(GLUT_BITMAP_8_BY_13, (const unsigned char*)text.c_str());
        for(int i = 0; i < str_len; i++){
            glRasterPos2f(x+ w/2 - str_width/2 + i*9, y + h/2 - 4);
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13, (int)text[i]);
        }
        
        glFlush();
    }
};

// test 


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

Button btn1(50, 50, 160, 90);

void mouseFunc(int button, int state, int x, int y){
    btn1.update(x, y, button, state);
    //btn1.render();
    
}

void passiveMotionFunc(int x, int y){
    btn1.update(x, y);
}

void display(void){
    btn1.render();  //for first rander

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

    glutCreateWindow("Button Model Demo");

    // default ClearColor  glClear(GL_COLOR_BUFFER_BIT);

    glutReshapeFunc(reshape);
    glutMouseFunc(mouseFunc);
    glutPassiveMotionFunc(passiveMotionFunc);
    glutDisplayFunc(display);

    glutMainLoop();
    return 0;
}
