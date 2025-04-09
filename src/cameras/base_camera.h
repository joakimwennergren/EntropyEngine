// Copyright 2025 Joakim Wennergren
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#ifndef ENTROPY_CAMERAS_BASE_H
#define ENTROPY_CAMERAS_BASE_H

#include <glm/glm.hpp>

namespace Entropy::Cameras {
  struct BaseCamera {
    virtual ~BaseCamera() = default;
    virtual void Zoom(const float delta) = 0;
    virtual void Pan(const float deltaX, const float deltaY) = 0;
    virtual void SetPerspective(uint32_t width, uint32_t height, float znear, float zfar) = 0;
    glm::vec3 cameraPosition{};
    float zoomFactor = 1.0f;
    glm::mat4 projection;
    glm::mat4 view;
  };
} // namespace Entropy::Cameras

#endif // ENTROPY_CAMERAS_BASE_H