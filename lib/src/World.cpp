#include "World.h"
#include "Transform.h"
#include "glad/glad.h"
#include "Shader.h"
#include "StaticColliderComponent.h"
#include "DynamicColliderComponent.h"

// set up vertex data (and buffer(s)) and configure vertex attributes
std::vector <float> vertices;
std::vector <int> indices;
std::vector<std::string> faces;

// Define the static Singleton pointer
World* World::inst_ = nullptr;

World* World::getInstance() {
    if (inst_ == nullptr) {
        inst_ = new World();
    }
    return(inst_);
}

World::World() {
    vertices.insert(vertices.end(), {
        -1.0, 1.0, 1.0,    -1, 1, 1,    0.0, 1.0, //0
        1.0, 1.0, 1.0,     1, 1, 1,     1.0, 1.0, //1
        1.0, -1.0, 1.0,    1, -1, 1,    1.0, 0.0, //2
        -1.0, -1.0, 1.0,   -1, -1, 1,   0.0, 0.0, //3
        -1.0, 1.0, -1.0,   -1, 1, -1,   1.0, 1.0, //4
        1.0, 1.0, -1.0,    1, 1, -1,    0.0, 1.0, //5
        1.0, -1.0, -1.0,   1, -1, -1,   0.0, 0.0, //6
        -1.0, -1.0, -1.0,  -1, -1, -1,  1.0, 0.0  //7
    });

    indices.insert(indices.end(), {
        0, 1, 2,
        0, 3, 2,
        1, 2, 6,
        1, 5, 6,
        0, 1, 5,
        0, 4, 5,
        0, 4, 7,
        0, 3, 7,
        4, 5, 6,
        4, 7, 6,
        7, 6, 2,
        7, 3, 2
    });

    faces = {
        "res/textures/skybox/right.jpg",
        "res/textures/skybox/left.jpg",
        "res/textures/skybox/top.jpg",
        "res/textures/skybox/bottom.jpg",
        "res/textures/skybox/front.jpg",
        "res/textures/skybox/back.jpg"
    };

    transform = new Transform(this);
    glm::vec3 translation;
    translation.x = 0.0f;
    translation.y = 0.0f;
    translation.z = 0.0f;

    unsigned int iVBO;
    glGenBuffers(1, &iVBO);
    glBindBuffer(GL_ARRAY_BUFFER, iVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3), &translation, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    unsigned int VBO, EBO;
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

void World::setShader(Shader* s){
    shader = s;
    loadCubeMap();
}

void World::loadCubeMap() {
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

void World::renderEntity() {
    this->updateComponents();
    this->updateSelfAndChild();

    glBindVertexArray(VAO);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textures);
    glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, 1);

    for (auto & i : children) {
        i->renderEntity();
    }
}

std::vector<StaticColliderComponent*> World::getStaticColliders(){
    if(staticColComp.empty())
        reloadLists();
    return staticColComp;
}

std::vector<DynamicColliderComponent*> World::getDynamicColliders(){
    if(dynamicColComp.empty())
        reloadLists();
    return dynamicColComp;
}

void World::reloadLists(){
    for(auto* child : children){
        child->getComponentsByType<StaticColliderComponent>(staticColComp);
        child->getComponentsByType<DynamicColliderComponent>(dynamicColComp);
    }
}
