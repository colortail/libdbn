#! /usr/bin/env python

from distutils.core import setup, Extension

MOD = "libdbn"
setup(name = MOD,
	version = "1.0",
	author = "colortail",
	ext_modules = [Extension(MOD, ['../libdbn.cpp',
		'../BNet.cpp',
		'../Clique.cpp',
		'../DBNet.cpp',
		'../Factor.cpp',
#		'../Graph.h',
#		'../GraphMatrix.h',
		'../InfEngine.cpp',
		'../InferStrategy.cpp',
		'../InOutUtils.cpp',
		'../JTree.cpp',
		'../Metric.cpp',
		'../utils.cpp',
		'../Test.cpp'
		])])

