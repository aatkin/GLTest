#include "../include/main.hpp"

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

int main()
{
    /** Create window and context for OpenGL to draw in. */
    GLFWwindow* window = create_glfw();
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    glewInit();

    std::cout << "OpenGL version " << glGetString(GL_VERSION) << std::endl;

    /** Vertex Array Object */
    GLuint vao_triangle;
    glGenVertexArrays(1, &vao_triangle);
    glBindVertexArray(vao_triangle);

    /** Vertex Buffer Object */
    GLuint vbo;
    glGenBuffers(1, &vbo);

    float vertices[] =
    {
        -0.5f,  0.5f,
        0.5f, 0.5f,
        0.5f, -0.5f,
        -0.5f, -0.5f
    };

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    /** Element Buffer Object */
    GLuint elements[] =
    {
        0, 1, 2,
        0, 1, 3
    };

    GLuint ebo;
    glGenBuffers(1, &ebo);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

    /** Load shaders for OpenGL to use. */
    GLuint vertexShader = loadShaderFromFile("shaders/triangle.vert", GL_VERTEX_SHADER);
    GLuint fragmentShader = loadShaderFromFile("shaders/triangle.frag", GL_FRAGMENT_SHADER);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glBindFragDataLocation(shaderProgram, 0, "outColor");
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

    clock_t start_t, end_t;
    double total;
    std::string str;

    /** Main loop of the program. */
    while(!glfwWindowShouldClose(window))
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);

        /** Start timing */
        start_t = clock();

        /** Clear color bit and buffers, draw stuff and wait for rendering to finish */
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glFinish();

        /** Stop timer and display it on window title */
        end_t = clock();
        total = (double)(end_t - start_t);
        str = std::to_string(total) + "ms";
        glfwSetWindowTitle(window, str.c_str());

        /** Swap back buffer to front and poll for any events */
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    glDeleteProgram(shaderProgram);
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);

    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);

    glDeleteVertexArrays(1, &vao_triangle);
    return 0;
}
