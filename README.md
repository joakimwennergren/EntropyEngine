<a id="readme-top"></a>

<!-- PROJECT LOGO -->
<br />
<div align="center">
  <a href="https://github.com/joakimwennergren/EntropyEngine">
    <img src="https://github.com/joakimwennergren/EntropyEngine/blob/main/.github/images/entropy.png" alt="Logo" width="240" height="240">
  </a>
<h3>Entropy Engine</h3>

![GitHub Actions Workflow Status](https://img.shields.io/github/actions/workflow/status/joakimwennergren/EntropyEngine/cmake-multi-platform.yml?branch=main&style=for-the-badge)
[![MIT](https://img.shields.io/badge/license-MIT-blue.svg?style=for-the-badge)](https://github.com/joakimwennergren/EntropyEngine/blob/main/LICENSE.txt)
</div>

<!-- ABOUT THE PROJECT -->

## About The Project

Entropy Engine is a custom game engine built from the ground up with performance and modularity in mind. It leverages the Vulkan graphics API for high-efficiency rendering and uses the Flecs Entity Component System (ECS) to enable flexible and scalable game architecture.

The engine is written primarily in C++, and designed for developers who want low-level control over systems while still benefiting from modern scripting workflows.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Getting Started

### Prerequisites
* A C++17-compatible compiler (e.g. Clang or GCC)
* CMake (version 3.16 or higher recommended)
* Git
* Vulkan SDK 1.3.275.0

#### Clone the Repository
```console
git clone https://github.com/joakimwennergren/EntropyEngine.git
cd EntropyEngine
```
#### Generate Build Files
```console
mkdir build
cd build
cmake .. 
```

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- USAGE EXAMPLES -->

## Usage

### Running Tests
```console
cd build
ctest
```

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- CONTRIBUTING -->

## Contributing

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- LICENSE -->

## License

Distributed under the MIT License. See `LICENSE.txt` for more information.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- CONTACT -->

## Contact

Joakim Wennergren - joakim.wennergren@databeams.se

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- ACKNOWLEDGMENTS -->

## Acknowledgments

Useful resources while creating this project.

* [Vulkan Tutorial](https://vulkan-tutorial.com/)
* [Vulkan Examples](https://github.com/SaschaWillems/Vulkan)

<p align="right">(<a href="#readme-top">back to top</a>)</p>
