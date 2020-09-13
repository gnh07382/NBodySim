#ifndef DRAWLINES_H
#define DRAWLINES_H

#include <glad/glad.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"

#include <string>
#include <vector>

template <typename data>
class DrawLines
{
private:
	unsigned int VBO, VAO;
    void setupMesh()
    {
        // create buffers/arrays
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        
        glBindVertexArray(VAO);
        // load data into vertex buffers
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // A great thing about structs is that their memory layout is sequential for all its items.
        // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
        // again translates to 3/2 floats which translates to a byte array.
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(data), &vertices[0], GL_STATIC_DRAW);

        // set the vertex attribute pointers
        // vertex Positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(data), (void*)0);
       
        glBindVertexArray(0);
    }

public:
	DrawLines(std::vector<data> line)
    {
        
    }
    void drawline(Shader& shader)
    {
        glUseProgram(shader);
        glBindVertexArray(VAO);
        glDrawArrays();
    }

    std::vector<data> vertices;
};

#endif