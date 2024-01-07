#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cglm/cglm.h>
#include <cglm/types.h>
#include <cglm/mat4.h>
#include <cglm/affine-pre.h>
#include <cglm/affine.h>
#include <cglm/util.h>
#include <cglm/cam.h>
#include <cglm/vec3.h>

#include <math.h>
#include <stb_image.h>

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

  glEnable(GL_DEPTH_TEST);

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

  // Cube Vertices (Horrifying)
  float vertices[] = {
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,
      0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
      -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

      -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,

      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
      0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,
      0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

      -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f,
  };

  unsigned int VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // === Textures ===

  unsigned int texture1;

  glGenTextures(1, &texture1);
  glBindTexture(GL_TEXTURE_2D, texture1);

  // Set the texture wrapping parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  // Set texture filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Load image, create texture and generate mipmaps
  int width, height, nrChannels;

  stbi_set_flip_vertically_on_load(true);

  unsigned char *data = stbi_load("../assets/textures/grass.png", &width,
                                  &height, &nrChannels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    printf("Failed to load texture");
  }

  stbi_image_free(data);

  // Tell each OpenGL texture sampler which texture it belongs to
  glUseProgram(shaderProgram);
  glUniform1i(glGetUniformLocation(shaderProgram, "grass"), 0);

  // GLM
  mat4 model;
  glm_mat4_identity(model);
  mat4 view;
  glm_mat4_identity(view);
  mat4 projection;
  glm_mat4_identity(projection);

  vec3 cameraPos;
  glm_vec3_make((vec3){0.0f, 0.0f, 0.0f}, cameraPos);

  vec3 cameraTarget;
  glm_vec3_make((vec3){0.0f, 0.0f, 0.0f}, cameraTarget);

  vec3 cameraDirection;
  glm_vec3_sub(cameraPos, cameraTarget, cameraDirection);
  glm_normalize(cameraDirection);

  vec3 up;
  glm_vec3_make((vec3){0.0f, 1.0f, 0.0f}, up);

  vec3 cameraRight;
  glm_cross(up, cameraDirection, cameraRight);
  glm_normalize(cameraRight);

  vec3 cameraUp;
  glm_cross(cameraDirection, cameraRight, cameraUp);

  const float radius = 10.0f;

  unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
  unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
  unsigned int projectionLoc =
      glGetUniformLocation(shaderProgram, "projection");

  // glm_translate(view, (vec3){0.0f, 0.0f, -3.0f});
  glm_perspective(glm_rad(45.0f), (float)WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f,
                  100.0f, projection);

  // Projection Matrix doesn't change often
  glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, projection[0]);

  // === Camera ===

  // Main loop
  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    // === Rendering === //
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Shader
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glUseProgram(shaderProgram);

    // === Coordinates ===
    // glm_rotate(model, glm_rad(2.5f), (vec3){0.0f, 1.0f, 0.0f});

    float camX = sin(glfwGetTime()) * radius;
    float camZ = cos(glfwGetTime()) * radius;

    glm_lookat((vec3){camX, 0.0f, camZ}, (vec3){0.0f, 0.0f, 0.0f},
               (vec3){0.0f, 1.0f, 0.0f}, view);

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model[0]);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view[0]);

    // Draw
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // === Update === //
    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  // Cleanup
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shaderProgram);

  glfwTerminate();
  return 0;
}
