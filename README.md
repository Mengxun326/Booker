# Booker

Booker is a native and extensible toolkit for ebooks and comic archives written in C++17.

It focuses on conversion, processing and future lightweight library management, while keeping both CLI and GUI as first-class citizens.

[![Development](https://img.shields.io/badge/development-early-orange)
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![Release](https://img.shields.io/github/v/release/DostLeFan/Booker?include_prereleases&label=version)](https://github.com/DostLeFan/Booker/releases)
[![CI Status](https://github.com/DostLeFan/Booker/actions/workflows/release.yml/badge.svg)](https://github.com/DostLeFan/Booker/actions/workflows/release.yml)
[![CI](https://github.com/DostLeFan/Booker/actions/workflows/PR.yml/badge.svg)](https://github.com/DostLeFan/Booker/actions/workflows/PR.yml)
[![Language](https://img.shields.io/github/languages/top/DostLeFan/Booker?color=00599C&logo=c%2B%2B)](https://github.com/DostLeFan/Booker)
[![CMake](https://img.shields.io/badge/build-CMake-064F8C?logo=cmake&logoColor=white)](https://cmake.org/)
[![C++17](https://img.shields.io/badge/standard-C%2B%2B17-blue?logo=c%2B%2B)](https://isocpp.org/)

# Table of Contents

* [Philosophy](#philosophy)
* [Current Status](#current-status)
* [Features](#features)
	* [Current](#current)
	* [Planned](#planned)
* [Dependencies](#dependencies)
* [Architecture](#architecture)
* [Build](#build)
	* [About build](#about-build)
	* [Requirements](#requirements)
	* [Cross-platform build commands](#cross-platform-build-commands)
* [Roadmap](#roadmap)
	* [Core](#core)
	* [Conversion](#conversion)
	* [Ecosystem](#ecosystem)
* [Contributing](#contributing)
* [License](#license)

# Philosophy

Booker is built around a simple idea:

ebook tools should adapt to users, not the opposite.

The project started from real-world needs: converting and processing PDF, CBZ and CBR files quickly and cleanly.

Over time, Booker aims to evolve into a modular ecosystem for ebook and comic workflows, including lightweight library management and community-driven extensions.

Booker is designed to remain:
- native
- modular
- extensible
- cross-platform
- usable from both CLI and GUI

# Current Status

Booker is currently in early development.

The current focus is:
- archive handling
- conversion pipelines
- core architecture
- CLI/GUI foundations

Supported formats and functionalities are still evolving.

# Features

## Current
- CBZ support
- CBR support
- PDF support
- Archive abstraction layer
- Shared core between CLI and GUI

## Planned
- EPUB support
- Metadata handling
- Batch processing
- Lightweight library management
- Future plugin system
- GUI (library is not decided)

# Dependencies

Dependencies are managed by CMake, with `FetchContent` and `ExternalProject`, so, if you are on Windows, MacOS, or Linux (Ubuntu is OK for build, not built for other distributions), you don't have to manage them.

But, there is a list of the dependencies used, and why :
- Zlib : Useful for some other dependencies, and for compress and deflate zip for CBZ archives
- libjpeg (and libjpeg-turbo) : Useful for Poppler and take in charge a part of JPEG workflows
- Freetype : Useful for Poppler
- LibPNG : Useful for Poppler
- Iconv (and win-iconv for Windows) : Useful for Poppler
- Poppler : To read PDF and rasterization of pages
- RarLib : My own wrapper around RAR executables ; to compress CBR archives
- unarr : To deflate CBR archives
- imageinfo : To get images width and height
- LibHaru : To write PDF
- stb_image and stb_image_write : To help libjpeg
- libwebp : To take in charge WEBP format
- CLI11 : To create a CLI interface

# Architecture

Booker is organized around a shared core architecture:

```
Booker/
├── core/          # Shared library: archive handling, document model, format handlers, and other logic features
│   ├── include/   # Public headers (booker namespace)
│   └── src/       # Implementations
├── cli/           # Command-line interface (uses core)
├── gui/           # GUI application (uses core)
├── locales/       # Localization files (future feature)
└── CMakeLists.txt # Root build configuration
```


# Build

## About build

CMake take in charge the dependencies listed above. So, the first build is very slow (around 10~15 minutes). Don't be afraid by that, it's normal.

## Requirements

- C++17 compiler (MSVC, MinGW, GCC, Clang)
- CMake
- Git

## Cross-platform build commands

```bash
git clone https://github.com/DostLeFan/Booker.git
cd Booker

# And then, use these two commands :
# The two presets are build and release
cmake --preset YOUR_PRESET -G "YOUR_GENERATOR"
cmake --build --preset YOUR_PRESET

# For example, on Linux
cmake --preset debug -G "Unix Makefiles"
cmake --build --preset debug
```

# Roadmap

There is a minimal roadmap for future developments.

## Core
- [x] ZIP handling
- [x] RAR handling
- [ ] Metadata abstraction

## Conversion
- [X] CBZ -> CBR
- [X] CBZ -> PDF
- [X] CBR -> CBZ
- [X] CBR -> PDF
- [X] PDF -> CBZ
- [X] PDF -> CBR
- [ ] Batch conversion

## Ecosystem
- [ ] Plugin system
- [ ] Lightweight library management

# Contributing

Booker is community-driven.

Feature ideas, discussions and contributions are welcome, but the project aims to remain coherent, modular and maintainable.

Before implementing large features, please open an issue or discussion first.

# License

Copyright (C) 2026 DostLeFan. This software is distributed under the terms of the GNU General Public License v3 or later.