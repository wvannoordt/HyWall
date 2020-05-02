#ifndef EQ_TYPES_H
#define EQ_TYPES_H

namespace HyCore
{
    enum momentum
    {
        allmaras         = 1,
        ODE              = 2
    };

    enum turbulence
    {
        linear           = 1,
        spallartAllmaras = 2
    };

    enum energy
    {
        croccoBusemann   = 1,
        ODE              = 2
    };
}

#endif
