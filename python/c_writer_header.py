from document.cwalker import *

class C_WRITER_HEADER(CWalker):
	def __init__(self, document, target_dir):
		CWalker.__init__(self, document, target_dir)
		self.file_tag = '_H_' + self.get_file_tag(document['file_name']) + '_WRITER_HEADER'
	
	def on_document_begin(self, document):
		ofile_name = self.target_dir + '/' + self.file_name + '_writer.h'
		self.fout = open(ofile_name, "w")

		self.print_file_prefix()
		self.print_line(0, '#ifndef ' + self.file_tag)
		self.print_line(0, '#define ' + self.file_tag)
		self.print_line(0, '#include "hotplatform/hp_platform.h"')
		self.print_line(0, '#include "hotlib/hp_error_code.h"')
		self.print_line(0, '#include "hotprotocol/hp_abstract_writer.h"')
		self.print_line(0, '#include "' + self.file_name + '.h"')


	def on_document_end(self, document):
		self.print_line(0, '#endif//' + self.file_tag)
		self.fout.close()

	def on_import(self, de_import):
		self.print_line(0, '#include "' + de_import['package_name'].rstrip('\.hd') + '_writer.h"')

	def get_enum_header(self, enum):
		return 'HP_ERROR_CODE write_' + enum['name'] + '(HPAbstractWriter *self, ' + enum['name'] + ' data)'

	def get_enum_name_header(self, enum):
		return 'HP_ERROR_CODE write_' + enum['name'] + '_name(HPAbstractWriter *self, ' + enum['name'] + ' data)'

	def get_enum_number_header(self, enum):
		return 'HP_ERROR_CODE write_' + enum['name'] + '_number(HPAbstractWriter *self, ' + enum['name'] + ' data)'

	def on_enum_begin(self, enum):
		self.print_line(0, self.get_enum_name_header(enum) + ';')
		self.print_line(0, self.get_enum_number_header(enum) + ';')
		self.print_line(0, self.get_enum_header(enum) + ';')

	def get_struct_header(self, struct):
		line = 'HP_ERROR_CODE write_' + struct['name'] + '(HPAbstractWriter *self, const ' + struct['name'] + ' *data'
		for value in struct['parameters']['par_list']:
			line = line + ' , '
			t = self.get_type(value['type'], None)
			if((self.find_symbol(t) == Walker.EN_HST_STRUCT) or (self.find_symbol(t) == Walker.EN_HST_UNION)):
				t = 'const ' + t + '*'
			line = line + t + ' ' + value['identifier']
		line = line + ')'
		return line

	def on_struct_begin(self, struct):
		self.print_line(0, self.get_struct_header(struct) + ';')

	def get_union_header(self, union):
		line = 'HP_ERROR_CODE write_' + union['name'] + '(HPAbstractWriter *self, const ' + union['name'] + ' *data'
		for value in union['parameters']['par_list']:
			line = line + ' , '
			t = self.get_type(value['type'], None)
			if((self.find_symbol(t) == Walker.EN_HST_STRUCT) or (self.find_symbol(t) == Walker.EN_HST_UNION)):
				t = 'const ' + t + '*'
			line = line + t + ' ' + value['identifier']
		line = line + ')'
		return line

	def on_union_begin(self, union):
		self.print_line(0, self.get_union_header(union) + ';')

def hpmain(document, output_dir):
	cw = C_WRITER_HEADER(document, output_dir)
	return cw.walk()
