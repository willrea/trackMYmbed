/* trackuino copyright (C) 2010  EA5HAV Javi
 *           copyright (C) 2014         Adam Green (https://github.com/adamgreen)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

/* Credit to:
 *
 * Michael Smith for his Example of Audio generation with two timers and PWM:
 * http://www.arduino.cc/playground/Code/PCMAudio
 *
 * Ken Shirriff for his Great article on PWM:
 * http://arcfn.com/2009/07/secrets-of-arduino-pwm.html
 *
 * The large group of people who created the free AVR tools.
 * Documentation on interrupts:
 * http://www.nongnu.org/avr-libc/user-manual/group__avr__interrupts.html
 */
#include "afsk.h"
#include "config.h"


const uint16_t Afsk::s_sinLookupTable[512] =
{
    32767, 33169, 33571, 33973, 34375, 34776, 35178, 35578, 35979, 36379, 36778, 37177, 37575, 37972, 38369, 38765,
    39160, 39554, 39946, 40338, 40729, 41118, 41507, 41893, 42279, 42663, 43046, 43427, 43806, 44184, 44560, 44934,
    45307, 45677, 46046, 46412, 46777, 47139, 47500, 47858, 48213, 48567, 48918, 49267, 49613, 49956, 50298, 50636,
    50972, 51305, 51635, 51962, 52287, 52608, 52927, 53242, 53554, 53864, 54170, 54473, 54772, 55069, 55362, 55651,
    55937, 56220, 56499, 56774, 57046, 57314, 57579, 57840, 58097, 58350, 58599, 58845, 59086, 59324, 59557, 59787,
    60012, 60234, 60451, 60664, 60873, 61077, 61278, 61474, 61665, 61853, 62036, 62214, 62388, 62558, 62723, 62884,
    63040, 63192, 63339, 63481, 63619, 63752, 63881, 64004, 64124, 64238, 64348, 64452, 64552, 64648, 64738, 64824,
    64905, 64981, 65052, 65118, 65180, 65236, 65288, 65335, 65377, 65414, 65446, 65473, 65495, 65512, 65525, 65532,
    65535, 65532, 65525, 65512, 65495, 65473, 65446, 65414, 65377, 65335, 65288, 65236, 65180, 65118, 65052, 64981,
    64905, 64824, 64738, 64648, 64552, 64452, 64348, 64238, 64124, 64004, 63881, 63752, 63619, 63481, 63339, 63192,
    63040, 62884, 62723, 62558, 62388, 62214, 62036, 61853, 61665, 61474, 61278, 61077, 60873, 60664, 60451, 60234,
    60012, 59787, 59557, 59324, 59086, 58845, 58599, 58350, 58097, 57840, 57579, 57314, 57046, 56774, 56499, 56220,
    55937, 55651, 55362, 55069, 54772, 54473, 54170, 53864, 53554, 53242, 52927, 52608, 52287, 51962, 51635, 51305,
    50972, 50636, 50298, 49956, 49613, 49267, 48918, 48567, 48213, 47858, 47500, 47139, 46777, 46412, 46046, 45677,
    45307, 44934, 44560, 44184, 43806, 43427, 43046, 42663, 42279, 41893, 41507, 41118, 40729, 40338, 39946, 39554,
    39160, 38765, 38369, 37972, 37575, 37177, 36778, 36379, 35979, 35578, 35178, 34776, 34375, 33973, 33571, 33169,
    32767, 32365, 31963, 31561, 31159, 30758, 30356, 29956, 29555, 29155, 28756, 28357, 27959, 27562, 27165, 26769,
    26374, 25980, 25588, 25196, 24805, 24416, 24027, 23641, 23255, 22871, 22488, 22107, 21728, 21350, 20974, 20600,
    20227, 19857, 19488, 19122, 18757, 18395, 18034, 17676, 17321, 16967, 16616, 16267, 15921, 15578, 15236, 14898,
    14562, 14229, 13899, 13572, 13247, 12926, 12607, 12292, 11980, 11670, 11364, 11061, 10762, 10465, 10172,  9883,
     9597,  9314,  9035,  8760,  8488,  8220,  7955,  7694,  7437,  7184,  6935,  6689,  6448,  6210,  5977,  5747,
     5522,  5300,  5083,  4870,  4661,  4457,  4256,  4060,  3869,  3681,  3498,  3320,  3146,  2976,  2811,  2650,
     2494,  2342,  2195,  2053,  1915,  1782,  1653,  1530,  1410,  1296,  1186,  1082,   982,   886,   796,   710,
      629,   553,   482,   416,   354,   298,   246,   199,   157,   120,    88,    61,    39,    22,     9,     2,
        0,     2,     9,    22,    39,    61,    88,   120,   157,   199,   246,   298,   354,   416,   482,   553,
      629,   710,   796,   886,   982,  1082,  1186,  1296,  1410,  1530,  1653,  1782,  1915,  2053,  2195,  2342,
     2494,  2650,  2811,  2976,  3146,  3320,  3498,  3681,  3869,  4060,  4256,  4457,  4661,  4870,  5083,  5300,
     5522,  5747,  5977,  6210,  6448,  6689,  6935,  7184,  7437,  7694,  7955,  8220,  8488,  8760,  9035,  9314,
     9597,  9883, 10172, 10465, 10762, 11061, 11364, 11670, 11980, 12292, 12607, 12926, 13247, 13572, 13899, 14229,
    14562, 14898, 15236, 15578, 15921, 16267, 16616, 16967, 17321, 17676, 18034, 18395, 18757, 19122, 19488, 19857,
    20227, 20600, 20974, 21350, 21728, 22107, 22488, 22871, 23255, 23641, 24027, 24416, 24805, 25196, 25588, 25980,
    26374, 26769, 27165, 27562, 27959, 28357, 28756, 29155, 29555, 29956, 30356, 30758, 31159, 31561, 31963, 32365,
};


void Afsk::tickerISR(void)
{
    static const uint32_t oneInFixed24_8 = (1 << 8);
    static const size_t   tableSize = sizeof(s_sinLookupTable)/sizeof(s_sinLookupTable[0]);
    // Fetch volatiles once at beginning of ISR and write out potentially modified values at end of ISR.
    const uint32_t        samplesPerBaud = m_samplesPerBaud;
    uint32_t              phase = m_phase;
    uint32_t              phaseDelta = m_phaseDelta;
    uint32_t              currentSampleInBaud = m_currentSampleInBaud;
    uint32_t              bitPos = m_bitPos;
    uint8_t               currentByte = m_currentByte;

    // If sent all samples for bit then only a fractional residual will be left in m_currentSampleInBaud.
    if (currentSampleInBaud < oneInFixed24_8)
    {
        // Just advancing to next most significant bit in current byte or to next byte in data?
        if (bitPos == 0)
        {
            volatile const uint8_t* pDataCurr = m_pDataCurr;

            // Advancing to next byte.
            if (pDataCurr >= m_pDataEnd)
            {
                // All bytes have been sent.
                m_ticker.detach();
                m_pRadio->disable();
                m_isSending = false;
                return;
            }
            currentByte = *pDataCurr;
            m_pDataCurr = pDataCurr + 1;
        }
        else
        {
            // Advance to next bit.  Send lsb to msb.
            currentByte >>= 1;
        }

        if ((currentByte & 1) == 0)
        {
            // Toggle tone (1200 <> 2200) on bit value of 0 (NRZI encoding).
            phaseDelta ^= m_phaseDeltaSwitch;
        }
    }

    // Advance to next sample for current tone.
    phase += phaseDelta;

    // Set output to next sample for current tone.
    uint16_t s = s_sinLookupTable[(phase >> 7) & (tableSize - 1)];
    if (PRE_EMPHASIS && phaseDelta == m_phaseDelta1200)
        s = s / 2 + 16384;
    m_pRadio->set(s);

    // Advance the fixed point sample counter by 1.0
    currentSampleInBaud += oneInFixed24_8;
    if (currentSampleInBaud >= samplesPerBaud)
    {
        bitPos = (bitPos + 1) & 7;
        // Keep track of residual so that fractional error can be accounted for across bits.
        currentSampleInBaud -= samplesPerBaud;
    }

    // Update volatiles now that we are completely done modifying them.
    m_phase = phase;
    m_phaseDelta = phaseDelta;
    m_currentSampleInBaud = currentSampleInBaud;
    m_bitPos = bitPos;
    m_currentByte = currentByte;
}
