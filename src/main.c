#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cglm/cglm.h>
#include <cglm/types.h>
#include <cglm/mat4.h>
#include <cglm/affine-pre.h>
#include <cglm/affine.h>
#include <cglm/util.h>
#include <cglm/cam.h>

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "shader.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

// Callback to resive viewport on window resize
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
  if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  GLFWwindow *window =
      glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Minecraft", NULL, NULL);
  if (window == NULL) {
    printf("Failed to create GFLW window\n");
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    printf("Failed to initialize GLAD\n");
    return -1;
  }

  // === Init Code for an Object ===

  // Shaders
  const char *vertexShaderSource =
      getShaderContent("../assets/shaders/vertex.vs");
  unsigned int vertexShader =
      createShader(vertexShaderSource, GL_VERTEX_SHADER);

  const char *fragmentShaderSource =
      getShaderContent("../assets/shaders/fragment.fs");
  unsigned int fragmentShader =
      createShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

  unsigned int shaderProgram = createProgram(vertexShader, fragmentShader);

  // Vertices for our triangles
  float vertices[] = {
      0.5f,  0.5f,  0.0f, //
      0.5f,  -0.5f, 0.0f, //
      -0.5f, -0.5f, 0.0f, //
      -0.5f, 0.5f,  0.0f, //
  };

  // Define two triangles based off of array of vertices
  unsigned int indices[] = {
      0, 1, 3, //
      1, 2, 3, //
  };

  // Create objects
  unsigned int VBO, VAO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  // Bind VAO first, then bind VBO & EBO, then configure vertex attribs
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // Unbind Buffers, do NOT unbind the EBO while a VAO is active
  // The EBO is automatically stored in the VAO so it is not necessary
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // === Coordinates ===

  // Model Matrix consists of transforms applied to every object
  mat4 model;
  glm_mat4_identity(model);

  glm_rotate(model, glm_rad(-55.0f),
             (vec3){1.0f, 0.0f, 0.0f}); // Rotate along x-axis

  // View Matrix... The camera
  mat4 view;
  glm_mat4_identity(view);
  glm_translate(view, (vec3){0.0f, 0.0f, -3.0f});

  // Projection Matrix
  mat4 projection;
  glm_perspective(glm_rad(45.0f), (float)WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f,
                  100.0f, projection);

  // Main loop
  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    // === Rendering === //
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);

    unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model[0]);

    unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view[0]);

    unsigned int projectionLoc =
        glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, projection[0]);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // === Update === //
    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  // Cleanup
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteProgram(shaderProgram);

  glfwTerminate();
  return 0;
}
