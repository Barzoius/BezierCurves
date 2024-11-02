#pragma once

#include "Bindable.hpp"
#include "VertexBuffer.hpp"


class VertexArray : public Bindable
{
public:

    VertexArray();
    ~VertexArray();

    void Bind() override;
    void Unbind() override;


    GLuint GetID();
private:
    GLuint VA_ID;
   // VertexBuffer vbuf;
};