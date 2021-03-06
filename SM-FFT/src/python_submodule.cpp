#include "../includes/python_submodule.h"

// See Pybind11 FAQ «How can I reduce the build time ?» :
// https://pybind11.readthedocs.io/en/stable/faq.html#how-can-i-reduce-the-build-time

//Python Binding and Time_Quad class instances.
PYBIND11_MODULE(fft_benchmark, m)
{
    m.doc() = " fft_benchmark is used to benchmark FFT's using FFTW on your PC. You can control core counts and FFT lengt.";
	init_fft(m);
	init_buffered_arrays(m);
}

