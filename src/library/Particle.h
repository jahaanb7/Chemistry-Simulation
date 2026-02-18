#include <glm/glm.hpp>

class Particle{
  public:
    glm::vec2 position; // 2D position vector (x,y)
    glm::vec2 velocity; // magnitude and direction

    // additional vars
    float mass;
    float radius;
    float charge;
    float damping = 0.995f;

    Particle(glm::vec2 pos, glm::vec2 vel, float m, float r)
      : position(pos), velocity(vel), mass(m), radius(r){}

    void updatePosition(float deltaTime){
        position += velocity * deltaTime;
    }

    void updatePositionGravity(float deltaTime, glm::vec2 gravityAcceleration){
        velocity += gravityAcceleration * deltaTime;
        velocity *= damping;
        position += velocity * deltaTime;
    }

    glm::vec2 setGravityAcceleration(float gravitationalConstant, float mass, glm::vec2 center){

        glm::vec2 particleDirecton = center - position;
        float distance = glm::length(particleDirecton);

        glm::vec2 unitDirection = particleDirecton / distance;

        float gravityConstant = (gravitationalConstant * mass / (distance*distance + 1.0f));
        glm::vec2 gravityAcceleration = unitDirection * (gravityConstant);

        return gravityAcceleration;
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