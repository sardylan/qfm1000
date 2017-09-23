# Building from sources

This guide describes step-to-step how to build qFM1000 source code using different Operating Systems and Distributions.

## Installing dependencies

In order to build the source code you have to install some tools and libraries into your system.

### GNU/Linux Debian-based (Debian, Ubuntu and derivates)

All commands which start with `sudo` require to be executed as **root** user. In Ubuntu, the standard way is to issue them using **sudo**. If your system doesn't provied that feature or your user is not in the *sudoers* group (sudo-enabled users) you can run these commands directly logging-in as root.

First of all, ensure to have latest versions (both repositories and installed software):

```bash
sudo apt-get update
sudo apt-get dist-upgrade
```

After that, you have to install tools and packages for getting and building the source code:

```bash
apt-get install git build-essential pkg-config cmake
```

Now you have to install Qt dependencies. You can build them by yourself or you can install a pre-compiled version taken from the official repositories of your distribution.

Development is always focused on the last library version, though you can use any version of Qt 5.
 
On both ***Ubuntu 16.04 (Xenial Xerus)*** and ***Debian 9 (Stretch)*** you can add them using this command:

```bash
apt-get install qt5-default libqt5serialport5-dev libqt5svg5-dev
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

Now you can compile to software:

```bash
make 
```

To speed-up the build you can add `-jn` option to make command, where `n` is the number of concurrent processes to run simultaneously, like this:

```bash
make -j8 
```

The compiled binary to run can be found in `src/qfm1000`, inside the build directory.
