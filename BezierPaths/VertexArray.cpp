#include "VertexArray.hpp"
#include <iostream>

VertexArray::VertexArray()
{
    glGenVertexArrays(1, &VA_ID);

}


VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &VA_ID);
}


void VertexArray::Bind()
{
    glBindVertexArray(VA_ID);

}


void VertexArray::Unbind()
{
    glBindVertexArray(0);
}




GLuint VertexArray::GetID()
{
    return VA_ID;
}