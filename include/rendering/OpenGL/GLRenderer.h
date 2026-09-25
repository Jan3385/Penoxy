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