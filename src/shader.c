#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glad/glad.h>

char *getShaderContent(const char *fileName) {
  FILE *fp;
  long size;
  char *shaderContent;

  // Read file to get size
  fp = fopen(fileName, "rb");
  if (fp == NULL) {
    return "";
  }

  fseek(fp, 0L, SEEK_END);
  size = ftell(fp) + 1;
  fclose(fp);

  // Read file
  fp = fopen(fileName, "r");
  shaderContent = calloc(1, size);
  fread(shaderContent, size - 1, 1, fp);
  fclose(fp);

  return shaderContent;
}

unsigned int createShader(const char *source, int shaderType) {
  unsigned int shader = glCreateShader(shaderType);
  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);

  int success;
  char infoLog[512];

  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    printf("Error: Could not compile %d\n%s\n", shaderType, infoLog);
  }

  return shader;
}

unsigned int createProgram(unsigned int vertexShader,
                           unsigned int fragmentShader) {
  unsigned int shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  int success;
  char infoLog[512];

  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    printf("Error: Could not link shaders to program\n%s\n", infoLog);
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return shaderProgram;
}
