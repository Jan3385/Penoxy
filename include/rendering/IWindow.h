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
struct Vec2 {
  Vec2() : x(0), y(0) {};
  Vec2(int x, int y) : x(x), y(y) {};
  int x;
  int y;
};

namespace Render {
enum class WindowMode {
  Windowed,
  Borderless,
  Fullscreen,
};
class IWindow {
public:
  virtual ~IWindow() { };   

  virtual bool SetTitle(std::string &name) = 0;
  
  virtual Vec2 GetViewport() { return this->viewport; };
protected:
  Vec2 viewport{0, 0};
};
};