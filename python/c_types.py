from hotpot.hotpot.hp_config import *
from hotpot.hotdata.syntactic_node import *
from common.common import *

def on_const(const):
	print_line(0, '#define\t' + const['identifier'] + ' ' + str(get_val(const['val'], None)))

def on_typedef(typedef):
	print_line(0, 'typedef ' + get_type(typedef['type'], None) + ' ' + typedef['name'] + ';')

def on_enum(enum):
	print_line(0, 'typedef enum _' + enum['name'])
	print_line(0, '{')
	for value in enum['enum_def_list']:
		print_line(1, value['identifier'] + ' = ' + str(get_val(value['val'], None)))

	print_line(0, '}' + enum['name'] + ';\n')
	counter = enum['name'] + '_NUM'
	for value in enum['type_annotations']['ta_list']:
		if(value['type'] == E_TA_COUNTER):
			counter = value['val']['val']['str']

	print_line(0, '#define ' + counter + ' ' + str(len(enum['enum_def_list'])));

def	on_object(type_name, object):
	print_line(0, 'typedef ' + type_name + ' _' + object['name'])
	print_line(0, '{')
	for value in object['field_list']['field_list']:
		if(value['type']['type'] == E_SNT_CONTAINER):
			if(value['type']['ct'] == E_CT_VECTOR):
				print_line(0, '\thpuint32 ' + value['args']['arg_list'][2]['ot'] + ';')

		line = get_type(value['type'], value['args']) + ' ' + value['identifier']
		if(value['type']['type'] == E_SNT_CONTAINER):
			if(value['type']['ct'] == E_CT_VECTOR):
				line = line + '[' + value['args']['arg_list'][1]['ot'] + ']'
			elif(value['type']['ct'] == E_CT_STRING):
				line = line + '[' + value['args']['arg_list'][0]['ot'] + ']'
		print_line(1, line + ';')
	print_line(0, '}' + object['name'] + ';\n')

od = './'
def hpmain(document):
	print_file_prefix()

	ofile_name = document['file_name'].strip('.hd') + '.h'
	fout = open(od + ofile_name, "w")

	file_tag = '_H_'
	for i in range(0, len(document['file_name'])):
		c = document['file_name'][i] 
		if(((c >= 'a') and ( c <= 'z')) or ((c >= 'A') and ( c <= 'Z')) or ((c >= '0') and ( c <= '9'))):
			file_tag += c
		else:
			file_tag += '_'

	file_tag +=  '_TYPES'
	print_line(0, '#ifndef ' + file_tag)
	print_line(0, '#define ' + file_tag)


	print_line(0, '#include "hotpot/hp_platform.h"\n')

	for value in document['definition_list']:
		if(value['type'] == E_DT_CONST):
			on_const(value['definition']['de_const'])
		elif(value['type'] == E_DT_TYPEDEF):
			on_typedef(value['definition']['de_typedef'])
		elif(value['type'] == E_DT_ENUM):
			on_enum(value['definition']['de_enum'])
		elif(value['type'] == E_DT_STRUCT):
			on_object('struct', value['definition']['de_struct'])
		elif(value['type'] == E_DT_UNION):
			on_object('union', value['definition']['de_union'])
	print_line(0, '#endif//' + file_tag)
	fout.close()
	return True
