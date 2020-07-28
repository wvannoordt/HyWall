# PropTreeLib
A flexible and feature-rich input file reader designed for use in scientific computing applications.

## Description
This project was inspired by extensive work with a number of scientific computing codes.
A typical structure for one of these codes is to have an input file that contains every runtime parameter.
The solver then reads this file and sets parameters accordingly.

The very nature of scientific computing means that every run counts. If you aren't debugging, the code is likely to run for
days, possibly even weeks, at a time. This costs a lot of money and effort, so it makes sense to make sure that every parameter is correct!

Sometimes, however, deadlines can get to us. Small mistakes are made, such as a typo on one input option that causes it to be ignored and/or
set to a default value. In some cases, this can cause an entire simulation to be garbage.

## Purpose
PropTreeLib is designed to fix the following symptoms of primitive input file readers, each of which has been found in an existing, large-scale scientific computing code:

* You run 10 different cases with slightly varying input parameters, but forgot to print the values of the parameters to the slurm log. Now half of the data is meaningless
and needs to be run again! With a `PropTreeLib::PropertyTree input` object, simply call `input.DebugPrint()` and everything is there.

* You set up the case and run, and get a segmentation fault when the input is read (`std::stoi` etc. give a segmentation fault if the input is
improperly formatted). PropTreeLib will identify all unrecognized or improperly formatted inputs.

* All of your runtime parameters have default values but you can't remember what subsection or name a certain one of them has.
Using a `PropTreeLib::Interactive i` object, and a command-line argument `PTL:defaultVals`, simply call `i.Run()` to create a
full input file with every option set to its default value (GUI generation coming soon!).

* There is some quantity that you compute using different methods, but your methods are (poorly) labeled with integers.
You can't remember what `method=1`, `method=2`, etc. correspond to. Using a `PropTreeLib::Variables::PTLEnum` or `PropTreeLib::Variables::PTLAutoEnum`
object, options like this can have meaningful enumeration-style labels.

* Your input file options are not comprehensively documented. All PropTreeLib variables take a string description argument so that
every input option is documented. This is a huge help to rapidly bring newcomers onboard with your code!

* You have multiple separate submodules / objects / libraries that all have different input files. With `PropTreeLib::PropertyTree subInput, mainInput`
object, you can branch `mainInput` into a subtree using `subInput.SetAsSubtree(mainInput["mySubSection"])`, so every component is initialized from one file.

* You have multiple parameters in your input file that, for some reason, all have to have the same value. The `PropTreeLib::PreProcessContext` object is a part of every
`PropTreeLib::PropertyTree` (and is automatically active) and allows for contextualized `#define` directives, e.g. `#define myValue 0.1`, and then
`someValue = $(myValue)`

## Building
PropTreeLib is current built with GNU make. A Cmake build system may come in the future.

To build and test, simply run `make test` from the top directory. If successful, the following message will appear following the build output:
```
PropTreeLib passed all tests.
```

## Usage

### Basic Example
This example shows how to read a simple integer value from a file `input.ptl`:

`main.cc`

```c++
#include "PropTreeLib.h"
int main(int argc, char** argv)
{
    PropTreeLib::PropertyTree input;
    int myInteger;

    //Bind string "myInteger" to integer myInteger. The string can be any string,
    //not just the variable name. The default value of myInteger is 1. Note the
    //string description is the second argument.
    input["myIntegerValue"].MapTo(&myInteger)
        = new PropTreeLib::Variables::PTLInteger(1, "Some integer value");

    //This function actually populates the tree structure with string data but
    //does not parse anything. Reading and parsing are kept separate so that the
    //tree can be used to read arbitrary data if need be.
    input.ReadInputFileToTreeData("input.ptl");

    //After this function call, myInteger has the value read from the file (5).
    //This function will throw an error if any input option cannot be
    //successfully parsed.
    input.StrictParse();

    //This simply prints every value in the tree to the screen.
    input.DebugPrint();

    //Every resource used by input is cleaned up when the object goes out of
    //scope. The object is memory-safe.
    return 0;
}
```

`input.ptl`
```
myInteger = 5
```

### Other Types
The reader is directed to [examples](https://github.com/wvannoordt/PropTreeLib/tree/master/examples) and [testing](https://github.com/wvannoordt/PropTreeLib/tree/master/testing)
for more examples of usage.

### Defaults
This example shows how to create a default-filled file called `def.ptl`:

`main.cc`
```c++
//Compiles to a.out
#include <iostream>
#include "PropTreeLib.h"
int main(int argc, char** argv)
{
    //This is the simplest example of a PTL input.

    PropTreeLib::PropertyTree input;

    //Define an interactive object
    PropTreeLib::Interactive i(argc, argv, &input);

    //define some input data
    std::string str;
    int aa, ab, ba, bb;
    double* ar;
    int n;

    input["SectionA"]["ValueA"].MapTo(&aa) = new PropTreeLib::Variables::PTLInteger(1, "aa");
    input["SectionA"]["ValueB"].MapTo(&ab) = new PropTreeLib::Variables::PTLInteger(1, "ab");
    input["SectionA"]["StringValue"].MapTo(&str) = new PropTreeLib::Variables::PTLString("defaultVal", "str");
    input["SectionB"]["ValueA"].MapTo(&ba) = new PropTreeLib::Variables::PTLInteger(1, "ba");
    input["SectionB"]["ValueB"].MapTo(&bb) = new PropTreeLib::Variables::PTLInteger(1, "bb");
    input["SectionB"]["arr"].MapTo(&ar, &n) = new PropTreeLib::Variables::PTLDynamicDoubleArray("array");

    //Outputs file "def.ptl" and kills program if program is run as "./a.out PTL:defaultVals"
    i.Run();

    return 0;
}
```

`def.ptl`:
```
// This file was generated with default values
SectionA
{
    StringValue = defaultVal
    ValueA = 1
    ValueB = 1
}
SectionB
{
    ValueA = 1
    ValueB = 1
    arr = []
}
```


## Supported Input Types
Any input type can be implemented by inheriting the `PropTreeLib::Variables::InputVariable` class. PropTreeLib currently has implementations for:

* `PropTreeLib::Variables::PTLInteger`: An integer value.
* `PropTreeLib::Variables::PTLBoolean`: A boolean value.
* `PropTreeLib::Variables::PTLDouble`: A double-precision value.
* `PropTreeLib::Variables::PTLString`: A string (`std::string`) value.
* `PropTreeLib::Variables::PTLEnum`: A string-based enumeration that maps to an integer value. Options are passed to the constructor as a colon-delimited list,
i.e. `"option1:option2:option3"`. It is recommended that each option has some corresponding, meaningful comparison when used to branch.
* `PropTreeLib::Variables::PTLAutoEnum`: An enumeration defined by an `enum` implementation, e.g. `enum myType{option1,option2}`. Since C++ is not reflective,
this type's constructor also takes a function pointer that returns the string representation of each option, e.g:
```c++
std::string myTypeStr(int i)
{
    switch (i)
    {
        case option1: return "option1";
        case option2: return "option2";
    }
    return PTL_AUTO_ENUM_TERMINATOR;
}
```
* `PropTreeLib::Variables::PTLDynamicDoubleArray`: An arbitrary-size array of double-precision values. This object maps to two outputs: an integer representing
the size of the array, and a `double*` that is heap-allocated, then deallocated when the relevant `PropTreeLib::PropertyTree` object goes out of
scope (**this is very important!**).

* `PropTreeLib::Variables::PTLStaticDoubleArray`: A fixed-size array of double-precision values. This object maps only to a `double*`, and is assumed to have a
fixed size known to the user at compile time. Other than the fact that this only maps to a single value, this object behaves identically to a
`PropTreeLib::Variables::PTLDynamicDoubleArray`.

* `PropTreeLib::Variables::PTLDynamicIntegerArray`: This object is identical to a `PropTreeLib::Variables::PTLDynamicDoubleArray`, just with an integer element type.
* `PropTreeLib::Variables::PTLStaticIntegerArray`: This object is identical to a `PropTreeLib::Variables::PTLStaticIntegerArray`, just with an integer element type.

Other types are still in development as needed, but the reader is encouraged to contribute implementations of more (preferably standard) types.




## License
[GNU GENERAL PUBLIC LICENSE](https://choosealicense.com/licenses/gpl-3.0/)
