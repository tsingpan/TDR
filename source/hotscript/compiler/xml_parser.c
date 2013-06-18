#include "xml_parser.h"
#include "xml_y.h"
#include "xml_l.h"
#include "hotpot/hp_error.h"

void debug_print_tab(hpuint32 n)
{
	hpuint32 i;
	for(i = 0;i < n;++i)
	{
		printf("\t");
	}
}
void debug_print(const XML_TREE *tree, hpint32 index, hpuint32 level)
{
	hpuint32 i;
	debug_print_tab(level);
	printf("%s\n", tree->element_list[index].name);
	if(tree->element_list[index].first_child_index != -1)
	{
		debug_print_tab(level);
		printf("{\n");

		for(i = tree->element_list[index].first_child_index; i != -1; i = tree->element_list[i].next_sibling_index)
		{
			debug_print(tree, i, level + 1);
		}
		debug_print_tab(level);
		printf("{\n");
	}
}

hpint32 xml_parser(XML_PARSER *self, FILE *fin)
{
	hpint32 ret;

	self->stack_num = 0;
	self->result = HP_INVALID_ERROR_CODE;
	self->tree.element_list_num = 0;
	self->stack[0].first_element_index = -1;
	self->stack[0].last_element_index = -1;
	
	yylex_init_extra(&self->scanner, &self->scanner);
	self->bs = yy_create_buffer(fin, YY_BUF_SIZE, self->scanner);
	self->bs->yy_bs_column = 1;
	yy_switch_to_buffer(self->bs, self->scanner);
	ret = yyparse(&self->scanner);
	if(ret == 0)
	{
		self->result = E_HP_NOERROR;
	}

	printf("----------------------------------------------------------\n");
	debug_print(&self->tree, self->tree.element_list_num - 1, 0);
	return self->result;
}
