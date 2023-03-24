#include "glad/glad.h"
#include "Shader.h"
#include "Model.h"
#include "Entity.h"

Entity::Entity(Shader* s){
    transform = new Transform(this);
    shader = s;
}

Entity::Entity(std::vector<float> Vertices, std::vector<int> Indices, Shader *s, int nr, bool isItSkybox) {
    transform = new Transform(this);
    vertices = std::move(Vertices);
    indices = std::move(Indices);
    number = nr;
    shader = s;
    isSkybox = isItSkybox;

    glm::vec3 translations[40000];
    int iter = 0;
    float offset = 0.1f;

    for (int y = -200; y < 200; y += 2)
    {
        for (int x = -200; x < 200; x += 2)
        {
            glm::vec3 translation;
            translation.x = (float)x / 10.0f + offset;
            translation.y = 0.0f;
            translation.z = (float)y / 10.0f + offset;;
            translations[iter++] = translation;
        }
    }

    unsigned int iVBO;
    glGenBuffers(1, &iVBO);
    glBindBuffer(GL_ARRAY_BUFFER, iVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * 40000, &translations[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
                 &vertices[0], GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(float),
                 &indices[0], GL_DYNAMIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glEnableVertexAttribArray(3);
    glBindBuffer(GL_ARRAY_BUFFER, iVBO); // this attribute comes from a different vertex buffer
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(3, 1); // tell OpenGL this is an instanced vertex attribute.
}

void Entity::loadCubemap(std::vector<std::string> faces) {
    glGenTextures(1, &textures);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textures);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {

        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            stbi_set_flip_vertically_on_load(false);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    shader->use();
    shader->setInt("skybox", 0);
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

//get component from vector by type
Component* Entity::getComponentByType(ComponentType checkType) {
    for (Component* comp : components) {
        if (comp->isComponentType(checkType)) {
            return comp;
        }
    }
    return nullptr;
}

void Entity::addChild(Entity* arg)
{
    children.push_back(arg);
    arg->parent = this;
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
    if (parent)
        transform->computeModelMatrix(parent->transform->getModelMatrix());
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

    if (isSkybox) {
        glBindVertexArray(VAO);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textures);
        glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, number);
    }
    else if (isModel) {
        shader->use();
        shader->setMat4("model", transform->getModelMatrix());
        model->Draw(*shader);
    }
    else {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures);
        glBindVertexArray(VAO);

        shader->use();
        shader->setMat4("model", transform->getModelMatrix());
        glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, number);
    }

    for (auto & i : children) {
        i->renderEntity();
    }
}

const std::vector<Entity *> &Entity::getChildren() const {
    return children;
}

Entity *Entity::getParent() const {
    return parent;
}

