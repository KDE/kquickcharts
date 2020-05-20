#!/bin/bash

glslang=$(PATH=./glslang/bin:$PATH which glslangValidator 2> /dev/null)
result=0

shaderdir=$1

if [ -z $glslang ]; then
    echo "Error: glslangValidator not found, cannot validate shaders!"
    exit 1
fi

function print_error
{
    echo "While validating $1:"
    echo "$2"
    result=1
}

function validate_vert
{
    output=$(cat "$shaderdir/header_desktop.glsl" "$shaderdir/$1" | $glslang --stdin -S vert -l -DVALIDATING)
    if [ $? -ne 0 ]; then print_error "$1" "$output"; fi
    output=$(cat "$shaderdir/header_desktop_core.glsl" "$shaderdir/$1" | $glslang --stdin -S vert -l -DVALIDATING)
    if [ $? -ne 0 ]; then print_error "$1" "$output"; fi
    output=$(cat "$shaderdir/header_es2.glsl" "$shaderdir/$1" | $glslang --stdin -S vert -l -DVALIDATING)
    if [ $? -ne 0 ]; then print_error "$1" "$output"; fi
    output=$(cat "$shaderdir/header_es3.glsl" "$shaderdir/$1" | $glslang --stdin -S vert -l -DVALIDATING)
    if [ $? -ne 0 ]; then print_error "$1" "$output"; fi
}

function validate_frag
{
    output=$(cat "$shaderdir/header_desktop.glsl" "$shaderdir/sdf.glsl" "$shaderdir/$1" | $glslang --stdin -S frag -l -DVALIDATING)
    if [ $? -ne 0 ]; then print_error "$1" "$output"; fi
    output=$(cat "$shaderdir/header_desktop_core.glsl" "$shaderdir/sdf.glsl" "$shaderdir/$1" | $glslang --stdin -S frag -l -DVALIDATING)
    if [ $? -ne 0 ]; then print_error "$1" "$output"; fi
    output=$(cat "$shaderdir/header_es2.glsl" "$shaderdir/sdf.glsl" "$shaderdir/$1" | $glslang --stdin -S frag -l -DVALIDATING)
    if [ $? -ne 0 ]; then print_error "$1" "$output"; fi
    output=$(cat "$shaderdir/header_es3.glsl" "$shaderdir/sdf.glsl" "$shaderdir/$1" | $glslang --stdin -S frag -l -DVALIDATING)
    if [ $? -ne 0 ]; then print_error "$1" "$output"; fi
}

validate_vert "piechart.vert"
validate_frag "piechart.frag"
validate_vert "linechart.vert"
validate_frag "linechart.frag"

if [ $result -eq 0 ]; then
    echo "Successfully validated shaders, no errors found."
fi
exit $result
