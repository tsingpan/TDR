<?php
	$json_content = file_get_contents($argv[1]);
	$json = json_decode($json_content, true);

	function do_field($field)
	{
		echo "\t\tprotected $" . $field['name'] . ";\n";
	}

	function get_php_type($type)
	{
		if(isset($type['ContainerType']))
		{
			return 'array';
		}
		else if(isset($type['ObjectType']))
		{
			return 'object';
		}
		else if(isset($type['SimpleType']))
		{
			if($type['SimpleType']['type'] == 'double')
			{
				return 'float';
			}
			else if($type['SimpleType']['type'] == 'char')
			{
				return 'string';
			}
			else
			{
				return 'integer';
			}
		}
	}

	function do_acccess($field_list)
	{
		echo "\t\tpublic function __get(\$property_name)\n";
		echo "\t\t{\n";
		echo "\t\t\treturn \$this->\$property_name;\n";
		echo "\t\t}\n";

		echo "\t\tpublic function __set(\$property_name, \$value)\n";
		echo "\t\t{\n";
		echo "\t\t\tswitch(\$property_name)\n";
		echo "\t\t\t{\n";
		
		foreach($field_list as $field)
		{
			echo "\t\t\t\tcase \"" . $field['name'] . "\":\n";
			echo "\t\t\t\t{\n";
			$t =  get_php_type($field['Type']);
			echo "\t\t\t\t\tif(!is_" . $t . "(\$value))\n";
			echo "\t\t\t\t\t{\n";
			echo "\t\t\t\t\t\tthrow new Exception(\$property_name . \" is not " . $t . "\");\n";
			echo "\t\t\t\t\t}\n";
			if($t == 'object')
			{
				echo "\t\t\t\t\tif(! \$value instanceof " . $field['Type']['ObjectType']['type'] . ")\n";
				echo "\t\t\t\t\t{\n";
				echo "\t\t\t\t\t\tthrow new Exception(\$property_name . \" is not " . $t . "\");\n";
				echo "\t\t\t\t\t}\n";
			}
			echo "\t\t\t\t}\n";
		}
		echo "\t\t\t}\n";
		echo "\t\t\t\$this->\$property_name = \$value;\n";
		echo "\t\t}\n";
	}
	echo "<?php\n";
	foreach($json['DefinitionList'] as $definition)
	{
			if(isset($definition['struct']))
			{
				$struct = $definition['struct'];
				echo "\n";
				echo "\tclass " . $struct['name'] . "\n";
				echo "\t{\n";
				foreach($struct['list'] as $field)
				{
					do_field($field);
				}
				echo "\n";
				do_acccess($struct['list']);
				echo "\t}\n";
			}

			if(isset($definition['union']))
			{
				$union = $definition['union'];
				echo "\n";
				echo "\tclass " . $union['name'] . "\n";
				echo "\t{\n";
				foreach($union['list'] as $field)
				{
					do_field($field);
				}
				echo "\n";
				do_acccess($union['list']);
				echo "\t}\n";
			}


			if(isset($definition['enum']))
			{
				$enum = $definition['enum'];
				echo "\n";
				echo "\tfinal class " . $enum['name'] . "\n";
				echo "\t{\n";
				foreach($enum['list'] as $enum_def)
				{
					echo "\t\tconst " . $enum_def['name'] . " = " . $enum_def['value'] . ";\n";
				}
				echo "\t}\n";
			}


			if(isset($definition['const']))
			{
					$const = $definition['const'];
					echo "\tdefine(\"" . $const['name'] . '", "' . $const['value'] . '");' . "\n";
			}
	}
	echo "?>\n";
?>
