#!/bin/bash

function render() {
    INPUT_FILE="application.svg"
    RESOLUTION="${1}"
    OUTPUT_FILE="${2}"

    RES_WIDTH="$(echo "${RESOLUTION}" | awk -F'x' '{print $1}')"
    RES_HEIGHT="$(echo "${RESOLUTION}" | awk -F'x' '{print $2}')"

    inkscape \
        --without-gui \
        --export-png="${OUTPUT_FILE}" \
        --export-width="${RES_WIDTH}" \
        --export-height="${RES_HEIGHT}" \
        ${INPUT_FILE}
}

echo
echo " ### Creating icons"

render 1024x1024 icon-1024.png
render 512x512 icon-512.png
render 256x256 icon-256.png
render 128x128 icon-128.png
render 48x48 icon-48.png
render 32x32 icon-32.png
render 16x16 icon-16.png

echo
echo " ### Assembling icons for MacOS"

mkdir -p macos

png2icns \
    macos/icon.icns \
    icon-1024.png \
    icon-512.png \
    icon-256.png \
    icon-128.png \
    icon-48.png \
    icon-32.png \
    icon-16.png

echo
echo " ### Converting icons for Windows"

mkdir -p windows

echo "icon-256.ico"
icotool --create --output=windows/icon-256.ico icon-256.png
echo "icon-48.ico"
icotool --create --output=windows/icon-48.ico icon-48.png
echo "icon-32.ico"
icotool --create --output=windows/icon-32.ico icon-32.png
echo "icon-16.ico"
icotool --create --output=windows/icon-16.ico icon-16.png

echo
echo " ### Icons created"
