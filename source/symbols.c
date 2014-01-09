#include "symbols.h"
#include <string.h>
#include "error/error_code_types.h"



const SYMBOL* symbols_find_by_string(SYMBOLS *self, const char* name)
{
	const SYMBOL *symbol;
	char global_name[TLIBC_MAX_IDENTIFIER_LENGTH * 2];

	if(self->domain[0])
	{
		snprintf(global_name, TLIBC_MAX_IDENTIFIER_LENGTH * 2, "%s:%s", self->domain, name);
	}
	else
	{
		snprintf(global_name, TLIBC_MAX_IDENTIFIER_LENGTH * 2, "%s", name);
	}

	if(!trie_retrieve(self->symbols, global_name, (void**)&symbol))
	{
		if(!trie_retrieve(self->symbols, name, (void**)&symbol))
		{
			return NULL;
		}
	}

	//printf("find: %s : %x\n", name, symbol);
	return symbol;
}

const SYMBOL* symbols_find(SYMBOLS *self, const tbytes* tok_identifier)
{
	char name[TLIBC_MAX_IDENTIFIER_LENGTH];
	memcpy(name, tok_identifier->ptr, tok_identifier->len);
	name[tok_identifier->len] = 0;
	return symbols_find_by_string(self, name);
}


const SYMBOL* symbols_find_by_string_local(SYMBOLS *self, const char* name)
{
	const SYMBOL *symbol;
	char global_name[TLIBC_MAX_IDENTIFIER_LENGTH * 2];

	if(self->domain[0])
	{
		snprintf(global_name, TLIBC_MAX_IDENTIFIER_LENGTH * 2, "%s:%s", self->domain, name);
	}
	else
	{
		snprintf(global_name, TLIBC_MAX_IDENTIFIER_LENGTH * 2, "%s", name);
	}

	if(!trie_retrieve(self->symbols, global_name, (void**)&symbol))
	{
		return NULL;
	}
	return symbol;
}

const SYMBOL* symbols_find_local(SYMBOLS *self, const tbytes* tok_identifier)
{
	char name[TLIBC_MAX_IDENTIFIER_LENGTH];
	memcpy(name, tok_identifier->ptr, tok_identifier->len);
	name[tok_identifier->len] = 0;
	return symbols_find_by_string_local(self, name);
}

tint32 symbols_save_string(SYMBOLS *self, const char *name, const SYMBOL *symbol)
{
	char global_name[TLIBC_MAX_IDENTIFIER_LENGTH * 2];



	if(self->domain[0])
	{
		snprintf(global_name, TLIBC_MAX_IDENTIFIER_LENGTH * 2, "%s:%s", self->domain, name);
	}
	else
	{
		snprintf(global_name, TLIBC_MAX_IDENTIFIER_LENGTH * 2, "%s", name);
	}

	//printf("save %s : %x\n", global_name, symbol);
	if(!trie_store_if_absent(self->symbols, global_name, symbol))
	{
		return E_TD_ERROR;
	}
	return E_TD_NOERROR;
}

tint32 symbols_save(SYMBOLS *self, const tbytes *tok_identifier, const SYMBOL *symbol)
{
	char name[TLIBC_MAX_IDENTIFIER_LENGTH];
	memcpy(name, tok_identifier->ptr, tok_identifier->len);
	name[tok_identifier->len] = 0;
	return symbols_save_string(self, name, symbol);
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
