#include "../includes/python_submodule.h"

// See Pybind11 FAQ «How can I reduce the build time ?» :
// https://pybind11.readthedocs.io/en/stable/faq.html#how-can-i-reduce-the-build-time

//Python Binding and Time_Quad class instances.
PYBIND11_MODULE(Histograms, m)
{
    m.doc() = " Fast histogram library for many types (integers and floats)";
	init_Histograms(m);
	// init_buffered_arrays(m);
}

