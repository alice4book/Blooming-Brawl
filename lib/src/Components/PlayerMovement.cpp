#define _USE_MATH_DEFINES

#include "PlayerMovement.h"
#include "Transform.h"
#include<cmath>
#include<math.h>
#include <iostream>


PlayerMovement::PlayerMovement(Entity *parent, Transform* transform, DynamicColliderComponent* collider, float speed, EPlayerID ID, glm::vec3 forward)
	: Component(parent)
	, forward(forward)
	, speed(speed)
	, transform(transform)
	, collider(collider)
    , ID(ID)
{
    setForward = forward;
}

void PlayerMovement::move(GLFWwindow* window)
{
    currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    if (ID == Player1) {
            previousForward = forward;
            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS
                || glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
                if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
                    //std::cout << "W" << std::endl;
                    forward.x = 1;
                    //transform->addToLocalPosition(forward * (speed*deltaTime));
                }
                else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
                    //std::cout << "S" << std::endl;
                    forward.x = -1;
                    //transform->addToLocalPosition(forward * (speed * deltaTime));
                }
                else {
                    forward.x = 0;
                }
                if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
                    //std::cout << "A" << std::endl;
                    forward.z = -1;
                    //transform->addToLocalPosition(forward * (speed * deltaTime));
                }
                else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
                    //std::cout << "D" << std::endl;
                    forward.z = 1;
                    //transform->addToLocalPosition(forward * (speed * deltaTime));
                }
                else {
                    forward.z = 0;
                }
                
                forward = glm::vec3{ forward.x,0,forward.z };
                forward = glm::normalize(forward);
                transform->addToLocalPosition(forward * (speed * deltaTime));
                if (previousForward != forward) {
                    //std::cout << "inny" << std::endl;
                    float angle = atan2(setForward.x * forward.z - forward.x * setForward.z, forward.x * forward.z + setForward.x * setForward.z) * (180.0 / M_PI);
                    if (angle == 0 || angle == -0) { angle = 180; }
                    if (setForward == forward) { angle = 0; }
                    if (angle < 0 && angle > -180) { angle = -180 - angle; }
                    //std::cout << -angle << std::endl;
                    transform->setLocalRotation({ 0,-angle,0 });
                }

                //std::cout << forward.x << " " << forward.y << " " << forward.z << std::endl;
            }

    }

    if (ID == Player2) {
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            //std::cout << "W" << std::endl;
            forward = glm::vec3{ 1,0,0 };
            transform->addToLocalPosition(forward * (speed * deltaTime));
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            //std::cout << "S" << std::endl;
            forward = glm::vec3{ -1,0,0 };
            transform->addToLocalPosition(forward * (speed * deltaTime));
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            //std::cout << "A" << std::endl;
            forward = glm::vec3{ 0,0,-1 };
            transform->addToLocalPosition(forward * (speed * deltaTime));
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            //std::cout << "D" << std::endl;
            forward = glm::vec3{ 0,0,1 };
            transform->addToLocalPosition(forward * (speed * deltaTime));
        }
    }
}
