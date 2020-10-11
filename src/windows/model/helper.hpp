/*
 * qFM1000
 * Copyright (C) 2017  Luca Cireddu
 * sardylan@gmail.com
 * http://www.lucacireddu.it
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation,
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not,
 *
 */

#ifndef __QFM1000_WINDOWS_MODEL_CTCSS_H
#define __QFM1000_WINDOWS_MODEL_CTCSS_H

#include <QtCore/QString>

const std::vector<QString> ctcssValues = {
        "OFF",
        "67.0",
        "71.9",
        "74.4",
        "77.0",
        "79.7",
        "82.5",
        "85.4",
        "88.5",
        "91.5",
        "94.8",
        "97.4",
        "100.0",
        "103.5",
        "107.2",
        "110.9",
        "114.8",
        "118.8",
        "123.0",
        "127.3",
        "131.8",
        "136.5",
        "141.3",
        "146.2",
        "151.4",
        "156.7",
        "162.2",
        "167.9",
        "173.8",
        "179.9",
        "186.2",
        "192.8",
        "203.5",
        "210.7",
        "218.1",
        "225.7",
        "233.6",
        "241.8",
        "250.3"
};

const std::vector<QString> powerValues = {
        "0 W",
        "1 W",
        "6 W",
        "10 W",
        "15 W",
        "25 W",
        "30 W"
};

const std::vector<QString> squelchValues = {
        "Always open",
        "9dB SINAD",
        "12dB SINAD",
        "15dB SINAD",
        "18dB SINAD",
        "21dB SINAD",
        "24dB SINAD"
};

const std::vector<QString> frequencyBandValues = {
        "E0 (68-88 MHz)",
        "B0 (132-156 MHz)",
        "A9 (146-174 MHz)",
        "K1 (174-208 MHz)",
        "K2 (192-225 MHz)",
        "K8 (225-235 MHz)",
        "K9 (225-235 MHz)",
        "TD (380-440 MHz)",
        "TM (400-440 MHz)",
        "TZ (410-430 MHz)",
        "T4 (425-450 MHz)",
        "U0 (440-470 MHz)",
        "W1 (470-500 MHz)",
        "W4 (500-520 MHz)"
};

#endif
