#pragma once

#if defined(__CYGWIN__) || defined(__MINGW64__)
    // see number from: sdkddkver.h
    // https://docs.microsoft.com/fr-fr/windows/desktop/WinProg/using-the-windows-headers
    #define _WIN32_WINNT 0x0602 // Windows 8
	#define WIN32_LEAN_AND_MEAN //Faster compilation (remove some unused API)
	#include <windows.h>
    #include <Processtopologyapi.h>
    #include <processthreadsapi.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
//#include <fcntl.h>
#include <string.h>
#include <assert.h>

#define _USE_MATH_DEFINES
#define PI M_PI
#include<math.h>
#include<tuple>

// #include<memory>

#include <pybind11/pybind11.h>
#include <pybind11/complex.h>
#include <pybind11/numpy.h>
namespace py = pybind11;
using namespace pybind11::literals;
typedef std::complex<float> complex_f;
typedef std::complex<double> complex_d;
typedef unsigned int uint;


#include <fftw3.h>
	

#include <iostream>
//#include <iomanip>

#include <omp.h>
//#include <ctime>
//#include <limits>

#define __STDC_FORMAT_MACROS
#include <inttypes.h>

