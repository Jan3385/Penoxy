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

namespace Render {
  enum class WindowMode {
    Windowed,
    Borderless,
    Fullscreen,
  };

  class GLRenderer : public Render::IRenderer {
  public:
    GLRenderer(WindowMode wm, Vec2 preferredWindowSize);
    ~GLRenderer();
    void Render() override;

    bool ShouldClose() override;

    void SetViewportSize(Vec2 size) override;
  private:
    GLFWwindow *window = nullptr;

    Vec2 windowSize{0, 0};
  };
};