#!/usr/bin/env bash

# This script is intended to be run from an Ubuntu Docker image (or other Debian-derived distros).

# build with:
cmake -S . -B "./build"
cmake --build "./build"