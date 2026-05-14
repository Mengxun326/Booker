# Booker

Booker is a native and extensible toolkit for ebooks and comic archives written in C++17.

It focuses on conversion, processing and future lightweight library management, while keeping both CLI and GUI as first-class citizens.

# Table of Contents

* [Philosophy](#philosophy)
* [Current Status](#current-status)
* [Features](#features)
	* [Current](#current)
	* [Planned](#planned)
* [Architecture](#architecture)
* [Build](#build)
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

# Architecture

Booker is organized around a shared core architecture:

core/
├── archive handling
├── utilities
cli/
gui/

# Build

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
- [ ] CBZ -> CBR
- [ ] CBZ -> PDF
- [ ] CBR -> CBZ
- [ ] CBR -> PDF
- [ ] PDF -> CBZ
- [ ] PDF -> CBR

## Ecosystem
- [ ] Plugin system
- [ ] Lightweight library management

# Contributing

Booker is community-driven.

Feature ideas, discussions and contributions are welcome, but the project aims to remain coherent, modular and maintainable.

Before implementing large features, please open an issue or discussion first.

# License

Copyright (C) 2026 DostLeFan. This software is distributed under the terms of the GNU General Public License v3 or later.