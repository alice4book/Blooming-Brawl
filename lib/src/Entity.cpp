#include "Shader.h"
#include "Model.h"
#include "Entity.h"

Entity::Entity() 
    : isModel(false)
    , parentTransform(nullptr)
{
    transform = new Transform(this);
}

Entity::Entity(Model* model, Shader* s, Shader* altShader)
    : model(model), shader(s), altShader(altShader)
{
    transform = new Transform(this);
    isModel = true;
}

Entity::Entity(Shader* s,Shader* altShader)
    : shader(s), altShader(altShader)
{
    transform = new Transform(this);
    isModel = false;
}

Entity::Entity(const std::string& path, Shader* s, Shader* altShader)
    : shader(s), altShader(altShader)
{
    transform = new Transform(this);
    model = new Model(path);//this, path);
    isModel = true;
}

Entity::~Entity()
{
//    for (auto component : components) {
//        delete component;
//    }
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
    arg->parentTransform = this->transform;
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
    if (parentTransform)
        transform->computeModelMatrix(parentTransform->getModelMatrix());
        
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

void Entity::switchShader(){
    if(altShader != nullptr)
        std::swap(shader, altShader);
}
