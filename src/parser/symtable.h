#ifndef AQUERY_SYM_TABLE_H
#define AQUERY_SYM_TABLE_H
#define SYM_TABLE_SIZE 32
#include "../types/aquery_types.h"

struct OrderNode;
typedef struct symentry {
  char *name;
  DataType type;
  struct symentry *next; // linking
  int order_dep; // order dependent
  int uses_agg; // uses an aggregate
  int is_odx; // is an order dependence annihilating elem
  struct OrderNode *order_spec; // any order information
} Symentry;

typedef struct symtable {
  Symentry *table[SYM_TABLE_SIZE];
  struct symtable *next; // to stack together symtables
} Symtable;

// Memory allocation and construction functions
Symentry *make_symentry(char *name, DataType type, int ord_d, int sz_p);
Symtable *make_symtable();

// Create table and put in built in functions
Symtable *init_symtable();

// Manipulating the symtable stack
Symtable *push_env(Symtable *curr_env);
Symtable *pop_env(Symtable *curr_env);

// Freeing memory and structures
void free_symtable(Symtable *symtable);
void free_names(Symentry *names[]);
void free_symchain(Symentry *this);

// Operations for symbol tables
int symhash(char *name);
Symentry *lookup_sym(Symtable *symtable, char *name);
void put_sym(Symtable *curr_env, char *name, DataType type, int order_d,
             int size_p);
struct LogicalQueryNode;
void add_order(Symtable *curr_env, char *name, struct LogicalQueryNode *sort);
void print_symtable(Symtable *env);

#endif