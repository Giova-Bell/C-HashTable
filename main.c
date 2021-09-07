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

#define HASHSIZE 100


//###################################################//
//## NODE STRUCTURE                                ##//
//###################################################//
union key
{
	char *char_key;
	int int_key;
};

union value
{
	char *char_value;
	int int_value;
	float float_value;
};

typedef struct node
{
	char *key_type;
	char *value_type;
	union value value;
	union key key; // needed to distinguish the element in case of collision
	struct node *next;
}
node;


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
//## POINTER TABLE && INITIALIZER                  ##//
//###################################################//
static node *root[HASHSIZE];

void initialize()
{
	for(int i = 0; i < HASHSIZE; i++)
	{
		root[i] = NULL;
	}
}


//###################################################//
//## HASH                                          ##//
//###################################################//
int hash(char *key_type, union key key)
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
	else if(strcmp(key_type, "int") == 0)
	{
		return key.int_key % HASHSIZE;
	}
	else
	{
		return -1;
	}
}


//###################################################//
//## SEARCH                                        ##//
//###################################################//
node *search(char *key_type, union key key)
{
	node *p;
	for(p = root[hash(key_type, key)]; p != NULL; p = p->next)
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

node **previousEl(char *key_type, union key key)
{
	node *p;
	node **previous = &root[hash(key_type, key)];
	for(p = root[hash(key_type, key)]; p != NULL; p = p->next)
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

node *lastEl(int hash) // search for last element if there is one
{
	node *p;
	for(p = root[hash]; p != NULL; p = p->next)
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
int add(char *key_type, char *value_type, union key key, union value value)
{
	if(search(key_type, key) == NULL)
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
		
		if((last = lastEl(hash(key_type, key))) == NULL) // if the chain struct dosen't exist
		{
			root[hash(key_type, key)] = p;
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
int del(char *key_type, union key key)
{
	if(valid_key_type(key_type))
	{
		node *p;
		node **previous;
		if((p = search(key_type, key)) != NULL) // se l'elemento esiste
		{
			previous = previousEl(key_type, key);
			*previous = p->next;;
				
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

void printAllEl()
{
	node *p;
	for(int i = 0; i < HASHSIZE; i++)
	{
		for(p = root[i]; p != NULL; p = p->next)
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
	initialize();
	
	add("char", "char", char_key("ciao"), char_value("bella"));
	add("int", "char", int_key(11), char_value("haha"));
	add("char", "int", char_key("loo"), int_value(44));
	add("int", "float", int_key(50), float_value(11.5));
	add("char", "float", char_key("ciao:)"), float_value(33.66));
	
	printAllEl();
	
	del("char", char_key("ciao"));
	
	printf("\n");
	printAllEl();
}
