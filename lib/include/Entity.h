#pragma once
#include <vector>
#include <string>
#include "glm/glm.hpp"

class Model;
class Shader;
class Transform;
class Component;
class Entity
{
public:
    //Scene graph
    std::vector<Component*> components;
    std::vector<Entity*> children;
    Shader* shader{};
    Shader* altShader{};
    Transform* transform;
    Transform* parentTransform = nullptr;
    Model* model{};
    bool isModel;
    //for pickup 
    glm::vec3 color = {0,0,0};

    Entity();

    Entity(Model* model, Shader* s, Shader* altShader = nullptr);

    explicit Entity(Shader* s, Shader* altShader = nullptr);

    Entity(const std::string& path, Shader* s, Shader* shader = nullptr);

    ~Entity();

    //add new component
    void addComponent(Component* comp);

    //get component from vector by type
    template<typename T>
    bool getComponentsByType(std::vector<T*>* compType = nullptr);

    //update all components
    void updateComponents();

    void addChild(Entity* arg);

    //Update transform if it was changed
    void updateSelfAndChild();

    //Force update of transform even if local space don't change
    void forceUpdateSelfAndChild();

    //Draw
    virtual void renderEntity();

    //For pickup
    void setColor(glm::vec3 col);

    [[nodiscard]] const std::vector<Entity *> &getChildren() const;

    void switchShader();
};

template<typename T>
bool Entity::getComponentsByType(std::vector<T*>* compType) {
    bool isEmpty = false;
    for (Component* comp : components) {
        if (dynamic_cast<T*>(comp) != nullptr) {
            isEmpty = true;

            if (compType == nullptr)
                break;

            compType->push_back((T*) comp);
        }
    }
    return isEmpty;
}
