#include "../includes/python_submodule.h"

// See Pybind11 FAQ «How can I reduce the build time ?» :
// https://pybind11.readthedocs.io/en/stable/faq.html#how-can-i-reduce-the-build-time

//Python Binding and Time_Quad class instances.
PYBIND11_MODULE(Fast_convolution, m)
{
    m.doc() = " test";
	init_FastConvolution(m);
	init_buffered_arrays(m);
}
