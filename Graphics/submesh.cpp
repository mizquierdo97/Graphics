#include "submesh.h"
#include<QtOpenGL>
#include <QOpenGLFunctions>
#include<QOpenGLFunctions_3_3_Core>
#include "resources.h"
SubMesh::SubMesh(VertexFormat vertexFormat, void* data, int size): ibo(QOpenGLBuffer::IndexBuffer)
{
    this->vertexFormat = vertexFormat;
    this->data = static_cast<unsigned char*>(data);
    this->dataSize = size;
    glfuncs = QOpenGLContext::currentContext()->functions();
}

SubMesh::SubMesh(VertexFormat vertexFormat, void *data, int size, unsigned int *indices, int indices_count): ibo(QOpenGLBuffer::IndexBuffer)
{
    this->vertexFormat = vertexFormat;
    //this->data = static_cast<unsigned char*>(data);
    this->data = new unsigned char[size];
    std::memcpy(this->data, data, size);
    this->dataSize = size;
    this->indicesCount = indices_count;
    this->indices = new unsigned int[indices_count];
    std::memcpy(this->indices, indices, indices_count * sizeof(unsigned int));
    glfuncs = QOpenGLContext::currentContext()->functions();
}

void SubMesh::Update()
{
    if(vao.isCreated()) return;
    vao.create();
    vao.bind();

    vbo.create();
    vbo.bind();
    vbo.setUsagePattern(QOpenGLBuffer::UsagePattern::StaticDraw);
    vbo.allocate(data, int(dataSize));
    delete[] data;
    data = nullptr;

    if(indices != nullptr)
    {
        ibo.create();
        ibo.bind();
        ibo.setUsagePattern(QOpenGLBuffer::UsagePattern::StaticDraw);
        ibo.allocate(indices, int(indicesCount * sizeof(unsigned int)));
        delete[] indices;
        indices = nullptr;
    }

    for(int location = 0; location < 3; ++ location)
    {
        VertexAttribute &attr = vertexFormat.attribute[location];

        if (attr.enabled)
        {
            glfuncs->glEnableVertexAttribArray(GLuint(location));
            glfuncs->glVertexAttribPointer(GLuint(location), attr.ncomp, GL_FLOAT,
                                           GL_FALSE, vertexFormat.size, (void*) (attr.offset));
        }
    }

    //RELEASE
    vao.release();
    vbo.release();
    if(ibo.isCreated())
    {
        ibo.release();
    }

}

void SubMesh::Draw()
{
    Update();
    int numVertices = dataSize / vertexFormat.size;


    if(GLTexture != nullptr)
        glBindTexture(GL_TEXTURE_2D, GLTexture->textureId());
    //GLuint textureId = resources->textureResources[0]->textureID;
    vao.bind();
    if(indicesCount > 0)
    {
        glfuncs->glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, nullptr);
    }
    else
    {
        glfuncs->glDrawArrays(GL_TRIANGLES, 0, numVertices);
    }
    vao.release();
}

void SubMesh::Destroy()
{
    if(vbo.isCreated())vbo.destroy();
    if(ibo.isCreated())ibo.destroy();
    if(vao.isCreated())vao.destroy();
}
