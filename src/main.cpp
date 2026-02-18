#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp> 
#include <glm/gtc/constants.hpp>
#include "library/Particle.h"

// screen size vars
const float WIDTH = 800.0f;
const float HEIGHT = 600.0f;

bool mouseMode = false; 

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

    if(particle.position.x + particle.radius > WIDTH){
        particle.position.x = WIDTH - particle.radius;
        particle.velocity.x *= -1.0f;
    }

    if(particle.position.x - particle.radius < 0){
        particle.position.x = particle.radius;
        particle.velocity.x *= -1.0f;
    }

    if(particle.position.y + particle.radius > HEIGHT){
        particle.position.y = HEIGHT - particle.radius;
        particle.velocity.y *= -1.0f;
    }

    if(particle.position.y - particle.radius < 0){
        particle.position.y = particle.radius;
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
    glm::vec2 position(20.0f, 20.0f);

    const float magnitude = 400.0f;
    const float angleDegree = 90.0f;
    const float angleRadians = glm::radians(angleDegree);

    float directionX = magnitude*(cos(angleRadians));
    float directionY = magnitude*(sin(angleRadians));

    glm::vec2 velocity = glm::vec2(0, HEIGHT);

    const float mass = 60.0f;
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

        glm::vec2 gravityAcceleration = particle.setGravityAcceleration(4000.0, 100.0, glm::vec2(WIDTH/2.0, HEIGHT/2.0));
        particle.updatePositionGravity(deltaTime, gravityAcceleration);

        particle.drawParticle();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

