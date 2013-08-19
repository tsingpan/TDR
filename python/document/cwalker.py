from hotpot.hotpot.hp_config import *
from hotpot.hotdata.syntactic_node import *
from document.walker import *
import sys
import os

class CWalker(Walker):
	def __init__(self, document):
		Walker.__init__(self, document)
