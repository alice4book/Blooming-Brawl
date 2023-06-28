#pragma once
#include <vector>
#include <string>
#include "glm/glm.hpp"
#include <memory>

class Model;
class Shader;
class Transform;
class Component;
class Animator;
class TimeManager;
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
    std::shared_ptr<Animator> animator = nullptr;
    //for pickup 
    glm::vec3 color = { 0,0,0 };

    Entity();

    Entity(Model* model, Shader* s, Shader* altShader = nullptr);

    explicit Entity(Shader* s, Shader* altShader = nullptr);

    Entity(const std::string& path, Shader* s, Shader* shader = nullptr);

    ~Entity();

    //add new component
    void addComponent(Component* comp);

    void enableAllComponents(bool value);

    //get component from vector by type
    template<typename T>
    bool getComponentsByType(std::vector<T*>* compType = nullptr);

    template<typename T>
    bool getComponent(T* componentType);

    //update all components
    void updateComponents();

    void addChild(Entity* arg);

    //Update transform if it was changed
    void updateSelfAndChild();

    //Force update of transform even if local space don't change
    void forceUpdateSelfAndChild();

    //Draw
    virtual void renderEntity();
    virtual void renderEntity(Shader* shader);

    //For pickup
    void setColor(glm::vec3 col);

    [[nodiscard]] const std::vector<Entity*>& getChildren() const;

    void switchShader();

    void clearChildren();

    void clearComponents();

    void setupAnimator(std::shared_ptr<Animator> nanimator);
};

template<typename T>
bool Entity::getComponentsByType(std::vector<T*>* compType) {
    bool isEmpty = false;
    if (compType == nullptr)
        return false;
    for (Component* comp : components) {
        if (dynamic_cast<T*>(comp) != nullptr) {
            isEmpty = true;

            compType->push_back((T*)comp);
        }
    }
    return isEmpty;
}

template<typename T>
bool Entity::getComponent(T* componentType) {
    bool isEmpty = false;
    if (componentType == nullptr)
        return false;
    for (Component* comp : components) {
        if (dynamic_cast<T*>(comp) != nullptr) {
            isEmpty = true;

            componentType = (T*)comp;
        }
    }
    return isEmpty;
}
