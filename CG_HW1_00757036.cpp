// CG_HW1_00757036.cpp
// 2020/10/13 14:44
// by Eric W. 00757036

#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <cmath>

#include <GL/glut.h>

#define _main_

#define winWH
const int window_width  = 600,
          window_height = 600;
#include "color.h"
#include "canvas.cpp"
#include "button.cpp"
#include "label.cpp"

// ==========================================================

ButtonGroup bg_Tools;
Button btn_tools_1(0,   0, 50, 50, "Eraser"),
       btn_tools_2(50,  0, 50, 50, "Clear"),
       btn_tools_3(0,  50, 50, 50, "Poly"),
       btn_tools_4(50, 50, 50, 50, "Text"),
       btn_tools_5(0, 100, 50, 50, "Line"),
       btn_tools_6(50,100, 50, 50, "Circle"),
       btn_tools_7(0, 150, 50, 50, "Cursor"),
       btn_tools_8(50,150, 50, 50, "Pencil");

ButtonGroup bg_fillMode;
Button btn_fillMode_1(0, 0, 100, 25, "Fill: OFF");

ButtonGroup bg_size;
Button btn_size_minus(0,  0, 25, 25, "-"),
       btn_size_plus( 75, 0, 25, 25, "+");

ButtonGroup bg_text_size;
Button btn_text_size_0(0, 0, 100, 25),
       btn_text_size_1(0, 25, 100, 25);

ButtonGroup bg_color;
Button btn_color_input(15, 0, 70, 23, "R: G: B:"),
       btn_color_B( 0, 25, 50-4, 25-1, ""),
       btn_color_Y(52, 25, 50-4, 25-1, ""),
       btn_color_R( 0, 50, 50-4, 25-1, ""),
       btn_color_G(52, 50, 50-4, 25-1, ""),
       btn_color_W( 0, 75, 50-4, 25-1, ""),
      btn_color_Bk(52, 75, 50-4, 25-1, "");

ButtonGroup bg_top;
Button btn_top_save2(0,   572, 100, 28, "SAVE"),
       btn_top_load2(100, 572, 100, 28, "LOAD"),
       btn_top_grid(200, 572, 100, 28, "GRID: OFF"),
       btn_top_spacer(300, 572, 300, 28, "");


Label lb_tools(15, 560, "Tools"),
      lb_fillMode(15, 334, "FillMode"),
      lb_size_title(15, 284, "Size"),
      lb_size_scale(39, 259, ":1.0"),
      lb_color_title(15, 210, "Color");

Canvas cvs(130, 100, 450, 450);

// =====events=and=main======================================
void reshape(int width, int height){

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, (double) width, 0.0, (double) height);
    glViewport(0,0,width, height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glutPostRedisplay();
}

void keyboardFunc(unsigned char key, int x, int y){
    cvs.updateKey(key);  //click ESC to cancle polygon drawing
}

void mouseFunc(int button, int state, int x, int y){
//glClear(GL_COLOR_BUFFER_BIT);
    cvs.update(x, y, button, state);
    bg_Tools.update(x, y, button, state);
    bg_fillMode.update(x, y, button, state);
    bg_size.update(x, y, button, state);
    bg_color.update(x, y, button, state);
    bg_top.update(x, y, button, state);

    lb_tools.update();
    lb_fillMode.update();
    lb_size_title.update();
    lb_size_scale.update();
    lb_color_title.update();
    
}

void motionFunc(int x, int y){
//glClear(GL_COLOR_BUFFER_BIT);
    cvs.update(x, y, GLUT_LEFT_BUTTON, GLUT_DOWN);
    bg_Tools.update(x, y);
    bg_fillMode.update(x, y);
    bg_size.update(x, y);
    bg_color.update(x, y);
    bg_top.update(x, y);

}

void passiveMotionFunc(int x, int y){
//glClear(GL_COLOR_BUFFER_BIT);
    cvs.update(x, y);
    bg_Tools.update(x, y);
    bg_fillMode.update(x, y);
    bg_size.update(x, y);
    bg_color.update(x, y);
    bg_top.update(x, y);

    lb_tools.render();
    lb_fillMode.render();
    lb_size_title.render();
    lb_size_scale.render();
    lb_color_title.render();

    glFinish();
}

void display(){
//glClear(GL_COLOR_BUFFER_BIT);
    cvs.render();
    bg_Tools.render();
    bg_fillMode.render();
    bg_size.render();
    bg_color.render();
    bg_top.render();
    
    lb_tools.render();
    lb_fillMode.render();
    lb_size_title.render();
    lb_size_scale.render();
    lb_color_title.render();


}
//-----
void fillModeBtnFunc(){
    btn_fillMode_1.text=std::string("Fill: ") +
        (cvs.toggleFillMode()?std::string("ON"):std::string("OFF"));
}

void sizeModBtnFunc(float delta){
    lb_size_scale.content=std::string(":")+std::to_string(cvs.paintSizeAdd(delta)).substr(0, 3);
}

void gridBtnFunc(){
    btn_top_grid.text=std::string("GRID: ") +
        (cvs.toggleGrid()?std::string("ON"):std::string("OFF"));
}

enum FileState{
    SAVE, LOAD, CLEAR, QUIT
};
void fileMenuFunc(int val){
    switch (val)
    {
    case SAVE:
        cvs.save2();
        break;
    case LOAD:
        cvs.load2();
        break;
    case CLEAR:
        cvs.clearCanvas();
        break;
    case QUIT:
        fprintf(stderr, "Exit\n");
        exit(0);
        break;
    }
}
typedef int menu_t;
menu_t topMenu,
       fileMenu;

void autoSave(){
    glutTimerFunc(60*1000, [](int x){ /*cvs.saveSnapshot();*/fprintf(stderr, "X");autoSave(); }, 0);
}
//------------------

int main(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(window_width, window_height);
    glClearColor(191.0/255, 191.0/255, 191.0/255, 1);
    


    bg_Tools.addButton(&btn_tools_1);  btn_tools_1.upF=[]{cvs.setState(Canvas::State::ERASE);};
    bg_Tools.addButton(&btn_tools_2);  btn_tools_2.upF=[]{cvs.clearCanvas();};
    bg_Tools.addButton(&btn_tools_3);  btn_tools_3.upF=[]{cvs.setState(Canvas::State::POLYGON);};
    bg_Tools.addButton(&btn_tools_4);  btn_tools_4.upF=[]{cvs.setState(Canvas::State::TEXT);};
    bg_Tools.addButton(&btn_tools_5);  btn_tools_5.upF=[]{cvs.setState(Canvas::State::LINE);};
    bg_Tools.addButton(&btn_tools_6);  btn_tools_6.upF=[]{cvs.setState(Canvas::State::CIRCLE);};
    bg_Tools.addButton(&btn_tools_7);  btn_tools_7.upF=[]{cvs.setState(Canvas::State::CURSOR);};
    bg_Tools.addButton(&btn_tools_8);  btn_tools_8.upF=[]{cvs.setState(Canvas::State::POINT);};
    bg_Tools.moveTo(5, 350);

    bg_fillMode.addButton(&btn_fillMode_1); btn_fillMode_1.upF = fillModeBtnFunc;
    bg_fillMode.moveTo(5, 300);

    bg_size.addButton(&btn_size_minus);  btn_size_minus.upF=[]{sizeModBtnFunc(-0.5);};
    bg_size.addButton(&btn_size_plus);   btn_size_plus.upF=[]{sizeModBtnFunc(+0.5);};
    bg_size.moveTo(5, 250);

    lb_size_scale.background = true;  lb_size_scale.background_color=Color(191.0/255, 191.0/255, 191.0/255);

    bg_color.addButton(&btn_color_input);  btn_color_input.upF=[]{cvs.setColorFromKeyboard();};
    bg_color.addButton(&btn_color_B);  btn_color_B.background_color=Color(0, 0, 1);  btn_color_B.upF=[]{cvs.foreground_color[0]=Color(0, 0, 1);};
    bg_color.addButton(&btn_color_Y);  btn_color_Y.background_color=Color(1, 1, 0);  btn_color_Y.upF=[]{cvs.foreground_color[0]=Color(1, 1, 0);};
    bg_color.addButton(&btn_color_R);  btn_color_R.background_color=Color(1, 0, 0);  btn_color_R.upF=[]{cvs.foreground_color[0]=Color(1, 0, 0);};
    bg_color.addButton(&btn_color_G);  btn_color_G.background_color=Color(0, 1, 0);  btn_color_G.upF=[]{cvs.foreground_color[0]=Color(0, 1, 0);};
    bg_color.addButton(&btn_color_W);  btn_color_W.background_color=Color(1, 1, 1);  btn_color_W.upF=[]{cvs.foreground_color[0]=Color(1, 1, 1);};
    bg_color.addButton(&btn_color_Bk);  btn_color_Bk.background_color=Color();      btn_color_Bk.upF=[]{cvs.foreground_color[0]=Color();};
    bg_color.moveTo(5, 100);

    bg_top.addButton(&btn_top_save2);  btn_top_save2.upF=[]{cvs.save2();};
    bg_top.addButton(&btn_top_load2);  btn_top_load2.upF=[]{cvs.load2();};
    bg_top.addButton(&btn_top_grid);  btn_top_grid.upF=gridBtnFunc;
    bg_top.addButton(&btn_top_spacer);
    bg_top.outline = false;

    glutCreateWindow("Paint!");  //****************************
    glClearColor(191.0/255, 191.0/255, 191.0/255, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboardFunc);
    glutMouseFunc(mouseFunc);
    glutMotionFunc(motionFunc);
    glutPassiveMotionFunc(passiveMotionFunc);
    glutDisplayFunc(display);

    //-----
    fileMenu = glutCreateMenu(fileMenuFunc);
    glutAddMenuEntry("Save color buffer", SAVE);
    glutAddMenuEntry("Soad color buffer", LOAD);
    glutAddMenuEntry("Clear", CLEAR);
    glutAddMenuEntry("Quit", QUIT);

    topMenu = glutCreateMenu([](int x){ return;});
    glutAddSubMenu("File", fileMenu);

    glutAttachMenu(GLUT_RIGHT_BUTTON);
    //-----
    autoSave();

    glutMainLoop();

    return 0;
}