// cube_glfw.cpp — interactive cube (rotate with mouse drag)
// Build (Apple Silicon):
//   g++ -std=c++17 cube_glfw.cpp -o cube_glfw \
//       -I/opt/homebrew/include -L/opt/homebrew/lib -lglfw \
//       -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
//   # swap /opt/homebrew → /usr/local if you’re on an Intel-brew install

#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>
#include <OpenGL/glu.h>
#include <iostream>

// ───────── input-state globals ─────────
static bool  g_dragging   = false;
static double g_prevX = 0.0, g_prevY = 0.0;
static float g_yaw   = 0.0f;   // rotation around Y
static float g_pitch = 0.0f;   // rotation around X
constexpr float SENS = 0.3f;   // degrees per pixel

// ───────── helpers ─────────
static void errorCallback(int, const char* m){ std::cerr<<"GLFW error: "<<m<<'\n'; }
static void keyCallback(GLFWwindow* w,int key,int, int action,int){
    if (key==GLFW_KEY_ESCAPE && action==GLFW_PRESS) glfwSetWindowShouldClose(w,GLFW_TRUE);
}
static void mouseButtonCallback(GLFWwindow*, int button, int action, int){
    if (button==GLFW_MOUSE_BUTTON_LEFT){
        g_dragging = (action==GLFW_PRESS);
    }
}
static void cursorPosCallback(GLFWwindow*, double x, double y){
    if (!g_dragging){ g_prevX=x; g_prevY=y; return; }
    double dx = x - g_prevX;
    double dy = y - g_prevY;
    g_prevX = x; g_prevY = y;

    g_yaw   += static_cast<float>(dx * SENS);
    g_pitch += static_cast<float>(dy * SENS);
}

void drawCube(float s){
    float v=s;
    glBegin(GL_QUADS);
    // –Z
    glNormal3f(0,0,-1); glVertex3f( v,-v,-v); glVertex3f(-v,-v,-v);
                         glVertex3f(-v, v,-v); glVertex3f( v, v,-v);
    // +Z
    glNormal3f(0,0, 1); glVertex3f(-v,-v, v); glVertex3f( v,-v, v);
                         glVertex3f( v, v, v); glVertex3f(-v, v, v);
    // +X
    glNormal3f(1,0,0);  glVertex3f( v,-v, v); glVertex3f( v,-v,-v);
                         glVertex3f( v, v,-v); glVertex3f( v, v, v);
    // –X
    glNormal3f(-1,0,0); glVertex3f(-v,-v,-v); glVertex3f(-v,-v, v);
                         glVertex3f(-v, v, v); glVertex3f(-v, v,-v);
    // –Y
    glNormal3f(0,-1,0); glVertex3f(-v,-v,-v); glVertex3f( v,-v,-v);
                         glVertex3f( v,-v, v); glVertex3f(-v,-v, v);
    // +Y
    glNormal3f(0,1,0);  glVertex3f( v, v,-v); glVertex3f(-v, v,-v);
                         glVertex3f(-v, v, v); glVertex3f( v, v, v);
    glEnd();
}

int main(){
    glfwSetErrorCallback(errorCallback);
    if(!glfwInit()) return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,1);

    GLFWwindow* win = glfwCreateWindow(800,600,"Interactive cube",nullptr,nullptr);
    if(!win){ glfwTerminate(); return -1; }
    glfwMakeContextCurrent(win);
    glfwSwapInterval(1);

    glfwSetKeyCallback(win,keyCallback);
    glfwSetMouseButtonCallback(win,mouseButtonCallback);
    glfwSetCursorPosCallback(win,cursorPosCallback);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    GLfloat lightPos[4]={2.f,2.f,2.f,1.f};
    glLightfv(GL_LIGHT0,GL_POSITION,lightPos);

    while(!glfwWindowShouldClose(win)){
        int w,h; glfwGetFramebufferSize(win,&w,&h);
        float ratio=w/static_cast<float>(h?h:1);

        glViewport(0,0,w,h);
        glClearColor(0.1f,0.15f,0.18f,1.f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(60.0,ratio,0.1,100.0);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(3,2,4, 0,0,0, 0,1,0);

        glRotatef(g_pitch,1,0,0);
        glRotatef(g_yaw,  0,1,0);

        drawCube(1.0f);

        glfwSwapBuffers(win);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
