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
        // �쳣����
        if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            std::cout << "��ȡģ�ͳ��ִ���: " << import.GetErrorString() << std::endl;
            exit(-1);
        }
        // ģ���ļ����·��
        std::string rootPath = filepath.substr(0, filepath.find_last_of('/'));

        // ѭ������ mesh
        for (int i = 0; i < scene->mNumMeshes; i++)
        {
            // ���õ�ǰmesh
            meshes.push_back(Mesh());
            Mesh& mesh = meshes.back();

            // ��ȡ assimp �Ķ�ȡ���� aimesh ����
            aiMesh* aimesh = scene->mMeshes[i];

            // ���ǽ����ݴ��ݸ������Զ����mesh
            for (int j = 0; j < aimesh->mNumVertices; j++)
            {
                // ����
                vmath::vec3 vvv;
                vvv[0] = aimesh->mVertices[j].x;
                vvv[1] = aimesh->mVertices[j].y;
                vvv[2] = aimesh->mVertices[j].z;
                mesh.vertexPosition.push_back(vvv);

                // ����
                vvv[0] = aimesh->mNormals[j].x;
                vvv[1] = aimesh->mNormals[j].y;
                vvv[2] = aimesh->mNormals[j].z;
                mesh.vertexNormal.push_back(vvv);

                // ��������: �����������롣assimp Ĭ�Ͽ����ж���������� ����ȡ��һ����0������
                vmath::vec2 vv(0, 0);
                if (aimesh->mTextureCoords[0])
                {
                    vv[0] = aimesh->mTextureCoords[0][j].x;
                    vv[1] = aimesh->mTextureCoords[0][j].y;
                }
                mesh.vertexTexcoord.push_back(vv);
            }

            // ����в��ʣ���ô���ݲ���
            if (aimesh->mMaterialIndex >= 0)
            {
                // ��ȡ��ǰ aimesh �Ĳ��ʶ���
                aiMaterial* material = scene->mMaterials[aimesh->mMaterialIndex];

                // ��ȡ diffuse ��ͼ�ļ�·������ ����ֻȡ1����ͼ ���� 0 ����
                aiString aistr;
                material->GetTexture(aiTextureType_DIFFUSE, 0, &aistr);
                std::string texpath = aistr.C_Str();
                texpath = rootPath + '/' + texpath;   // ȡ���·��

                // ���û���ɹ�������ô������
                if (textureMap.find(texpath) == textureMap.end())
                {
                    // ��������
                    GLuint tex;
                    glGenTextures(1, &tex);
                    glBindTexture(GL_TEXTURE_2D, tex);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
                    int textureWidth, textureHeight;
                    unsigned char* image = SOIL_load_image(texpath.c_str(), &textureWidth, &textureHeight, 0, SOIL_LOAD_RGB);
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);   // ��������
                    delete[] image;

                    textureMap[texpath] = tex;
                }

                // ��������
                mesh.diffuseTexture = textureMap[texpath];
            }

            // ������Ƭ����
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
