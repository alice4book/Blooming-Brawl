#ifndef OPENGLGP_WORLD_H
#define OPENGLGP_WORLD_H

#include "Entity.h"

class StaticColliderComponent;
class DynamicColliderComponent;
class World : public Entity{
public:
    // This is how clients can access the single instance
    static World* getInstance();

    void setShader(Shader* s);

    void renderEntity() override;

    std::vector<StaticColliderComponent*> getStaticColliders();
    std::vector<DynamicColliderComponent*> getDynamicColliders();

    void reloadLists(Entity* e = nullptr);

private:
    unsigned int textures{};
    std::vector <float> vertices;
    std::vector <int> indices;
    unsigned int VAO{};
    std::vector<StaticColliderComponent*> staticColComp;
    std::vector<DynamicColliderComponent*> dynamicColComp;

    void loadCubeMap();


    static World* inst_;   // The one, single instance
    World(); // private constructor
    World(const World&);
    World& operator=(const World&);
};



#endif //OPENGLGP_WORLD_H
