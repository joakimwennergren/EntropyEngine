#!/bin/bash

# Directory where GLSL shaders reside
shaders_dir="."

# Array with shaders to compile
shaders_to_compile=("2dshader")

for shader in "${shaders_to_compile[@]}"; do

    if [ -f "$shaders_dir/$shader/$shader.vert" ]; then
        # Compile combined vertex shader
        /Users/joakimwennergren/VulkanSDK/1.4.309.0/macOS/bin/glslc "$shaders_dir/$shader/$shader.vert" -o "$shaders_dir/compiled/$shader/${shader}_vert.spv"
    fi

    if [ -f "$shaders_dir/$shader/$shader.frag" ]; then
        # Compile combined fragment shader
        /Users/joakimwennergren/VulkanSDK/1.4.309.0/macOS/bin/glslc "$shaders_dir/$shader/$shader.frag" -o "$shaders_dir/compiled/$shader/${shader}_frag.spv"
    fi

done

echo "done"