#pragma once

#define EXIT glfwTerminate(); \
return -1;

#define BindMatericesBlock(NAME,SHADER_VAR, SLOT) unsigned int NAME##MatricesUniformBlockIdx = glGetUniformBlockIndex(SHADER_VAR.ProgramID, "Materices"); \
glUniformBlockBinding(SHADER_VAR.ProgramID, NAME##MatricesUniformBlockIdx, SLOT);