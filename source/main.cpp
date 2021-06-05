#include "main.h"
#include "timer.h"
#include "ball.h"

using namespace std;

GLMatrices Matrices;
GLuint programID;
GLFWwindow *window;

Ball ball1;

float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
//float camera_rotation_angle = 0;
float camera_x = 0, camera_y = 0, camera_z = 45, obj_x = 0, obj_y = 0, obj_z = 0;

bool isSpinning = false;

Timer t60(1.0 / 60);

void draw()
{
    // clear the color and depth in the frame buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    glUseProgram(programID);

    // Eye - Location of camera
    // All inputs are converted to sin*cos functions so the camera can correctly revolve around the object. @ is multiplied simply to have distance from the object.
    glm::vec3 eye(
        2 * sin(camera_x * M_PI / 180.0f) * cos(camera_y * M_PI / 180.0f),
        2 * cos(camera_z * M_PI / 180.0f) * sin(camera_y * M_PI / 180.0f),
        2 * sin(camera_z * M_PI / 180.0f) * cos(camera_x * M_PI / 180.0f));
    //glm::vec3 eye(camera_x, camera_y, camera_z);
    //do uncomment the above line if  regular vector movement was expected.
    //since i have already implemented rotation in the original eye vector, the logic behind it is:
    //components of a unit vector are sin(x) and cos(x). Therefore to move around the x axis, the eye
    //vector must be along the lines of (k, sin(x), cos(x))
    //this is expanded to what is above.

    // Target - Where is the camera looking at
    glm::vec3 target(0, 0, 0);

    // Up - Up vector defines tilt of camera
    glm::vec3 up(1, 0, 0);

    // Compute Camera matrix (view)
    Matrices.view = glm::lookAt(eye, target, up); // Rotating Camera for 3D

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    glm::mat4 MVP; // MVP = Projection * View * Model

    // Scene render
    //set position is used to change the position of the object
    ball1.set_position(obj_x, obj_y, obj_z);
    ball1.draw(VP);
}

void tick_input(GLFWwindow *window)
{
    //move camera
    //here, moving the camera essentially rotates it around the axes. I have intentially implemented it in this fashion as implementing simple increments to the eye vector doesnt allow one to view the full vector.
    int tab = glfwGetKey(window, GLFW_KEY_TAB); //y
    int e = glfwGetKey(window, GLFW_KEY_E);     //y
    int w = glfwGetKey(window, GLFW_KEY_W);     //x
    int s = glfwGetKey(window, GLFW_KEY_S);     //x
    int d = glfwGetKey(window, GLFW_KEY_D);     //z
    int a = glfwGetKey(window, GLFW_KEY_A);     //z

    //move object
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);      //z
    int left = glfwGetKey(window, GLFW_KEY_LEFT);        //z
    int down = glfwGetKey(window, GLFW_KEY_DOWN);        //y
    int up = glfwGetKey(window, GLFW_KEY_UP);            //y
    int shift = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT); //x
    int space = glfwGetKey(window, GLFW_KEY_SPACE);      //x

    //spinning object, default positions
    isSpinning = glfwGetKey(window, GLFW_KEY_0);
    int one = glfwGetKey(window, GLFW_KEY_1);
    int two = glfwGetKey(window, GLFW_KEY_2);
    int three = glfwGetKey(window, GLFW_KEY_3);

    if (w)
        camera_x += 1;
    else if (s)
        camera_x -= 1;
    else if (tab)
        camera_y += 1;
    else if (e)
        camera_y -= 1;
    else if (a)
        camera_z += 1;
    else if (d)
        camera_z -= 1;

    else if (left)
        obj_y += 0.1;
    else if (right)
        obj_y -= 0.1;
    else if (up)
        obj_x += 0.1;
    else if (down)
        obj_x -= 0.1;
    else if (space)
        obj_z += 0.1;
    else if (shift)
        obj_z -= 0.1;

    //default positions, can be accessed by pressing 1, 2, and 3
    else if (one)
    {
        camera_z = 45;
        camera_x = 45;
        camera_y = 45;
    }
    else if (two)
    {
        camera_z = 0;
        camera_y = 45;
        camera_x = 0;
    }
    else if (three)
    {
        camera_z = 0;
        camera_y = 45;
        camera_x = -30;
    }
}

void tick_elements()
{
    if (isSpinning)
        ball1.tick();
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height, int type)
{
    ball1 = Ball(0, 0, 0, type);

    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("../source/shaders/shader.vert", "../source/shaders/shader.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");

    reshapeWindow(window, width, height);

    glClearColor(COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f);
    glClearDepth(1.0f);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}

int main(int argc, char **argv)
{
    //command line handler
    int type = 0;
    if (argc > 2)
        return 0;
    if (argc == 2)
        type = argv[1][0] - '0';
    if (type > 2)
        return 0;

    srand(time(0));
    int width = 600;
    int height = 600;

    window = initGLFW(width, height);

    initGL(window, width, height, type);

    while (!glfwWindowShouldClose(window))
    {
        // Process timers

        if (t60.processTick())
        {
            // 60 fps
            // OpenGL Draw commands
            draw();
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);

            tick_elements();
            tick_input(window);
        }

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

    quit(window);
}

bool detect_collision(bounding_box_t a, bounding_box_t b)
{
    return (abs(a.x - b.x) * 2 < (a.width + b.width)) &&
           (abs(a.y - b.y) * 2 < (a.height + b.height));
}

void reset_screen()
{
    float top = screen_center_y + 4 / screen_zoom;
    float bottom = screen_center_y - 4 / screen_zoom;
    float left = screen_center_x - 4 / screen_zoom;
    float right = screen_center_x + 4 / screen_zoom;
    Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}
