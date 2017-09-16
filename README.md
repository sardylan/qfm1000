# qFm1000
> Simple tool for editing channels and configuration parameters of Philips FM1000 radio.

[![Build Status Travis (Linux)](https://travis-ci.org/sardylan/qfm1000.svg?branch=master)](https://travis-ci.org/sardylan/qfm1000)
[![Build status AppVeyor (Windows)](https://ci.appveyor.com/api/projects/status/hysni857ux6k3wyg?svg=true)](https://ci.appveyor.com/project/sardylan/qfm1000)

qFm1000 is a simple tool for editing channels and configuration parameters saved into the 24C16 EEPROM mounted inside
any Philips FM1000 radio.

It's based on the Qt/C++ cross-platform framework and can be built using cmake in Linux, Mac and Windows with gcc,
clang or Visual Studio.

![qFm1000](doc/img/window_main_00.png)

## Releases

### Linux

There is no pre-compiled version for Linux. Still working in a DEB release for Debian/Ubuntu distros.

### MAC

There is no pre-compiled version for MAC. Still working to produce a official DMG release.

### Windows

Windows build are created using the official releases of Qt 5.9.0 for VS 2017 (64bit only) and
Visual Studio Community 2017 edition on Windows 10 Professional.

Windows users must install **Visual C++ Redistributable for Visual Studio 2017** of the same architecture
to use this software. The correct installation package can be installed using the one which is already
shipped with ZIP release (English language), or can be downloaded from the official Microsoft website.

## Changelog

* 0.2.0
    * ADD: Simple error messages for invalid files
    * ADD: Tests for EEPROM methods
    * ADD: Travis builds
    * ADD: AppVeyor builds
    * FIX: EEPROM flags bitwise
* 0.1.3
    * ADD: "Save as" feature
* 0.1.0
    * CHANGE: New interface
    * ADD: TOT feature
* 0.0.0
    * Initial commit (import from qCw)

## Contributing

1. [Fork it](<https://github.com/sardylan/qfm1000/fork>).
2. Create your feature branch (`git checkout -b feature_foo`)
3. Commit your changes (`git commit -am "Add some foo"`)
4. Push to the branch in your repository (`git push origin feature_foo`)
5. Create a new Pull Request

## Development environment

The whole project is based on CMake and developed under GNU/Linux Ubuntu 17.04 using JetBrains CLion IDE, but can be
developed using any IDE, included [VIm](https://en.wikipedia.org/wiki/Vim_(text_editor)) or [Notepad](https://en.wikipedia.org/wiki/Microsoft_Notepad)

Please have a look to [BUILD.md](BUILD.md) for installing needed tools and libraries.

## Meta

Luca Cireddu - [@sardylan](https://twitter.com/sardylan) - sardylan@gmail.com

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public
License as published by the Free Software Foundation, either version 3 of the License.

See [LICENSE.md](LICENSE.md) for more information.
