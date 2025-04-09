#include "orthographic_camera.h"

using namespace Entropy::Cameras;

OrthographicCamera::OrthographicCamera() {
  cameraPosition = glm::vec3(0.0f, 0.0f, 1024.0f);
  view = lookAt(glm::vec3(0.0f, 0.0f, 1024.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f));
}

void OrthographicCamera::Zoom(const float delta) {
  zoomFactor += delta;
  if (zoomFactor < 0.1f)
    zoomFactor = 0.1f;
}

void OrthographicCamera::Pan(const float deltaX, const float deltaY) {
  cameraPosition.x = deltaX;
  cameraPosition.y = deltaY;
}

void OrthographicCamera::SetPerspective(const uint32_t width,
                                        const uint32_t height,
                                        const float z_near, const float z_far) {
  projection =
      glm::ortho(cameraPosition.x,
                 static_cast<float>(width) / zoomFactor + cameraPosition.x,
                 static_cast<float>(height) / zoomFactor + cameraPosition.y,
                 cameraPosition.y, z_near, z_far);
}