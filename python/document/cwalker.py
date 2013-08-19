from hotpot.hotpot.hp_config import *
from hotpot.hotdata.syntactic_node import *
from document.walker import *
import sys
import os

class CWalker(Walker):
	def __init__(self, document):
		Walker.__init__(self, document)

	def get_file_tag(self, filename):
		file_tag = ''
		for i in range(0, len(filename)):
			c = filename[i] 
			if(((c >= 'a') and ( c <= 'z')) or ((c >= 'A') and ( c <= 'Z')) or ((c >= '0') and ( c <= '9'))):
				file_tag += c
			else:			
				file_tag += '_'
		return file_tag


	def get_symbol_access_by_type_suffix(self, identifier, type):
		if(type['type'] == E_SNT_CONTAINER):
			if(type['ct'] == E_CT_VECTOR):
				return '[i]'
		return ''

	def get_symbol_access_by_type_prefix(self, identifier, type, args):
		if(type['type'] == E_SNT_SIMPLE):
			return ''
		elif(type['type'] == E_SNT_CONTAINER):
			if(type['ct'] == E_CT_VECTOR):
				if(args['arg_list'][0]['type'] == E_SNT_SIMPLE):
					return ''
				else:
					return '&'
			elif(type['ct'] == E_CT_STRING):
				return ''
		elif(type['type'] == E_SNT_REFER):
			if(self.enum_list[type['ot']] == None):
				return '&'
			else:
				return ''
		return ''

	def get_symbol_access_by_type_prefix_reverse(self, identifier, type, args):
		if(type['type'] == E_SNT_SIMPLE):
			return ''
		elif(type['type'] == E_SNT_CONTAINER):
			if(type['ct'] == E_CT_VECTOR):
				if(args['arg_list'][0]['type'] == E_SNT_SIMPLE):
					return ''
				else:
					return '*'
			elif(type['ct'] == E_CT_STRING):
				return ''
		elif(type['type'] == E_SNT_REFER):
			if(self.enum_list[type['ot']] == None):
				return '*'
			else:
				return ''
		return ''

	def get_symbol_access(self, identifier, obj):
		if(obj != None):
			for v in obj['parameters']['par_list']:
				if(v['identifier'] == identifier):
					prefix = self.get_symbol_access_by_type_prefix(identifier, v['type'], v['args'])
					suffix = self.get_symbol_access_by_type_suffix(identifier, v['type'], v['args'])
					return prefix + identifier + suffix

			for v in obj['field_list']['field_list']:
				if(v['identifier'] == identifier):
					prefix = self.get_symbol_access_by_type_prefix(identifier, v.type, v.args)
					suffix = self.get_symbol_access_by_type_suffix(identifier, v.type, v.args)
				return prefix + 'data->' + identifier + suffix

		return identifier

	def get_val(self, val, obj):
		if val['type'] == E_SNVT_IDENTIFIER :
			return self.get_symbol_access(val['val']['identifier'], obj)
		elif val['type'] == E_SNVT_CHAR:
			return "'" + val['val']['c'] + "'"
		elif val['type'] == E_SNVT_DOUBLE:
			return val['val']['d']
		elif val['type'] == E_SNVT_BOOL:
			return val['val']['b']
		elif val['type'] == E_SNVT_STRING:
			return '"' + val['val']['str'] + '"'
		elif val['type'] == E_SNVT_INT64:
			return val['val']['i64']
		elif val['type'] == E_SNVT_UINT64:
			return val['val']['ui64']
		elif val['type'] == E_SNVT_HEX_INT64:
			return val['val']['hex_i64']
		elif val['type'] == E_SNVT_HEX_UINT64:
			return val['val']['hex_ui64']

	def get_type(self, type, args):
		if(type['type'] == E_SNT_SIMPLE):
			if(type['st'] == E_ST_CHAR):
				return 'hpchar'
			elif(type['st'] == E_ST_DOUBLE):
				return 'hpdouble'
			elif(type['st'] == E_ST_BOOL):
				return 'hpbool'
			elif(type['st'] == E_ST_INT8):
				return 'hpint8'
			elif(type['st'] == E_ST_INT16):
				return 'hpint16'
			elif(type['st'] == E_ST_INT32):
				return 'hpint32'
			elif(type['st'] == E_ST_INT64):
				return 'hpint64'
			elif(type['st'] == E_ST_UINT8):
				return 'hpuint8'
			elif(type['st'] == E_ST_UINT16):
				return 'hpuint16'
			elif(type['st'] == E_ST_UINT32):
				return 'hpuint32'
			elif(type['st'] == E_ST_UINT64):
				return 'hpuint64'
		elif(type['type'] == E_SNT_CONTAINER):
			if(type['ct'] == E_CT_VECTOR):
				return args['arg_list'][0]['ot']
			elif(type['ct'] == E_CT_STRING):
				return 'hpchar'
		elif(type['type'] == E_SNT_REFER):
			return type['ot'];
