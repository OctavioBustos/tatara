#include <gtest/gtest.h>
#include "../src/tp3.h"
#include "timing.h"
#include <iostream>

TEST(test_multiplicacion, test_matriz_cuadrada) {
    Matriz M1 = {
        {1,2,3,4},
        {5,4,3,2},
        {6,6,6,7},
        {1,2,2,1}
    };

    Matriz M2 = {
        {2,0,0,0},
        {0,2,0,0},
        {0,0,2,0},
        {0,0,0,2}
    };

    // producto con dimension potencia de 2
    auto M3 = multiplicar_strassen(M1,M2,1);
    EXPECT_EQ(M3, Matriz({
                             {2,   4,  6,  8},
                             {10,  8,  6,  4},
                             {12, 12, 12, 14},
                             {2,   4,  4,  2}
                         }
                  ));
}

#ifdef TEST_TIMING

TEST(test_multiplicacion, test_matriz_gigante) {
    size_t T0, T1;
    // una matriz un poco más grande
    Matriz M4 = crear(1024,1.0), M5, M6;
    TIME(M5 = multiplicar_strassen(M4, M4, 32), RUN_TIMES, T0);
    EXPECT_FLOAT_EQ(M5[0][0], M5.size());

    TIME(M6 = multiplicar(M4, M4), RUN_TIMES, T1);
    EXPECT_FLOAT_EQ(M5[0][0], M5.size());
    auto d = double(T0)/T1;
    std::cout << "T0: " << T0 << std::endl;
    std::cout << "T1: " << T1 << std::endl;
    std::cout << "D: " << d << std::endl;
    EXPECT_LT(d, 0.4);
}
#endif
