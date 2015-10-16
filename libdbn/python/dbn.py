#! /usr/bin/env python
# libdbn python api by colortail
from ctypes import *
from ctypes.util import find_library
from os import path
import sys

# Linux so shared object library 
# G++ compiler 
# ll = ctypes.cdll.LoadLibrary

# indows dynamic link library
# Microsoft Visual studio
#ll = windll.LoadLibrary
# Note that: the target(x86 or amd64)
#lib = ll("./libdbn.dll")

#test
#lib.foo("dir")

import libdbn

print libdbn.foo('dir')
