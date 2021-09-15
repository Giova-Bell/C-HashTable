#ifndef CLASS
#define CLASS

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

typedef struct HashTable
{
	int HASHSIZE;
	node* (*Search)(struct HashTable *self, char *key_type, union key key);
	int (*Add)(struct HashTable *self, char *key_type, char *value_type, union key key, union value value);
	int (*Delete)(struct HashTable *self, char *key_type, union key key);
	node *root[100];
}
HashTable;

void init(HashTable *table);
node *search(HashTable *self, char *key_type, union key key);
int add(HashTable *self, char *key_type, char *value_type, union key key, union value value);
int del(HashTable *self, char *key_type, union key key);

#endif
