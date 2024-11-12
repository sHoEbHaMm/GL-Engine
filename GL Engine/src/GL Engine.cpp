/* Vendor includes */
#include "config.h"

/* My includes */
#include "GLFWindow.h"
#include "Shader.h"
#include "Texture.h"
#include "TestCoordinates.h"

int main()
{
    /* Window creation code */
    GLFWwindow* window = GLFWindow::CreateWindow(800, 600);

    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, GLFWindow::framebuffer_size_callback);

    /* glad: load all OpenGL function pointers */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    /* Shaders */
    Shader shader("Shader/ShadersFiles/vShader.txt", "Shader/ShadersFiles/fShader.txt");

    /* Vertex Array Object */
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    /* Vertex Buffer Object */
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(CubeVertices), CubeVertices, GL_STATIC_DRAW);

    /* Element Buffer Object */
    //unsigned int EBO;
    //glGenBuffers(1, &EBO);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    /* Vertex Attribute Pointer */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    //glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    /* Texture */
    Texture texture("Resources/Textures/Wood.jpg");

    /* Render Loop */
    while (!glfwWindowShouldClose(window))
    {
        /* Input */
        GLFWindow::processInput(window);

        /* Render here */
        //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClearColor(sin(glfwGetTime()), cos(glfwGetTime()), tan(glfwGetTime()), 1.0f);
        glEnable(GL_DEPTH_TEST);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        texture.Bind();
        shader.useProgram();
        glBindVertexArray(VAO);


        // create transformations

        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        unsigned int projLoc = glGetUniformLocation(shader.GetID(), "projection");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -8.0f));
        view = glm::rotate(view, (float)glm::radians(200.0f * sin(glfwGetTime())), glm::vec3(0.0f, 1.0f, 0.0f));
        unsigned int viewLoc = glGetUniformLocation(shader.GetID(), "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

        for (unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle;
            if (i == 0) { 
                angle = 20.0f * glfwGetTime();
            }
            else {
                angle = 20.0f * glfwGetTime() * i;
            }
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            unsigned int modelLoc = glGetUniformLocation(shader.GetID(), "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        /* glfw: swap buffers and poll IO events(keys pressed / released, mouse moved etc.) */
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &VBO);
    //glDeleteBuffers(1, &EBO);
    glad_glDeleteVertexArrays(1, &VAO);

    /* glfw: terminate, clearing all previously allocated GLFW resources.*/
    glfwTerminate();
    return 0;
}

