from hotpot.hotpot.hp_config import *
from hotpot.hotdata.syntactic_node import *
from common.common import *

def print_val(val):
	if val['type'] == E_SNVT_IDENTIFIER:
		print_line(0, val['val']['identifier'])
	elif val['type'] == E_SNVT_CHAR:
		print_line(0, "'")
		if(val['val']['c'] == '\n'):
			print_line(0, '\\n')
		else:
			print_line(0, val['val']['c'])
		print_line(0, "'")
	elif val['type'] == E_SNVT_DOUBLE:
		print_line(0, val['val']['d'])
	elif val['type'] == E_SNVT_BOOL:
		print_line(0, val['val']['b'])
	elif val['type'] == E_SNVT_STRING:
		#需要处理字符串中的转义
		print_line(0, '"')
		print_line(0, val['val']['str'])
		print_line(0, '"')
	elif val['type'] == E_SNVT_INT64:
		print_line(0, val['val']['i64'])
	elif val['type'] == E_SNVT_UINT64:
		print_line(0, val['val']['ui64'])
	elif val['type'] == E_SNVT_HEX_INT64:
		print_line(0, val['val']['hex_i64'])
	elif val['type'] == E_SNVT_HEX_UINT64:
		print_line(0, val['val']['hex_ui64'])

def on_const(const):
	print_line(0, '#define\t' + const['identifier'] + ' ' + str(get_val(const['val'], None)))


def print_type(type, args):
	if(type.type == E_SNT_SIMPLE):
		if(type.st == E_ST_CHAR):
			print_line(0, 'hpchar')
		elif(type.st == E_ST_DOUBLE):
			print_line(0, 'hpdouble')
		elif(type.st == E_ST_BOOL):
			print_line(0, 'hpbool')
		elif(type.st == E_ST_INT8):
			print_line(0, 'hpint8')
		elif(type.st == E_ST_INT16):
			print_line(0, 'hpint16')
		elif(type.st == E_ST_INT32):
			print_line(0, 'hpint32')
		elif(type.st == E_ST_INT64):
			print_line(0, 'hpint64')
		elif(type.st == E_ST_UINT8):
			print_line(0, 'hpuint8')
		elif(type.st == E_ST_UINT16):
			print_line(0, 'hpuint16')
		elif(type.st == E_ST_UINT32):
			print_line(0, 'hpuint32')
		elif(type.st == E_ST_UINT64):
			print_line(0, 'hpuint64')
	elif(type.type == E_SNT_CONTAINER):
		if(type.ct == E_CT_VECTOR):
			print_line(0, args.arg_list[1].ot)
		elif(type.ct == E_CT_STRING):
			print_line(0, 'hpchar')
	elif(type.type == E_SNT_REFER):
		print_line(0, type['ot']);

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

def hpmain(document):
	print_file_prefix()

	file_tag = document['file_name']
	file_tag = '_H_' + document['file_name'].replace('[^a-zA-Z0-9]', '_') + '_TYPES'
	print_line(0, '#ifndef ' + file_tag)
	print_line(0, '#define ' + file_tag)


	print_line(0, '#include "hotpot/hp_platform.h"\n')
#	if(ifiles != None):
#		for v in ifiles:
#			print_line(0, '#include "' + v + '"')
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
