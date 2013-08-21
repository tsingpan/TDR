from hotpot.hp_config import *
from hotdata.syntactic_node import *
from document.walker import *
import sys
import os

class PYTHON_TYPES(Walker):
	def __init__(self, document, target_dir):
		Walker.__init__(self, document, target_dir)

	def get_val(self, val):
		if val['type'] == E_SNVT_IDENTIFIER:
			return val['val']['identifier']
		elif val['type'] == E_SNVT_CHAR:
			if(val['val']['c'] == '\n'):
				return '\'\\n\''
			else:
				return '\'' + val['val']['c'] + '\''
		elif val['type'] == E_SNVT_DOUBLE:
			return val['val']['d']
		elif val['type'] == E_SNVT_BOOL:
			return val['val']['b']
		elif val['type'] == E_SNVT_STRING:
			return '\"' + val['val']['str'] + '\"'
		elif val['type'] == E_SNVT_INT64:
			return val['val']['i64']
		elif val['type'] == E_SNVT_UINT64:
			return val['val']['ui64']
		elif val['type'] == E_SNVT_HEX_INT64:
			return val['val']['hex_i64']
		elif val['type'] == E_SNVT_HEX_UINT64:
			return val['val']['hex_ui64']	
		pass

	def on_document_begin(self, document):
		ofile_name = self.target_dir + '/' + self.file_name + '.py'
		self.fout = open(ofile_name, "w")

	def on_document_end(self, document):
		self.fout.close()

	def on_import(self, de_import):
		self.print_line(0, 'from ' + de_import['package_name'].rstrip('\.hd') + ' import *')

	def on_const(self, const):
		self.print_line(0, const['identifier'] + ' = ' + str(self.get_val(const['val'])))

	def on_enum_begin(self, enum):
		for value in enum['enum_def_list']:
			self.print_line(0, value['identifier'] + ' = ' + str(self.get_val(value['val'])))

def hpmain(document, output_dir):
	cw = PYTHON_TYPES(document, output_dir)
	return cw.walk()
