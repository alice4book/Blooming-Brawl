#include <vector>

#include "Model.h"
#include "Transform.h"

//class Component;
class Entity
{
public:
    //Scene graph
    std::vector<Component*> components;
    std::vector<Entity*> children;
    Entity* parent = nullptr;
    Shader* shader = nullptr;
    Transform transform;
    std::vector <float> vertices;
    std::vector <int> indices;
    unsigned int VBO, VAO, EBO;
    bool isSkybox = false;
    bool isModel = false;
    int number;
    unsigned int textures;

    // constructor, expects a filepath to a 3D model.
    Entity(std::vector <float> Vertices, std::vector <int> Indices, Shader* s, int nr = 1, bool isItSkybox = false);

    Entity(std::string path, Shader* s, bool gamma = false);

    //add new component
    void addComponent(Component* comp);

    //get component from vector by type
    Component* getComponentByType(ComponentType checkType);

    void loadCubemap(std::vector<std::string> faces);

    void addTexture(Shader* shader, const char* path);

    void addChild(Entity* arg);

    //Update transform if it was changed
    void updateSelfAndChild();

    //Force update of transform even if local space don't change
    void forceUpdateSelfAndChild();

    //Draw
    void renderEntity();

    [[nodiscard]] const std::vector<Entity *> &getChildren() const;

    [[nodiscard]] Entity *getParent() const;
};