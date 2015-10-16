#! /usr/bin/env python

from distutils.core import setup, Extension

MOD = "libdbn"
setup(name = MOD,
	version = "1.0",
	author = "colortail",
	ext_modules = [Extension(MOD, ['../libdbn.cpp'])])