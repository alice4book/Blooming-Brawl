#include "Shader.h"
#include "Model.h"
#include "Entity.h"

Entity::Entity() 
    : isModel(false)
    , parentTranform(nullptr) 
{
    transform = new Transform(this);
}

Entity::Entity(Model* model, Shader* s)
    : model(model)
    , parentTranform(nullptr)
{
    transform = new Transform(this);
    shader = s;
    isModel = true;
}

Entity::Entity(Shader* s)
    : parentTranform(nullptr)
{
    transform = new Transform(this);
    shader = s;
    isModel = false;
}

Entity::Entity(const std::string& path, Shader* s)
    : parentTranform(nullptr) 
{
    transform = new Transform(this);
    model = new Model(path);//this, path);
    shader = s;
    isModel = true;
}

Entity::~Entity()
{
    for (auto component : components) {
        //delete component;
    }
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
    arg->parentTranform = this->transform;
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
    if (parentTranform)
        transform->computeModelMatrix(parentTranform->getModelMatrix());
    else
        transform->computeModelMatrix();

    for (auto&& child : children)
    {
        child->forceUpdateSelfAndChild();
    }
}

//Draw
void Entity::renderEntity() {
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
