// canvas_model.cpp
// 2020/10/12 02:05
// by Eric W. 00757036
#include <cstdio>
#include <cstdlib>
#include <string>
#include <cmath>
#include <vector>

#include <GL/glut.h>

const int window_width = 400,
          window_height = 400;

class Canvas {
public:
    enum State{
        CURSOR,
        POINT,
        ERACE,
        LINE,
        POLYGON,
        CIRCLE,
        TEXT
    };
    int x = -1,
        y = -1,
        w = 0,
        h = 0;

    unsigned char snapshot[512*512][3];  // max w,h is 512
    //Color background_color;

    // ------------------------
    State draw_mode = POINT;
    int x_from = -1,
        y_from = -1;
    int x_current = -1,
        y_current = -1;

    float paint_size = 1.0;
    bool penDown = false;

    std::vector<std::pair<int, int>> polygonVec;
    //Color fore_color[2];

    Canvas(){}
    Canvas(int x, int y, int w, int h):x(x), y(y), w(w), h(h){}

    void setState(const Canvas::State state){
        draw_mode = state;
    }

    void saveSnapshot(){
        glReadPixels(x, y, w, h, GL_RGB, GL_UNSIGNED_BYTE, snapshot);
    }
    void loadSnapshot(){
        glRasterPos2i(x, y);
        glDrawPixels(w, h, GL_RGB, GL_UNSIGNED_BYTE, snapshot);
    }

    void drawCircle(int center_x, int center_y, int border_x, int border_y){
        const int sides = 32;
        const double radius = sqrt((border_x-center_x)*(border_x-center_x)+
                              (border_y-center_y)*(border_y-center_y));
        glPushMatrix();
            glTranslated(center_x, center_y, 0);
            glBegin(GL_LINE_LOOP);
                const double deg2rad = 3.1415926535897932384626433832795/180.0;
                for(int t = 0; t < 360; t+=360.0 / sides){
                    glVertex2f(cos(t*deg2rad) * radius, sin(t*deg2rad) * radius);
                }
            glEnd();
        glPopMatrix();
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
        //fprintf(stderr, "in in in\n");
        glClear(GL_COLOR_BUFFER_BIT);
        switch (draw_mode) {
        case CURSOR:
            /* DO NOTHING */
            break;
        case POINT:
            /* code */
            loadSnapshot();
            glLineWidth(paint_size);
            glColor3f(1.0, 1.0, 1.0);
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
        case ERACE:
            /* code */
            // draw rect that color is background color
            //if mouseDown
            //     put a backgroundColor-rect at the cursor
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
                    glColor3f(1.0, 1.0, 1.0);
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
                glColor3f(1.0, 1.0, 1.0);
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
                glColor3f(1.0, 1.0, 1.0);
                if(penDown && (x_from==x_current && y_from==y_current)){
                    penDown = false;
                    // draw a polygon with the vector
                    glBegin(GL_POLYGON);
                    for(auto it = polygonVec.begin()+1; it != polygonVec.end(); it++){
                        glVertex2f(it->first, it->second);
                        glVertex2f((it-1)->first, (it-1)->second);
                    }
                    glEnd();
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
                    glColor3f(1.0, 1.0, 1.0);
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
                glColor3f(1.0, 1.0, 1.0);
                drawCircle(x_from, y_from, x_current, y_current);
                fprintf(stderr, "stop\n");
                saveSnapshot();
            }
            break;
        case TEXT:
            /* code */
            break;
        default:
            break;
        }

        render();
    }
    void render(){
        //glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(1.0, 1.0, 1.0);

        //glRectf(x, y, x+w, y+h);

        glFinish();
    }
};

Canvas cvs1(50, 50, 300, 300);

void reshape(int width, int height){
    fprintf(stderr, "reshaped\n");
    cvs1.saveSnapshot();
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
    //cvs1.render();  //for first
    

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