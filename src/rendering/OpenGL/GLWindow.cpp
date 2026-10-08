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
#include "rendering/OpenGL/GLRenderer.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "debug/Logger.h"

std::unordered_map<GLFWwindow*, Render::GLWindow*> Render::GLWindow::activeWindows{};

void Render::GLWindow::SetViewport(Vec2i wm) {
  glViewport(0, 0, wm.x, wm.y);
  this->viewport = wm;  
}

void UpdateViewport(GLFWwindow* window, int width, int height) {
  Render::GLWindow *w = Render::GLWindow::activeWindows[window];

  w->SetViewport({width, height});
}

void KeyboardInputCallback(GLFWwindow* window, int key, int scancode [[maybe_unused]], int action, int mods){
  Render::GLWindow *w = Render::GLWindow::activeWindows[window];

  if (action == GLFW_RELEASE) return;

  switch (key)
  {
  case GLFW_KEY_BACKSPACE:  w->PushUTF32CharToQueue(UTF32_BACKSPACE); break;
  case GLFW_KEY_TAB:        w->PushUTF32CharToQueue(UTF32_TAB);       break;
  case GLFW_KEY_ENTER:      w->PushUTF32CharToQueue(UTF32_ENTER);     break;
  case GLFW_KEY_ESCAPE:     w->PushUTF32CharToQueue(UTF32_ESCAPE);    break;
  case GLFW_KEY_DELETE:     w->PushUTF32CharToQueue(UTF32_DELETE);    break;
  case GLFW_KEY_LEFT:       w->PushUTF32CharToQueue(UTF32_LEFT);      break;
  case GLFW_KEY_RIGHT:      w->PushUTF32CharToQueue(UTF32_RIGHT);     break;
  case GLFW_KEY_UP:         w->PushUTF32CharToQueue(UTF32_UP);        break;
  case GLFW_KEY_DOWN:       w->PushUTF32CharToQueue(UTF32_DOWN);      break;
  case GLFW_KEY_HOME:       w->PushUTF32CharToQueue(UTF32_HOME);      break;
  case GLFW_KEY_END:        w->PushUTF32CharToQueue(UTF32_END);       break;
  case GLFW_KEY_PAGE_UP:    w->PushUTF32CharToQueue(UTF32_PAGE_UP);   break;
  case GLFW_KEY_PAGE_DOWN:  w->PushUTF32CharToQueue(UTF32_PAGE_DOWN); break;
  case GLFW_KEY_INSERT:     w->PushUTF32CharToQueue(UTF32_INSERT);    break;
  
  default:
    break;
  }

  if (action == GLFW_PRESS && mods & GLFW_MOD_CONTROL){
    switch (key)
    {
    case GLFW_KEY_C:  w->PushUTF32CharToQueue(UTF32_COPY);  break;
    case GLFW_KEY_V:  w->PushUTF32CharToQueue(UTF32_PASTE); break;
    case GLFW_KEY_X:  w->PushUTF32CharToQueue(UTF32_CUT);   break;
    
    default:
      break;
    }
  }
}

void KeyboardCharacterInputCallback(GLFWwindow* window, unsigned int codePoint){
  Render::GLWindow *w = Render::GLWindow::activeWindows[window];

  w->PushUTF32CharToQueue(static_cast<char32_t>(codePoint));
}

void CursorPositionCallback(GLFWwindow *window, double xpos, double ypos)
{
  //ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);

  Render::GLWindow *w = Render::GLWindow::activeWindows[window];
  Debug::Assert(w, "Window not found by GLFWwindow!");

  w->TriggerMouseMovementCallback({(float)xpos, (float)ypos});
}

void Render::GLWindow::SetCursorMode(CursorMode mode)
{
  switch (mode)
  {
  case CursorMode::Normal:  glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);    break;
  case CursorMode::Hidden:  glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);    break;
  case CursorMode::Trapped: glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  break;
  default:
    Debug::LogWarn("Invalid cursor mode set!");
    return;
    break;
  }
}

void Render::GLWindow::SetCursorShape(CursorShape shape) {
  uint32_t GLFWShape = GLFW_ARROW_CURSOR;
  switch (shape)
  {
  case CursorShape::Normal:           GLFWShape = GLFW_ARROW_CURSOR;      break;
  case CursorShape::Beam:             GLFWShape = GLFW_IBEAM_CURSOR;      break;
  case CursorShape::Crosshair:        GLFWShape = GLFW_CROSSHAIR_CURSOR;  break;
  case CursorShape::Hand:             GLFWShape = GLFW_HAND_CURSOR;       break;
  case CursorShape::HorizontalResize: GLFWShape = GLFW_HRESIZE_CURSOR;    break;
  case CursorShape::VerticalResize:   GLFWShape = GLFW_VRESIZE_CURSOR;    break;
  default:
    Debug::LogWarn("Invalid cursor mode set!");
    return;
    break;
  }

  if (this->cursor) glfwDestroyCursor(this->cursor);

  this->cursor = glfwCreateStandardCursor(GLFWShape);

  glfwSetCursor(this->window, this->cursor);
}

bool Render::GLWindow::LoadCharFromQueue(char32_t *c){
  if (!c) { // Clear queue
    this->UTF32CharQueue = std::queue<char32_t>();
    return false;
  }

  if (this->UTF32CharQueue.size() == 0){ // End Of Queue
    *c = 0x0000;
    return false;
  }

  *c = this->UTF32CharQueue.front();
  this->UTF32CharQueue.pop();
  return true;
}

Render::GLWindow::~GLWindow() {
  if(this->window != nullptr) {
      GLWindow::activeWindows.erase(this->window);
      glfwDestroyWindow(this->window);
      this->window = nullptr;
  }
}

Render::GLWindow::GLWindow(WindowMode wm, Vec2i preferredWindowSize) {
  Debug::LogTrace("Begining window creation");
  switch (wm) {
  case WindowMode::Windowed: {
    Debug::LogSpam("Windowed window");
    this->window = glfwCreateWindow(preferredWindowSize.x, preferredWindowSize.y, PROJECT_NAME, nullptr, nullptr);
    this->MakeContext();
    this->viewport = preferredWindowSize;
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

    this->viewport = preferredWindowSize;

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
  glfwSetCharCallback(this->window, KeyboardCharacterInputCallback);
  glfwSetKeyCallback(this->window, KeyboardInputCallback);
  glfwSetCursorPosCallback(this->window, CursorPositionCallback);

  Debug::Assert(this->window, "Window failed to create!");
}

void Render::GLWindow::MakeContext() {
  glfwMakeContextCurrent(this->window);
}

void Render::GLWindow::SwapBuffers() {
  // Clear input queue if not used
  this->LoadCharFromQueue(nullptr);

  glfwPollEvents();

  glfwSwapBuffers(window);
}

bool Render::GLWindow::SetTitle(std::string &name) {
  if (!this->window) [[unlikely]] {
    Debug::LogWarn("Trying to set the name of invalid window");
    return false;
  }

  glfwSetWindowTitle(this->window, name.c_str());

  return !CheckGLErrors();
}

void Render::GLWindow::SetMouseMovementCallback(void(* mMCallback)(Vec2f mousePos))
{
  this->mouseMovementCallback = mMCallback;
}

bool Render::GLWindow::ShouldClose() {
  if(!this->window) [[unlikely]] {
    Debug::LogWarn("OpenGL window has an empty pointer for some reason");
    return true;
  }

  return glfwWindowShouldClose(this->window);
}
