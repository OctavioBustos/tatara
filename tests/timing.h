#ifndef TIMING_H_INCLUDED
#define TIMING_H_INCLUDED

#include <chrono>

const unsigned RUN_TIMES = 10;

#define TIME(EXPR_TO_EVAL,N,var_t)\
{ size_t count = 0;\
for (int j=0; j < N; ++j) {\
auto t0 = std::chrono::system_clock::now();\
EXPR_TO_EVAL;\
auto t1 = std::chrono::system_clock::now();\
count += (t1-t0).count();}\
var_t = count / N; }


#endif // TIMING_H_INCLUDED
