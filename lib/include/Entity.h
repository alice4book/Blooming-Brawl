#include <vector>
#include <string>

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
    Shader* shader;
    Transform* transform;
    Model* model;
    bool isModel;

    Entity();

    Entity(const std::string& path, Shader* s);

    //add new component
    void addComponent(Component* comp);

    //get component from vector by type
    template<typename T>
    void getComponentsByType(std::vector<T*>& compType);

    //update all components
    void updateComponents();

    void addChild(Entity* arg);

    //Update transform if it was changed
    void updateSelfAndChild();

    //Force update of transform even if local space don't change
    void forceUpdateSelfAndChild();

    //Draw
    virtual void renderEntity();

    [[nodiscard]] const std::vector<Entity *> &getChildren() const;
};

template<typename T>
void Entity::getComponentsByType(std::vector<T*>& compType) {
    for (Component* comp : components) {
        if (dynamic_cast<T*>(comp) != nullptr) {
            compType.push_back((T*) comp);
        }
    }
}