#pragma once

//TODO: move Vec2 definition
struct Vec2 {
  Vec2(int x, int y) : x(x), y(y) {};
  int x;
  int y;
};


namespace Render {
  class IRenderer {
  public:
    virtual ~IRenderer() { };
    virtual void Render() = 0;
    
    virtual bool ShouldClose() = 0;

    virtual void MoveViewBy(Vec2 deltaPos) { 
      this->view.x += deltaPos.x; 
      this->view.y += deltaPos.y;
    };
    virtual Vec2 GetViewPos() { return this->view; };
    virtual void SetViewportSize(Vec2 size) = 0;
  protected:
    Vec2 view{0, 0};
  };
};