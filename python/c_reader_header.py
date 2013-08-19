from document.cwalker import *

class C_READER_HEADER(CWalker):
	def __init__(self, document, output_dir):
		CWalker.__init__(self, document)
		self.file_tag = '_H_' + self.get_file_tag(document['file_name']) + '_READER_HEADER'
		self.output_dir = output_dir
	
	def on_document_begin(self, document):
		ofile_name = self.output_dir + '/' + document['file_name'].rstrip('.hd') + '_reader.h'
		self.fout = open(ofile_name, "w")

		self.print_file_prefix()
		self.print_line(0, '#ifndef ' + self.file_tag)
		self.print_line(0, '#define ' + self.file_tag)
		self.print_line(0, '#include "hotpot/hp_platform.h"')
		self.print_line(0, '#include "' + document['file_name'].rstrip('.hd') + '.h')

	def on_document_end(self, document):
		self.print_line(0, '#endif//' + self.file_tag)
		self.fout.close()

	def on_enum_begin(self, enum):
		self.print_line(0, 'HP_ERROR_CODE read_' + enum['name'] + '_name(HPAbstractReader *self, ' + enum['name'] + ' *data);')
		self.print_line(0, 'HP_ERROR_CODE read_' + enum['name'] + '_number(HPAbstractReader *self, ' + enum['name'] + ' *data);')
		self.print_line(0, 'HP_ERROR_CODE read_' + enum['name'] + '(HPAbstractReader *self, ' + enum['name'] + ' *data);')
		
	def on_struct_begin(self, struct):
		line = 'HP_ERROR_CODE read_' + struct['name'] + '(HPAbstractReader *self, ' + struct['name'] + ' *data'
		for value in struct['parameters']['par_list']:
			line = line + ' , '
			line = line + self.get_type(value['type'], None)
			line = line + ' const' + self.get_symbol_access_by_type_prefix_reverse(value['identifier'], value['type']) + value['identifier']
		line = line + ');'
		self.print_line(0, line)


	def on_union_begin(self, union):
		line = 'HP_ERROR_CODE read_' + union['name'] + '(HPAbstractReader *self, ' + union['name'] + ' *data'
		for value in union['parameters']['par_list']:
			line = line + ' , '
			t = self.get_type(value['type'], None)
			if((self.find_symbol(t) == Walker.EN_HST_STRUCT) or (self.find_symbol(t) == Walker.EN_HST_UNION)):
				t = 'const ' + t + '*'
			line = line + t + ' ' + value['identifier']
		line = line + ');'
		self.print_line(0, line)

def hpmain(document, output_dir):
	cw = C_READER_HEADER(document, output_dir)
	return cw.walk()
