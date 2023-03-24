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
    Entity* parent = nullptr;
    Shader* shader = nullptr;
    Transform* transform;
    Model* model;
    std::vector <float> vertices;
    std::vector <int> indices;
    unsigned int VBO, VAO, EBO;
    bool isSkybox = false;
    bool isModel = false;
    int number;
    unsigned int textures;

    explicit Entity(Shader* s);

    Entity(const std::string& path, Shader* s);

    Entity(std::vector <float> Vertices, std::vector <int> Indices, Shader* s, int nr = 1, bool isItSkybox = false);

    void loadCubemap(std::vector<std::string> faces);

    //add new component
    void addComponent(Component* comp);

    //get component from vector by type
    Component* getComponentByType(ComponentType checkType);

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