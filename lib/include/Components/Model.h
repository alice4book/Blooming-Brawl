#ifndef MODEL_H
#define MODEL_H

#include "assimp/scene.h"
#include "Mesh.h"
#include "Transform.h"
#include <map>
#include <vector>
#include <string>
#include <AssimpGLMHelpers.h>

struct BoneInfo
{
    //index in finalBoneMatrices
    int id;
    //offset matrix transforms vertex from model space to bone space
    glm::mat4 offset;
};

unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);

class Model {//: public Component {
public:
    // model data 
    std::vector<Texture> textures_loaded;    // stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    std::vector<Mesh> meshes;

    std::string directory;
    std::string pathString;
    bool gammaCorrection;

    std::map<std::string, BoneInfo> m_BoneInfoMap;
    int m_BoneCounter = 0;

    std::map<std::string, BoneInfo>& getBoneInfoMap();
    int& getBoneCount();
    void setVertexBoneData(Vertex& vertex, int boneID, float weight);
    void extractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene);

    // constructor, expects a filepath to a 3D model.
    //Model(Entity *parent, std::string const &path, bool gamma = false);

    Model(std::string const& path, bool gamma = false);

    // draws the model, and thus all its meshes
    void Draw(Shader& shader);

private:

    void SetVertexBoneDataToDefault(Vertex& vertex);

    void SetVertexBoneData(Vertex& vertex, int boneID, float weight);

    // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void loadModel(std::string const& path);

    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void processNode(aiNode* node, const aiScene* scene);

    Mesh processMesh(aiMesh* mesh, const aiScene* scene);

    void ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene)
    {
        auto& boneInfoMap = m_BoneInfoMap;
        int& boneCount = m_BoneCounter;

        for (int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
        {
            int boneID = -1;
            std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
            if (boneInfoMap.find(boneName) == boneInfoMap.end())
            {
                BoneInfo newBoneInfo;
                newBoneInfo.id = boneCount;
                newBoneInfo.offset = AssimpGLMHelpers::ConvertMatrixToGLMFormat(mesh->mBones[boneIndex]->mOffsetMatrix);
                boneInfoMap[boneName] = newBoneInfo;
                boneID = boneCount;
                boneCount++;
            }
            else
            {
                boneID = boneInfoMap[boneName].id;
            }
            assert(boneID != -1);
            auto weights = mesh->mBones[boneIndex]->mWeights;
            int numWeights = mesh->mBones[boneIndex]->mNumWeights;

            for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
            {
                int vertexId = weights[weightIndex].mVertexId;
                float weight = weights[weightIndex].mWeight;
                assert(vertexId <= vertices.size());
                SetVertexBoneData(vertices[vertexId], boneID, weight);
            }
        }
    }

    // checks all material textures of a given type and loads the textures if they're not loaded yet.
    // the required info is returned as a Texture struct.
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName);
};
#endif