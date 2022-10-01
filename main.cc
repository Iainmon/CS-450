#include <iostream>
#include <vector>

#include <math.h>
#define TWO_PI 6.2831

#define GL_SILENCE_DEPRECATION 1

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


template<typename T>
struct vector {
    std::vector<T> v;
    vector(T x,T y,T z) {
        std::vector<T> _v = {x, y, z};
        v = _v;
    }
    T& operator[](size_t i) { return v[i]; }
};
using point = vector<float>;
const point origin = point(0.0,0.0,0.0);



std::vector<point>
circle(point origin, float radius, int num_points) {
    std::vector<point> points;

    for (int i = 0; i < num_points; i++) {
        float theta = ((float) i / (float) num_points) * TWO_PI;

        float x = radius * cos(theta);
        float y = radius * sin(theta);

        points.push_back(point(x,y,0.0));
    }
    return points;
}

void axes(float r) {
    glBegin(GL_LINE_STRIP);
        glColor3f(1.0,0.0,0.0);
        glVertex3f(0.0,0.0,0.0);
        glVertex3f(1.0 * r,0.0,0.0);
    glEnd();
    glBegin(GL_LINE_STRIP);
        glColor3f(0.0,1.0,0.0);
        glVertex3f(0.0,0.0,0.0);
        glVertex3f(0.0,1.0 * r,0.0);
    glEnd();
    glBegin(GL_LINE_STRIP);
        glColor3f(0.0,0.0,1.0);
        glVertex3f(0.0,0.0,0.0);
        glVertex3f(0.0,0.0,1.0 * r);
    glEnd();
}

int main_window;

int window_width = 640;
int window_height = 400;

float mouse_x = 0.0;
float mouse_y = 0.0;

#define NUM_POINTS 100


float u_time = 0.0;
float dt = 0.0;

void tick() {
    glutSetWindow(main_window);
    int ticks = glutGet(GLUT_ELAPSED_TIME);
    float last_time = u_time;
    float next_time = (float) ticks / 1000.0;
    dt = next_time - last_time;
    u_time = next_time;
}

const float rotation_intensity = 3.0;

void display()
{
    glutSetWindow(main_window);

    glDrawBuffer( GL_BACK );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );


    tick();
    std::cout << "Delta time: " << dt << std::endl;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    glViewport(0, 0, window_width, window_height);
    float aspect = ((float) window_width / (float) window_height);
    gluPerspective(70.f, aspect, 0.1f, 100.f );
    // gluPerspective(70.f, 1.f, 0.1f, 100.f );

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    

    float ex = rotation_intensity * cos(u_time);
    float ey = rotation_intensity * sin(u_time);
    gluLookAt(ex,ey,5.0, 0.0,0.0,0.0, 0.0,1.0,0.0);

    axes(1.0);

    auto solid = circle(origin,0.7,15);
    glBegin(GL_LINE_LOOP);
        glColor3f(0.1,0.2,0.4);
        for (auto & p : solid) {
            glVertex3f(p[0],p[1],p[2] - 3.0);
            glVertex3f(p[0],p[1],p[2] + 3.0);
        }
    glEnd();
    solid = circle(origin,0.7,50);
    glBegin(GL_TRIANGLE_FAN);
        glColor3f(0.1,0.2,0.4);
        for (auto & p : solid) {
            glVertex3f(p[0],p[1],p[2] + 3.0);
        }
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
        glColor3f(0.1,0.2,0.4);
        for (auto & p : solid) {
            glVertex3f(p[0],p[1],p[2] - 3.0);
        }
    glEnd();


    auto shape = circle(origin,1.0,NUM_POINTS);

    glBegin(GL_LINE_LOOP);
        glColor3f(0.4,0.2,0.8);
        for (auto & p : shape) {
            glVertex3f(p[0],p[1],p[2]);
        }
    glEnd();
    glBegin(GL_LINE_LOOP);
        glColor3f(0.4,0.8,0.2);
        for (auto & p : shape) {
            glVertex3f(p[0],p[1],p[2] -2.0);
        }
    glEnd();
    glBegin(GL_LINE_LOOP);
        glColor3f(0.9,0.4,0.2);
        for (auto & p : shape) {
            glVertex3f(p[0],p[1],p[2] +2.0);
        }
    glEnd();

    glutSwapBuffers();
    glFlush();
}

void animate() {
    glutSetWindow(main_window);
    glutPostRedisplay();
}

#define FIXED_UPDATE_INTERVAL_MS 100

void fixed_update(int value) {
    
}

void reshape (int w, int h) {
    window_width = w;
    window_height = h;
}

void keyboard(unsigned char key, int x, int y) {
   switch (key) {
      case 27:
      case 'q':
      case 'Q':
         exit(0);
         break;
   }
}

void mouse_motion(int x,int y) {
    mouse_x = (float) x / (float) window_width;
    mouse_y = (float) y / (float) window_height;
    std::cout << "Mouse X: " << mouse_x << std::endl;
    std::cout << "Mouse Y: " << mouse_y << std::endl;
}

int main(int argc, char **argv)
{

    glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    // glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(window_width, window_height);
    glutInitWindowPosition(100, 100);
    main_window = glutCreateWindow("My Title");
    glutSetWindow(main_window);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMotionFunc(mouse_motion);
    glutPassiveMotionFunc(mouse_motion);
    glutIdleFunc(animate);
    glutTimerFunc(FIXED_UPDATE_INTERVAL_MS, fixed_update, 0);

    glutSetWindow(main_window);
    glutMainLoop();
    
    return 0;
}
