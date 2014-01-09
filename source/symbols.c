#include "symbols.h"
#include <string.h>
#include "error/error_code_types.h"

const SYMBOL* symbols_search_identifier(SYMBOLS *self, const tbytes* tok_identifier, tbool back_searching)
{
	char name[TLIBC_MAX_IDENTIFIER_LENGTH];
	memcpy(name, tok_identifier->ptr, tok_identifier->len);
	name[tok_identifier->len] = 0;
	
	return symbols_search_string(self, name, back_searching);
}

tint32 symbols_save(SYMBOLS *self, const char *name, const SYMBOL *symbol)
{
	char global_name[TLIBC_MAX_IDENTIFIER_LENGTH];

	if(self->domain[0])
	{
		snprintf(global_name, TLIBC_MAX_IDENTIFIER_LENGTH, "%s:%s", self->domain, name);
		global_name[TLIBC_MAX_IDENTIFIER_LENGTH - 1] = 0;
	}
	else
	{
		snprintf(global_name, TLIBC_MAX_IDENTIFIER_LENGTH, "%s", name);
		global_name[TLIBC_MAX_IDENTIFIER_LENGTH - 1] = 0;
	}

	//printf("save %s : %x\n", global_name, symbol);
	if(!trie_store_if_absent(self->symbols, global_name, symbol))
	{
		return E_TD_ERROR;
	}
	return E_TD_NOERROR;
}

void symbols_domain_begin(SYMBOLS *self, const tbytes *tok_identifier)
{
	memcpy(self->domain, tok_identifier->ptr, tok_identifier->len);
	self->domain[tok_identifier->len] = 0;
}

void symbols_domain_end(SYMBOLS *self)
{
	self->domain[0] = 0;
}


const SYMBOL* symbols_search_string(SYMBOLS *self, const char* name, tbool back_searching)
{
	const SYMBOL *symbol;
	char global_name[TLIBC_MAX_IDENTIFIER_LENGTH];

	if(self->domain[0])
	{
		snprintf(global_name, TLIBC_MAX_IDENTIFIER_LENGTH, "%s:%s", self->domain, name);
		global_name[TLIBC_MAX_IDENTIFIER_LENGTH - 1] = 0;
	}
	else
	{
		snprintf(global_name, TLIBC_MAX_IDENTIFIER_LENGTH, "%s", name);
		global_name[TLIBC_MAX_IDENTIFIER_LENGTH - 1] = 0;
	}
	
	if(!trie_retrieve(self->symbols, global_name, (void**)&symbol))
	{
		if(!back_searching)
		{
			goto ERROR_RET;	
		}
		if(!trie_retrieve(self->symbols, name, (void**)&symbol))
		{
			goto ERROR_RET;
		}
	}

	return symbol;
ERROR_RET:
	return NULL;
}
