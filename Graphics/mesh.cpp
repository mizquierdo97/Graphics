#include "mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <QFile>
#include <iostream>
#include "resources.h"

Mesh::Mesh()
{

}

void Mesh::loadModel(const QString filename)
{
    Assimp::Importer import;

    const aiScene* scene = import.ReadFile(filename.toStdString(), aiProcessPreset_TargetRealtime_MaxQuality);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }
    processNode(scene->mRootNode, scene);

}

void Mesh::Render()
{
       for(unsigned int i = 0; i < submeshes.size(); i++)
       {
           submeshes[i]->Draw();
       }
}

void Mesh::processNode(aiNode *node, const aiScene *scene)
{
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        SubMesh* subMesh = processMesh(mesh, scene);
        //subMesh->Update();
        submeshes.push_back(subMesh);

    }

    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

SubMesh *Mesh::processMesh(aiMesh *mesh, const aiScene *scene)
{
    QVector<float> vertices;
    QVector<unsigned int> indices;

    bool hasTexCoords = false;

    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        vertices.push_back(mesh->mVertices[i].x);
        vertices.push_back(mesh->mVertices[i].y);
        vertices.push_back(mesh->mVertices[i].z);

        vertices.push_back(mesh->mNormals[i].x);
        vertices.push_back(mesh->mNormals[i].y);
        vertices.push_back(mesh->mNormals[i].z);

        if(mesh->mTextureCoords[0])
        {
            hasTexCoords = true;
            vertices.push_back(mesh->mTextureCoords[0][i].x);
            vertices.push_back(mesh->mTextureCoords[0][i].y);
        }
    }

    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    VertexFormat vertexFormat;
    vertexFormat.setVertexAttribute(0,0,3);
    vertexFormat.setVertexAttribute(1, 3 * sizeof (float), 3);

    QOpenGLTexture* texture = nullptr;
    for (unsigned int i = 0 ; i < scene->mNumMaterials ; i++)
    {
        const aiMaterial* pMaterial = scene->mMaterials[i];
        if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
                   aiString path;

                   if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
                   {
                    for(int n = 0; n < resources->textureResources.size(); n++)
                    {
                        QString name = resources->textureResources[n]->name;
                        if(name.compare(path.C_Str()) == 0)
                        {
                            resources->textureResources[n]->Load();
                            texture = resources->textureResources[n]->GLTexture;
                        }
                    }
              }
        }
    }
    if(hasTexCoords)
    {       
        vertexFormat.setVertexAttribute(2, 6* sizeof (float), 2);
    }


    return new SubMesh(vertexFormat,
                       &vertices[0], vertices.size() * sizeof (float),
                       & indices[0], indices.size(), texture);;

}
