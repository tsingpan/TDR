from document.dwalker import *

class C_TYPES(DWalker):
	def __init__(self, document, output_dir):
		DWalker.__init__(self, document, output_dir)
		self.file_tag = '_H_'
		for i in range(0, len(document['file_name'])):
			c = document['file_name'][i] 
			if(((c >= 'a') and ( c <= 'z')) or ((c >= 'A') and ( c <= 'Z')) or ((c >= '0') and ( c <= '9'))):
				self.file_tag += c
			else:
				self.file_tag += '_'
		self.file_tag +=  '_TYPES'
		

	def on_document_begin(self, document):
		self.print_file_prefix()
		self.print_line(0, '#ifndef ' + self.file_tag)
		self.print_line(0, '#define ' + self.file_tag)
		self.print_line(0, '#include "hotpot/hp_platform.h"\n')

	def on_document_end(self, document):
		self.print_line(0, '#endif//' + self.file_tag)

	def on_import(self, de_import):
		self.print_line(0, '#include "' + de_import['package_name'].rstrip('\.hd') + '.h"')

	def on_unix_comment(self, de_unix_comment):
		if(not de_unix_comment['empty']):
			self.print_line(0, '//' + de_unix_comment['text'])

	def on_const(self, const):
		self.print_line(0, '#define ' + const['identifier'] + ' ' + str(self.get_val(const['val'], None)))

	def on_typedef(self, typedef):
		self.print_line(0, 'typedef ' + self.get_type(typedef['type'], None) + ' ' + typedef['name'] + ';')

	def on_enum_begin(self, enum):
		self.print_line(0, 'typedef enum _' + enum['name'])
		self.print_line(0, '{')

	def on_enum_field(self, enum_field):
		self.print_line(1, enum_field['identifier'] + ' = ' + str(self.get_val(enum_field['val'], None)))

	def on_enum_end(self, enum):
		self.print_line(0, '}' + enum['name'] + ';')
		for value in enum['type_annotations']['ta_list']:
			if(value['type'] == E_TA_COUNTER):
				self.print_line(0, '#define ' + value['val']['val']['str'] + ' ' + str(len(enum['enum_def_list'])));

	def on_struct_begin(self, struct):
		self.print_line(0, 'typedef struct ' ' _' + struct['name'])
		self.print_line(0, '{')

	def on_struct_field(self, struct_field):
		if(struct_field['type']['type'] == E_SNT_CONTAINER):
			if(struct_field['type']['ct'] == E_CT_VECTOR):
				self.print_line(0, '\thpuint32 ' + struct_field['args']['arg_list'][2]['ot'] + ';')

		line = self.get_type(struct_field['type'], struct_field['args']) + ' ' + struct_field['identifier']
		if(struct_field['type']['type'] == E_SNT_CONTAINER):
			if(struct_field['type']['ct'] == E_CT_VECTOR):
				line = line + '[' + struct_field['args']['arg_list'][1]['ot'] + ']'
			elif(struct_field['type']['ct'] == E_CT_STRING):
				line = line + '[' + struct_field['args']['arg_list'][0]['ot'] + ']'
		self.print_line(1, line + ';')


	def on_struct_end(self, struct):
		self.print_line(0, '}' + struct['name'] + ';')

	def on_union_begin(self, union):
		self.print_line(0, 'typedef union ' ' _' + union['name'])
		self.print_line(0, '{')

	def on_union_field(self, union_field):
		if(union_field['type']['type'] == E_SNT_CONTAINER):
			if(union_field['type']['ct'] == E_CT_VECTOR):
				self.print_line(0, '\thpuint32 ' + union_field['args']['arg_list'][2]['ot'] + ';')

		line = self.get_type(union_field['type'], union_field['args']) + ' ' + union_field['identifier']
		if(union_field['type']['type'] == E_SNT_CONTAINER):
			if(union_field['type']['ct'] == E_CT_VECTOR):
				line = line + '[' + union_field['args']['arg_list'][1]['ot'] + ']'
			elif(union_field['type']['ct'] == E_CT_STRING):
				line = line + '[' + union_field['args']['arg_list'][0]['ot'] + ']'
		self.print_line(1, line + ';')

	def on_union_end(self,union):
		self.print_line(0, '}' + union['name'] + ';')

def hpmain(document, output_dir):
	cw = C_TYPES(document, output_dir)
	cw.walk()
