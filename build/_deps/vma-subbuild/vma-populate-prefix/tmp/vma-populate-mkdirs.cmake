# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/joakim-wennergren/CLionProjects/EntropyEngine/build/_deps/vma-src"
  "/home/joakim-wennergren/CLionProjects/EntropyEngine/build/_deps/vma-build"
  "/home/joakim-wennergren/CLionProjects/EntropyEngine/build/_deps/vma-subbuild/vma-populate-prefix"
  "/home/joakim-wennergren/CLionProjects/EntropyEngine/build/_deps/vma-subbuild/vma-populate-prefix/tmp"
  "/home/joakim-wennergren/CLionProjects/EntropyEngine/build/_deps/vma-subbuild/vma-populate-prefix/src/vma-populate-stamp"
  "/home/joakim-wennergren/CLionProjects/EntropyEngine/build/_deps/vma-subbuild/vma-populate-prefix/src"
  "/home/joakim-wennergren/CLionProjects/EntropyEngine/build/_deps/vma-subbuild/vma-populate-prefix/src/vma-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/joakim-wennergren/CLionProjects/EntropyEngine/build/_deps/vma-subbuild/vma-populate-prefix/src/vma-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/joakim-wennergren/CLionProjects/EntropyEngine/build/_deps/vma-subbuild/vma-populate-prefix/src/vma-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
