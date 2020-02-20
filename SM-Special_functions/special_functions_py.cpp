#include "special_functions_py.h"

void init_special_functions(py::module &m)
{
	m.def("Fresnel_Cosine_Integral", &Fresnel_Cosine_Integral) ;
	m.def("Fresnel_Sine_Integral", &Fresnel_Sine_Integral) ;
	m.def("Tukey", &Tukey, "x"_a.noconvert() , "alpha"_a.noconvert() , "x_max"_a.noconvert() );
	m.def("Tukey_modifed", &Tukey_modifed, "x"_a.noconvert() , "x_1"_a.noconvert() , "x_2"_a.noconvert() , "x_max"_a.noconvert() );
}