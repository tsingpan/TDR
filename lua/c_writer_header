loadfile(lua_dir .. "lib/hotdata/syntactic_node.lua")();
loadfile(lua_dir .. "lib/common.lua")();

function on_enum_name(enum)
	print_line(0, 'HP_ERROR_CODE write_' .. enum.name .. '_name(HPAbstractWriter *self, const ' .. enum.name .. ' data);')
end

function on_enum_number(enum)
	print_line(0, 'HP_ERROR_CODE write_' .. enum.name .. '_number(HPAbstractWriter *self, const ' .. enum.name .. ' data);')
end


function on_enum(enum)
	
	on_enum_name(enum)
	on_enum_number(enum)

	print_line(0, 'HP_ERROR_CODE write_' .. enum.name .. '(HPAbstractWriter *self, const ' .. enum.name .. ' data);')


	enum_list[enum.name] = true
end


function on_struct(object)
	t = 0;
	line = 'HP_ERROR_CODE write_' .. object.name .. '(HPAbstractWriter *self, const ' .. object.name .. ' *data'
	for key, value in pairs(object.parameters.par_list) do
		line = line .. ' , '
		line = line .. get_type(value.type, nil)
		line = line .. ' const ' .. get_symbol_access_by_type_prefix_reverse(value.identifier, value.type) .. value.identifier
	end
	line = line .. ');'
	print_line(t, line)
end


function on_union(object)
	t = 0;
	line = 'HP_ERROR_CODE write_' .. object.name .. '(HPAbstractWriter *self, const ' .. object.name .. ' *data'
	for key, value in pairs(object.parameters.par_list) do
		line = line .. ' , '
		line = line .. 'const ' .. get_type(value.type, nil)
		line = line .. ' ' .. get_symbol_access_by_type_prefix_reverse(value.identifier, value.type) .. value.identifier
	end
	line = line .. ');'
	print_line(t, line)
end

function main(document)
	print_file_prefix()

	file_tag = document.file_name
	file_tag = '_H_' .. string.gsub(document.file_name, '[^a-zA-Z0-9]', '_') .. '_WRITER'
	io.write('#ifndef ' .. file_tag .. '\n')
	io.write('#define ' .. file_tag .. '\n')

	print_line(0, '#include "hotpot/hp_platform.h"')
	print_line(0, '#include "hotpot/hp_error_code.h"')
	print_line(0, '#include "hotprotocol/hp_abstract_writer.h"')

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
	io.write('#endif//' .. file_tag .. '\n')
end

main(document)

