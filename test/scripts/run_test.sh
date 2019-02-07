#!/bin/bash

TEST=../build/test

$TEST -v ../../res/simple_vertex_shader.glsl -f ../../res/simple_frag_shader.glsl
