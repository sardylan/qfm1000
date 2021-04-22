/*
 * qFM1000
 * Copyright (C) 2021  Luca Cireddu - IS0GVH
 * sardylan@gmail.com - is0gvh@gmail.com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


#include <QtCore/QtCore>
#include <QtCore/QtMath>

#include "values.hpp"

using namespace qfm1000::eeprom;

Frequency Values::parseFrequencyString(const QString &frequencyString) {
    if (frequencyString.contains(QRegExp(R"(^\d{3}\.{0,1}$)"))) {
        QRegExp regExp(R"(^(\d{3})\.{0,1}$)");
        regExp.indexIn(frequencyString);
        return regExp.cap(1).leftJustified(3, '0').toUInt() * 1000000;
    } else if (frequencyString.contains(QRegExp(R"(^\d{3}\.{0,1}\d{0,3}$)"))) {
        QRegExp regExp(R"(^(\d{3})\.{0,1}(\d{0,3})$)");
        regExp.indexIn(frequencyString);
        return regExp.cap(1).toUInt() * 1000000
               + regExp.cap(2).leftJustified(3, '0').toUInt() * 1000;
    } else if (frequencyString.contains(QRegExp(R"(^\d{3}\.{0,1}\d{0,3}\.{0,1}\d{0,3}$)"))) {
        QRegExp regExp(R"(^(\d{3})\.{0,1}(\d{0,3})\.{0,1}(\d{0,3})$)");
        regExp.indexIn(frequencyString);
        return regExp.cap(1).toUInt() * 1000000
               + regExp.cap(2).leftJustified(3, '0').toUInt() * 1000
               + regExp.cap(3).leftJustified(3, '0').toUInt();
    }

    return 0u;
}

QString Values::frequency(Frequency frequency) {
    QString frequencyNumbers = QString("%1").arg(frequency, 9, 10, QLatin1Char('0'));
    QString MHz = frequencyNumbers.mid(0, 3);
    QString kHz = frequencyNumbers.mid(3, 3);
    QString hz = frequencyNumbers.mid(6, 3);

    QString frequencyString = QString("%1.%2").arg(MHz, kHz);
    if (hz.at(0) != '0')
        frequencyString.append(".").append(hz.midRef(0, 1));

    return frequencyString;
}

QString Values::frequencyBand(FrequencyBand frequencyBand) {
    switch (frequencyBand) {

        case FrequencyBand::E0:
            return "E0 (68-88 MHz)";

        case FrequencyBand::B0:
            return "B0 (132-156 MHz)";

        case FrequencyBand::A9:
            return "A9 (146-174 MHz)";

        case FrequencyBand::K1:
            return "K1 (174-208 MHz)";

        case FrequencyBand::K2:
            return "K2 (192-225 MHz)";

        case FrequencyBand::K8:
            return "K8 (225-235 MHz)";

        case FrequencyBand::K9:
            return "K9 (225-235 MHz)";

        case FrequencyBand::TD:
            return "TD (380-440 MHz)";

        case FrequencyBand::TM:
            return "TM (400-440 MHz)";

        case FrequencyBand::TZ:
            return "TZ (410-430 MHz)";

        case FrequencyBand::T4:
            return "T4 (425-450 MHz)";

        case FrequencyBand::U0:
            return "U0 (440-470 MHz)";

        case FrequencyBand::W1:
            return "W1 (470-500 MHz)";

        case FrequencyBand::W4:
            return "W4 (500-520 MHz)";

        default:
            return "";
    }
}

QString Values::flag(Flag flag) {
    switch (flag) {

        case Flag::ENABLED:
            return "enabled";

        case Flag::DISABLED:
            return "disabled";

        default:
            return "";
    }
}


QString Values::ctcss(CTCSS ctcss) {
    switch (ctcss) {

        case CTCSS::TONE_OFF:
            return "OFF";

        case CTCSS::TONE_67_0:
            return "67.0 Hz";

        case CTCSS::TONE_71_9:
            return "71.9 Hz";

        case CTCSS::TONE_74_4:
            return "74.4 Hz";

        case CTCSS::TONE_77_0:
            return "77.0 Hz";

        case CTCSS::TONE_79_7:
            return "79.7 Hz";

        case CTCSS::TONE_82_5:
            return "82.5 Hz";

        case CTCSS::TONE_85_4:
            return "85.4 Hz";

        case CTCSS::TONE_88_5:
            return "88.5 Hz";

        case CTCSS::TONE_91_5:
            return "91.5 Hz";

        case CTCSS::TONE_94_8:
            return "94.8 Hz";

        case CTCSS::TONE_97_4:
            return "97.4 Hz";

        case CTCSS::TONE_100_0:
            return "100.0 Hz";

        case CTCSS::TONE_103_5:
            return "103.5 Hz";

        case CTCSS::TONE_107_2:
            return "107.2 Hz";

        case CTCSS::TONE_110_9:
            return "110.9 Hz";

        case CTCSS::TONE_114_8:
            return "114.8 Hz";

        case CTCSS::TONE_118_8:
            return "118.8 Hz";

        case CTCSS::TONE_123_0:
            return "123.0 Hz";

        case CTCSS::TONE_127_3:
            return "127.3 Hz";

        case CTCSS::TONE_131_8:
            return "131.8 Hz";

        case CTCSS::TONE_136_5:
            return "136.5 Hz";

        case CTCSS::TONE_141_3:
            return "141.3 Hz";

        case CTCSS::TONE_146_2:
            return "146.2 Hz";

        case CTCSS::TONE_151_4:
            return "151.4 Hz";

        case CTCSS::TONE_156_7:
            return "156.7 Hz";

        case CTCSS::TONE_162_2:
            return "162.2 Hz";

        case CTCSS::TONE_167_9:
            return "167.9 Hz";

        case CTCSS::TONE_173_8:
            return "173.8 Hz";

        case CTCSS::TONE_179_9:
            return "179.9 Hz";

        case CTCSS::TONE_186_2:
            return "186.2 Hz";

        case CTCSS::TONE_192_8:
            return "192.8 Hz";

        case CTCSS::TONE_203_5:
            return "203.5 Hz";

        case CTCSS::TONE_210_7:
            return "210.7 Hz";

        case CTCSS::TONE_218_1:
            return "218.1 Hz";

        case CTCSS::TONE_225_7:
            return "225.7 Hz";

        case CTCSS::TONE_233_6:
            return "233.6 Hz";

        case CTCSS::TONE_241_8:
            return "241.8 Hz";

        case CTCSS::TONE_250_3:
            return "250.3 Hz";

        default:
            return "";
    }
}

QString Values::power(Power power) {
    switch (power) {

        case Power::DISABLED:
            return "disabled";

        case Power::WATT_1:
            return "1 W";

        case Power::WATT_6:
            return "6 W";

        case Power::WATT_10:
            return "10 W";

        case Power::WATT_15:
            return "15 W";

        case Power::WATT_25:
            return "25 W";

        default:
            return "";
    }
}

QString Values::squelch(Squelch squelch) {
    switch (squelch) {

        case Squelch::OPEN:
            return "Always open";

        case Squelch::SINAD_9DB:
            return "9dB SINAD";

        case Squelch::SINAD_12DB:
            return "12dB SINAD";

        case Squelch::SINAD_15DB:
            return "15dB SINAD";

        case Squelch::SINAD_18DB:
            return "18dB SINAD";

        case Squelch::SINAD_21DB:
            return "21dB SINAD";

        case Squelch::SINAD_24DB:
            return "24dB SINAD";

        default:
            return "";
    }
}

QString Values::shift(Frequency freqTx, Frequency freqRx) {
    qint64 shift = (qint64) freqTx - (qint64) freqRx;
    float khz = (float) shift / 1000;
    QString value = QString::number(khz, 'f', 1);

    if (freqTx > freqRx)
        value.insert(0, "+");

    return value;
}
