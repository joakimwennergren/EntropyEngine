# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/joakim-wennergren/CLionProjects/EntropyEngine/build/_deps/quill-src"
  "/home/joakim-wennergren/CLionProjects/EntropyEngine/build/_deps/quill-build"
  "/home/joakim-wennergren/CLionProjects/EntropyEngine/build/_deps/quill-subbuild/quill-populate-prefix"
  "/home/joakim-wennergren/CLionProjects/EntropyEngine/build/_deps/quill-subbuild/quill-populate-prefix/tmp"
  "/home/joakim-wennergren/CLionProjects/EntropyEngine/build/_deps/quill-subbuild/quill-populate-prefix/src/quill-populate-stamp"
  "/home/joakim-wennergren/CLionProjects/EntropyEngine/build/_deps/quill-subbuild/quill-populate-prefix/src"
  "/home/joakim-wennergren/CLionProjects/EntropyEngine/build/_deps/quill-subbuild/quill-populate-prefix/src/quill-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/joakim-wennergren/CLionProjects/EntropyEngine/build/_deps/quill-subbuild/quill-populate-prefix/src/quill-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/joakim-wennergren/CLionProjects/EntropyEngine/build/_deps/quill-subbuild/quill-populate-prefix/src/quill-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
