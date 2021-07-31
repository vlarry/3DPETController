#include "stdint.h"
//----------
#pragma once
//
//	Structure om font te definieren
//
typedef struct
{
    const uint8_t width;    /*!< Font width in pixels */
    uint8_t height;   /*!< Font height in pixels */
    const uint16_t *data; /*!< Pointer to data font data array */
} font_t;
//
//	De 3 fonts
//
extern font_t FontCourierNew_7x10;
extern font_t FontConsolas_8x12;
extern font_t FontYuGothicUI_8x11;
extern font_t FontVerdana_12x12;
extern font_t FontVerdana_16x16;
//
// ICONS
//
extern font_t ImageLampOff;
extern font_t ImageLampOn;
extern font_t ImageCelsius;
