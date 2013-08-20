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

def hpmain(document, output_dir):
	cw = C_READER(document, output_dir)
	return cw.walk()
'''

function on_struct(object)
	t = 0;
	line = 'HP_ERROR_CODE read_' .. object.name .. '(HPAbstractReader *self, ' .. object.name .. ' *data'
	for key, value in pairs(object.parameters.par_list) do
		line = line .. ' , '
		line = line .. get_type(value.type, nil)
		line = line .. ' *' .. value.identifier
	end
	line = line .. ')'
	print_line(t, line)
	print_line(t, '{')
	t = t + 1
	print_line(t, 'if(read_struct_begin(self, "' .. object.name .. '") != E_HP_NOERROR) goto ERROR_RET;')
	for key, value in pairs(object.field_list.field_list) do
		if(value.condition.empty == false)then			
			if(value.condition.exp.oper == E_EO_AND)then
				oper = '&'
			elseif(value.condition.exp.oper == E_EO_EQUAL)then
				oper = '=='
			end
			if(value.condition.exp.neg)then
				print_line(t, 'if (!(' .. get_val(value.condition.exp.op0, object) .. ' ' .. oper .. ' '.. get_val(value.condition.exp.op1, object) .. '))')
			else
				print_line(t, 'if (' .. get_val(value.condition.exp.op0, object) .. ' ' .. oper .. ' ' .. get_val(value.condition.exp.op1, object) .. ')')
			end
			print_line(t, '{')
			t = t + 1
		end

		if(value.type.type == E_SNT_CONTAINER)then
			if(value.type.ct == E_CT_VECTOR)then
				print_line(t, 'if(read_counter(self, "' .. value.args.arg_list[3].ot .. '", &data->' .. value.args.arg_list[3].ot .. ') != E_HP_NOERROR) goto ERROR_RET;');
				print_line(t, 'if(read_field_begin(self, "' .. value.identifier .. '") != E_HP_NOERROR) goto ERROR_RET;')
				print_line(t, '{')
				t = t + 1
				print_line(t, 'hpuint32 i;')
				print_line(t, 'if(read_vector_begin(self) != E_HP_NOERROR) goto ERROR_RET;')
				print_line(t, 'for(i = 0; i < data->' .. value.args.arg_list[3].ot .. '; ++i)')
				print_line(t, '{')
				t = t + 1
				print_line(t, 'if(read_vector_item_begin(self, i) != E_HP_NOERROR) goto ERROR_RET;')
				line = 'if(read_'
				line = line .. get_type(value.type, value.args)
				line = line .. '(self, &data->' .. value.identifier .. '[i]'
				for i=4, #value.args.arg_list do
					line = line .. ', ' .. get_symbol_access(value.args.arg_list[i].ot, object)
				end
				line = line .. ') != E_HP_NOERROR) goto ERROR_RET;'
				print_line(t, line)
				print_line(t, 'if(read_vector_item_end(self, i) != E_HP_NOERROR) goto ERROR_RET;')
				t = t - 1
				print_line(t, '}')
				print_line(t, 'if(read_vector_end(self) != E_HP_NOERROR) goto ERROR_RET;')
				t = t - 1
				print_line(t, '}')
				print_line(t, 'if(read_field_end(self, "' .. value.identifier .. '") != E_HP_NOERROR) goto ERROR_RET;')
			elseif(value.type.ct == E_CT_STRING)then
				print_line(t, 'if(read_field_begin(self, "' .. value.identifier .. '") != E_HP_NOERROR) goto ERROR_RET;')
				print_line(t, 'if(read_string(self, data->' .. value.identifier .. ', ' .. get_type(value.args.arg_list[1]) .. ') != E_HP_NOERROR) goto ERROR_RET;')
				print_line(t, 'if(read_field_end(self, "' .. value.identifier .. '") != E_HP_NOERROR) goto ERROR_RET;')
			end
		else
			print_line(t, 'if(read_field_begin(self, "' .. value.identifier .. '") != E_HP_NOERROR) goto ERROR_RET;')
			line = 'if(read_' .. get_type(value.type, value.args) .. '(self, &data->' .. value.identifier
				for ak, av in pairs(value.args.arg_list) do
					line = line .. ', ' .. get_symbol_access(av.ot, object)
				end
			line = line .. ') != E_HP_NOERROR) goto ERROR_RET;'
			print_line(t, line)
			print_line(t, 'if(read_field_end(self, "' .. value.identifier .. '") != E_HP_NOERROR) goto ERROR_RET;')
		end

		if(value.condition.empty == false)then
			t = t - 1
			print_line(t, '}')
		end
	end
	print_line(t, 'if(read_struct_end(self, "' .. object.name .. '") != E_HP_NOERROR) goto ERROR_RET;')
	print_line(1, 'return E_HP_NOERROR;')
	print_line(0, 'ERROR_RET:')
	print_line(1, 'return E_HP_ERROR;')

	t = t - 1
	print_line(t, '}')
end


function on_union(object)
	t = 0;
	line = 'HP_ERROR_CODE read_' .. object.name .. '(HPAbstractReader *self, ' .. object.name .. ' *data'
	for key, value in pairs(object.parameters.par_list) do
		line = line .. ' , '
		line = line .. ' ' .. get_type(value.type, nil)
		line = line .. ' ' .. get_symbol_access_by_type_prefix_reverse(value.identifier, value.type) .. value.identifier
	end
	line = line .. ')'
	print_line(t, line)
	print_line(t, '{')
	t = t + 1
	print_line(t, 'if(read_struct_begin(self, "' .. object.name .. '") != E_HP_NOERROR) goto ERROR_RET;')

	sw = 's'
	for key, value in pairs(object.ta.ta_list) do
		if(value.type == E_TA_SWITCH)then
			sw = value.val.val.identifier
		end
	end

	print_line(t, 'switch(' .. get_symbol_access(sw, object) .. ')')
	print_line(t, '{')
	t = t + 1
	for key, value in pairs(object.field_list.field_list) do
		print_line(t, 'case ' .. get_val(value.condition.exp.op1, object) .. ':')
		print_line(t, '{')
		t = t + 1
		if(value.type.type == E_SNT_CONTAINER)then
			if(value.type.ct == E_CT_VECTOR)then
				print_line(t, 'if(read_counter(self, "' .. value.args.arg_list[3].ot .. '", &data->' .. value.args.arg_list[3].ot .. ') != E_HP_NOERROR) goto ERROR_RET;');
				print_line(t, 'if(read_field_begin(self, "' .. value.identifier .. '") != E_HP_NOERROR) goto ERROR_RET;')
				print_line(t, '{')
				t = t + 1
				print_line(t, 'hpuint32 i;')
				print_line(t, 'if(read_vector_begin(self) != E_HP_NOERROR) goto ERROR_RET;')
				print_line(t, 'for(i = 0; i < data->' .. value.args.arg_list[3].ot .. '; ++i)')
				print_line(t, '{')
				t = t + 1
				print_line(t, 'if(read_vector_item_begin(self, i) != E_HP_NOERROR) goto ERROR_RET;')
				line = 'if(read_'
				line = line .. get_type(value.type, value.args)
				line = line .. '(self, &data->' .. value.identifier .. '[i]'
				for i=4, #value.args.arg_list do
					line = line .. ', ' .. get_symbol_access(value.args.arg_list[i].ot, object)
				end
				line = line .. ') != E_HP_NOERROR) goto ERROR_RET;'
				print_line(t, line)
				print_line(t, 'if(read_vector_item_end(self, i) != E_HP_NOERROR) goto ERROR_RET;')
				t = t - 1
				print_line(t, '}')
				print_line(t, 'if(read_vector_end(self) != E_HP_NOERROR) goto ERROR_RET;')
				t = t - 1
				print_line(t, '}')
				print_line(t, 'if(read_field_end(self, "' .. value.identifier .. '") != E_HP_NOERROR) goto ERROR_RET;')
			elseif(value.type.ct == E_CT_STRING)then
				print_line(t, 'if(read_field_begin(self, "' .. value.identifier .. '") != E_HP_NOERROR) goto ERROR_RET;')
				print_line(t, 'if(read_string(self, data->' .. value.identifier .. ', ' .. get_type(value.args.arg_list[1]) .. ') != E_HP_NOERROR) goto ERROR_RET;')
				print_line(t, 'if(read_field_end(self, "' .. value.identifier .. '") != E_HP_NOERROR) goto ERROR_RET;')
			end
		else
			print_line(t, 'if(read_field_begin(self, "' .. value.identifier .. '") != E_HP_NOERROR) goto ERROR_RET;')
			line = 'if(read_' .. get_type(value.type, value.args) .. '(self, &data->' .. value.identifier
				for ak, av in pairs(value.args.arg_list) do
					line = line .. ', ' .. get_symbol_access(av.ot, object)
				end
			line = line .. ') != E_HP_NOERROR) goto ERROR_RET;'
			print_line(t, line)
			print_line(t, 'if(read_field_end(self, "' .. value.identifier .. '") != E_HP_NOERROR) goto ERROR_RET;')
		end
		print_line(t, 'break;')
		t = t - 1
		print_line(t, '}')
	end
	t = t - 1
	print_line(t, '}')
	print_line(t, 'if(read_struct_end(self, "' .. object.name .. '") != E_HP_NOERROR) goto ERROR_RET;')
	print_line(1, 'return E_HP_NOERROR;')
	print_line(0, 'ERROR_RET:')
	print_line(1, 'return E_HP_ERROR;')
	t = t - 1
	print_line(t, '}')
end
function main(document)
	print_file_prefix()

	print_line(0, '#include "hotpot/hp_platform.h"')
	print_line(0, '#include "hotpot/hp_error_code.h"')
	print_line(0, '#include "hotprotocol/hp_abstract_reader.h"')
	print_line(0, '#include <string.h>')

	if(ifiles ~= nil) then
		for k, v in pairs(ifiles) do
			print_line(0, '#include "' .. v .. '"')
		end
	end

	for key, value in pairs(document['definition_list']) do
		if(value.type == E_DT_ENUM)then
			on_enum(value.definition.de_enum)
		elseif(value.type == E_DT_STRUCT)then
			on_struct(value.definition.de_struct)
		elseif(value.type == E_DT_UNION)then
			on_union(value.definition.de_union)
		end
	end
end

main(document)
'''
