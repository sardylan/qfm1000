/*
 * qFM1000
 * Copyright (C) 2020  Luca Cireddu (sardylan@gmail.com)
 * https://www.lucacireddu.it/
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

#ifndef __QFM1000__EEPROM__EEPROM_H
#define __QFM1000__EEPROM__EEPROM_H

#include <QtCore/QtGlobal>
#include <QtCore/QByteArray>
#include <QtCore/QVector>

#define EEPROM_SIZE 2048

#define CHANNELS_COUNT 100
#define CHANNELS_LENGTH 8

#define OFFSET_CHANNEL_FIRST 0x26
#define OFFSET_TOT 0x719
#define OFFSET_STARTUP_CHANNEL 0x6D7
#define OFFSET_BUTTONS 0x729
#define OFFSET_LOW_POWER 0x71E

#define OFFSET_LAST_USED_CHANNEL 0x1
#define OFFSET_LAST_USED_CTCSS 0x2

namespace qfm1000::eeprom {

    enum FrequencyBand {
        E0,
        B0,
        A9,
        K1,
        K2,
        K8,
        K9,
        TD,
        TM,
        TZ,
        T4,
        U0,
        W1,
        W4
    };

    class EEPROM {

    public:

        EEPROM();

        ~EEPROM();

        void clear();

        const QByteArray &getData();

        void setData(const QByteArray &value);

        unsigned int getChannelRxFreq(int channel, FrequencyBand frequencyBand);

        void setChannelRxFreq(int channel, unsigned int freq, FrequencyBand frequencyBand);

        unsigned int getChannelTxFreq(int channel, FrequencyBand frequencyBand);

        void setChannelTxFreq(int channel, unsigned int freq, FrequencyBand frequencyBand);

        quint8 getChannelRxCtcss(int channel);

        void setChannelRxCtcss(int channel, int ctcss);

        quint8 getChannelTxCtcss(int channel);

        void setChannelTxCtcss(int channel, int ctcss);

        unsigned int getChannelPower(int channel);

        void setChannelPower(int channel, unsigned int power);

        unsigned int getChannelSquelch(int channel);

        void setChannelSquelch(int channel, unsigned int squelch);

        bool getChannelSelectiveCalling(int channel);

        void setChannelSelectiveCalling(int channel, bool selectiveCalling);

        bool getChannelCpuOffset(int channel);

        void setChannelCpuOffset(int channel, bool cpuOffset);

        int getDefaultChannel();

        void setDefaultChannel(int defaultChannel);

        quint8 getTot();

        void setTot(int tot);

        int getLowPower();

        void setLowPower(int lowPower);

    private:

        QByteArray data;

        void assign(int pos, quint8 value);

        static bool isValidChannelNumber(int channel);

        static unsigned int wordToFrequency(FrequencyBand frequencyBand, quint16 word);

        static quint16 frequencyToWord(FrequencyBand frequencyBand, unsigned int frequency);

    };

}

#endif
