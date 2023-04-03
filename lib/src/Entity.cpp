#include "Shader.h"
#include "Model.h"
#include "Entity.h"

Entity::Entity() : isModel(false) {
    transform = new Transform(this);
}

Entity::Entity(const std::string& path, Shader* s){
    transform = new Transform(this);
    model = new Model(this, path);
    shader = s;
    isModel = true;
}

//add new component
void Entity::addComponent(Component* comp) {
    components.push_back(comp);
}

void Entity::updateComponents()
{
    for (Component* comp : components) {
        comp->update();
    }
}

void Entity::addChild(Entity* arg)
{
    children.push_back(arg);
}


//Update transform if it was changed
void Entity::updateSelfAndChild()
{
    if (!transform->isDirty())
        return;

    forceUpdateSelfAndChild();
}

//Force update of transform even if local space don't change
void Entity::forceUpdateSelfAndChild()
{
    transform->computeModelMatrix();

    for (auto&& child : children)
    {
        child->forceUpdateSelfAndChild();
    }
}

//Draw
void Entity::renderEntity() {
    this->updateComponents();
    this->updateSelfAndChild();

    if (isModel) {
        shader->use();
        shader->setMat4("model", transform->getModelMatrix());
        model->Draw(*shader);
    }

    for (auto & i : children) {
        i->renderEntity();
    }
}

const std::vector<Entity *> &Entity::getChildren() const {
    return children;
}
