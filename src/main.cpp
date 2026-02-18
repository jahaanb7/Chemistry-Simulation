#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp> 
#include <glm/gtc/constants.hpp>

// screen size vars
const int WIDTH = 800;
const int HEIGHT = 600;

bool mouseMode = false; 

struct Particle{
  public:
    glm::vec2 position; // 2D position vector (x,y)
    glm::vec2 velocity; // magnitude and direction

    glm::vec2 center = glm::vec2(WIDTH/2, 0);

    float distance = sqrt(pow((center.x - position.x),2.0) + pow((center.y - position.y),2.0));

    //gravity constant (not acceleration)
    float gravityConstant = 0.000000667 * (100/pow(distance, 2.0));
    glm::vec2 gravityAcceleration = glm::vec2(0.0f, -gravityConstant);

    // additional vars
    float mass;
    float radius;
    float charge;

    Particle(glm::vec2 pos, glm::vec2 vel, float m, float r)
      : position(pos), velocity(vel), mass(m), radius(r){}

    void updatePosition(float deltaTime) {
        velocity += gravityAcceleration * deltaTime;
        velocity *= 0.995f; // damping
        position += velocity * deltaTime;
    }

    void drawParticle() {
        glBegin(GL_TRIANGLE_FAN);

        int numSegments = 50;

        for(int i = 0; i < numSegments; i++){

            float theta = 2.0f * glm::pi<float>() * float(i) / float(numSegments);

            float x = radius * cos(theta);
            float y = radius * sin(theta);

            glVertex2f(x + position.x, y + position.y);
        }
        glEnd();
    }
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

Particle mouseMovement(GLFWwindow* window, Particle& particle){
    double xpos; 
    double ypos;

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    glfwGetCursorPos(window, &xpos, &ypos);

    float xPosMouse = static_cast<float>(xpos);
    float yPosMouse = HEIGHT - static_cast<float>(ypos);

    glm::vec2 mousePos(xPosMouse, yPosMouse);
    glm::vec2 direction = mousePos - particle.position;

    particle.velocity = direction * particle.velocity;

    return particle;
}

void boundingBoundary(Particle& particle){

    float damping = 0.97f; 

    if(particle.position.x + particle.radius > WIDTH){
        particle.position.x = WIDTH - particle.radius;
        particle.velocity.x *= 1 - damping;
        particle.velocity.x *= -1.0f;
    }

    if(particle.position.x - particle.radius < 0){
        particle.position.x = particle.radius;
        particle.velocity.x *= 1 - damping;
        particle.velocity.x *= -1.0f;
    }

    if(particle.position.y + particle.radius > HEIGHT){
        particle.position.y = HEIGHT - particle.radius;
        particle.velocity.y *= 1 - damping;
        particle.velocity.y *= -1.0f;
    }

    if(particle.position.y - particle.radius < 0){
        particle.position.y = particle.radius;
        particle.velocity.y *= 1 - damping;
        particle.velocity.y *= -1.0f;
    }
}

int main(void)
{
    if (!glfwInit()){
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Space Simulation", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, WIDTH, 0, HEIGHT, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //initialize Particle
    glm::vec2 position(400.0f, 300.0f);

    const float magnitude = 100.0f;
    const float angleDegree = 65.0f;
    const float angleRadians = glm::radians(angleDegree);

    float directionX = magnitude*(cos(angleRadians));
    float directionY = magnitude*(sin(angleRadians));

    glm::vec2 velocity = glm::vec2(directionX, directionY);

    const float mass = 10.0f;
    const float radius = 20.0f;

    Particle particle(position, velocity, mass, radius);

    float deltaTime = 0.0167f;

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        if(mouseMode){
            mouseMovement(window, particle);
        }

        boundingBoundary(particle);

        particle.updatePosition(deltaTime);

        particle.drawParticle();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

