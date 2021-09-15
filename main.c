/*
Author:	Giovanni Bellini
Date:	07/09/2021

Description:
an hashtable library

scheme:
|->NULL
|->(int-char_Key, int-char-float_Value)->NULL
|->NULL
|->()->()->NULL <- collisions are managed with a liked list
|->NULL
|->NULL
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_table_class.h"

//###################################################//
//## TYPE FUNCTIONS                                ##//
//###################################################//
int valid_key_type(char *key_type)
{
	if(strcmp(key_type, "char") == 0 || strcmp(key_type, "int") == 0)
	{
		return 1;
	}
	return 0;
}

int valid_value_type(char *value_type)
{
	if(strcmp(value_type, "char") == 0 || strcmp(value_type, "int") == 0 || strcmp(value_type, "float") == 0)
	{
		return 1;
	}
	return 0;
}

// transmorm the desired variable in union
union key char_key(char *key)
{
	return (union key)key;
}

union key int_key(int key)
{
	return (union key)key;
}

union value char_value(char *value)
{
	return (union value)value;
}

union value int_value(int value)
{
	return (union value)value;
}

union value float_value(float value)
{
	return (union value)(float)value;
}


//###################################################//
//## INITIALIZER                                   ##//
//###################################################//
//static node *root[HASHSIZE];
void init(HashTable *self)
{
	self->HASHSIZE = 100;
	
	for(int i = 0; i < self->HASHSIZE; i++)
	{
		self->root[i] = NULL;
	}
	
	self->Search = &search;
	self->Add = &add;
	self->Delete = &del;
}


//###################################################//
//## HASH                                          ##//
//###################################################//
int hash(char *key_type, union key key, int HASHSIZE)
{
	if(strcmp(key_type, "char") == 0)
	{
		unsigned hashval = 0;
	    for (; *key.char_key != '\0'; key.char_key++)
		{
	    	hashval = *key.char_key + 31 * hashval;
		}
	    
	    return hashval % HASHSIZE;
	}
	if(strcmp(key_type, "int") == 0)
	{
		return key.int_key % HASHSIZE;
	}
	
	return -1;
}


//###################################################//
//## SEARCH                                        ##//
//###################################################//
node *search(HashTable *self, char *key_type, union key key)
{
	node *p;
	for(p = self->root[hash(key_type, key, self->HASHSIZE)]; p != NULL; p = p->next)
	{
		if(strcmp(key_type, "char") == 0) // if the type of the key is char
		{
			if(strcmp(p->key.char_key, key.char_key) == 0) // if the key match with the key passed
			{
				return p;
			}
		}
		else if(strcmp(key_type, "int") == 0)
		{
			if(p->key.int_key == key.int_key)
			{
				return p;
			}
		}
		else
		{
			return NULL; // find a method to advise that a wrong type has been typed
		}
	}
	return NULL;
}

node **previousEl(HashTable *self, char *key_type, union key key)
{
	node *p;
	node **previous = &self->root[hash(key_type, key, self->HASHSIZE)];
	for(p = self->root[hash(key_type, key, self->HASHSIZE)]; p != NULL; p = p->next)
	{
		if(strcmp(key_type, "char") == 0) // if the type of the key is char
		{
			if(strcmp(p->key.char_key, key.char_key) == 0) // if the key match with the key passed
			{
				return previous;
			}
			else
			{
				previous = &p->next;
			}
		}
		else if(strcmp(key_type, "int") == 0)
		{
			if(p->key.int_key == key.int_key)
			{
				return previous;
			}
			else
			{
				previous = &p->next;
			}
		}
		else
		{
			return NULL; // find a method to advise that a wrong type has been typed
		}
	}
}

node *lastEl(HashTable *self, int hash) // search for last element if there is one
{
	node *p;
	for(p = self->root[hash]; p != NULL; p = p->next)
	{
		if(p->next == NULL)
		{
			return p;
		}
	}
	return NULL; // return NULL if there isn't yet a linked element
}


//###################################################//
//## ADD                                           ##//
//###################################################//
int add(HashTable *self, char *key_type, char *value_type, union key key, union value value)
{
	if(search(self, key_type, key) == NULL)
	{
		node *last;
		node *p;
		p = (node *) malloc(sizeof(node));
		
		// assign key with the right value type
		if(valid_key_type(key_type))
		{
			p->key_type = key_type;
		}
		else
		{
			p->key_type = "char";
		}
		
		p->key = key;
		
		// assign value with the right value type
		if(valid_value_type(value_type))
		{
			p->value_type = value_type;
		}
		else
		{
			p->value_type = "char";
		}
		
		p->value = value;
		
		p->next = NULL;
		
		if((last = lastEl(self, hash(key_type, key, self->HASHSIZE))) == NULL) // if the chain struct dosen't exist
		{
			self->root[hash(key_type, key, self->HASHSIZE)] = p;
		} else
		{
			last->next = p;
		}
		return 1; // operation successfull
	}
	return 0;
}


//###################################################//
//## DELETE                                        ##//
//###################################################//
int del(HashTable *self, char *key_type, union key key)
{
	if(valid_key_type(key_type))
	{
		node *p;
		node **previous;
		if((p = search(self, key_type, key)) != NULL) // se l'elemento esiste
		{
			previous = previousEl(self, key_type, key);
			*previous = p->next;
				
			//completely free the node
			free((void *) p);
		}
	}
	return 0;
}


//###################################################//
//## PRINT                                         ##//
//###################################################//
void printValue(node *p)
{
	if(strcmp(p->value_type, "float") == 0)
	{
		printf("%f", p->value.float_value);
	}
	else if(strcmp(p->value_type, "int") == 0)
	{
		printf("%d", p->value.int_value);
	}
	else
	{
		printf("%s", p->value.char_value);
	}
}

void printEl(node *p)
{	
	if(strcmp(p->key_type, "char") == 0)
	{
		printf("%s : ", p->key.char_key);
	}
	else
	{
		printf("%d : ", p->key.int_key);
	}
	printValue(p);
}

void printAllEl(HashTable *self)
{
	node *p;
	for(int i = 0; i < self->HASHSIZE; i++)
	{
		for(p = self->root[i]; p != NULL; p = p->next)
		{
			printEl(p);
			printf("\n");
			
			if(p->next == NULL)
			{
				break;
			}
		}
	}
}


//###################################################//
//## MAIN                                          ##//
//###################################################//

void main()
{
	// example
	HashTable letters1;
	init(&letters1);
	
	letters1.Add(&letters1, "char", "char", char_key("ciao"), char_value("bella"));
	letters1.Add(&letters1, "int", "int", int_key(11), int_value(44));
	
	printAllEl(&letters1);
	
	printf("\n");
	
	HashTable letters2;
	init(&letters2);
	
	letters2.Add(&letters2, "char", "char", char_key("ciao2"), char_value("bella2"));
	letters2.Add(&letters2, "int", "int", int_key(12), int_value(45));
	
	printAllEl(&letters2);
}
