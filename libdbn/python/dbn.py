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
import matplotlib.pyplot as plt
import networkx as nx
import json

class DBNUtils(object):
	
	def __init__(self):
		self.probnet = {}

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
			self.type = self.probnet['type']
			self.variable_size = self.probnet['variable_size']
			self.name = self.probnet['name']
			self.data = self.probnet['data']
		except ValueError,e:
			print "check the json validation plz!\n"
			traceback.print_exc()
		#graph structure
		if (self.data != None):
			self.graph = nx.DiGraph()
			i = 0
			for i, val in enumerate(self.data):
				self.graph.add_node(val['name'])
			for val in self.data:
				if val['pa'] == None:
					continue
				for paval in val['pa']:
					self.graph.add_edge(paval, val['name'])
		#conditional probability table

	def drawGraph(self):
		nx.draw(self.graph, node_size = 200,with_labels = True)
		plt.title(self.name)
		plt.show()

	def saveGraph(self, filename):
		nx.draw(self.graph)
		plt.savefig(filename);

if __name__ == '__main__':
	dbnUtils = DBNUtils()
	dbnUtils.loadjson('asian_net.json')
	#dbnUtils.drawGraph()



