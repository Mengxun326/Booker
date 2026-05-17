# Contributing to Booker

Thank you for your interest in Booker!

Booker is a community-driven project, and contributions of all kinds are welcome — bug reports, feature ideas, documentation improvements, and code. That said, the project aims to remain coherent, modular, and maintainable. Please read this guide before contributing.

---

## Table of Contents

- [Code of Conduct](#code-of-conduct)
- [Before You Start](#before-you-start)
- [Project Structure](#project-structure)
- [Build Instructions (Development)](#build-instructions-development)
- [Code Style](#code-style)
- [Submitting a Contribution](#submitting-a-contribution)
- [Reporting Bugs](#reporting-bugs)
- [Suggesting Features](#suggesting-features)

---

## Code of Conduct

Be respectful. Constructive criticism is welcome; personal attacks are not.

---

## Before You Start

**New to the project?** Look for issues labeled [`good first issue`](https://github.com/DostLeFan/Booker/issues?q=is%3Aissue+is%3Aopen+label%3A%22good+first+issue%22) — they are scoped, self-contained, and a good way to get familiar with the codebase.

- For **small fixes** (typos, minor bugs, documentation): feel free to open a PR directly.
- For **larger features or architectural changes**: please open an issue or a discussion first. This avoids wasted effort if the direction doesn't fit the project.
- Check the existing issues and PRs to avoid duplicates.

**Keep the philosophy in mind:** Booker is designed to stay native, modular, extensible, and cross-platform. Contributions that add heavy dependencies, break the CLI/GUI separation, or tightly couple modules must be discussed in an issue beforehand.

---

## Project Structure

```
Booker/
├── core/          # Shared library: archive handling, document model, format handlers, and other logic features
│   ├── include/   # Public headers (booker namespace)
│   └── src/       # Implementations
├── cli/           # Command-line interface (uses core)
├── gui/           # GUI application (uses core) - work in progress, to do
├── locales/       # Localization files (future feature)
└── CMakeLists.txt # Root build configuration
```

**Where to look depending on what you want to do:**

| Goal | Where to look |
|---|---|
| Add or fix a format (CBZ, CBR, PDF…) | `core/include/booker/format/`, `core/src/booker/format/` |
| Add archive handling | `core/include/booker/archive/`, `core/src/booker/archive/` |
| Work on the document model | `core/include/booker/document/`, `core/src/booker/document/` |
| Only add features related to the business logic (i.e., not the CLI or GUI) | `core` |
| Improve the CLI | `cli` |
| Improve the GUI (future) | `gui` |

---

## Build Instructions (Development)

### Requirements

- C++17 compiler (GCC, Clang, MSVC, MinGW)
- CMake ≥ 3.24
- Git

> **Note:** The first build will automatically download and compile several dependencies from source (zlib, Poppler, libjpeg-turbo, libpng, FreeType, minizip, unarr, RarLib). This can take **10–15 minutes** depending on your machine. Subsequent builds are much faster.

### Linux

```bash
git clone https://github.com/DostLeFan/Booker.git
cd Booker
cmake --preset debug -G "Unix Makefiles"
cmake --build --preset debug
```

### macOS

```bash
git clone https://github.com/DostLeFan/Booker.git
cd Booker
cmake --preset debug
cmake --build --preset debug
```

### Windows (MSVC)

```bash
git clone https://github.com/DostLeFan/Booker.git
cd Booker
cmake --preset debug
cmake --build --preset debug
```

Compiled binaries are placed in `bin/debug/`.

---

## Code Style

Booker follows a few simple conventions. Please try to match the surrounding code style when in doubt.

- **Standard**: C++17. Do not use C++20 features.
- **Namespace**: all core code lives in the `booker` namespace.
- **Headers**: include guards (`#ifndef DEF_XXX` / `#define DEF_XXX` / `#endif`) are, for the moment, preferred to `#pragma once`.
- **Naming**:
  - Classes and structs: `PascalCase`
  - Methods and functions: `camelCase`
  - Member variables: `m_camelCase` (`m_` prefix is for "Member")
  - Constants and enums: `UPPER_CASE`
- **No `using namespace std`** in headers.
- **Error handling**: prefer exceptions (see `booker::Exception`) or std exceptions over return codes where appropriate.
- **Documentation**: public API methods in headers should have a Doxygen `\brief` comment at the very least.

---

## Submitting a Contribution

1. **Fork** the repository and create a branch from `main`:
   ```bash
   git checkout -b feature/my-feature
   ```

2. **Make your changes.** Keep commits focused and write clear commit messages.

3. **Test your changes manually** before submitting. Automated tests are not yet in place, but please verify that existing conversions (CBZ<->CBR<->PDF) still work correctly before submitting.

4. **Open a Pull Request** against `main`. In the PR description, explain:
   - What the change does
   - Why it is needed
   - How you tested it

5. Be ready for feedback. PRs may require changes before being merged.

---

## Reporting Bugs

Open an issue and include:

- Your OS and compiler version
- The exact command you ran
- The expected vs. actual behavior
- If possible, a minimal reproducer (e.g. a small CBZ/CBR/PDF file that triggers the issue)

---

## Suggesting Features

Open an issue with the `enhancement` label. Describe:

- The problem you are trying to solve
- Your proposed solution
- Any alternatives you considered

For large changes, a discussion is preferred over a direct issue.