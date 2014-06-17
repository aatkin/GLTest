/** Copyright 2014 Anssi Kinnunen */

#include "main.hpp"

namespace GLTest
{
    gl_resources RESOURCES;

    /**
     *  Error-Callback function for GLFW.
     */
    void errorCallback(int error, const char *description)
    {
        std::cerr << description << " : " << error << std::endl;
    }

    /**
     * Create a GLFW-controlled window that OpenGL can use,
     * and return a pointer to it.
     */
    GLFWwindow *create_glfw(char *title)
    {
        glfwInit();
        glfwSetErrorCallback(errorCallback);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_SAMPLES, 0);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
        GLFWwindow *window = glfwCreateWindow(1280, 720,
                                              title, nullptr, nullptr);
        return window;
    }

    GLuint loadShaderFromFile(std::string path, GLenum shaderType)
    {
        // Open file
        GLuint shaderID = 0;
        std::string shaderString;
        std::ifstream sourceFile(path.c_str());

        // Source file loaded
        if(sourceFile)
            {
                // Get shader source
                shaderString.assign((std::istreambuf_iterator< char > (sourceFile)),
                                    std::istreambuf_iterator< char >());
                // Create shader ID
                shaderID = glCreateShader(shaderType);
                // Set shader source
                const GLchar *shaderSource = shaderString.c_str();
                glShaderSource(shaderID, 1, (const GLchar **) &shaderSource, NULL);
                // Compile shader source
                glCompileShader(shaderID);
                // Check shader for errors
                GLint shaderCompiled = GL_FALSE;
                glGetShaderiv(shaderID, GL_COMPILE_STATUS, &shaderCompiled);

                if(shaderCompiled != GL_TRUE)
                    {
                        std::cout << "Unable to compile shader: " <<
                                  shaderID << ", source: " << shaderSource <<
                                  std::endl;
                        glDeleteShader(shaderID);
                        shaderID = 0;
                    }
            }
        else
            {
                printf("Unable to open file %s\n", path.c_str());
            }

        return shaderID;
    }

    GLuint create_cube(float width, float height, float depth)
    {
        assert(width > 0.0f || height > 0.0f || depth > 0.0f);
        GLuint vbo;
        glGenBuffers(1, &vbo);
        GLfloat w = 0.001f * width, h = 0.001f * height, d = 0.001f * depth;
        GLfloat cube_vertices[] =
        {
            // Front
            -w, h, d, // 0
            w, h, d, // 1
            w, -h, d, // 2
            -w, -h, d, // 3
            // Back
            -w, h, -d, // 4
            w, h, -d, // 5
            w, -h, -d, // 6
            -w, -h, -d // 7
        };
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices,
                     GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        return vbo;
    }

    void clean_resources()
    {
        std::cout << "Cleaning up GLFW..." << std::endl;
        // Frees up any resources allocated by GLFW library
        glfwTerminate();

        while(!GLTest::RESOURCES.vertex_arrays.empty())
            {
                std::cout << "Cleaning up vertex array object..." << std::endl;
                glDeleteVertexArrays(1, GLTest::RESOURCES.vertex_arrays.back());
                GLTest::RESOURCES.vertex_arrays.pop_back();
            }

        while(!GLTest::RESOURCES.buffers.empty())
            {
                std::cout << "Cleaning up buffer object..." << std::endl;
                glDeleteBuffers(1, GLTest::RESOURCES.buffers.back());
                GLTest::RESOURCES.buffers.pop_back();
            }

        while(!GLTest::RESOURCES.shader_programs.empty())
            {
                std::cout << "Cleaning up shader program..." << std::endl;
                glDeleteProgram(GLTest::RESOURCES.shader_programs.back());
                GLTest::RESOURCES.shader_programs.pop_back();
            }
    }
} // namespace GLTest


int main()
{
    GLFWwindow *window = GLTest::create_glfw(const_cast<char *>("Hello wurld"));
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    glewInit();
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    std::cout << "OpenGL version " << glGetString(GL_VERSION) << std::endl;

    // Vertex Array Object
    GLuint vao;
    glGenVertexArrays(1, &vao);
    GLTest::RESOURCES.vertex_arrays.push_back(&vao);
    glBindVertexArray(vao);

    // Vertex Buffer Object
    GLuint vbo = GLTest::create_cube(350.0f, 350.0f, 350.0f);
    GLTest::RESOURCES.buffers.push_back(&vbo);

    // Pyramid colors
    GLuint vbo_color;
    glGenBuffers(1, &vbo_color);
    GLTest::RESOURCES.buffers.push_back(&vbo_color);
    GLfloat cube_colors[] =
    {
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,

        0.8f, 0.0f, 0.0f,
        0.8f, 0.0f, 0.0f,
        0.8f, 0.0f, 0.0f,
        0.8f, 0.0f, 0.0f
    };
    glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_colors), cube_colors, GL_STATIC_DRAW);

    // Element Buffer Object
    GLuint ebo;
    glGenBuffers(1, &ebo);
    GLTest::RESOURCES.buffers.push_back(&ebo);
    // Draw order only works on cubes created with create_cube() function
    GLuint cube_elements[] =
    {
        // Front face
        0, 1, 2,
        0, 2, 3,
        // Back face
        4, 5, 6,
        4, 6, 7,
        // Left face
        0, 3, 7,
        0, 7, 4,
        // Right face
        1, 2, 6,
        1, 6, 5,
        // Top face
        0, 1, 5,
        0, 5, 4,
        // Bottom face
        2, 3, 7,
        2, 7, 6
    };
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements), cube_elements,
                 GL_STATIC_DRAW);

    // Load shaders for OpenGL to use
    GLuint vertexShader = GLTest::loadShaderFromFile("shaders/triangle.vert",
                          GL_VERTEX_SHADER);
    GLuint fragmentShader = GLTest::loadShaderFromFile("shaders/triangle.frag",
                            GL_FRAGMENT_SHADER);
    GLuint shaderProgram = glCreateProgram();
    GLTest::RESOURCES.shader_programs.push_back(shaderProgram);
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glBindFragDataLocation(shaderProgram, 0, "outColor");
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);
    // Delete shaders after they have been linked to save memory
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);

    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
    GLint colorAttrib = glGetAttribLocation(shaderProgram, "color");
    glEnableVertexAttribArray(colorAttrib);
    glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Projection matrix uniform setup
    glm::mat4 projection = glm::perspective(45.0f, 16.0f / 9.0f, 0.1f, 100.0f);
    glm::mat4 view = glm::lookAt(
                         glm::vec3(0.0f, 1.5f, 2.5f),   // "Camera" position
                         glm::vec3(0, 0, 0),   // "Camera" look-at vector
                         glm::vec3(0, 1, 0)   // Head is up (set to 0,-1,0 to look upside-down)
                     );
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 MVP = projection * view * model;
    GLuint matrixID = glGetUniformLocation(shaderProgram, "MVP");
    glUniformMatrix4fv(matrixID, 1, GL_FALSE, glm::value_ptr(MVP));
    double start, stop, delta, rotation, elapsed_time = 0.0f;
    double x = 0.0f, y = 0.0f, move_speed = 0.0f;
    glm::mat4 rotate_m4, translate_m4, mult_m4;
    int frames = 0;
    struct timespec sleep_value = {0};
    int ms_multiplier = 1000000;
    const float TARGET_FPS = 60.0f;
    const float TARGET_MS = 1000.0f / TARGET_FPS;
    const GLuint CUBE_VERTICES = 36;
    // Set wireframe drawing on
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Main loop of the program
    while(!glfwWindowShouldClose(window))
        {
            glfwPollEvents();

            if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                glfwSetWindowShouldClose(window, GL_TRUE);

            start = glfwGetTime();

            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
            glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
            glDrawElements(GL_TRIANGLES, CUBE_VERTICES, GL_UNSIGNED_INT, 0);

            glfwSwapBuffers(window);

            // Rotate a full revolution once every second
            rotation = (2.0f * PI * delta);
            // Circle around at the current draw speed
            x = glm::cos(move_speed);
            y = glm::sin(move_speed);
            translate_m4 = glm::translate(glm::mat4(1.0f), glm::vec3(x, y / 2.0f, 0.0f));
            rotate_m4 = glm::rotate(rotate_m4, (glm::mediump_float)(rotation / 3.0f),
                                    glm::vec3(1.0f, 1.0f, 0.0f));
            mult_m4 = translate_m4 * rotate_m4;
            MVP = projection * view * model * mult_m4;
            glUniformMatrix4fv(matrixID, 1, GL_FALSE, glm::value_ptr(MVP));
            move_speed += delta;
            // Get elapsed time in seconds
            stop = glfwGetTime();
            delta = stop - start;

            if(elapsed_time >= 1.0f)
                {
                    std::string title = "Hello wurld - ";
                    title.append(std::to_string((elapsed_time * 1000.0f) / frames));
                    title.append("ms");
                    glfwSetWindowTitle(window, title.c_str());
                    elapsed_time = 0.0f;
                    frames = 0;
                }

            if((delta * 1000.0f) < TARGET_MS)
                {
                    sleep_value.tv_nsec = (TARGET_MS - (delta * 1000.0f)) * ms_multiplier;
                    nanosleep(&sleep_value, nullptr);
                    delta = glfwGetTime() - start;
                }

            elapsed_time += delta;
            frames = frames + 1;
        }

    GLTest::clean_resources();
    return 0;
}
