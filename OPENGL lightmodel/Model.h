#pragma once
#include "obj.h"
#include "Mesh.h"

class Model
{
public:
    std::vector<Mesh> meshes;
    std::map<std::string, GLuint> textureMap;
    Model() {}
    void load(std::string filepath)
    {
        Assimp::Importer import;
        const aiScene* scene = import.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs);
        // 异常处理
        if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            std::cout << "读取模型出现错误: " << import.GetErrorString() << std::endl;
            exit(-1);
        }
        // 模型文件相对路径
        std::string rootPath = filepath.substr(0, filepath.find_last_of('/'));

        // 循环生成 mesh
        for (int i = 0; i < scene->mNumMeshes; i++)
        {
            // 引用当前mesh
            meshes.push_back(Mesh());
            Mesh& mesh = meshes.back();

            // 获取 assimp 的读取到的 aimesh 对象
            aiMesh* aimesh = scene->mMeshes[i];

            // 我们将数据传递给我们自定义的mesh
            for (int j = 0; j < aimesh->mNumVertices; j++)
            {
                // 顶点
                vmath::vec3 vvv;
                vvv[0] = aimesh->mVertices[j].x;
                vvv[1] = aimesh->mVertices[j].y;
                vvv[2] = aimesh->mVertices[j].z;
                mesh.vertexPosition.push_back(vvv);

                // 法线
                vvv[0] = aimesh->mNormals[j].x;
                vvv[1] = aimesh->mNormals[j].y;
                vvv[2] = aimesh->mNormals[j].z;
                mesh.vertexNormal.push_back(vvv);

                // 纹理坐标: 如果存在则加入。assimp 默认可以有多个纹理坐标 我们取第一个（0）即可
                vmath::vec2 vv(0, 0);
                if (aimesh->mTextureCoords[0])
                {
                    vv[0] = aimesh->mTextureCoords[0][j].x;
                    vv[1] = aimesh->mTextureCoords[0][j].y;
                }
                mesh.vertexTexcoord.push_back(vv);
            }

            // 如果有材质，那么传递材质
            if (aimesh->mMaterialIndex >= 0)
            {
                // 获取当前 aimesh 的材质对象
                aiMaterial* material = scene->mMaterials[aimesh->mMaterialIndex];

                // 获取 diffuse 贴图文件路径名称 我们只取1张贴图 故填 0 即可
                aiString aistr;
                material->GetTexture(aiTextureType_DIFFUSE, 0, &aistr);
                std::string texpath = aistr.C_Str();
                texpath = rootPath + '/' + texpath;   // 取相对路径

                // 如果没生成过纹理，那么生成它
                if (textureMap.find(texpath) == textureMap.end())
                {
                    // 生成纹理
                    GLuint tex;
                    glGenTextures(1, &tex);
                    glBindTexture(GL_TEXTURE_2D, tex);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
                    int textureWidth, textureHeight;
                    unsigned char* image = SOIL_load_image(texpath.c_str(), &textureWidth, &textureHeight, 0, SOIL_LOAD_RGB);
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);   // 生成纹理
                    delete[] image;

                    textureMap[texpath] = tex;
                }

                // 传递纹理
                mesh.diffuseTexture = textureMap[texpath];
            }

            // 传递面片索引
            for (GLuint j = 0; j < aimesh->mNumFaces; j++)
            {
                aiFace face = aimesh->mFaces[j];
                for (GLuint k = 0; k < face.mNumIndices; k++)
                {
                    mesh.index.push_back(face.mIndices[k]);
                }
            }

            mesh.bindData();
        }
    }

    void draw(GLuint program)
    {
        for (int i = 0; i < meshes.size(); i++)
        {
            meshes[i].draw(program);
        }
    }
};
