#include "servicelocator.h"

ServiceLocator *ServiceLocator::_instance = nullptr;

ServiceLocator *ServiceLocator::GetInstance() {
  if (_instance == nullptr) {
    _instance = new ServiceLocator();
  }
  return _instance;
}
