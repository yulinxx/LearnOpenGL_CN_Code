// OpenGL使用纯VBO方式渲染出三角形，非VAO_AppNinja的博客-CSDN博客
// https://blog.csdn.net/a2831942318/article/details/122385250

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // 不使用VAO 1
    // 必须使用VAO渲染
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // 不使用VAO 2
    // 渲染出纯VBO三角形
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // left  
         0.5f, -0.5f, 0.0f, // right 
         0.0f,  0.5f, 0.0f  // top   
        }; 

    // VBO：Vertex Buffer Object，顶点缓冲对象。
    // 可以存储大量顶点，因而我们可以利用VBO一次性发送一大批数据到显卡上。
    // 顶点缓冲对象VBO是在显卡存储空间中开辟出的一块内存缓存区，用于存储顶点的各类属性信息，
    // 如顶点坐标，顶点法向量，顶点颜色数据等。在渲染时，可以直接从VBO中取出顶点的各类属性数据，
    // 由于VBO在显存而不是在内存中，不需要从CPU传输数据，处理效率更高。

    // VAO：Vertex Array Object，顶点数组对象。
    // 一个VAO，配置并告诉了我们该如何使用VBO（包括如何解读VBO中的数据），以及使用哪个VBO。使用VAO管理VBO
    // VBO保存了一个模型的顶点属性信息，每次绘制模型之前需要绑定顶点的所有信息，当数据量很大时，重复这样的动作变得非常麻烦。
    // VAO可以把这些所有的配置都存储在一个对象中，每次绘制模型时，只需要绑定这个VAO对象就可以了。
    // VAO是一个保存了所有顶点数据属性的状态结合，它存储了顶点数据的格式以及顶点数据所需的VBO对象的引用。
    // VAO本身并没有存储顶点的相关属性数据，这些信息是存储在VBO中的，VAO相当于是对很多个VBO的引用，把一些VBO组合在一起作为一个对象统一管理。
    // https://blog.csdn.net/dcrmg/article/details/53556664

    // EBO：Element Buffer Object，索引缓冲对象

    unsigned int VBO, VAO;

    // 不使用VAO 3
    // glGenVertexArrays(1, &VAO); // 生成一个VAO对象

    glGenBuffers(1, &VBO);  // 建VBO的第一步需要开辟（声明/获得）显存空间并分配VBO的ID,通过这个ID可以对特定的VBO内的数据进行存取操作。

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    // 绑定VAO对象, 执行VAO绑定之后其后的所有VBO配置都是这个VAO对象的一部分，可以说VBO是对顶点属性信息的绑定，VAO是对很多个VBO的绑定。
    // 不使用VAO 4
    // glBindVertexArray(VAO);  

    // 创建的VBO可用来保存不同类型的顶点数据,创建之后需要通过分配的ID绑定（bind）一下制定的VBO，
    // 对于同一类型的顶点数据一次只能绑定一个VBO。
    // 绑定操作通过 glBindBuffer 来实现，第一个参数指定绑定的数据类型，可以是
    // GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER, GL_PIXEL_PACK_BUFFER或者GL_PIXEL_UNPACK_BUFFER中的一个。
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // 调用 glBufferData 把用户定义的数据传输到当前绑定的显存缓冲区中。
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 顶点数据传入GPU之后，还需要通知OpenGL如何解释这些顶点数据，这个工作由函数glVertexAttribPointer完成：
    // 第一个参数指定顶点属性位置，与 顶点着色器 中 layout(location=0) 对应。
    // 第二个参数指定顶点属性大小。
    // 第三个参数指定数据类型。
    // 第四个参数定义是否希望数据被标准化。
    // 第五个参数是步长（Stride），指定在连续的顶点属性之间的间隔。
    // 第六个参数表示我们的位置数据在缓冲区起始位置的偏移量。

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0); 


    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        glUseProgram(shaderProgram);
        // glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

        // OpenGL中所有的图形都是通过分解成三角形的方式进行绘制，
        // glDrawArrays函数负责把模型绘制出来，它使用当前激活的着色器，
        // 当前VAO对象中的VBO顶点数据和属性配置来绘制出来基本图形。
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // 不使用VAO 5
        // glBindVertexArray(0); // no need to unbind it every time 
 
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    // 不使用VAO 6
    // glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

























// #include <glad/glad.h>
// #include <GLFW/glfw3.h>

// #include <iostream>

// void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// void processInput(GLFWwindow *window);

// // settings
// const unsigned int SCR_WIDTH = 800;
// const unsigned int SCR_HEIGHT = 600;

// const char* vertexShaderSource = "#version 330 core\n"
// "layout (location = 0) in vec3 aPos;\n"
// "void main()\n"
// "{\n"
// "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
// "}\0";

// const char* fragmentShaderSource = "#version 330 core\n"
// "out vec4 FragColor;\n"
// "void main()\n"
// "{\n"
// "   FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
// "}\0";


// int main()
// {
//     // glfw: initialize and configure
//     // ------------------------------
//     glfwInit();
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

//     // 必须使用VAO渲染
//     // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

//     // 渲染出纯VBO三角形
//     glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

// #ifdef __APPLE__
//     glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
// #endif

//     // glfw window creation
//     // --------------------
//     GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
//     if (window == NULL)
//     {
//         std::cout << "Failed to create GLFW window" << std::endl;
//         glfwTerminate();
//         return -1;
//     }
//     glfwMakeContextCurrent(window);
//     glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

//     // glad: load all OpenGL function pointers
//     // ---------------------------------------
//     if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//     {
//         std::cout << "Failed to initialize GLAD" << std::endl;
//         return -1;
//     }  

//     //
//     float vertices[] = {
//  -0.5f, -0.5f, 0.0f,
//   0.5f, -0.5f, 0.0f,
//   0.0f,  0.5f, 0.0f
//     };

//     // 生成一个VBO对象
//     unsigned int VBO;
//     glGenBuffers(1, &VBO);

//     // 生成一个VAO
//     //unsigned int VAO;
//     //glGenVertexArrays(1, &VAO);
//     //glBindVertexArray(VAO);

//     // 绑定到数组缓冲
//     glBindBuffer(GL_ARRAY_BUFFER, VBO);
//     // 复制顶点数据到缓冲内存
//     glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//     // printf("glBufferData sizeof(vertices)=%d\n", sizeof(vertices));
//     // sizeof(vertices)=36
    
//     // 设置顶点属性指针，并启用
//     //layout(location = 0 aPosition
//     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//     //location = 0
//     glEnableVertexAttribArray(0); //顶点坐标
//     // 解绑
//     // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
//     glBindBuffer(GL_ARRAY_BUFFER, 0);

//     /*三角形的位置数据不会改变，每次渲染调用时都保持原样，所以它的使用类型最好是GL_STATIC_DRAW。
//     如果，比如说一个缓冲中的数据将频繁被改变，
//     那么使用的类型就是GL_DYNAMIC_DRAW或GL_STREAM_DRAW，
//     这样就能确保显卡把数据放在能够高速写入的内存部分。*/

//     // 创建顶点着色器，
//     unsigned int vertexShader;
//     vertexShader = glCreateShader(GL_VERTEX_SHADER);
//     // 附加上着色器源代码
//     glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
//     // 编译着色器
//     glCompileShader(vertexShader);

//     // 检测是否编译成功
//     int success = 0;
//     char infoLog[512] = { 0 };
//     glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
//     if (!success)
//     {
//         glGetShaderInfoLog(vertexShader, sizeof(infoLog), NULL, infoLog);
//         std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
//     }
//     else
//     {
//         std::cout << "SHADER::VERTEX::COMPILATION SUCCESS!";
//     }

//     // 创建片段着色器
//     unsigned int fragmentShader;
//     fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//     // 附加上片段着色器源代码
//     glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
//     // 编译
//     glCompileShader(fragmentShader);

//     // 判断编译结果
//     glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
//     if (!success)
//     {
//         glGetShaderInfoLog(fragmentShader, sizeof(infoLog), NULL, infoLog);
//         std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
//     }
//     else
//     {
//         std::cout << "SHADER::FRAGMENT::COMPILATION SUCCESS!" << std::endl;
//     }

//     // 创建着色器程序
//     unsigned int shaderProgram;
//     shaderProgram = glCreateProgram();
//     // 链接之前编译的着色器
//     glAttachShader(shaderProgram, vertexShader);
//     glAttachShader(shaderProgram, fragmentShader);
//     glLinkProgram(shaderProgram);

//     // 判断着色器程序链接结果
//     glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
//     if (!success) {
//         glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
//         std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
//     }
//     else
//     {
//         std::cout << "SHADER::LinkProgram SUCCESS!" << std::endl;
//     }

//     // 删除之前链接过的对象
//     glDeleteShader(vertexShader);
//     glDeleteShader(fragmentShader);

//     // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
//     // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
//     //glBindVertexArray(0);

//     // render loop
//     // -----------
//     while (!glfwWindowShouldClose(window))
//     {
//         // input
//         // -----
//         processInput(window);

//         // render
//         // ------
//         glClearColor(0.1f, 0.3f, 0.2f, 1.0f);
//         glClear(GL_COLOR_BUFFER_BIT);

//         // 激活程序对象
//         glUseProgram(shaderProgram);

//         //glBindVertexArray(VAO);
//         //glBindVertexArray(VAO);
//         //glBindBuffer(GL_ARRAY_BUFFER, VBO);     
//         glDrawArrays(GL_TRIANGLES, 0, 3);

//         //glDrawElements(GL_TRIANGLES, 1, GL_UNSIGNED_SHORT, 0);

//         // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
//         // -------------------------------------------------------------------------------
//         glfwSwapBuffers(window);
//         glfwPollEvents();
//     }

//     glDeleteBuffers(1, &VBO);
//     glDeleteProgram(shaderProgram);

//     // glfw: terminate, clearing all previously allocated GLFW resources.
//     // ------------------------------------------------------------------
//     glfwTerminate();
//     return 0;
// }

// // process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// // ---------------------------------------------------------------------------------------------------------
// void processInput(GLFWwindow *window)
// {
//     if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//         glfwSetWindowShouldClose(window, true);
// }

// // glfw: whenever the window size changed (by OS or user resize) this callback function executes
// // ---------------------------------------------------------------------------------------------
// void framebuffer_size_callback(GLFWwindow* window, int width, int height)
// {
//     // make sure the viewport matches the new window dimensions; note that width and 
//     // height will be significantly larger than specified on retina displays.
//     //glViewport(0, 0, width, height);
//     glViewport(0, 0, 100, 100);
// }