#include "submesh.h"


SubMesh::SubMesh(VertexFormat vertexFormat, void* data, int size)
{

}

SubMesh::SubMesh(VertexFormat vertexFormat, void *data, int size, unsigned int *indices, int indices_count)
{

}

void SubMesh::Update()
{
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

    for(int location = 0; location < 10; ++ location)
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
    int numVertices = dataSize / vertexFormat.size;
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
