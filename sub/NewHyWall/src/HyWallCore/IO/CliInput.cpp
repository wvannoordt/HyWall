#include "CliInput.h"
namespace HyWall
{
    CliInput::CliInput(void){}

    CliInput::CliInput(int argc_in, char** argv_in)
    {
        argc  = argc_in;
        argv = argv_in;
    }
}
