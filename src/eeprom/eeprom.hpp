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


#ifndef __QFM1000__EEPROM__EEPROM__H
#define __QFM1000__EEPROM__EEPROM__H

#include <QtCore/QtGlobal>
#include <QtCore/QByteArray>

#define EEPROM_SIZE 2048

#define CHANNELS_COUNT 100

#define OFFSET_CHANNEL_FIRST 0x26
#define OFFSET_TOT 0x719
#define OFFSET_STARTUP_CHANNEL 0x6D7
#define OFFSET_BUTTONS 0x729
#define OFFSET_LOW_POWER 0x71E

#define OFFSET_LAST_USED_CHANNEL 0x1
#define OFFSET_LAST_USED_CTCSS 0x2

namespace qfm1000::eeprom {

    typedef quint8 Channel;
    typedef quint32 Frequency;
    typedef quint8 CTCSS;
    typedef quint8 TOT;
    typedef quint8 Power;
    typedef quint8 Squelch;

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

        bool setData(const QByteArray &newValue);

        Frequency getChannelRxFreq(Channel channel, FrequencyBand frequencyBand);

        void setChannelRxFreq(Channel channel, Frequency freq, FrequencyBand frequencyBand);

        Frequency getChannelTxFreq(Channel channel, FrequencyBand frequencyBand);

        void setChannelTxFreq(Channel channel, Frequency freq, FrequencyBand frequencyBand);

        CTCSS getChannelRxCtcss(Channel channel);

        void setChannelRxCtcss(Channel channel, CTCSS ctcss);

        CTCSS getChannelTxCtcss(Channel channel);

        void setChannelTxCtcss(Channel channel, CTCSS ctcss);

        Power getChannelPower(Channel channel);

        void setChannelPower(Channel channel, Power power);

        Squelch getChannelSquelch(Channel channel);

        void setChannelSquelch(Channel channel, Squelch squelch);

        bool getChannelSelectiveCalling(Channel channel);

        void setChannelSelectiveCalling(Channel channel, bool selectiveCalling);

        bool getChannelCpuOffset(Channel channel);

        void setChannelCpuOffset(Channel channel, bool cpuOffset);

        Channel getDefaultChannel();

        void setDefaultChannel(Channel defaultChannel);

        TOT getTot();

        void setTot(TOT tot);

        Power getLowPower();

        void setLowPower(Power lowPower);

    private:

        QByteArray data;

        void assign(int pos, quint8 value);

        static bool isValidChannelNumber(Channel channel);

        static Frequency wordToFrequency(quint16 word, FrequencyBand frequencyBand);

        static quint16 frequencyToWord(Frequency frequency, FrequencyBand frequencyBand);

    };

}

#endif
