#include "PlayerMovement.h"
#include <iostream>
#include "../imgui_impl/imgui_impl_opengl3_loader.h"
#include "Transform.h"

PlayerMovement::PlayerMovement(Entity *parent, Transform* transform, DynamicColliderComponent* collider, float speed, EPlayerID ID, glm::vec3 forward)
	: Component(parent)
	, forward(forward)
	, speed(speed)
	, transform(transform)
	, collider(collider)
    , ID(ID)
{
}

void PlayerMovement::move(GLFWwindow* window)
{
    if (ID == Player1) {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            //std::cout << "W" << std::endl;
            forward = glm::vec3{ 1,0,0 };
            transform->addToLocalPosition(forward * speed);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            //std::cout << "S" << std::endl;
            forward = glm::vec3{ -1,0,0 };
            transform->addToLocalPosition(forward * speed);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            //std::cout << "A" << std::endl;
            forward = glm::vec3{ 0,0,-1 };
            transform->addToLocalPosition(forward * speed);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            //std::cout << "D" << std::endl;
            forward = glm::vec3{ 0,0,1 };
            transform->addToLocalPosition(forward * speed);
        }
    }

    if (ID == Player2) {
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            //std::cout << "W" << std::endl;
            forward = glm::vec3{ 1,0,0 };
            transform->addToLocalPosition(forward * speed);
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            //std::cout << "S" << std::endl;
            forward = glm::vec3{ -1,0,0 };
            transform->addToLocalPosition(forward * speed);
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            //std::cout << "A" << std::endl;
            forward = glm::vec3{ 0,0,-1 };
            transform->addToLocalPosition(forward * speed);
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            //std::cout << "D" << std::endl;
            forward = glm::vec3{ 0,0,1 };
            transform->addToLocalPosition(forward * speed);
        }
    }
}
