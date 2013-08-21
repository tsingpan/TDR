import json
from document.walker import *

class HP_JSON(Walker):
	def __init__(self, document, target_dir):
		Walker.__init__(self, document, target_dir)

	def on_document_begin(self, document):
		ofile_name = self.target_dir + '/' + self.file_name + '.json'
		self.fout = open(ofile_name, "w")
		self.fout.write(json.dumps(document))

	def on_document_end(self, document):
		self.fout.close()

def hpmain(document, target_dir):
	cw = HP_JSON(document, target_dir)
	return cw.walk()
