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

namespace Render {
  class IRenderer {
  public:
    virtual ~IRenderer() { };
    virtual void Render() = 0;
    
    virtual IWindow* GetWindow() = 0;
    
    virtual void SetVSYNC(bool enable) = 0;

    virtual bool ShouldClose() = 0;
  protected:
  };
};