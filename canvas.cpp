// Canvas.h
// 2020/10/12 18:40
// by Eric W. 00757036

#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <cmath>

#include <GL/glut.h>

#include "color.h"

#ifndef winWH
    #define winWH
    const int window_width = 400,
              window_height = 400;
#endif

class Canvas {
public:
    enum State{
        CURSOR,
        POINT,
        ERASE,
        LINE,
        POLYGON,
        CIRCLE,
        TEXT
    };
    
    int x = -1,
        y = -1,
        w = 0,
        h = 0;

    unsigned char snapshot[1024*1024][3];  // max w,h is 1024
    unsigned char snapshot2[1024*1024][3];

    Color background_color{1.0, 1.0, 1.0};  //white
    Color foreground_color[2] = {{0.0, 0.0, 1.0},  //black
                                 {1.0, 1.0, 1.0}};  //white
    bool fillMode = false;

    // ------------------------
    State draw_mode = CURSOR;
    int x_from = -1,
        y_from = -1;
    int x_current = -1,
        y_current = -1;

    float paint_size = 1.0;
    bool penDown = false;

    bool grid = false;

    bool toInit = true;
    std::vector<std::pair<int, int>> polygonVec;
    
    int fontSize = 0;  //0, 1
    void* fontnsize[2] = { GLUT_BITMAP_8_BY_13,  //const!!
                           GLUT_BITMAP_9_BY_15};
    int row_start = -1;  //text start from x

    Canvas(){}
    Canvas(int x, int y, int w, int h):x(x), y(y), w(w), h(h){}

    void setState(Canvas::State state){
        draw_mode = state;
        penDown = false;
    }

    void setFillMode(bool s){  //true, false
        fillMode = s;
        penDown = false;
    }

    float paintSizeAdd(float delta){  //+-
        if(paint_size+delta >= 0){
            paint_size+=delta;
        }
        return paint_size;
    }
    float fontSizeAdd(int delta){  //+-
        if(0<=fontSize+delta && fontSize+delta<=1){
            fontSize+=delta;
        }
        return fontSize;
    }

    bool toggleFillMode(){
        fillMode = !fillMode;
        penDown = false;
        return fillMode;
    }

    bool toggleGrid(){
        grid = !grid;
        update(x+w/2, y+h/2);
        return grid;
    }

    void saveSnapshot(){
        /*
        if(grid){  //WTF
            toggleGrid();
            glReadPixels(x, y, w, h, GL_RGB, GL_UNSIGNED_BYTE, snapshot);
            toggleGrid();
            return;
        }
        */
        glReadPixels(x, y, w, h, GL_RGB, GL_UNSIGNED_BYTE, snapshot);
    }
    void loadSnapshot(){
        glRasterPos2i(x, y);
        glDrawPixels(w, h, GL_RGB, GL_UNSIGNED_BYTE, snapshot);
    }

    void save2(){
        if(grid){
            toggleGrid();
            glReadPixels(x, y, w, h, GL_RGB, GL_UNSIGNED_BYTE, snapshot2);
            toggleGrid();
            return;
        }
        glReadPixels(x, y, w, h, GL_RGB, GL_UNSIGNED_BYTE, snapshot2);
    }
    void load2(){
        glRasterPos2i(x, y);
        glDrawPixels(w, h, GL_RGB, GL_UNSIGNED_BYTE, snapshot2);
        saveSnapshot();
    }
    void clearCanvas(){
        canclePoly();
        clear();
        saveSnapshot();
    }

    void clear(){
        // set canvas background color
        glClear(GL_COLOR_BUFFER_BIT);  // clear the out-of-area
        glColor3f(background_color.r,
                  background_color.g,
                  background_color.b);
        glLineWidth(1.0);
        glBegin(GL_POLYGON);
            glVertex2f(x, y);
            glVertex2f(x+w, y);
            glVertex2f(x+w, y+h);
            glVertex2f(x, y+h);
        glEnd();
        glFinish();
        fprintf(stderr, "Cleared\n");
    }

    void init(){
        // reset all state default
        fprintf(stderr, "Init()\n");
        // clear the canvas 
        clear();
        // reset the buffer
        saveSnapshot();
        save2();
    }

    void drawCircle(int center_x, int center_y, int border_x, int border_y){
        const int sides = 32;
        const double radius = sqrt((border_x-center_x)*(border_x-center_x)+
                              (border_y-center_y)*(border_y-center_y));
        glPushMatrix();
            glTranslated(center_x, center_y, 0);
            if(fillMode){
                glBegin(GL_POLYGON);
            }else{
            glBegin(GL_LINE_LOOP);}
                const double deg2rad = 3.1415926535897932384626433832795/180.0;
                for(int t = 0; t < 360; t+=360.0 / sides){
                    glVertex2f(cos(t*deg2rad) * radius, sin(t*deg2rad) * radius);
                }
            glEnd();
        glPopMatrix();
    }

    void putText(unsigned char ch){
        glColor3f(foreground_color[0].r,
                  foreground_color[0].g,
                  foreground_color[0].b);
        glRasterPos2f(x_from, y_from);
        glutBitmapCharacter(fontnsize[fontSize], (int)ch);
        x_from+=(8+fontSize);
        saveSnapshot();
        update(x_from, y_from);
    }

    void newLine(){
        x_from=row_start;  //reset x_from
        y_from -= (13+fontSize);
    }

    void backspace(){  //SPECIAL
        if(x_from>=row_start){
            x_from-=(8+fontSize);
            glColor3f(background_color.r,
                      background_color.g,
                      background_color.b);
            glBegin(GL_POLYGON);
                glVertex2f(x_from, y_from);
                glVertex2f(x_from+8+fontSize, y_from);
                glVertex2f(x_from+8+fontSize, y_from+13+fontSize);
                glVertex2f(x_from, y_from+13+fontSize);
            glEnd();
            saveSnapshot();
            update(x_from, y_from);
        }
    }

    void canclePoly(){
        polygonVec.clear();
        penDown = false;
    }

    void updateKey(unsigned char key){  //bad
        if(draw_mode==POLYGON && key == 27){  //ESC
            canclePoly();
        }else if(draw_mode==TEXT && 32<=key &&  key<=126){
            putText(key);
        }else if(key==10 || key==13){  //ENTER
            newLine();
        }else if(key==8){  //backspace
            backspace();
        }
    }

    void setColorFromKeyboard(){
        int r=0, g=0, b=0;
        fprintf(stderr, "Input the R G B (up to 255): ");
        fscanf(stdin, "%d %d %d", &r, &g, &b);
        foreground_color[0] = Color(r/255.0, g/255.0,b/255.0);
    }

    //  catch motion, mouse, passiveMotion
    void update(const int cursor_x, const int cursor_y, int button=-1, int state=-1){
        x_current = cursor_x,  //cord in canvas
        y_current = window_height-cursor_y;
        //check mouse is in the area
        if((x_current < x || x_current > (x+w)) || 
            (y_current < y || y_current > (y+h))){
            return ;  // out of area
        }
        //clear when first load canvas or reset
glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(foreground_color[0].r,
                  foreground_color[0].g,
                  foreground_color[0].b);
        switch (draw_mode) {
        case CURSOR:
            /* DO NOTHING */
            loadSnapshot();
            break;
        case POINT:
            /* code */
            loadSnapshot();
            glLineWidth(paint_size);
            if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN){
                if(penDown){
                    //draw from last point to current
                    glBegin(GL_LINES);
                        glVertex2f(x_from, y_from);
                        glVertex2f(x_current, y_current);
                    glEnd();
                    x_from = x_current,
                    y_from = y_current;
                    //save
                    saveSnapshot();
                }else{  //first point
                    penDown = true;
                    glBegin(GL_POINTS);
                        glVertex2f(x_current, y_current-1);  // minus 1 !!
                    glEnd();
                    x_from = x_current,
                    y_from = y_current;
                    saveSnapshot();
                }
            }
            if(button==GLUT_LEFT_BUTTON && state==GLUT_UP){
                penDown = false;
                //draw to the current point
                glBegin(GL_LINES);
                    glVertex2f(x_from, y_from);
                    glVertex2f(x_current, y_current);
                glEnd();
                //save
                saveSnapshot();
            }

            break;
        case ERASE:
            /* code */
            // draw rect that color is background color
            //if mouseDown
            //     put a backgroundColor-rect at the cursor
            loadSnapshot();
            if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN){
                glColor3f(background_color.r,
                          background_color.g,
                          background_color.b);
                float sizeTmp = paint_size;
                glPointSize(30.0);
                glBegin(GL_POINTS);
                    glVertex2i(x_current, y_current);
                glEnd();
                glPointSize((GLfloat)sizeTmp);
                saveSnapshot();
            }
            break;
        case LINE:
            /* code */
            // 2 way to perform :
            //  mouseUp to MouseUp   or
            //  mouseDown to mouseUp
            // way 2:
            // load buffer
            //if mouse down
            //      set x_from, y_from, set penDown=true
            //      draw line to cursor dynamicaly
            loadSnapshot();
            if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN){
                if(!penDown){  //set start
                    penDown = true;
                    x_from = x_current;
                    y_from = y_current;
                    fprintf(stderr, "start: %d, %d\n", x_from, y_from);
                }else{  //set current
                    glLineWidth(paint_size);
                    glBegin(GL_LINES);
                        glVertex2f(x_from, y_from);
                        glVertex2f(x_current, y_current);;
                    glEnd();
                    fprintf(stderr, "drag from %d, %d to %d, %d\n", x_from, y_from, cursor_x, window_height-cursor_y);
                }
            }
            //if penDown and mouseUp
            //      draw line to cursor
            //      save buffer
            //      set penDown=false
            if(penDown && button==GLUT_LEFT_BUTTON && state==GLUT_UP){
                penDown = false;
                glLineWidth(paint_size);
                glBegin(GL_LINES);
                    glVertex2f(x_from, y_from);
                    glVertex2f(x_current, y_current);;
                glEnd();
                fprintf(stderr, "stop\n");
                saveSnapshot();
            }
        break;
        case POLYGON:
            /* code */
            loadSnapshot();
            if(button==GLUT_LEFT_BUTTON && state==GLUT_UP){
                //  double click event
                glLineWidth(paint_size);
                if(penDown && (x_from==x_current && y_from==y_current)){
                    penDown = false;
                    // draw a polygon with the vector
                    if(fillMode){
                        glBegin(GL_POLYGON);
                            for(auto it = polygonVec.begin()+1; it != polygonVec.end(); it++){
                                glVertex2f(it->first, it->second);
                                glVertex2f((it-1)->first, (it-1)->second);
                            }
                        glEnd();
                    }else{
                        glBegin(GL_LINE_LOOP);
                            for(auto it = polygonVec.begin(); it != polygonVec.end(); it++){
                                glVertex2f(it->first, it->second);
                            }
                        glEnd();
                    }
                    // clear the vec
                    polygonVec.clear();
                    saveSnapshot();
                }else{  //  click event
                    // addition function  dynamicaly draw line when hover
                    penDown = true;
                    // push new point into a vertor
                    x_from = x_current,
                    y_from = y_current;
                    polygonVec.push_back({x_current, y_current});
                    // then draw lines with the vector
                    // do not save
                }
            }
            if(penDown){
                if(state==-1 || state==GLUT_DOWN){  // hover and mouseDown
                    glBegin(GL_LINES);
                        glVertex2f(polygonVec.back().first, polygonVec.back().second);
                        glVertex2f(x_current, y_current);
                    glEnd();
                }
                glBegin(GL_LINES);
                    for(auto it = polygonVec.begin()+1; it != polygonVec.end(); it++){
                        glVertex2f(it->first, it->second);
                        glVertex2f((it-1)->first, (it-1)->second);
                    }
                glEnd();
            }
            break;
        case CIRCLE:
            /* code */
            // draw logic similar to LINE Mode !!
            loadSnapshot();
            if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN){
                if(!penDown){  //set start
                    penDown = true;
                    x_from = x_current;
                    y_from = y_current;
                    fprintf(stderr, "start: %d, %d\n", x_from, y_from);
                }else{  //set current
                    glLineWidth(paint_size);
                    drawCircle(x_from, y_from, x_current, y_current);
                    fprintf(stderr, "drag from %d, %d to %d, %d\n", x_from, y_from, x_current, window_height-cursor_y);
                }
            }
            //if penDown and mouseUp
            //      draw line to cursor
            //      save buffer
            //      set penDown=false
            if(penDown && button==GLUT_LEFT_BUTTON && state==GLUT_UP){
                penDown = false;
                glLineWidth(paint_size);
                drawCircle(x_from, y_from, x_current, y_current);
                fprintf(stderr, "stop\n");
                saveSnapshot();
            }
            break;
        case TEXT:
            /* code */
            loadSnapshot();
            if(button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN){
                // have to renew fontsize_scale
                //set row_start
                row_start = x_from = x_current;
                y_from = y_current;
            }
            break;
        default:
            break;
        }

        render();
    }
    void render(){
        //glClear(GL_COLOR_BUFFER_BIT);
        if(toInit){
            clear();
            saveSnapshot();
            toInit = false;
        }
        if(grid){
            glColor3f(.75, .75, .75);
            glLineWidth(1.0);
            glBegin(GL_LINES);
                for(int ix = 0; ix <= window_width; ix+=(window_width/10)){    
                    glVertex2f(ix, y);
                    glVertex2f(ix, y+h);
                }
                for(int iy = 0; iy <= window_height; iy+=(window_height/10)){
                    glVertex2f(x, iy);
                    glVertex2f(x+w, iy);
                }
            glEnd();
        }
        glFinish();

    }
};


// test --------------------
#ifndef _main_
Canvas cvs1(50, 50, 300, 300);

void reshape(int width, int height){
    fprintf(stderr, "reshaped\n");
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, (double) width, 0.0, (double) height);
    glViewport(0,0,width, height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glutPostRedisplay();
}

void mouseFunc(int button, int state, int x, int y){
    cvs1.update(x, y, button, state);
}

void motionFunc(int x, int y){
    cvs1.update(x, y, GLUT_LEFT_BUTTON, GLUT_DOWN);
    
    fprintf(stderr, "motion\n");
}

void passiveMotionFunc(int x, int y){
    cvs1.update(x, y);
}

void display(void){
    cvs1.render();  //for first

    glFlush();
    fprintf(stderr, "displayed\n");
}


int main(int argc, char **argv){
    glutInit(&argc, argv);
    glutInitWindowSize(window_width, window_height);

    glutCreateWindow("Canvas Model Demo");

    glutReshapeFunc(reshape);
    glutMouseFunc(mouseFunc);
    glutMotionFunc(motionFunc);
    glutPassiveMotionFunc(passiveMotionFunc);
    glutDisplayFunc(display);

    glutMainLoop();
    return 0;
}
#endif