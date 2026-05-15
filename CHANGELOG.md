# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

## [0.0.9] - 2026-05-15

### Added

- io directory in core
- TempDirectory class to manage a temporary directory
- DirectoryUtils files to get utilities functions
- Document sub-module in core, to handle very basic and primitive ebook
- Format sub-module in core, to handle very basic and primitive ebook
- Set up Doxygen documentation
- ImageUtils and StringUtils
- CbzHandler and CbrHandler
- PdfHandler
- booker namespace
- Some other libs
- A mini CLI to test conversions
- CBZ -> PDF is tested, seems OK

### Changed

- README.md is now written in basic first version (yay !)

## [0.0.8] - 2026-05-11

### Added

- A file "ArchiveUtils", to contain all utilities relative to archives
- A function to detect file format (zip, rar4, rar5 and pdf)

### Fixed

- Report "EntryInfo" declaration into "ArchiveUtils"

## [0.0.7] - 2026-05-11

### Added

- Rar wrapper (reader) in C++ around unarr
- Rar writer exporting Rar class of RarLib

## [0.0.6] - 2026-05-11

### Added

- minizip dependency
- RarLib dependency for RAR compression
- unarr dependency for RAR decompression
- Zip wrapper in C++ around minizip

## [0.0.5] - 2026-05-10

### Fixed

- CI workflow take in charge two generated binaries
- Fixed this changelog again

## [0.0.4] - 2026-05-10

### Changed

- Splitted code into core, cli and gui for future development

## [v0.0.3] - 2026-05-10

### Fixed

- Fixed MacOS compilation

## [0.0.2] - 2026-05-10

### Added

- Added "locales" directory - for a future functionnality

### Fixed

- Fixed this changelog file - I forgot a lot of things, duh

## [0.0.1] - 2026-05-10

### Fixed

- Fix project name in CI
- Fix project name in CMakeLists.txt
- Fix compilation workflow on Linux and MacOS

## [0.0.0] - 2026-05-10

### Added

- Basic initialisation of repository
- Basic function, converting each file of a PDF into JPG, to test GitHub CI configuration - this function will be removed

[unreleased]: https://github.com/DostLeFan/Booker/compare/v0.0.9...HEAD
[0.0.9]: https://github.com/DostLeFan/Booker/compare/v0.0.8...v0.0.9
[0.0.8]: https://github.com/DostLeFan/Booker/compare/v0.0.7...v0.0.8
[0.0.7]: https://github.com/DostLeFan/Booker/compare/v0.0.6...v0.0.7
[0.0.6]: https://github.com/DostLeFan/Booker/compare/v0.0.5...v0.0.6
[0.0.5]: https://github.com/DostLeFan/Booker/compare/v0.0.4...v0.0.5
[0.0.4]: https://github.com/DostLeFan/Booker/compare/v0.0.3...v0.0.4
[0.0.3]: https://github.com/DostLeFan/Booker/compare/v0.0.2...v0.0.3
[0.0.2]: https://github.com/DostLeFan/Booker/compare/v0.0.1...v0.0.2
[0.0.1]: https://github.com/DostLeFan/Booker/compare/v0.0.0...v0.0.1
[0.0.0]: https://github.com/DostLeFan/Booker/releases/tag/v0.0.0