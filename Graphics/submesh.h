#ifndef SUBMESH_H
#define SUBMESH_H

#include "vertexformat.h"
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLFunctions>

class SubMesh
{
public:
    SubMesh(VertexFormat vertexFormat, void* data, int size);
    SubMesh(VertexFormat vertexFormat, void* data, int size, unsigned int* indices, int indices_count);
    ~SubMesh();

    void Update();
    void Draw();
    void Destroy();

private:

    unsigned char* data = nullptr;
    size_t dataSize = 0;

unsigned int* indices = nullptr;
    size_t indicesCount = 0;

    VertexFormat vertexFormat;
    QOpenGLBuffer vbo;
    QOpenGLBuffer ibo;
    QOpenGLVertexArrayObject vao;

    QOpenGLFunctions* glfuncs;

};

#endif // SUBMESH_H
