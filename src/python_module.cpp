#include "../includes/python_module.h"

// See Pybind11 FAQ «How can I reduce the build time ?» :
// https://pybind11.readthedocs.io/en/stable/faq.html#how-can-i-reduce-the-build-time

//Python Binding and Time_Quad class instances.
PYBIND11_MODULE(time_domain, m)
{
	m.doc() = " Time_domain is a python library that calls subjascent custom C++ classes/functions dedicated to on the fly/fast calculations for time domain experiments.";
	init_TimeQuad(m);
	init_fft(m);
	init_Histograms(m);
	init_FastConvolution(m);
	init_buffered_array(m);
}

