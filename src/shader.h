#ifndef SHADER_H
#define SHADER_H

// Read in a GLSL shader from a file
char *getShaderContent(const char *fileName);

unsigned int createShader(const char *source, int shaderType);

unsigned int createProgram(unsigned int vertexShader,
                           unsigned int fragmentShader);

#endif
