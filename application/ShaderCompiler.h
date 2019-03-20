#ifndef SHADERCOMPILER_H
#define SHADERCOMPILER_H

#include "scene/RenderKey.h"

#include <string>

void compileVertexShader(const std::string &name);
void compilePixelShader(const std::string &name);

std::string pixelShaderName(const std::string &name, RenderKey::Features features);

void compilePixelShaderSet(const std::string &name, RenderKey::Features features);

#endif // SHADERCOMPILER_H
