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

#include "rendering/OpenGL/GLRenderer.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <format>

#include "config.h"
#include "debug/Logger.h"
#include "Engine.h"

void UpdateViewport(GLFWwindow* window [[maybe_unused]], int width, int height)
{
    glViewport(0, 0, width, height);
    Engine::instance->renderer->SetViewportSize({width, height});
}

Render::GLRenderer::GLRenderer(WindowMode wm, Vec2 preferredWindowSize) {

  Debug::LogInfo(std::format("Creating a window with w:{0} h:{1}", preferredWindowSize.x, preferredWindowSize.y));
  
  // InitializeGLFW
  glfwSetErrorCallback([](int error, const char* description) {
      Debug::LogError(std::format("GLFW Error ({0}): {1}", error, description));
  });

  if(glfwPlatformSupported(GLFW_PLATFORM_WIN32)) 
      glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_WIN32);
  else if(glfwPlatformSupported(GLFW_PLATFORM_WAYLAND)) 
      glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_WAYLAND);
  else if(glfwPlatformSupported(GLFW_PLATFORM_X11)) 
      glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
  else glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_NULL);

  if (!glfwInit()) {
      Debug::LogFatal("Failed to initialize GLFW");
      return;
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VER_MAJOR);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_VER_MINOR);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  Debug::LogInfo(std::format("GLFW platform: {0}", glfwGetPlatform()));

  // Creating the window
  Debug::LogTrace("Begining window creation");
  switch (wm) {
  case WindowMode::Windowed: {
    Debug::LogSpam("Windowed window");
    this->window = glfwCreateWindow(preferredWindowSize.x, preferredWindowSize.y, PROJECT_NAME, nullptr, nullptr);
    this->SetViewportSize(preferredWindowSize);
    break;
  }
  case WindowMode::Borderless:
  case WindowMode::Fullscreen: {
    Debug::LogSpam("Borderless or Fullscreen window");

    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode = glfwGetVideoMode(monitor);

    Debug::Assert(monitor, "Failed to get primary monitor!");
    Debug::Assert(mode, "Failed to get monitor video mode!");

    GLFWmonitor *windowMonitor = wm == WindowMode::Windowed ? nullptr : monitor;

    this->window = glfwCreateWindow(mode->width, mode->height, "Planet renderer", windowMonitor, nullptr);
    this->SetViewportSize({mode->width, mode->height});

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

  Debug::Assert(this->window, "Window failed to create!");

  // window setup
  glfwMakeContextCurrent(this->window);
  glfwSetFramebufferSizeCallback(this->window, UpdateViewport);

  // GLAD setup
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      Debug::LogFatal("Failed to initialize GLAD");
      return;
  }

  // OpenGL setup
  // Enabling alpha blending
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // ImGui setup
  Debug::LogTrace("Setting up ImGui");
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(OPENGL_VERSION);


  GLenum err;
  while ((err = glGetError()) != GL_NO_ERROR) {
      Debug::LogError("OpenGL error during renderer initialization: " + std::to_string(err));
  }
}

Render::GLRenderer::~GLRenderer() {
  Debug::LogTrace("OpenGL Renderer destructor triggered");

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  if(this->window != nullptr) {
      glfwDestroyWindow(this->window);
      this->window = nullptr;
  }
}

void Render::GLRenderer::Render() {
  // skip rendering on minimised window
  if(this->windowSize.x == 0 || this->windowSize.y == 0) return;

  // solid screen color
  glClearColor(0.2f, 0.2f, 0.8f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  // TODO: move to own module
  ImGui_ImplGlfw_NewFrame();
  ImGui_ImplOpenGL3_NewFrame();
  ImGui::NewFrame();

  ImGui::Begin("Stuff?");
  static bool whatever = false;
  ImGui::Checkbox("Hell yeah!", &whatever);
  ImGui::End();

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  glfwSwapBuffers(window);
}

bool Render::GLRenderer::ShouldClose() {
  if(!this->window) [[unlikely]] {
    Debug::LogWarn("OpenGL window has an empty pointer for some reason");
    return true;
  }

  return glfwWindowShouldClose(this->window);
}

void Render::GLRenderer::SetViewportSize(Vec2 size) {
  this->windowSize.x = size.x;
  this->windowSize.y = size.y;
}
