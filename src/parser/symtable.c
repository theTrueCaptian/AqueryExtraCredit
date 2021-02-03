/*
        Building out a symbol table to use with bison parser. We maintain the
   symbol table as  a
        stack of hash tables (so that we can push/pop easily as we enter
   different scopes). Within
        a table we handle collisions via chaining.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../ast/ast.h"
#include "symtable.h"
#include "../types/aquery_types.h"
#define SYM_TABLE_DEBUG 0
#define SYM_PRINT_DEBUG(str)                                                   \
  if (SYM_TABLE_DEBUG)                                                         \
  printf("---->SYM TABLE DEBUGGING: %s\n", str)
#define STAND_ALONE 0

// Allocate memory for a symbol table entry, copy variable name, initialize and
// return
// Exits with error if memory is not succesfully allocated with malloc
Symentry *make_symentry(char *name, DataType type, int order_d, int agg) {
  SYM_PRINT_DEBUG("MEMALLOC: symbol entry");
  Symentry *newentry = malloc(sizeof(Symentry));

  if (newentry == NULL) {
    printf("Symtable: Unable to allocate Symentry\n");
    exit(1);
  }

  newentry->name = strdup(name); // copy the string name
  newentry->type = type;
  newentry->order_dep = order_d;
  newentry->uses_agg = agg; // uses an aggregate
  newentry->is_odx = 0; // is order dependence annihilating
  newentry->next = NULL;
  newentry->order_spec = NULL;

  return newentry;
}

// Allocate memory for symbol table and initializes struct. Exits with error if
// memory allocation fails
Symtable *make_symtable() {
  SYM_PRINT_DEBUG("MEMALLOC: symbol table");
  Symtable *symtable = malloc(sizeof(Symtable));

  if (symtable == NULL) {
    printf("Unable to allocate Symtable\n");
    exit(1);
  }

  int i;
  for (i = 0; i < SYM_TABLE_SIZE; i++) {
    symtable->table[i] = NULL;
  }

  symtable->next = NULL;
  return symtable;
}

// Creates a new symbol table and pushes it onto the stack of symbol tables,
// returns
// a pointer to the top of the stack
Symtable *push_env(Symtable *curr_env) {
  SYM_PRINT_DEBUG("pushing context (new symbol table added to stack)");
  Symtable *new_env = make_symtable();
  new_env->next = curr_env;
  return new_env;
}

// Pops the top of the stack of symbol tables, frees the popped table
// and returns a pointer to the new top of the stack
Symtable *pop_env(Symtable *curr_env) {
  SYM_PRINT_DEBUG(
      "popping current context (top of stack symbol table removed)");
  Symtable *next_env = curr_env->next;
  free_symtable(curr_env);
  return next_env;
}

// Frees all memory associated with a symbol table
//(this needs to be called on every table in a symbol table stack to correctly
//free the whole symbol table)
void free_symtable(Symtable *symtable) {
  SYM_PRINT_DEBUG("freeing symbol table");
  free_names(symtable->table);
  free(symtable);
}

// Frees the array of symbol table entries held in a symbol table structure.
void free_names(Symentry *names[]) {
  SYM_PRINT_DEBUG("MEMDEALLOC: freeing names");
  int i;
  Symentry *this;

  for (i = 0; i < SYM_TABLE_SIZE; i++) {
    if ((this = names[i]) != NULL) {
      free_symchain(this);
    }
  }
}

// Follows a chain of symbol table entries and frees each of them
void free_symchain(Symentry *this) {
  SYM_PRINT_DEBUG("MEMDEALLOC: freeing chain of symbols");
  Symentry *delete;

  for (delete = NULL;
       this != NULL;) { // follow linked list of entries and free all
    delete = this;
    this = this->next; // advance our pointer along the chain of collisions
    free(delete->name); // free copy of variable name
    free(delete); // free structure as a whole
  }
}

// Create a hash code for a name to correctly index into a symbol table name
// array
int symhash(char *name) {
  int code, i;
  char c;

  for (code = 0, i = 0; (c = name[i]) != '\0'; i++) {
    code += c;
  }

  return code % SYM_TABLE_SIZE;
}

// Searches for a given symbol in our symbol table. It scans the stack of symbol
// tables in order, and returns the first match
// If no match is found, NULL is returned
Symentry *lookup_sym(Symtable *symtable, char *name) {
  SYM_PRINT_DEBUG("looking up a symbol in symbol table");
  Symtable *curr_env = symtable;
  Symentry *result = NULL;
  Symentry *entry = NULL;
  int hash_index = symhash(name);

  if (symtable == NULL) {
    return result; // no point in looking for anything if we don't have an env
  }

  for (result = NULL; result == NULL && curr_env != NULL;
       curr_env = curr_env->next) {
    for (entry = curr_env->table[hash_index]; entry != NULL;
         entry = entry->next) {
      if (strcmp(entry->name, name) == 0) { // we found our symbol
        result = entry;
        break;
      }
    }
  }

  return result;
}

// Places a given symbol into our symbol table (creating any necessary structure
// along the way)
// Symbols are placed in stack order, with the latest put shadowing repetitions
// placed prior
void put_sym(Symtable *curr_env, char *name, DataType type, int ord_d,
             int uses_agg) {
  SYM_PRINT_DEBUG("putting symbol into symbol table");
  Symentry *new_entry = make_symentry(name, type, ord_d, uses_agg);
  int hash_index = symhash(name);
  new_entry->next = curr_env->table[hash_index];
  curr_env->table[hash_index] = new_entry; // we place new symbol at start of
                                           // list, this achieves shadowing
                                           // semantics in same scope
}

// Add order information to a given identifier
void add_order(Symtable *curr_env, char *name, LogicalQueryNode *sort) {
  SYM_PRINT_DEBUG("adding order information to identifier in symtable");
  Symentry *entry = lookup_sym(curr_env, name);

  if (entry == NULL) {
    printf("Symtable: Attempted to add order to non-existing identifier\n");
    exit(1);
  } else {
    entry->order_spec = (sort == NULL) ? NULL : sort->params.order;
  }
}

// Yes...breaks are redundant here, but leaving in case we change this later
const char *print_type_name(DataType type) {
  switch (type) {
  case INT_TYPE:
    return "int";
    break;
  case FLOAT_TYPE:
    return "float";
    break;
  case DATE_TYPE:
    return "date";
    break;
  case BOOLEAN_TYPE:
    return "boolean";
    break;
  case HEX_TYPE:
    return "hex";
    break;
  case TABLE_TYPE:
    return "table";
    break;
  case VIEW_TYPE:
    return "view";
    break;
  case FUNCTION_TYPE:
    return "function";
    break;
  case CALL_TYPE:
    return "call";
    break;
  case UNKNOWN_TYPE:
    return "unknown";
    break;
  default:
    return "Error: undef";
    break;
  }
}

// Printing the symbol
void print_symtable(Symtable *symtable) {
  int scope, i;
  Symtable *env;
  Symentry *entry = NULL;
  printf("Symtable\n");
  printf("%12.12s %12.12s %12.12s\n", "n_scopes_out", "var_name", "var_type");
  printf("---------------------------------------------\n");

  for (env = symtable, scope = 0; env != NULL; env = env->next, scope++) {
    for (i = 0; i < SYM_TABLE_SIZE; i++) {
      for (entry = env->table[i]; entry != NULL; entry = entry->next) {
        printf("%9.3d %16.12s %10.12s\n", scope, entry->name,
               print_type_name(entry->type));
      }
    }
  }
}

Symtable *init_symtable() {
  const char *built_in_names[] = {
      "abs",  "avg",    "avgs",      "count", "deltas", "distinct", "drop",
      "fill", "first",  "last",      "max",   "maxs",   "min",      "mins",
      "mod",  "next",   "prev",      "prd",   "prds",   "reverse",  "sum",
      "sums", "stddev", "make_null", "sqrt", "ratios"};

  const int built_in_order_dep[] = {1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0,
                                    1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 1};

  const int is_odx[] = {0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1,
                        0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0};

  Symtable *env = make_symtable();
  Symentry *info;
  int i, len = sizeof(built_in_names) / sizeof(char *);

  // populate with information about built-ins properties, note that we
  // duplicate the string, as we need these to be persistent
  for (i = 0; i < len; i++) {
    put_sym(env, strdup(built_in_names[i]), FUNCTION_TYPE,
            built_in_order_dep[i], 1);
    info = lookup_sym(env, (char *)built_in_names[i]);
    info->is_odx = is_odx[i];
  }

  return env;
}

// A simple set of tests for our symbol table implementation
void ignore_symtable_test() {
  SYM_PRINT_DEBUG("running symbol table tests");
  // scope 1
  char *names[][4] = {
      {"var_1", "var_2", "var_3", "var_4"}, // scope 0
      {"var_1", "var_5", "var_6", "var_5"}, // scope 1
  };

  DataType types[][4] = {
      {INT_TYPE, FLOAT_TYPE, FUNCTION_TYPE, TABLE_TYPE}, // scope 0
      {FUNCTION_TYPE, INT_TYPE, TABLE_TYPE, FUNCTION_TYPE}, // scope 1
  };

  int depth = 2, len = 4, i, j;
  Symtable *symtable = init_symtable();
  Symentry *entry = NULL;

  // inserting symbols for scope 0
  i = 0;
  for (j = 0; j < len; j++) {
    printf("Putting:%s with type %s\n", names[i][j],
           print_type_name(types[i][j]));
    put_sym(symtable, names[i][j], types[i][j], 0, 0);
  }

  // retrieveing symbols for scope 0
  i = 0;
  for (j = 0; j < len; j++) {
    entry = lookup_sym(symtable, names[i][j]);

    if (entry == NULL) {
      printf("--> Warning: %s not found\n", names[i][j]);
    } else {
      printf("Found %s with type %s\n", names[i][j],
             print_type_name(entry->type));
    }
  }

  // Create another scope
  symtable = push_env(symtable);

  // inserting symbols for scope 1
  i = 1;
  for (j = 0; j < len; j++) {
    printf("Putting:%s with type %s\n", names[i][j],
           print_type_name(types[i][j]));
    put_sym(symtable, names[i][j], types[i][j], 0, 0);
  }

  // Looking up all symbols
  for (i = 0; i < depth; i++) {
    for (j = 0; j < len; j++) {
      entry = lookup_sym(symtable, names[i][j]);

      if (entry == NULL) {
        printf("--> Warning: %s not found\n", names[i][j]);
      } else {
        printf("Found %s with type %s\n", names[i][j],
               print_type_name(entry->type));
      }
    }
  }

  print_symtable(symtable);
  symtable = pop_env(symtable); // remove scope 1
  symtable = pop_env(symtable); // remove scope 0
}

#if STAND_ALONE
int main(int argc, char *argv[]) {
  ignore_symtable_test();
  return 0;
}
#endif
