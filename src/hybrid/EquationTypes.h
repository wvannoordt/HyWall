#ifndef EQ_TYPES_H
#define EQ_TYPES_H
#include "HybridComputing.h"

#pragma HWPP include

namespace HyCore
{
    namespace equationType
    {
        enum equationTypeenum
        {
            explicitEq     = 83982,
            algebraicEq    = 20943,
            differentialEq = 79828
        };
    }

    namespace momentum
    {
        enum momentumenum
        {
            allmaras         = 34534,
            ODE              = 26323,
            fromFile         = 12342
        };
    }
    
    namespace yscale
    {
        enum yscaleenum
        {
            trettelLarsson = 12,
            yPlus = 14,
            mixed = 16
        };
    }

    namespace turbulence
    {
        enum turbulenceenum
        {
            linear           = 29823,
            ODE              = 23414,
            vanDriest        = 82984,
            fromFile         = 12343,
            pnlm             = 19283
        };
    }

    namespace energy
    {
        enum energyenum
        {
            croccoBusemann   = 98304,
            ODE              = 70209,
            linear           = 27498,
            fromFile         = 12344
        };
    }
}

#endif
