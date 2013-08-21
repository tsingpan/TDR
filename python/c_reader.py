from c_reader_header import *

class C_READER(C_READER_HEADER):
	def __init__(self, document, output_dir):
		C_READER_HEADER.__init__(self, document, output_dir)
	
	def on_document_begin(self, document):
		ofile_name = self.output_dir + '/' + document['file_name'].rstrip('.hd') + '_reader.c'
		self.fout = open(ofile_name, "w")

		self.print_file_prefix()
		self.print_line(0, '#include "hotpot/hp_platform.h"')
		self.print_line(0, '#include "' + document['file_name'].rstrip('.hd') + '.h"')
		self.print_line(0, '#include "' + document['file_name'].rstrip('.hd') + '_reader.h"')

	def on_document_end(self, document):
		self.fout.close()

	def create_enum_name(self, enum):
		self.print_line(0, self.get_enum_name_header(enum))
		self.print_line(0, '{')
		self.print_line(1, 'char name[MAX_IDENTIFIER_LENGTH];')
		self.print_line(1, 'if(read_enum_name(self, name, MAX_IDENTIFIER_LENGTH) != E_HP_NOERROR) goto ERROR_RET;')
		for value in enum['enum_def_list']:
			self.print_line(1, 'if(strcmp(name, \"' + value['identifier'] + '\") == 0)')
			self.print_line(1, '{')
			self.print_line(2, '*data = ' + value['identifier'] + ';')
			self.print_line(2, 'goto done;')
			self.print_line(1, '}')
		self.print_line(0, 'ERROR_RET:')
		self.print_line(1, 'return E_HP_ERROR;')
		self.print_line(0, 'done:')
		self.print_line(1, 'return E_HP_NOERROR;')
		self.print_line(0, '}')

	def create_enum_number(self, enum):
		self.print_line(0, self.get_enum_number_header(enum))
		self.print_line(0, '{')
		self.print_line(1, 'return read_enum_number(self, (hpint32*)data);')
		self.print_line(0, '}')


	def on_enum_begin(self, enum):
		self.create_enum_name(enum)
		self.create_enum_number(enum)

		self.print_line(0, self.get_enum_header(enum))
		self.print_line(0, '{')
		self.print_line(1, 'if(read_' + enum['name'] + '_name(self, data) != E_HP_NOERROR) goto ERROR_RET;')
		self.print_line(1, 'if(read_' + enum['name'] + '_number(self, data) != E_HP_NOERROR) goto ERROR_RET;')
		self.print_line(1, 'return E_HP_NOERROR;')
		self.print_line(0, 'ERROR_RET:')
		self.print_line(1, 'return E_HP_ERROR;')
		self.print_line(0, '}')

	def on_struct_begin(self, struct):
		self.print_line(0, self.get_struct_header(struct))
		self.print_line(0, '{')
		self.print_line(1, 'if(read_struct_begin(self, "' + struct['name'] + '") != E_HP_NOERROR) goto ERROR_RET;')

	def on_struct_field(self, struct_field):
		t = 1
		if(struct_field['condition']['empty'] == False):
			if(struct_field['condition']['exp']['oper'] == E_EO_AND):
				oper = '&'
			elif(struct_field['condition']['exp']['oper'] == E_EO_EQUAL):
				oper = '=='

			op0str = str(self.get_val(struct_field['condition']['exp']['op0']))
			op1str = str(self.get_val(struct_field['condition']['exp']['op1']))

			if(struct_field['condition']['exp']['neg']):
				self.print_line(t, 'if(!(' +  op0str + ' ' + oper + ' '+ op1str + '))')
			else:
				self.print_line(t, 'if(' + op0str + ' ' + oper + ' ' + op1str + ')')

			self.print_line(1, '{')
			t = t + 1
		#condition

		if(struct_field['type']['type'] == E_SNT_CONTAINER):
			if(struct_field['type']['ct'] == E_CT_VECTOR):
				self.print_line(t, 'if(read_counter(self, "' + struct_field['args']['arg_list'][2]['ot'] + '", &data->' + struct_field['args']['arg_list'][2]['ot'] + ') != E_HP_NOERROR) goto ERROR_RET;');
				self.print_line(t, 'if(read_field_begin(self, "' + struct_field['identifier'] + '") != E_HP_NOERROR) goto ERROR_RET;')
				self.print_line(t, '{')
				t = t + 1
				self.print_line(t, 'hpuint32 i;')
				self.print_line(t, 'if(read_vector_begin(self) != E_HP_NOERROR) goto ERROR_RET;')
				self.print_line(t, 'for(i = 0; i < data->' + struct_field['args']['arg_list'][2]['ot'] + '; ++i)')
				self.print_line(t, '{')
				t = t + 1
				self.print_line(t, 'if(read_vector_item_begin(self, i) != E_HP_NOERROR) goto ERROR_RET;')
				line = 'if(read_'
				line = line + self.get_type(struct_field['type'], struct_field['args'])
				line = line + '(self, &data->' + struct_field['identifier'] + '[i]'
				line = line + ') != E_HP_NOERROR) goto ERROR_RET;'

				self.print_line(t, line)
				self.print_line(t, 'if(read_vector_item_end(self, i) != E_HP_NOERROR) goto ERROR_RET;')
				t = t - 1
				self.print_line(t, '}')
				self.print_line(t, 'if(read_vector_end(self) != E_HP_NOERROR) goto ERROR_RET;')
				t = t - 1
				self.print_line(t, '}')
				self.print_line(t, 'if(read_field_end(self, "' + struct_field['identifier'] + '") != E_HP_NOERROR) goto ERROR_RET;')
			elif(struct_field['type']['ct'] == E_CT_STRING):
				self.print_line(t, 'if(read_field_begin(self, "' + struct_field['identifier'] + '") != E_HP_NOERROR) goto ERROR_RET;')
				self.print_line(t, 'if(read_string(self, data->' + struct_field['identifier'] + ', ' + struct_field['args']['arg_list'][0]['ot'] + ') != E_HP_NOERROR) goto ERROR_RET;')
				self.print_line(t, 'if(read_field_end(self, "' + struct_field['identifier'] + '") != E_HP_NOERROR) goto ERROR_RET;')
		else:
			self.print_line(t, 'if(read_field_begin(self, "' + struct_field['identifier'] + '") != E_HP_NOERROR) goto ERROR_RET;')
			line = 'if(read_' + self.get_type(struct_field['type'], struct_field['args']) + '(self, &data->' + struct_field['identifier']
			for arg in struct_field['args']['arg_list']:
				line = line + ', data->' + arg['ot']
			line = line + ') != E_HP_NOERROR) goto ERROR_RET;'
			self.print_line(t, line)
			self.print_line(t, 'if(read_field_end(self, "' + struct_field['identifier'] + '") != E_HP_NOERROR) goto ERROR_RET;')

		
		#condition
		if(struct_field['condition']['empty'] == False):
			self.print_line(1, '}')

	def on_struct_end(self, struct):
		self.print_line(1, 'if(read_struct_end(self, "' + struct['name'] + '") != E_HP_NOERROR) goto ERROR_RET;')
		self.print_line(1, 'return E_HP_NOERROR;')
		self.print_line(0, 'ERROR_RET:')
		self.print_line(1, 'return E_HP_ERROR;')
		self.print_line(0, '}')

	def on_union_begin(self, union):
		self.print_line(0, self.get_union_header(union))
		self.print_line(0, '{')
		self.print_line(1, 'if(read_struct_begin(self, \"' + union['name'] + '\") != E_HP_NOERROR) goto ERROR_RET;')

		sw = 's'
		for value in union['ta']['ta_list']:
			if(value['type'] == E_TA_SWITCH):
				sw = value['val']['val']['identifier']

		self.print_line(1, 'switch(data->' + sw + ')')
		self.print_line(1, '{')

	def on_union_field(self, union_field):
		self.print_line(2, 'case ' + self.get_val(union_field['condition']['exp']['op1']) + ':')
		self.print_line(2, '{')
		if(union_field['type']['type'] == E_SNT_CONTAINER):
			if(union_field['type']['ct'] == E_CT_STRING):
				self.print_line(3, 'if(read_field_begin(self, "' + union_field['identifier'] + '") != E_HP_NOERROR) goto ERROR_RET;')
				self.print_line(3, 'if(read_string(self, data->' + union_field['identifier'] + ', ' + union_field['args']['arg_list'][0]['ot'] + ') != E_HP_NOERROR) goto ERROR_RET;')
				self.print_line(3, 'if(read_field_end(self, "' + union_field['identifier'] + '") != E_HP_NOERROR) goto ERROR_RET;')
		else:
			self.print_line(3, 'if(read_field_begin(self, "' + union_field['identifier'] + '") != E_HP_NOERROR) goto ERROR_RET;')
			line = 'if(read_' + self.get_type(union_field['type'], union_field['args']) + '(self, &data->' + union_field['identifier']
			for arg in union_field['args']['arg_list']:
				line = line + ', data->' + arg['ot']
			line = line + ') != E_HP_NOERROR) goto ERROR_RET;'
			self.print_line(3, line)
			self.print_line(3, 'if(read_field_end(self, "' + union_field['identifier'] + '") != E_HP_NOERROR) goto ERROR_RET;')

		self.print_line(3, 'break;')
		self.print_line(2, '}')

	def on_union_end(self, union):
		self.print_line(1, '}')
		self.print_line(1, 'if(read_struct_end(self, "' + union['name'] + '") != E_HP_NOERROR) goto ERROR_RET;')
		self.print_line(1, 'return E_HP_NOERROR;')
		self.print_line(0, 'ERROR_RET:')
		self.print_line(1, 'return E_HP_ERROR;')
		self.print_line(0, '}')

def hpmain(document, output_dir):
	cw = C_READER(document, output_dir)
	return cw.walk()
