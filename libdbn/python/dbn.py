#! /usr/bin/env python
# libdbn python api by colortail

#from ctypes import *
#from ctypes.util import find_library
#from os import path
#import sys

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
import traceback
import re

import libdbn
import numpy as np
import networkx as nw
import json

class DBNUtils(object):
	
	def __init__(self):
		self.probnet = {}

	def __setattr__(self, key, value):
		self.probnet[key] = value

	def loadjson(self, filename):
		f = open(filename)
		try:
			jsonstr = f.read()
		finally:
			f.close()
		#print jsonstr
		try:
			pattern = re.compile("//*.*/*/",re.S)
			jsonstr, n = re.subn(pattern, "", jsonstr)
			self.probnet = json.loads(jsonstr)
		except ValueError,e:
			print "check the json validation plz!\n"
			traceback.print_exc()

if __name__ == '__main__':
	dbnUtils = DBNUtils()
	dbnUtils.loadjson('asian_net.json')



