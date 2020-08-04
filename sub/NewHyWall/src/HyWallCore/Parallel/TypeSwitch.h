#ifndef TYPESWITCH_H
#define TYPESWITCH_H

#define BOOL_T 0
#define CHAR_T 1
#define INT_T 2
#define FLOAT_T 3
#define DOUBLE_T 4

template<typename T>
struct typeswitch {};

template<>
struct typeswitch<bool>
{
    enum { value = BOOL_T };
};

template<>
struct typeswitch<char>
{
    enum { value = CHAR_T };
};

template<>
struct typeswitch<int>
{
    enum { value = INT_T };
};

template<>
struct typeswitch<float>
{
    enum { value = FLOAT_T };
};

template<>
struct typeswitch<double>
{
    enum { value = DOUBLE_T };
};

#define htypeid(myTypeIn) (switch_value<myTypeIn>::value)
#define mpitypeid(myTypeIn) (GetMpiType(typeswitch<myTypeIn>::value))
static inline MPI_Datatype GetMpiType(int i)
{
    switch (i)
    {
        case BOOL_T: return MPI_C_BOOL;
        case CHAR_T: return MPI_CHAR;
        case INT_T: return MPI_INT;
        case FLOAT_T: return MPI_FLOAT;
        case DOUBLE_T: return MPI_DOUBLE;
        default: return MPI_DATATYPE_NULL;
    }
}
#endif