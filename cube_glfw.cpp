// cube_glfw.cpp  — minimal spinning cube (OpenGL 2.1 + GLFW)
// macOS build (Intel or Apple Silicon):
//   g++ -std=c++17 cube_glfw.cpp -o cube_glfw \
//       -I/opt/homebrew/include -L/opt/homebrew/lib -lglfw \
//       -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
//   # ^ replace /opt/homebrew with /usr/local if you’re on an Intel-brew install
//   ./cube_glfw
//
// Controls:  Esc or close-window  →  quit.

#define GL_SILENCE_DEPRECATION           // hide Apple’s deprecation spam
#include <GLFW/glfw3.h>
#include <OpenGL/glu.h>                  // adds gluPerspective / gluLookAt
#include <iostream>
#include <cmath>

// ---------------- helpers ----------------
static void errorCallback(int, const char* msg) { std::cerr << "GLFW error: " << msg << '\n'; }
static void keyCallback(GLFWwindow* w,int key,int, int action,int){
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(w, GLFW_TRUE);
}

void drawCube(float s) {
    float v = s;                  // half-extent
    glBegin(GL_QUADS);

    // –Z
    glNormal3f(0,0,-1);  glVertex3f( v,-v,-v); glVertex3f(-v,-v,-v);
                         glVertex3f(-v, v,-v); glVertex3f( v, v,-v);
    // +Z
    glNormal3f(0,0, 1);  glVertex3f(-v,-v, v); glVertex3f( v,-v, v);
                         glVertex3f( v, v, v); glVertex3f(-v, v, v);
    // +X
    glNormal3f(1,0,0);   glVertex3f( v,-v, v); glVertex3f( v,-v,-v);
                         glVertex3f( v, v,-v); glVertex3f( v, v, v);
    // –X
    glNormal3f(-1,0,0);  glVertex3f(-v,-v,-v); glVertex3f(-v,-v, v);
                         glVertex3f(-v, v, v); glVertex3f(-v, v,-v);
    // –Y
    glNormal3f(0,-1,0);  glVertex3f(-v,-v,-v); glVertex3f( v,-v,-v);
                         glVertex3f( v,-v, v); glVertex3f(-v,-v, v);
    // +Y
    glNormal3f(0,1,0);   glVertex3f( v, v,-v); glVertex3f(-v, v,-v);
                         glVertex3f(-v, v, v); glVertex3f( v, v, v);

    glEnd();
}

// ---------------- main ----------------
int main() {
    glfwSetErrorCallback(errorCallback);
    if (!glfwInit()) return -1;

    // Ask for an OpenGL 2.1 **compatibility** context (keeps fixed-function pipeline)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    //  (no profile hints — they’re only valid for 3.2+)

    GLFWwindow* win = glfwCreateWindow(800, 600, "Voxel-style cube", nullptr, nullptr);
    if (!win) { glfwTerminate(); return -1; }
    glfwMakeContextCurrent(win);
    glfwSetKeyCallback(win, keyCallback);
    glfwSwapInterval(1);                        // enable vsync

    std::cout << "GL version: " << glGetString(GL_VERSION) << '\n';

    // Basic fixed-function setup
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    GLfloat lightPos[4] = {2.0f, 2.0f, 2.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    double t0 = glfwGetTime();

    while (!glfwWindowShouldClose(win)) {
        int w, h;  glfwGetFramebufferSize(win, &w, &h);
        float ratio = w / static_cast<float>(h ? h : 1);

        glViewport(0, 0, w, h);
        glClearColor(0.1f, 0.15f, 0.18f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(60.0, ratio, 0.1, 100.0);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(3, 2, 4,   // eye
                  0, 0, 0,   // look-at
                  0, 1, 0);  // up

        double t = glfwGetTime() - t0;
        glRotatef(static_cast<float>(t * 25.0),  0, 1, 0);  // yaw
        glRotatef(static_cast<float>(t * 18.0),  1, 0, 0);  // pitch

        drawCube(1.0f);

        glfwSwapBuffers(win);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
