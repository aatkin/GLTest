#include "main.hpp"

namespace GLTest
{
    /**
     *  Error-Callback function for GLFW.
     */
    void errorCallback(int error, const char *description)
    {
        std::cerr << description << " : " << error << std::endl;
    }

    /**
     * Create a GLFW-controlled window that OpenGL can use, and return a pointer to it.
     */
    GLFWwindow *create_glfw()
    {
        glfwInit();

        glfwSetErrorCallback(errorCallback);

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

        GLFWwindow* window = glfwCreateWindow(960, 540, "Hello wurld", nullptr, nullptr); // Windowed

        return window;
    }

    GLuint loadShaderFromFile( std::string path, GLenum shaderType )
    {
        //Open file
        GLuint shaderID = 0;
        std::string shaderString;
        std::ifstream sourceFile( path.c_str() );

        //Source file loaded
        if( sourceFile )
        {
            //Get shader source
            shaderString.assign( ( std::istreambuf_iterator< char >( sourceFile ) ), std::istreambuf_iterator< char >() );
            //Create shader ID
            shaderID = glCreateShader( shaderType );

            //Set shader source
            const GLchar* shaderSource = shaderString.c_str();
            glShaderSource( shaderID, 1, (const GLchar**)&shaderSource, NULL );

            //Compile shader source
            glCompileShader( shaderID );

            //Check shader for errors
            GLint shaderCompiled = GL_FALSE;
            glGetShaderiv( shaderID, GL_COMPILE_STATUS, &shaderCompiled );
            if( shaderCompiled != GL_TRUE )
            {
                std::cout << "Unable to compile shader: " << shaderID << ", source: " << shaderSource << std::endl;
                glDeleteShader( shaderID );
                shaderID = 0;
            }
        }
        else
        {
            printf( "Unable to open file %s\n", path.c_str() );
        }

        return shaderID;
    }
}

int main()
{
    /** Create window and context for OpenGL to draw in. */
    GLFWwindow* window = GLTest::create_glfw();
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    glewInit();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
//    glEnable(GL_STENCIL_TEST);

    std::cout << "OpenGL version " << glGetString(GL_VERSION) << std::endl;

    /** Vertex Array Object */
    GLuint vao_triangle;
    glGenVertexArrays(1, &vao_triangle);
    glBindVertexArray(vao_triangle);

    /** Vertex Buffer Object */
    GLuint vbo;
    glGenBuffers(1, &vbo);
    GLfloat pyramid_vertices[] =
    {
        /** Top */
        0.0f, 0.5f, 0.0f,
        /** Bottom */
        -0.25f, 0.0f, 0.25f,
        0.25f, 0.0f, 0.25f,
        0.25f, 0.0f, -0.25f,
        -0.25f, 0.0f, -0.25f
    };
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyramid_vertices), pyramid_vertices, GL_STATIC_DRAW);

    /** Pyramid colors */
    GLuint vbo_color;
    glGenBuffers(1, &vbo_color);
    GLfloat pyramid_colors[] =
    {
        /** Bottom face */
        1.0f, 0.0f, 0.0f, 1.0f,
        /** Back face */
        0.8f, 0.0f, 0.0f, 1.0f,
        /** Left face */
        0.6f, 0.0f, 0.0f, 1.0f,
        /** Right face */
        0.4f, 0.0f, 0.0f, 1.0f,
        /** Front face */
        0.2f, 0.0f, 0.0f, 1.0f
    };
    glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyramid_colors), pyramid_colors, GL_STATIC_DRAW);

    /** Element Buffer Object */
    GLuint ebo;
    glGenBuffers(1, &ebo);
    GLuint elements[] =
    {
        /** Bottom face */
        1, 2, 3,
        1, 3, 4,
        /** Back face */
        0, 3, 4,
        /** Left face */
        0, 4, 1,
        /** Right face */
        0, 3, 2,
        /** Front face */
        0, 2, 1
    };
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

    /** Load shaders for OpenGL to use. */
    GLuint vertexShader = GLTest::loadShaderFromFile("shaders/triangle.vert", GL_VERTEX_SHADER);
    GLuint fragmentShader = GLTest::loadShaderFromFile("shaders/triangle.frag", GL_FRAGMENT_SHADER);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glBindFragDataLocation(shaderProgram, 0, "outColor");
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

    GLint colorAttrib = glGetAttribLocation(shaderProgram, "color");
    glEnableVertexAttribArray(colorAttrib);
    glVertexAttribPointer(colorAttrib, 4, GL_FLOAT, GL_FALSE, 0, 0);

    /** Projection matrix uniform setup */
    glm::mat4 projection = glm::perspective(45.0f, 16.0f / 9.0f, 0.1f, 100.0f);
    glm::mat4 view = glm::lookAt(
        glm::vec3(0.5, 1.0, 1.5), // Camera is at (0, 0, 5), in World Space
        glm::vec3(0, 0, 0), // and looks at the origin
        glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
    );
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 MVP = projection * view * model;

    GLuint matrixID = glGetUniformLocation(shaderProgram, "MVP");
    glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);

    clock_t start_t, end_t;
//    int total;
//    std::string str;
    double delta;
    double rotate_factor = 0.10f;

    /** Main loop of the program. */
    while(!glfwWindowShouldClose(window))
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);

        start_t = clock();

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//        glClear(GL_COLOR_BUFFER_BIT);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
        glVertexAttribPointer(colorAttrib, 4, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

        glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
//        glFinish();


        glfwSwapBuffers(window);
//        glfwWaitEvents();
        glfwPollEvents();

        end_t = clock();
        delta = (double)(end_t - start_t) * rotate_factor;
//        model = glm::mat4(1.0f);
        model = glm::rotate(model, (glm::mediump_float)delta, glm::vec3(0.25f, 1.0f, 0.5f));
        MVP = projection * view * model;
        glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);
//        total = (int)(end_t - start_t);
//        str = std::to_string(total) + "ms";
//        std::cout << str << std::endl;
//        glfwSetWindowTitle(window, str.c_str());
    }

    glfwTerminate();
    glDeleteProgram(shaderProgram);
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);

    glDeleteBuffers(1, &vbo);

    glDeleteVertexArrays(1, &vao_triangle);
    return 0;
}
