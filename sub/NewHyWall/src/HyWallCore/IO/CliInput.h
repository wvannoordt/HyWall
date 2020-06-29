#ifndef CLIINPUT_H
#define CLIINPUT_H

namespace HyWall
{
    class CliInput
    {
        public:
            CliInput(void);
            CliInput(int argc_in, char** argv_in);
        private:
            int argc;
            char** argv;
    };
}

#endif
