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

#include "rendering/IWindow.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <unordered_map>

namespace Render {
class GLWindow : public Render::IWindow{
public:
  ~GLWindow();
  GLWindow(WindowMode wm, Vec2 preferredWindowSize);

  void MakeContext();
  void SwapBuffers();

  bool SetTitle(std::string &name) override;

  bool ShouldClose();

  GLFWwindow* GetGLFWWindow() { return this->window; };

  static std::unordered_map<GLFWwindow*, GLWindow*> activeWindows;
  void SetViewport(Vec2 wm);
protected:
  GLFWwindow *window = nullptr;
private:
};
};