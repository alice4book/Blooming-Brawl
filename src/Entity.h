#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <glad/glad.h>
#include <Shader.h>
#include <Model.h>
#include <Transform.h>

class Entity
{
public:
    //Scene graph
    std::vector<Entity*> children;
    Entity* parent = nullptr;
    Model* model = nullptr;
    Shader* shader = nullptr;
    std::vector <float> vertices;
    std::vector <int> indices;
    unsigned int VBO, VAO, EBO;
    bool isSkybox = false;
    bool isModel = false;
    bool arrow = false;
    int number;
    unsigned int textures;
    Transform transform;

    // constructor, expects a filepath to a 3D model.
    Entity(std::vector <float> Vertices, std::vector <int> Indices, Shader* s, int nr = 1, bool isItSkybox = false){
        vertices = Vertices;
        indices = Indices;
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
    };

    Entity(std::string path, Shader* s, bool gamma = false) {
        model = new Model(path, gamma);
        shader = s;
        isModel = true;
    }

    void loadCubemap(std::vector<std::string> faces)
    {
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

    void addTexture(Shader* shader, const char* path) {
        glGenTextures(1, &textures);
        glBindTexture(GL_TEXTURE_2D, textures);
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // load image, create texture and generate mipmaps
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
        unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);
        shader->use();
        shader->setInt("ourTexture", 0);
    }

    void addChild(Entity* arg)
    {
        children.push_back(arg);
        arg->parent = this;
    }

    //Update transform if it was changed
    void updateSelfAndChild()
    {
        if (!transform.isDirty())
            return;

        forceUpdateSelfAndChild();
    }

    //Force update of transform even if local space don't change
    void forceUpdateSelfAndChild()
    {
        if (parent)
            transform.computeModelMatrix(parent->transform.getModelMatrix());
        else
            transform.computeModelMatrix();

        for (auto&& child : children)
        {
            child->forceUpdateSelfAndChild();
        }
    }

    void drawArrow(glm::vec3 dir) {
        dir = normalize(dir);
        vertices = { 0.5, 0.0, 0.1,
                    -0.5, 0.0, 0.1,
                    0.0,  0.0, -0.3,
                     dir.x/ 2.0f,dir.y/ 2.0f,dir.z/2.0f };
        indices = {
            0,1,2,
            0,1,3,
            1,2,3,
            0,2,3
        };
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
            &vertices[0], GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(float),
            &indices[0], GL_DYNAMIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }

    //Draw
    void renderEntity() {
        this->updateSelfAndChild();

        if (isSkybox) {
            glBindVertexArray(VAO);
            glBindTexture(GL_TEXTURE_CUBE_MAP, textures);
            glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, number);
        }
        else if (isModel) {
            shader->use();
            shader->setMat4("model", transform.getModelMatrix());
            model->Draw(*shader);
        }
        else {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, textures);
            glBindVertexArray(VAO);

            shader->use();
            shader->setMat4("model", transform.getModelMatrix());
            glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, number);
        }
        
        for (int i = 0; i < children.size(); i++) {
            children[i]->renderEntity();
        }
    }

};