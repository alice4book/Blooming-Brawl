#include "Shader.h"
#include "Model.h"
#include "Entity.h"
#include "Animator.h"

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

Entity::Entity(Shader* s, Shader* altShader)
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
    delete transform;

    /*
    for (auto component : components) {
       /delete component;
    }
    components.clear();
    for (auto child : children) {
        delete child;
    }
    children.clear();
    Need to add parent entity
    if (parent) {
        for (int i = 0; i < parent->children.size(); i++) {
            if (parent->children[i] == this) {
                std::cout << i << std::endl;
                parent->children.erase(parent->children.begin() + i);
                break;
            }
        }
        for (int i = children.size() - 1; i >= 0; i--) {
            std::cout << i << std::endl;
            parent->addChild(children[i]);
            children.pop_back();
        }
    }
    std::cout << "deleted" << std::endl;
    */
    //std::cout << " Deleted " << std::endl;
}

//add new component
void Entity::addComponent(Component* comp) {
    components.push_back(comp);
}

void Entity::enableAllComponents(bool value)
{
    for (auto comp : components) {
        comp->enable(value);
    }
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
        shader->setVec3("color", this->color);
        shader->setMat4("model", transform->getModelMatrix());

        if (animator != nullptr)
        {
            auto transforms = animator->GetFinalBoneMatrices();
            for (int i = 0; i < transforms.size(); ++i)
                shader->setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);
        }
        model->Draw(*shader);
    }
    for (auto& i : children) {
        i->renderEntity();
    }
}

void Entity::renderEntity(Shader* shader) {
    this->updateSelfAndChild();

    if (isModel) {
        shader->use();
        shader->setMat4("model", transform->getModelMatrix());
        model->Draw(*shader);
    }
    for (auto& i : children) {
        i->renderEntity();
    }
}

void Entity::setColor(glm::vec3 col) {
    this->color = col;
}

const std::vector<Entity*>& Entity::getChildren() const {
    return children;
}

void Entity::switchShader() {
    if (altShader != nullptr)
        std::swap(shader, altShader);
}

void Entity::clearChildren() {
    if (!children.empty())
        children.clear();
}

void Entity::clearComponents() {
    if (!components.empty())
        components.clear();
}

void Entity::setupAnimator(std::shared_ptr<Animator> nanimator)
{
    animator = nanimator;
}
