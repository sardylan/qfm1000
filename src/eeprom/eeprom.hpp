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
#include <QtCore/QObject>
#include <QtCore/QByteArray>

#define EEPROM_SIZE 2048

#define CHANNELS_COUNT 100

#define OFFSET_CHANNEL_FIRST 0x26
#define OFFSET_CHANNEL_FIRST_ALTERNATIVE 0x21
#define OFFSET_STARTUP_CHANNEL 0x6D7
#define OFFSET_KEY_BEEP 0x6EB
#define OFFSET_TOT 0x719
#define OFFSET_LOW_POWER 0x71E
#define OFFSET_BUTTONS 0x729

#define OFFSET_LAST_USED_CHANNEL 0x1
#define OFFSET_LAST_USED_CTCSS 0x2

namespace qfm1000::eeprom {

    typedef quint8 Channel;
    typedef quint32 Frequency;
    typedef quint8 TOT;

    enum class FrequencyBand {
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

    enum class Flag : bool {
        ENABLED = true,
        DISABLED = false
    };

    enum class CTCSS : quint8 {
        TONE_OFF = 0,
        TONE_67_0 = 1,
        TONE_71_9 = 2,
        TONE_74_4 = 3,
        TONE_77_0 = 4,
        TONE_79_7 = 5,
        TONE_82_5 = 6,
        TONE_85_4 = 7,
        TONE_88_5 = 8,
        TONE_91_5 = 9,
        TONE_94_8 = 10,
        TONE_97_4 = 11,
        TONE_100_0 = 12,
        TONE_103_5 = 13,
        TONE_107_2 = 14,
        TONE_110_9 = 15,
        TONE_114_8 = 16,
        TONE_118_8 = 17,
        TONE_123_0 = 18,
        TONE_127_3 = 19,
        TONE_131_8 = 20,
        TONE_136_5 = 21,
        TONE_141_3 = 22,
        TONE_146_2 = 23,
        TONE_151_4 = 24,
        TONE_156_7 = 25,
        TONE_162_2 = 26,
        TONE_167_9 = 27,
        TONE_173_8 = 28,
        TONE_179_9 = 29,
        TONE_186_2 = 30,
        TONE_192_8 = 31,
        TONE_203_5 = 32,
        TONE_210_7 = 33,
        TONE_218_1 = 34,
        TONE_225_7 = 35,
        TONE_233_6 = 36,
        TONE_241_8 = 37,
        TONE_250_3 = 38
    };

    enum class Power : quint8 {
        DISABLED = 0,
        WATT_1 = 1,
        WATT_6 = 2,
        WATT_10 = 3,
        WATT_15 = 4,
        WATT_25 = 5
    };

    enum class Squelch : quint8 {
        OPEN = 0,
        SINAD_9DB = 1,
        SINAD_12DB = 2,
        SINAD_15DB = 3,
        SINAD_18DB = 4,
        SINAD_21DB = 5,
        SINAD_24DB = 6
    };

    class EEPROM : public QObject {
    Q_OBJECT

    public:

        explicit EEPROM(QObject *parent = nullptr);

        ~EEPROM() override;

        void clear();

        const QByteArray &getData();

        bool setData(const QByteArray &newValue);

        [[nodiscard]] FrequencyBand getFrequencyBand() const;

        void setFrequencyBand(FrequencyBand newValue);

        [[nodiscard]] int getFirstChannelOffset() const;

        void setFirstChannelOffset(int newValue);

        Frequency getChannelRxFreq(Channel channel);

        void setChannelRxFreq(Channel channel, Frequency freq);

        Frequency getChannelTxFreq(Channel channel);

        void setChannelTxFreq(Channel channel, Frequency freq);

        CTCSS getChannelRxCtcss(Channel channel);

        void setChannelRxCtcss(Channel channel, CTCSS ctcss);

        CTCSS getChannelTxCtcss(Channel channel);

        void setChannelTxCtcss(Channel channel, CTCSS ctcss);

        Power getChannelPower(Channel channel);

        void setChannelPower(Channel channel, Power power);

        Squelch getChannelSquelch(Channel channel);

        void setChannelSquelch(Channel channel, Squelch squelch);

        Flag getChannelSelectiveCalling(Channel channel);

        void setChannelSelectiveCalling(Channel channel, Flag selectiveCalling);

        Flag getChannelCpuOffset(Channel channel);

        void setChannelCpuOffset(Channel channel, Flag cpuOffset);

        Channel getStartupChannel();

        void setStartupChannel(Channel startupChannel);

        Flag getKeyBeep();

        void setKeyBeep(Flag keyBeep);

        TOT getTot();

        void setTot(TOT tot);

        Power getLowPower();

        void setLowPower(Power lowPower);

        [[nodiscard]] int computeOffset(Channel channel) const;

        static void registerMetaType();

    private:

        QByteArray data;

        FrequencyBand frequencyBand;
        int firstChannelOffset;

        bool add300Mhz;

        bool detectRadioType();

        void assign(int pos, quint8 value);

        [[nodiscard]] Frequency wordToFrequency(quint16 word) const;

        [[nodiscard]] quint16 frequencyToWord(Frequency frequency) const;

        static bool isValidChannelNumber(Channel channel);

    signals:

        void byteUpdated(int pos, quint8 value);

    };

}

Q_DECLARE_METATYPE(qfm1000::eeprom::FrequencyBand)

Q_DECLARE_METATYPE(qfm1000::eeprom::Flag)

Q_DECLARE_METATYPE(qfm1000::eeprom::CTCSS)

Q_DECLARE_METATYPE(qfm1000::eeprom::Power)

Q_DECLARE_METATYPE(qfm1000::eeprom::Squelch)

#endif
