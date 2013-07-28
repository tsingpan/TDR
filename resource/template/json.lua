JSON = loadfile(root_dir .. "resource/template/json_lib.lua")();
print(JSON:encode(hd))
for key, value in pairs(hd) do
	print(key)
end
