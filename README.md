# 3dmath

A header-only 3D math library used for building apps like [MeshViewer](https://github.com/mdh81/meshviewer)

[![Quality](https://github.com/mdh81/3dmath/actions/workflows/cmake-single-platform.yml/badge.svg)](https://github.com/mdh81/3dmath/actions/workflows/cmake-single-platform.yml)

## Python Bindings

Python bindings are generated using the excellent [pybind11](https://github.com/pybind/pybind11) library

### Building and testing python bindings

```
# Activate a python virtual environment
$ cd <path to 3dmath>
$ python3 -m venv .venv
$ source .venv/bin/activate
$ cmake --build <build_dir> --parallel -t math3d
$ pip -install py/
$ python
> import math3d
```


