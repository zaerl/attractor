# Changelog

All notable changes to Attractor are documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added

### Changed

### Fixed
- Fixed enumeration assertions not compiling with MSVC in C

## [1.3.0] - 2026-11-16

### Added
- Added support for C++ same enumeration assertions

### Changed

### Fixed

## [1.2.0] - 2026-11-13

### Added

### Changed
- Removed `_POSIX_C_SOURCE` declaration
- Updated the README.md file

### Fixed
- Fall back to `timespec_get` when `clock_gettime` is hidden

## [1.1.0] - 2026-11-13

### Added
- Accept NULL in C++ assertions on 64-bit MinGW
- Added GitHub CI

### Changed
- Cast double epsilon values to long double explicitly
- Moved timing to Attractor
- Changed the "NO" word to "FAIL"

### Fixed
- Fixed header strict warnings
- Fixed printf format checks and long double output on MinGW

## [1.0.0] - 2026-11-13

### Added
- C and C++ assertions
