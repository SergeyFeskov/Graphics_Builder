#ifndef funcs_cpp_included
    #define func_cpp_included
    #include <cmath>
    #include "funcs.h"

    double f_sin(double params[])
    {
        return sin(params[0]);
    }

    double f_cos(double params[])
    {
        return cos(params[0]);
    }

    double f_tg(double params[])
    {
        return tan(params[0]);
    }

    double f_ctg(double params[])
    {
        return 1/tan(params[0]);
    }

    double f_abs(double params[]){
        return abs(params[0]);
    }

    double f_sqr(double params[]){
        return sqrt(params[0]);
    }
#endif