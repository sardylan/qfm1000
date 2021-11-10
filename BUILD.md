# Building from sources

This guide describes step-to-step how to build qFM1000 source code using different Operating Systems and Distributions.

## Installing dependencies

In order to build the source code you have to install some tools and libraries into your system.

### GNU/Linux Debian-based (Debian, Ubuntu and derivates)

All commands which start with `sudo` require to be executed as **root** user.
In Ubuntu, the standard way is to issue them using **sudo**.
If your system doesn't provied that feature or your user is not in the *sudoers* group (sudo-enabled users)
you can run these commands directly logging-in as root.

First of all, ensure to have latest versions (both repositories and installed software):

```bash
sudo apt-get update
sudo apt-get dist-upgrade
```

After that, you have to install tools and packages for getting and building the source code:

```bash
sudo apt-get install git build-essential pkg-config cmake
```

Now you have to install Qt dependencies. You can build them by yourself or you can install a pre-compiled
version taken from the official repositories of your distribution.

Development is always focused on the last library version, though you can use any version of Qt 5.
 
On both **Ubuntu 20.04 *(Focal Fossa)*** and **Debian 10 *(Buster)*** you can add them using this command:

```bash
sudo apt-get install qt5-default libqt5serialport5-dev libqt5svg5-dev
```

After installing all the necessary dependencies, to obtain the source code you can clone the project from Github:

```bash
mkdir ~/git
cd ~/git
git clone https://github.com/sardylan/qfm1000.git
```

Create a separate build directory and move into:

```bash
mkdir ~/git/qfm1000-build
cd ~/git/qfm1000-build
```

Prepare project to be compiled:

```bash
cmake ~/git/qfm1000
```

### Windows

**TO-DO**

### MacOS

**TO-DO**

## Compiling software

Now you can compile to software:

```bash
cmake --build . --parallel 
```

The compiled binary to run can be found in `src/app/qfm1000`, inside the build directory.

## Run tests

Use this command if you want to run available tests:

```bash
ctest --output-on-failure
```

## Create packages

Packages are created using cmake CPack feature. To create a package, run this command:

```bash
cpack
```

The output will be the following:

- Linux: `.deb` package
- MacOS: `.app` folder, as software bundle
- Windows: `.msi` installer, using WiX Toolset
