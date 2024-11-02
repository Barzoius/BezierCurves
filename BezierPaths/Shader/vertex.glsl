#version 330 core
layout (location = 0) in vec2 aPos; 
  
out vec4 vertexColor; 

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;


void main()
{
    gl_Position = model * vec4(aPos, 0.0, 1.0); 
    vertexColor = vec4(0.5, 1.0, 0.0, 1.0); 
}