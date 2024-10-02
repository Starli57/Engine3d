#include <pybind11/pybind11.h>
namespace py = pybind11;

int add(int i, int j) {
    return i + j;
}

PYBIND11_MODULE(Editor, m) 
{
    m.doc() = "pybind11 example plugin";

    m.def("EngineAdd", &add, "A function that adds two numbers");
}