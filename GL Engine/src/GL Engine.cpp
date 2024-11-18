/* Vendor includes */
#include "config.h"
#include <stb_image.h>

/* My includes */
#include "GLFWindow.h"
#include "Shader.h"
#include "Texture.h"
#include "TestCoordinates.h"
#include "Camera.h"

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

/* Camera */
Camera camera;

int main()
{
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    float currentFrame;

    /* Window creation code */
    GLFWwindow* window = GLFWindow::CreateWindow(800, 600);

    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, GLFWindow::framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);


    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    /* glad: load all OpenGL function pointers */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    /* Shaders */
    Shader shader("Shader/ShadersFiles/V_ObjectShader.txt", "Shader/ShadersFiles/F_ObjectShader.txt");
    Shader lightSourceShader("Shader/ShadersFiles/V_LightSource.txt", "Shader/ShadersFiles/F_LightSource.txt");

    /* Vertex Array Object */
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    /* Vertex Buffer Object */
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesWnormalWtextures), verticesWnormalWtextures, GL_STATIC_DRAW);

    glBindVertexArray(VAO);

    /* Vertex Attribute Pointer */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindVertexArray(lightCubeVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    /* Texture */
    unsigned int diffuseMap = Texture::loadTexture("resources/textures/container2.png");
    unsigned int specularMap = Texture::loadTexture("resources/textures/container2_specular.png");

    shader.useProgram();

    unsigned int diffuse = glGetUniformLocation(shader.GetID(), "material.diffuse");
    glUniform1i(diffuse, 0);

    unsigned int specular = glGetUniformLocation(shader.GetID(), "material.specular");
    glUniform1i(specular, 1);

    /* Render Loop */
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        /* Input */
        GLFWindow::processInput(window);
        camera.processKeyboardInput(window, deltaTime);

        /* Render here */
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glEnable(GL_DEPTH_TEST);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /* GAME OBJECT CODE - BIG CUBE */
        shader.useProgram();

        /* Change light color every frame */
        glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

        /* Change light source position every frame */
        glm::vec3 LightPos(2 * sin(glfwGetTime()), 0.0f, 1.5 * cos(glfwGetTime()));
        unsigned int lightPos = glGetUniformLocation(shader.GetID(), "lightPos");
        glUniform3f(lightPos, LightPos.x, LightPos.y, LightPos.z);

        /* Transformations for object */
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(camera.GetZoom()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        unsigned int projLoc = glGetUniformLocation(shader.GetID(), "projection");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glm::mat4 view = camera.GetCameraView();
        unsigned int viewLoc = glGetUniformLocation(shader.GetID(), "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        unsigned int modelLoc = glGetUniformLocation(shader.GetID(), "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        unsigned int viewPos = glGetUniformLocation(shader.GetID(), "viewPos");
        glUniform3f(viewPos, camera.GetCameraPos().x, camera.GetCameraPos().y, camera.GetCameraPos().z);

        /* Material setting */
        unsigned int matShininess = glGetUniformLocation(shader.GetID(), "material.shininess");
        glUniform1f(matShininess, 64.0f);

        unsigned int lightAmbient = glGetUniformLocation(shader.GetID(), "light.ambient");
        unsigned int lightDiffuse = glGetUniformLocation(shader.GetID(), "light.diffuse");
        unsigned int lightSpecular = glGetUniformLocation(shader.GetID(), "light.specular");

        glUniform3f(lightAmbient, 0.2f, 0.2f, 0.2f);
        glUniform3f(lightDiffuse, 0.5f, 0.5f, 0.5f);
        glUniform3f(lightSpecular, 1.0f, 1.0f, 1.0f);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        /* LIGHT SOURCE CODE - SMALL CUBE */
        lightSourceShader.useProgram();

        /* Change light source visual every frame */
        unsigned int sourceColor = glGetUniformLocation(lightSourceShader.GetID(), "sourceColor");
        glUniform3f(sourceColor, lightColor.x, lightColor.y, lightColor.z);

        unsigned int lProj = glGetUniformLocation(lightSourceShader.GetID(), "projection");
        unsigned int lView = glGetUniformLocation(lightSourceShader.GetID(), "view");
        unsigned int lModel = glGetUniformLocation(lightSourceShader.GetID(), "model");
        glUniformMatrix4fv(lProj, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(lView, 1, GL_FALSE, glm::value_ptr(view));

        model = glm::mat4(1.0f);
        model = glm::translate(model, LightPos);
        model = glm::rotate(model, (float)glm::radians(glfwGetTime() * 25.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.1f));
        glUniformMatrix4fv(lModel, 1, GL_FALSE, glm::value_ptr(model));

        glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        /* glfw: swap buffers and poll IO events(keys pressed / released, mouse moved etc.) */
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &VBO);
    glad_glDeleteVertexArrays(1, &VAO);

    /* glfw: terminate, clearing all previously allocated GLFW resources.*/
    glfwTerminate();
    return 0;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    camera.processMouseInput(window, xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.processZoom(window, xoffset, yoffset);
}