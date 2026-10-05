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
  this->window = new GLWindow(wm, preferredWindowSize);

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

  ImGui_ImplGlfw_InitForOpenGL(window->GetGLFWWindow(), true);
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

  delete this->window;
}

void Render::GLRenderer::Render() {
  if(!window){
    Debug::LogSpam("Empty window for GL rendering!");
    return;
  }

  // skip rendering on minimised window
  if(window->GetView().x == 0 || window->GetView().y == 0) {
    Debug::LogSpam("Window size at (0, 0)");
    return;
  }

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

  window->SwapBuffers();
}

bool Render::GLRenderer::ShouldClose() {
  return window->ShouldClose();
}
