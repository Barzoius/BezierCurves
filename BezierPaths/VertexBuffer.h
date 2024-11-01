#pragma once

#include "Bindable.h"

#include <vector>

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

template<typename VERTEX>
class VertexBuffer : public Bindable
{
public:

    VertexBuffer()
    {
        glGenBuffers(1, &ID);
    }


    ~VertexBuffer()
    {
        glDeleteBuffers(1, &ID);
    }


    void Bind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, ID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * maxVerticies, nullptr, GL_DYNAMIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
        glEnableVertexAttribArray(0);


    }

    void BindSub()
    {
        glBindBuffer(GL_ARRAY_BUFFER, ID);
        glBufferSubData(GL_ARRAY_BUFFER, 0, mVertices.size() * sizeof(glm::vec2), mVertices.data());

    }


    void Unbind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }



    void AddVertexCap(int maxV)
    {
        maxVerticies = maxV;
    }

    size_t GetVertexSize() const
    {
        return sizeVERTEX;
    }

    GLuint GetID() const
    {
        return ID;
    }

private:
    GLuint ID;
    size_t sizeVERTEX;
    std::vector<VERTEX> mVertices; 

    int maxVerticies;
};
