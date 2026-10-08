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

#include <string>

//TODO: move Vec2 definition
struct Vec2i {
  Vec2i() : x(0), y(0) {};
  Vec2i(int x, int y) : x(x), y(y) {};
  int x;
  int y;
};
struct Vec2f {
  Vec2f() : x(0), y(0) {};
  Vec2f(float x, float y) : x(x), y(y) {};
  float x;
  float y;
};

namespace Render {
enum class WindowMode {
  Windowed,
  Borderless,
  Fullscreen,
};
enum class CursorMode{
    Normal,
    Hidden,
    Trapped
};

#define UTF32_BACKSPACE 8
#define UTF32_TAB       9
#define UTF32_ENTER     10
#define UTF32_ESCAPE    27
#define UTF32_DELETE    127
#define UTF32_LEFT      0x110000 + 1
#define UTF32_RIGHT     0x110000 + 2
#define UTF32_UP        0x110000 + 3
#define UTF32_DOWN      0x110000 + 4
#define UTF32_HOME      0x110000 + 5
#define UTF32_END       0x110000 + 6
#define UTF32_PAGE_UP   0x110000 + 7
#define UTF32_PAGE_DOWN 0x110000 + 8
#define UTF32_INSERT    0x110000 + 9
// Special character for ctrl+c
#define UTF32_COPY      0x110000 + 10
// Special character for ctrl+v
#define UTF32_PASTE     0x110000 + 11
// Special character for ctrl+x
#define UTF32_CUT       0x110000 + 12

class IWindow {
public:
  virtual ~IWindow() { };   

  virtual bool SetTitle(std::string &name) = 0;
  virtual void SetCursorMode(CursorMode mode) = 0;

  /// @brief Loads one char at a time from the internal window keyboard even queue in order
  /// @param c Output parameter. `0x0000` when end of queue. If `c` is nullptr entire queue gets cleared
  /// @return `true` if loaded successfully, `false` if load failed or end of queue
  virtual bool LoadCharFromQueue(char32_t *c) = 0;

  virtual void SetMouseMovementCallback(void (*mMCallback)(Vec2f mousePos)) = 0;
  
  virtual Vec2i GetViewport() { return this->viewport; };
protected:
  void (*mouseMovementCallback)(Vec2f mousePos) = nullptr;
  Vec2i viewport{0, 0};
};
};

inline constexpr bool IsSpecialUTF32Char(char32_t c) {
  if (c == UTF32_BACKSPACE)  return true; 
  if (c == UTF32_TAB)        return true; 
  if (c == UTF32_ENTER)      return true; 
  if (c == UTF32_ESCAPE)     return true; 
  if (c == UTF32_DELETE)     return true; 

  if (c >= UTF32_LEFT && c <= UTF32_CUT) return true;

  return false;
}