/** Penoxy
 *
 * Copyright (C) 2026 Penoxy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 *
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "rendering/IRenderer.h"
#include "rendering/OpenGL/GLWindow.h"
#include "debug/Logger.h"

namespace Render {
/// @brief Checks and prints any GL errors present
/// @return `false` if no errors found; `true` if any errors are present during calling
static inline bool CheckGLErrors(){
  bool errFound = false;

  GLenum err;
  while ((err = glGetError()) != GL_NO_ERROR) {
      errFound = true;
      Debug::LogError("OpenGL error during renderer initialization: " + std::to_string(err));
  }

  return errFound;
}

class GLRenderer : public Render::IRenderer {
public:
  GLRenderer(WindowMode wm, Vec2i preferredWindowSize);
  ~GLRenderer();
  void Render() override;

  IWindow* GetWindow() override;
  void SetVSYNC(bool enable) override;

  bool ShouldClose() override;
private:
  GLWindow *window = nullptr;
};
};