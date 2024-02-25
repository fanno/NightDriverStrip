//+--------------------------------------------------------------------------
//
// File:        ledstripgfx.cpp
//
// NightDriverStrip - (c) 2018 Plummer's Software LLC.  All Rights Reserved.
//
// This file is part of the NightDriver software project.
//
//    NightDriver is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    NightDriver is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with Nightdriver.  It is normally found in copying.txt
//    If not, see <https://www.gnu.org/licenses/>.
//
// Description:
//
//    Code for handling LED strips
//
// History:     Jul-22-2023         Rbergen      Created
//
//---------------------------------------------------------------------------

#include "globals.h"
#include "ledstripgfxrgbw.h"
#include "systemcontainer.h"



void LEDStripGFXRGBW::PostProcessFrame(uint16_t localPixelsDrawn, uint16_t wifiPixelsDrawn)
{
    auto pixelsDrawn = wifiPixelsDrawn > 0 ? wifiPixelsDrawn : localPixelsDrawn;

    // If we drew no pixels, there's nothing to post process
    if (pixelsDrawn == 0)
    {
        debugV("Frame draw ended without any pixels drawn.");
        return;
    }

    // If we've drawn anything from either source, we can now show it if we have LEDs to output to
    if (FastLED.count() == 0)
    {
        debugW("Draw loop is drawing before LEDs are ready, so delaying 100ms...");
        delay(100);
        return;
    }

    auto& effectManager = g_ptrSystem->EffectManager();

// FastLED with RGBW
    CRGBW leds[NUM_LEDS];
//    CRGB *ledsRGB;

//devices[0]->GetLEDCount()

    for (int i = 0; i < NUM_CHANNELS; i++) 
    {
        for (int ix = 0; ix < NUM_LEDS; ix++) 
        {
            leds[ix].b = 255;
            leds[ix].g = 0;
            leds[ix].r = 0;
            leds[ix].w = 0;
        }
//        CRGB *ledsRGB = (CRGB *) &leds[0];

effectManager.g(i)->leds = (CRGB *) &leds[0];

//        FastLED[i].setLeds(ledsRGB, pixelsDrawn);

      FastLED[i].setLeds(effectManager.g(i)->leds, pixelsDrawn);

      fadeLightBy(FastLED[i].leds(), FastLED[i].size(), 255 - g_ptrSystem->DeviceConfig().GetBrightness());
    }
    FastLED.show(g_Values.Fader); //Shows the pixels

    g_Values.FPS = FastLED.getFPS();
    g_Values.Brite = 100.0 * calculate_max_brightness_for_power_mW(g_ptrSystem->DeviceConfig().GetBrightness(), POWER_LIMIT_MW) / 255;
    g_Values.Watts = calculate_unscaled_power_mW(effectManager.g()->leds, pixelsDrawn) / 1000; // 1000 for mw->W
}
