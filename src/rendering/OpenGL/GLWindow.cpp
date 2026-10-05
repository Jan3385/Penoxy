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

#include "rendering/OpenGL/GLWindow.h"

#include "Debug/Logger.h"

std::unordered_map<GLFWwindow*, Render::GLWindow*> Render::GLWindow::activeWindows{};

void Render::GLWindow::SetViewport(Vec2 wm) {
  glViewport(0, 0, wm.x, wm.y);
  this->view = wm;  
}

void UpdateViewport(GLFWwindow* window, int width, int height) {
  Render::GLWindow *w = Render::GLWindow::activeWindows[window];

  w->SetViewport({width, height});
}

Render::GLWindow::~GLWindow() {
  if(this->window != nullptr) {
      GLWindow::activeWindows.erase(this->window);
      glfwDestroyWindow(this->window);
      this->window = nullptr;
  }
}

Render::GLWindow::GLWindow(WindowMode wm, Vec2 preferredWindowSize) {
  Debug::LogTrace("Begining window creation");
  switch (wm) {
  case WindowMode::Windowed: {
    Debug::LogSpam("Windowed window");
    this->window = glfwCreateWindow(preferredWindowSize.x, preferredWindowSize.y, PROJECT_NAME, nullptr, nullptr);
    this->MakeContext();
    this->view = preferredWindowSize;
    break;
  }
  case WindowMode::Borderless:
  case WindowMode::Fullscreen: {
    Debug::LogSpam("Borderless or Fullscreen window");

    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode = glfwGetVideoMode(monitor);

    Debug::Assert(monitor, "Failed to get primary monitor!");
    Debug::Assert(mode, "Failed to get monitor video mode!");

    GLFWmonitor *windowMonitor = wm == WindowMode::Borderless ? nullptr : monitor;

    this->window = glfwCreateWindow(mode->width, mode->height, PROJECT_NAME, windowMonitor, nullptr);

    this->view = preferredWindowSize;

    if (wm == WindowMode::Borderless) {
      glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_FALSE);
      glfwSetWindowPos(window, 0, 0); 
    }
    break;
  }
  default: {
    Debug::LogFatal("Undefined Window Mode set!");
    break;
  }
  }

  GLWindow::activeWindows[this->window] = this;
  glfwSetFramebufferSizeCallback(this->window, UpdateViewport);

  Debug::Assert(this->window, "Window failed to create!");
}

void Render::GLWindow::MakeContext() {
  glfwMakeContextCurrent(this->window);
}

void Render::GLWindow::SwapBuffers() {
  glfwSwapBuffers(window);
}

bool Render::GLWindow::ShouldClose() {
  if(!this->window) [[unlikely]] {
    Debug::LogWarn("OpenGL window has an empty pointer for some reason");
    return true;
  }

  return glfwWindowShouldClose(this->window);
}
