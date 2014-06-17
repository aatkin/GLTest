/** Copyright 2014 Anssi Kinnunen */

#ifndef MAIN_HPP_INCLUDED
#define MAIN_HPP_INCLUDED

#define PI 3.141592653589793234
#define GLEW_STATIC
#define GLM_FORCE_RADIANS

#include <GL/glew.h>
#include <GL/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

struct gl_resources
{
    std::vector<GLuint> shader_programs;
    std::vector<GLuint*> buffers;
    std::vector<GLuint*> vertex_arrays;
};

void errorCallback(int error, const char *description);
GLFWwindow *create_glfw(char* title);
GLuint loadShaderFromFile(std::string path, GLenum shaderType);
GLuint create_cube(unsigned int width, unsigned int height, unsigned int depth);
void clean_resources();
#endif // MAIN_HPP_INCLUDED
