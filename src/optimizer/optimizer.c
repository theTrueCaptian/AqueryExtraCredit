/*
        This file contains functions necessary for aquery optimizations
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../types/aquery_types.h"
#include "../ast/ast.h"
#include "../ast/ast_print.h"
#include "optimizer.h"

#define STAND_ALONE 0
#define OPTIM_DEBUG 0
#define OPTIM_PRINT_DEBUG(str)                                                 \
  if (OPTIM_DEBUG)                                                             \
  printf("---->OPTIM DEBUGGING: %s\n", str)
#define NAMESIZE 100

extern const char *ExprNodeTypeName[]; // aquery_print.c
extern int query_line_num, query_col_num; // aquery.y
extern int silence_warnings; // aquery.y options

const char *UNKNOWN_TABLE_NM = "UNKNOWN";

#if !STAND_ALONE
extern int optim_level; // aquery.y
extern Symtable *env; // aquery.y
#endif

#if STAND_ALONE
int optim_level = 1;
Symtable *env;
#endif

GenList *list_alloc(void *data) {
  GenList *new = malloc(sizeof(GenList));
  new->id = 0;
  new->data = data;
  new->next = NULL;
  return new;
}

GenList *list_append(GenList *list, void *data) {
  GenList *curr = list, *prev = NULL;

  while (curr != NULL) { // search for last position in list
    prev = curr;
    curr = curr->next;
  }

  if (prev == NULL) { // list was empty
    return list_alloc(data);
  } else {
    GenList *new = list_alloc(data);
    prev->next = new;
    new->id = prev->id + 1; // adjust id accordingly
    return list;
  }
}

GenList *list_prepend(GenList *list, void *data) {
  GenList *new = list_alloc(data);

  if (list == NULL) {
    return new;
  } else {
    new->next = list;
    new->id = list->id - 1; // adjust id accordingly
    return new;
  }
}

// Compares data pointer, removes a node if data values are equal
// This solely removes the item from the list, any freeing done on the data has
// to be handled by caller
GenList *list_remove_first(GenList *list, void *data) {
  GenList *curr = list, *prev = NULL;

  while (curr != NULL) {
    if (curr->data == data) { // we found the element to remove
      if (prev == NULL) { // elem is at start of list
        GenList *new_list = curr->next;
        free(curr);
        return new_list;
      } else {
        prev->next = curr->next;
        free(curr);
        return list;
      }
    }

    prev = curr;
    curr = curr->next;
  }
  // must return list in case nothing matched
  return list;
}

// apply an unary function at every element of a list
void list_foreach(GenList *list, void (*fun)(GenList *)) {
  GenList *curr = list;

  while (curr != NULL) {
    fun(curr);
    curr = curr->next;
  }
}

// apply a  binary function at every element of a lst, additional argument is
// passed
// in as void pointer
void list_foreach_warg(GenList *list, void (*fun)(GenList *, void *),
                       void *arg) {
  GenList *curr = list;

  while (curr != NULL) {
    fun(curr, arg);
    curr = curr->next;
  }
}

// calculates length of a list
int list_length(GenList *list) {
  int len = 0;
  GenList *curr = list;

  while (curr != NULL) {
    len++;
    curr = curr->next;
  }

  return len;
}

void print_str(GenList *elem) {
  char *str = (char *)elem->data;
  printf("%s\n", str);
}

// Creating deep copies of LogicalQueryNodes in order to reproduce the original
// plan
// before any optimizations, so we can create multiple options
LogicalQueryNode *deepcopy_LogicalQueryNode(LogicalQueryNode *node) {
  OPTIM_PRINT_DEBUG("deep copying logical query node");
  LogicalQueryNode *copy = NULL;

  if (node != NULL) {
    copy = make_EmptyLogicalQueryNode(node->node_type);
    copy->order_dep = node->order_dep;
    copy->tables_involved = deepcopy_IDListNode(node->tables_involved);

    // copying any information necessary
    switch (node->node_type) {
    case SIMPLE_TABLE: // intentional fall through
    case ALIAS: // intentional fall through
      copy->params.name = strdup(node->params.name);
      break;
    case INNER_JOIN_ON: // intentional fall through
    case FULL_OUTER_JOIN_ON: // intentional fall through
    case FILTER_WHERE: // intentional fall through
    case FILTER_HAVING: // intentional fall through
    case EXPLICIT_VALUES: // intentional fall through
      copy->params.exprs = deepcopy_ExprNode(node->params.exprs);
      break;
    case INNER_JOIN_USING: // intentional fall through
    case FULL_OUTER_JOIN_USING: // intentional fall through
    case DELETION: // intentional fall through
    case COL_NAMES: // intentional fall through
      copy->params.cols = deepcopy_IDListNode(node->params.cols);
      break;
    case PROJECT_SELECT: // intentional fall through
    case PROJECT_UPDATE: // intentional fall through
    case GROUP_BY: // intentional fall through
      copy->params.namedexprs = deepcopy_NamedExprNode(node->params.namedexprs);
      break;
    case SORT: // intentional fall through
    case SORT_COLS: // intentional fall through
    case SORT_EACH_COLS: // intentional fall through
      copy->params.order = deepcopy_OrderNode(node->params.order);
      break;
    default:
      break;
    }

    copy->arg = deepcopy_LogicalQueryNode(node->arg);
    copy->next_arg = deepcopy_LogicalQueryNode(node->next_arg);
  }

  return copy;
}

NamedExprNode *deepcopy_NamedExprNode(NamedExprNode *node) {
  OPTIM_PRINT_DEBUG("deep copying named expression node");
  NamedExprNode *copy = NULL;
  char *name = NULL;

  if (node != NULL) {
    if (node->name != NULL) {
      name = strdup(node->name);
    }

    copy = make_NamedExprNode(name, deepcopy_ExprNode(node->expr));
    copy->order_dep = node->order_dep;
    copy->sub_order_dep = node->sub_order_dep;
    copy->next_sibling = deepcopy_NamedExprNode(node->next_sibling);
  }

  return copy;
}

IDListNode *deepcopy_IDListNode(IDListNode *node) {
  OPTIM_PRINT_DEBUG("deep copying id list node");
  IDListNode *copy = NULL;

  if (node != NULL) {
    copy = make_IDListNode(strdup(node->name),
                           deepcopy_IDListNode(node->next_sibling));
  }

  return copy;
}

OrderNode *deepcopy_OrderNode(OrderNode *node) {
  OPTIM_PRINT_DEBUG("deep copying order node");
  OrderNode *copy = NULL;

  if (node != NULL) {
    copy = make_OrderNode(node->node_type, deepcopy_ExprNode(node->col));
    copy->next = deepcopy_OrderNode(node->next);
  }

  return copy;
}

// Create a deepcopy of an expression AST
ExprNode *deepcopy_ExprNode(ExprNode *node) {
  OPTIM_PRINT_DEBUG("deep copying expression node");
  ExprNode *copy = NULL;

  if (node != NULL) {
    // Create an empty expression node with your general info
    copy = make_EmptyExprNode(node->node_type);
    copy->data_type = node->data_type;
    copy->order_dep = node->order_dep;
    copy->sub_order_dep = node->sub_order_dep;
    copy->uses_agg = node->uses_agg;
    copy->is_odx = node->is_odx;
    copy->tables_involved = deepcopy_IDListNode(node->tables_involved);

    // Copy any data necessary, note strings need to be copied deeply too
    // We free some stuff as we manipulate optimizations
    switch (node->node_type) {
    case CONSTANT_EXPR:
      if (node->data_type == FLOAT_TYPE) {
        copy->data.fval = node->data.fval;
      } else if (node->data_type == INT_TYPE ||
                 node->data_type == BOOLEAN_TYPE) {
        copy->data.ival = node->data.ival;
      } else { // strings, hex, dates...
        copy->data.str = strdup(node->data.str);
      }
      break;
    case ID_EXPR: // intentional fall through
    case BUILT_IN_FUN_CALL: // intentional fall through
    case UDF_CALL:
      copy->data.str = strdup(node->data.str);
      break;
    case EVERY_N_IX:
      copy->data.ival = node->data.ival;
      break;
    default: // nothing to do for now
      break;
    }

    copy->first_child = deepcopy_ExprNode(node->first_child);
    copy->next_sibling = deepcopy_ExprNode(node->next_sibling);
  }

  return copy;
}

ExprNode *append_Exprs(ExprNode *existing, ExprNode *new) {
  ExprNode *curr = existing, *prev = NULL;

  while (curr != NULL) {
    prev = curr;
    curr = curr->next_sibling;
  }

  if (prev == NULL) {
    return new;
  } else {
    prev->next_sibling = new;
    return existing;
  }
}

/*
//returns 1 if name is in list, 0 otherwise, uses strcmp

int in_IDList(const char *name, IDListNode *list)
{
    IDListNode *curr;

    for(curr = list; curr != NULL; curr = curr->next_sibling)
    {
        if(strcmp(name, curr->name) == 0)
        {
            return 1;
        }
    }

    return 0;
}
*/

int same_name(const char *name1, const char *name2) {
  const char *dot1 = strchr(name1, '.');
  const char *dot2 = strchr(name2, '.');

  if ((dot1 == NULL && dot2 == NULL) ||
      (dot1 != NULL && dot2 != NULL)) { // if neither use correlation names, or
                                        // both do, then compare directly
    return strcmp(name1, name2);
  } else { // compare only column name, ignore correlation name
    if (dot1 != NULL) {
      return strcmp(dot1 + 1, name2);
    } else {
      return strcmp(name1, dot2 + 1);
    }
  }
}

// returns true if the name is already in the list
// handles column references that use a correlation name
int in_IDList(const char *name, IDListNode *list) {
  IDListNode *curr;

  for (curr = list; curr != NULL; curr = curr->next_sibling) {
    if (same_name(name, curr->name) == 0) {
      return 1;
    }
  }

  return 0;
}

// returns 1 if any element of l1 is in l2
int any_in_IDList(IDListNode *l1, IDListNode *l2) {
  IDListNode *curr = l1;

  while (curr != NULL) {
    if (in_IDList(curr->name, l2)) {
      return 1;
    }

    curr = curr->next_sibling;
  }

  return 0;
}

// returns length of an IDListNode
int length_IDList(IDListNode *l) {
  IDListNode *curr = l;
  int len = 0;

  while (curr != NULL) {
    len++;
    curr = curr->next_sibling;
  }

  return len;
}

// return 1 if first list is a subset of l2 (returns true if l1 and l2 are set
// equivalent
// as well, so is_setEqual_IDLists should be used first in any kind of branching
// that
// has different option for set equivalence and subset
int is_subset_IDLists(IDListNode *l1, IDListNode *l2) {
  IDListNode *curr = l1;
  int in_list = 1;

  while (curr != NULL && in_list) {
    in_list = in_IDList(curr->name, l2);
    curr = curr->next_sibling;
  }

  return in_list;
}

// return 1 if they are set equivalent (i.e. order irrelevant)
int is_setEqual_IDLists(IDListNode *l1, IDListNode *l2) {
  if (length_IDList(l1) != length_IDList(l2)) { // don't even bother checking if
                                                // lengths don't match, since
                                                // these are sets
    // diff length mean different elements
    return 0;
  } else {
    // since length matches, a subset check returning true means they are the
    // same set
    return is_subset_IDLists(l1, l2);
  }
}

/// Unions x and y, so that there are no repetitions. Returns pointer to result
// Works by modifying pointers x and y when necessary, so neither x nor y should
// be
// relied upon after use
IDListNode *unionIDList(IDListNode *x, IDListNode *y) {
  OPTIM_PRINT_DEBUG("---->new union");
  IDListNode *curr_y, *next_y, *to_free;

  if (y == NULL) { // no point searching concatenation location if y is empty
    return x;
  } else if (x == NULL) {
    return y;
  } else {
    for (curr_y = y; curr_y != NULL; curr_y = next_y) {
      if (!in_IDList(curr_y->name, x)) {
        next_y = curr_y->next_sibling; // save down the sibling for y
        curr_y->next_sibling = x; // cons element in y to x
        x = curr_y; // set x to head of consed list
      } else { // if we didn't add it, free-it
        to_free = curr_y;
        next_y = curr_y->next_sibling;
        free_single_IDListNode(to_free);
        to_free = NULL;
      }
    }

    return x;
  }
}

// Account for interactions between order-dependent and order-independent
// columns. Adds otherwise order-independent cols to sorting list
// based on interactions
IDListNode *add_interactionsToSort(GenList *interact, IDListNode *need_sort) {
  int added = 1;
  GenList *curr_interact = NULL;
  IDListNode *col = NULL;

  while (added != 0) {
    added = 0;
    for (curr_interact = interact; curr_interact != NULL;
         curr_interact = curr_interact->next) {
      OPTIM_PRINT_DEBUG("Checking related:");
      // print_id_list(curr_interact->list, n, &n);
      for (col = (IDListNode *)curr_interact->data; col != NULL;
           col = col->next_sibling) {
        if (in_IDList(col->name, need_sort)) {
          OPTIM_PRINT_DEBUG("found contaminated list");
          need_sort = unionIDList(
              need_sort, (IDListNode *)curr_interact->data); // add to sort
          curr_interact->data = NULL; // we have added everything we needed,
                                      // rest has been freed during union
          added = 1;
          break;
        }
      }
    }
  }
  // Anything that is not null in potential was never added to sort list and
  // will never be added, so free it
  GenList *to_free_nested = NULL;
  IDListNode *to_free_list = NULL;

  curr_interact = interact;
  while (curr_interact != NULL) {
    to_free_nested = curr_interact;
    curr_interact = curr_interact->next;

    if (to_free_nested->data != NULL) { // free any contents
      OPTIM_PRINT_DEBUG("freeing a list of ids");
      free_IDListNode((IDListNode *)to_free_nested->data);
    }

    free(to_free_nested);
  }

  return need_sort;
}

// Collect the columns that need to be sorted given dependencies in an
// expression AST
IDListNode *collect_sortCols(ExprNode *od_expr, int in_proj) {
  IDListNode *need_sort = NULL;
  GenList *potential = NULL;

  IDListNode *top =
      collect_sortCols0(od_expr, in_proj, &need_sort, &potential, in_proj);
  potential = list_prepend(potential, top);

  OPTIM_PRINT_DEBUG("returned from collect_sortCols0");
  return add_interactionsToSort(potential, need_sort);
}

// Collect the columns that need to be sorted given dependencies in a named
// expression AST
IDListNode *collect_sortColsNamedExpr(NamedExprNode *nexprs) {
  IDListNode *need_sort = NULL;
  GenList *potential = NULL;
  IDListNode *top = NULL;

  NamedExprNode *curr_nexpr = nexprs;
  int x = 0;

  while (curr_nexpr != NULL) {
    OPTIM_PRINT_DEBUG("running over named expression");
    // print_expr(curr_nexpr->expr, x, &x);
    // add from start and require order annihilating not just order independent
    // to avoid adding to list
    top = collect_sortCols0(curr_nexpr->expr, 1, &need_sort, &potential, 1);
    potential = list_prepend(potential, top);

    curr_nexpr = curr_nexpr->next_sibling;
  }

  OPTIM_PRINT_DEBUG("returned from collect_sortCols0");
  // TODO: free up memory for structures used in this search
  return add_interactionsToSort(potential, need_sort);
}

// returns a string with dot access: table_name.col_name from an colDotAccess
// node.
char *qualify_name(char *table_name, char *col_name) {
  char *qualified_name =
      malloc(sizeof(char) * (strlen(table_name) + strlen(col_name) +
                             2)); // 1 for "." and another for null termination
  strcpy(qualified_name, table_name);
  strcat(qualified_name, ".");
  strcat(qualified_name, col_name);
  return qualified_name;
}

int is_name(ExprNodeType type) {
  return type == ID_EXPR || type == ALLCOLS_EXPR || type == COLDOTACCESS_EXPR ||
         type == ROWID_EXPR;
}

IDListNode *collect_sortCols0(ExprNode *node, int add_flag,
                              IDListNode **need_sort_ptr,
                              GenList **potential_ptr, int in_proj) {
  IDListNode *child = NULL;
  IDListNode *sibling = NULL;
  int new_add_flag = 0;

  if (node == NULL) { // nothing to do if we're at the end
    return NULL;
  } else if (is_name(node->node_type)) {
    OPTIM_PRINT_DEBUG("found id");

    // what name should we be adding to our lists?
    char *name = (node->node_type == COLDOTACCESS_EXPR)
                     ? qualify_name(node->first_child->data.str,
                                    node->first_child->next_sibling->data.str)
                     : strdup(node->data.str);

    if (add_flag) {
      OPTIM_PRINT_DEBUG("adding id to sort list");
      *need_sort_ptr = unionIDList(
          *need_sort_ptr,
          make_IDListNode(name, NULL)); // union with list of def sorted
      collect_sortCols0(node->next_sibling, add_flag, need_sort_ptr,
                        potential_ptr, in_proj); // explore sibling
      return NULL; // already appending to need_sort here, don't need to add to
                   // potential, so just return a null
    } else {
      OPTIM_PRINT_DEBUG("sending id back up");
      child = make_IDListNode(name, NULL); // return id to add to potential
      sibling = collect_sortCols0(node->next_sibling, add_flag, need_sort_ptr,
                                  potential_ptr, in_proj);
      return unionIDList(child, sibling);
    }
  } else if (node->node_type == CALL_EXPR) {
    // whether or not we continue to add to sort list can change based on
    // operator properties
    // whether we want to add things based solely on order-dependence or on
    // whether it
    // eliminates the need for order is based on whether or not we are in a
    // project
    // e.g. f(x)={x} select f(c1) from t assuming asc c10 where f(c1) > 10
    // we don't want to sort c1 prior to the selection, f(c1) is indeed order
    // independent
    // however, we do want to sort it for the projection, as the result requires
    // that we order c1
    new_add_flag =
        node->order_dep || ((in_proj && node->is_odx) ? node->order_dep : 1);

    if (!new_add_flag) { // we have reached an order-annihilating function call,
                         // like max/min
      OPTIM_PRINT_DEBUG("found order annihilating call, exploring child");
      child = collect_sortCols0(node->first_child, new_add_flag, need_sort_ptr,
                                potential_ptr, in_proj);
      OPTIM_PRINT_DEBUG("appending potential list at order annihilating call");
      *potential_ptr = list_prepend(
          *potential_ptr, child); // we stop "bubbling up" interactions here
      OPTIM_PRINT_DEBUG("exploring call's sibling");
      return collect_sortCols0(node->next_sibling, add_flag, need_sort_ptr,
                               potential_ptr, in_proj); // send back
                                                        // interactions in
                                                        // sibling node, not
                                                        // your own anymore
    } else {
      OPTIM_PRINT_DEBUG("found order dependent call, exploring args");
      child = collect_sortCols0(
          node->first_child, new_add_flag, need_sort_ptr, potential_ptr,
          in_proj); // no need to add to potential, adding to need_sort
      OPTIM_PRINT_DEBUG(
          "done exploring order dependent call, returning list of ");
      return collect_sortCols0(node->next_sibling, add_flag, need_sort_ptr,
                               potential_ptr,
                               in_proj); // return interactions in sibling node
    }
  } else {
    OPTIM_PRINT_DEBUG("found random node, exploring child");
    child = collect_sortCols0(node->first_child, add_flag | node->order_dep,
                              need_sort_ptr, potential_ptr, in_proj);
    OPTIM_PRINT_DEBUG("exploring random node's sibling");
    sibling = collect_sortCols0(node->next_sibling, add_flag, need_sort_ptr,
                                potential_ptr, in_proj);
    return unionIDList(child, sibling);
  }
}

// Extract all ids referenced in an expression
IDListNode *collect_AllCols(ExprNode *node) {
  OPTIM_PRINT_DEBUG("collecting all col refs");
  IDListNode *child = NULL;
  IDListNode *sibling = NULL;

  if (node == NULL) {
    return NULL;
  } else if (node->node_type == ID_EXPR) {
    child = make_IDListNode(strdup(node->data.str), NULL);
    sibling = collect_AllCols(node->next_sibling);
    return unionIDList(child, sibling);
  } else if (node->node_type == COLDOTACCESS_EXPR) { // TODO: figure out how to
                                                     // handle dot column
                                                     // accesses
    char *qualified_name = qualify_name(
        node->first_child->data.str, node->first_child->next_sibling->data.str);
    child = make_IDListNode(qualified_name, NULL);
    sibling = collect_AllCols(node->next_sibling);
    return unionIDList(child, sibling);
  } else if (node->node_type == BUILT_IN_FUN_CALL ||
             node->node_type == UDF_CALL) {
    return collect_AllCols(node->next_sibling);
  } else {
    child = collect_AllCols(node->first_child);
    sibling = collect_AllCols(node->next_sibling);
    return unionIDList(child, sibling);
  }
}

IDListNode *collect_AllColsProj(LogicalQueryNode *node) {
  OPTIM_PRINT_DEBUG("collecting all col refs in named expr");
  IDListNode *result = NULL;
  NamedExprNode *curr = node->params.namedexprs;

  while (curr != NULL) {
    result = unionIDList(result, collect_AllCols(curr->expr));
    curr = curr->next_sibling;
  }

  return result;
}


IDListNode *collect_AllColsGroupby(LogicalQueryNode *groupby) {
  OPTIM_PRINT_DEBUG("collecting all col refs in named expr for group by ");
  IDListNode *result = NULL;
  NamedExprNode *curr = groupby->params.namedexprs;

  while (curr != NULL) {
    result = unionIDList(result, collect_AllCols(curr->expr));
    curr = curr->next_sibling;
  }
  return result;
}


// Add element add to end of list, expression order might matter, so this
// maintains
ExprNode *append_toExpr(ExprNode *list, ExprNode *add) {
  ExprNode *prev, *curr;

  for (prev = curr = list; curr != NULL;
       prev = curr, curr = curr->next_sibling) {
    /* searching for first empty spot at end of list */
  }

  if (prev == NULL) {
    return add;
  } else {
    prev->next_sibling = add;
    return list;
  }
}

// An abstraction of a pattern we commonly need to for expression nodes:
// separating a list of expressions into 2, upon the first true instance of
// predicate
// Renders original list useless, since manipulates pointers
void partExpr_atFirst(ExprNode *orig, ExprNode **before, ExprNode **after,
                      int (*pred)(ExprNode *)) {
  OPTIM_PRINT_DEBUG(
      "partitioning expression list based on first instance of predicate");
  ExprNode *curr, *next;
  int hit_pred = 0;

  for (next = NULL, curr = orig; curr != NULL; curr = next) {
    next = curr->next_sibling; // store next expression
    curr->next_sibling = NULL; // remove link between current and next
                               // expression (need for predicates to work in
                               // isolation and to add to new list)

    if (!hit_pred) { // only test predicate if we haven't already tripped it
      hit_pred = pred(curr);
    }

    // if predicate hasn't tripped, then add to before list
    if (!hit_pred) {
      *before = append_toExpr(*before, curr);
    } else {
      *after = append_toExpr(*after, curr);
    }
  }
}

// groups list of expressions into 2 lists, one where the predicate is true, and
// the other where it is false
// Renders original list useless, since manipulates pointers
void groupExpr_onUnaryPred(ExprNode *orig, ExprNode * * true, ExprNode * *false,
                           int (*pred)(ExprNode *)) {
  OPTIM_PRINT_DEBUG(
      "grouping expr list into true and false based on predicate");
  ExprNode *curr, *next;

  for (next = NULL, curr = orig; curr != NULL; curr = next) {
    next = curr->next_sibling; // store next expression
    curr->next_sibling = NULL; // remove link between current and next
                               // expression (need for predicates to work in
                               // isolation and to add to new list)

    // add to appropriate list based on predicate
    if (pred(curr)) {
      *true = append_toExpr(*true, curr);
    } else {
      *false = append_toExpr(*false, curr);
    }
  }
}

void groupExpr_onBinaryPred(ExprNode *orig, ExprNode * * true,
                            ExprNode * *false, int (*pred)(ExprNode *, void *),
                            void *data) {
  OPTIM_PRINT_DEBUG(
      "grouping expr list into true and false based on predicate");
  ExprNode *curr, *next;

  for (next = NULL, curr = orig; curr != NULL; curr = next) {
    next = curr->next_sibling; // store next expression
    curr->next_sibling = NULL; // remove link between current and next
                               // expression (need for predicates to work in
                               // isolation and to add to new list)

    // add to appropriate list based on predicate
    if (pred(curr, data)) {
      *true = append_toExpr(*true, curr);
    } else {
      *false = append_toExpr(*false, curr);
    }
  }
}

int is_od(ExprNode *expr) { return expr->order_dep || expr->sub_order_dep; }

int uses_agg(ExprNode *expr) { return expr->uses_agg; }

// Partition a list of expressions into all expressions up to (but not
// including) the first order-dependent operation
// So your lists is (all_order_independent_ops_before_first_OD) (remaining)
void partExpr_OnOrder(ExprNode *expr, ExprNode **order_indep,
                      ExprNode **remaining) {
  OPTIM_PRINT_DEBUG("partitioning expression list based on first OD");
  partExpr_atFirst(expr, order_indep, remaining, is_od);
}

// Parition a list of expression into all expression up to (but not including)
// the first use of an agreggate
void partExpr_OnAgg(ExprNode *expr, ExprNode **no_agg, ExprNode **agg) {
  OPTIM_PRINT_DEBUG("partitioning expression list based on first agg");
  partExpr_atFirst(expr, no_agg, agg, uses_agg);
}

// New type of node needed to plug into query step sorting certain columns
LogicalQueryNode *make_specCols(IDListNode *cols) {
  LogicalQueryNode *spec_cols = make_EmptyLogicalQueryNode(COL_NAMES);
  spec_cols->params.cols = cols;
  return spec_cols;
}

// Sort cols based on order node
LogicalQueryNode *make_sortCols(OrderNode *order, IDListNode *cols) {
  LogicalQueryNode *sort = make_EmptyLogicalQueryNode(SORT_COLS);
  sort->params.order = order;
  sort->next_arg = make_specCols(cols);
  return sort;
}

// Sort-each cols based on order node
LogicalQueryNode *make_sortEachCols(OrderNode *order, IDListNode *cols) {
  LogicalQueryNode *sort = make_EmptyLogicalQueryNode(SORT_EACH_COLS);
  sort->params.order = order;
  sort->next_arg = make_specCols(cols);
  return sort;
}

// if want is a prefix of have, then return null, else return want...
OrderNode *get_NeededSort(OrderNode *have, OrderNode *want) {
  OPTIM_PRINT_DEBUG("searching for incremental sort needed");
  OrderNode *want_curr, *have_curr;

  want_curr = want;
  have_curr = have;

  while (want_curr != NULL && have_curr != NULL) {
    if (want_curr->node_type == have_curr->node_type &&
        strcmp(want_curr->col->data.str, have_curr->col->data.str) ==
            0) { // no need do anythign with a given order spec, if we already
                 // have it sorted
      // along that dimension
      OPTIM_PRINT_DEBUG("found matching sort prefix along 1 dimension");
      want_curr = want_curr->next;
      have_curr = have_curr->next;
    } else { // none matching
      OPTIM_PRINT_DEBUG("found end of matching sort prefix");
      break;
    }
  }

  if (want_curr == NULL) { // we don't need any order info anymore, so free
    free_OrderNode(want);
    want = NULL;
  }

  return want; // return either NULL if we had the order, otherwise whole order
               // required
}

// Name of table source in a dot expression
char *get_table_src(ExprNode *expr) {
  OPTIM_PRINT_DEBUG("extracting table name");
  // int x = 0;
  // print_expr(expr, x, &x);
  if (expr == NULL || expr->node_type != COLDOTACCESS_EXPR) {
    // printf("Error: attempting to extract table name from non-dot-access
    // expression\n");
    return NULL;
  } else {
    return expr->first_child->data.str;
  }
}

int is_comparison(ExprNodeType type) {
  return type == EQ_EXPR || type == LT_EXPR || type == LE_EXPR ||
         type == GT_EXPR || type == GE_EXPR || type == NEQ_EXPR;
}

// Is an expression that should be interpreted as a join clause
int is_JoinClause(ExprNode *expr) {
  OPTIM_PRINT_DEBUG("testing expression for join clause status");
  if (expr == NULL) {
    return 0;
  } else {
    if (expr->node_type != EQ_EXPR) {
      return 0;
    } else {
      ExprNode *left = expr->first_child;
      ExprNode *right = expr->first_child->next_sibling;
      int involves_dot_accesses = left->node_type == COLDOTACCESS_EXPR &&
                                  right->node_type == COLDOTACCESS_EXPR;
      IDListNode *left_names = collect_TablesExpr(left);
      IDListNode *right_names = collect_TablesExpr(right);
      IDListNode *names = unionIDList(left_names, right_names);
      return involves_dot_accesses && !in_IDList(UNKNOWN_TABLE_NM, names) &&
             length_IDList(names) > 1; // we need to know the tables being used,
                                       // and there must be more than 1 table
                                       // referenced
    }
  }
}

// Separate join clauses from normal filters
void groupExpr_OnJoin(ExprNode *expr, ExprNode **join_filters,
                      ExprNode **other_filters) {
  OPTIM_PRINT_DEBUG("grouping join expressions");
  groupExpr_onUnaryPred(expr, join_filters, other_filters, is_JoinClause);
}

// returns 1 if filter is equality filter
int is_eq_filter(ExprNode *expr) { return expr->node_type == EQ_EXPR; }

// Separate equality filters and others
void groupExpr_OnEqFilter(ExprNode *expr, ExprNode **eq_filters,
                          ExprNode **other_filters) {
  OPTIM_PRINT_DEBUG("grouping equality expressions");
  groupExpr_onUnaryPred(expr, eq_filters, other_filters, is_eq_filter);
}

// Return true if expr involves a subset of names (i.e. all tables referenced in
// expr are already in names)
int Expr_has_subset_tables(ExprNode *expr, void *names) {
  IDListNode *expr_names = collect_TablesExpr(expr);
  return is_subset_IDLists(expr_names, names);
}

// Return true if expr has exact names
int Expr_has_exact_tables(ExprNode *expr, void *names) {
  IDListNode *expr_names = collect_TablesExpr(expr);
  return is_setEqual_IDLists(expr_names, names);
}

// Return true if expr has UNKNOWN names
int Expr_has_unknown_table(ExprNode *expr) {
  return in_IDList(UNKNOWN_TABLE_NM, collect_TablesExpr(expr));
}

// Separate clauses into those referencing a subset of names
void groupExpr_OnSubsetTables(ExprNode *expr, ExprNode **have, ExprNode **dont,
                              IDListNode *names) {
  OPTIM_PRINT_DEBUG("grouping expressions on subset table refs");
  groupExpr_onBinaryPred(expr, have, dont, Expr_has_subset_tables, names);
}

void groupExpr_OnEqualTables(ExprNode *expr, ExprNode **match, ExprNode **dont,
                             IDListNode *names) {
  OPTIM_PRINT_DEBUG("grouping expressions on equal table refs");
  groupExpr_onBinaryPred(expr, match, dont, Expr_has_exact_tables, names);
}

void groupExpr_OnUnknownTables(ExprNode *expr, ExprNode **have,
                               ExprNode **dont) {
  groupExpr_onUnaryPred(expr, have, dont, Expr_has_unknown_table);
}

// What tables are involved in an expression
// Be careful to NULL out next_sibling for a node, if you just want expressions
// at that node, otherwise, it will chase the pointers
// and return entire tree's result
IDListNode *collect_TablesExpr(ExprNode *node) {
  IDListNode *child = NULL, *sibling = NULL;

  OPTIM_PRINT_DEBUG("collecting tables involved in expression");

  if (node == NULL) {
    return NULL;
  } else if (node->tables_involved !=
             NULL) { // if we already have it, just create a copy of it (need to
                     // copy to be able to union without messing up results)
    child = deepcopy_IDListNode(node->tables_involved);
    sibling = collect_TablesExpr(node->next_sibling);
    return unionIDList(child, sibling);
  } else if (node->node_type == ID_EXPR) {
    child = make_IDListNode(strdup(UNKNOWN_TABLE_NM), NULL);
    node->tables_involved = deepcopy_IDListNode(child);
    sibling = collect_TablesExpr(node->next_sibling);
    return unionIDList(child, sibling);
  } else if (node->node_type == COLDOTACCESS_EXPR) {
    child = make_IDListNode(strdup(get_table_src(node)), NULL);
    node->tables_involved = deepcopy_IDListNode(child);
    sibling = collect_TablesExpr(node->next_sibling);
    return unionIDList(child, sibling);
  } else {
    child = collect_TablesExpr(node->first_child);
    node->tables_involved = deepcopy_IDListNode(child);
    sibling = collect_TablesExpr(node->next_sibling);
    return unionIDList(child, sibling);
  }
}

IDListNode *collect_TablesFrom(LogicalQueryNode *ts) {
  IDListNode *child = NULL, *sibling = NULL;

  if (ts == NULL) {
    return NULL;
  } else if (ts->tables_involved !=
             NULL) { // we already have it, just return a copy
    child = deepcopy_IDListNode(ts->tables_involved);
    sibling = collect_TablesFrom(ts->next_arg);
    return unionIDList(child, sibling);

  } else if (ts->node_type == SIMPLE_TABLE) {
    child = make_IDListNode(strdup(ts->params.name), NULL);
    ts->tables_involved = deepcopy_IDListNode(child); // memoize
    sibling = collect_TablesFrom(ts->next_arg);
    return unionIDList(child, sibling);
  } else if (ts->node_type == ALIAS) {
    // in the case of the alias, the alias name is the table name as far as we
    // are concerned
    child = make_IDListNode(strdup(ts->params.name), NULL);
    ts->tables_involved = deepcopy_IDListNode(child); // memoize
    sibling = collect_TablesFrom(ts->next_arg);
    return unionIDList(child, sibling);
  } else {
    child = collect_TablesFrom(ts->arg);
    ts->tables_involved = deepcopy_IDListNode(child); // memoize
    sibling = collect_TablesFrom(ts->next_arg);
    return unionIDList(child, sibling);
  }
}

// Split from clause into a list of list of units to use for joins, returns a
// generic list
GenList *split_from(LogicalQueryNode *node) {
  if (node == NULL) {
    return NULL;
  } else if (node->node_type == CARTESIAN_PROD) { // break up crosses
    return list_prepend(split_from(node->next_arg), (void *)node->arg);
  } else { // return joins as given and others as well
    return list_alloc((void *)node);
  }
}

// Places equality-based filters as deeply as possible (even if that means
// before  join)
// Meanwhile, non-equality based filters (ie. all others) are placed on top of
// the join
// Operation closest to the necessary tables
// If it is not a join, but rather a cartesian product, we allow the filter to
// move deeper
LogicalQueryNode *deposit_one_filter_deeply(LogicalQueryNode *node,
                                            ExprNode *filter) {
  OPTIM_PRINT_DEBUG("depositing one filter");
  if (node == NULL || filter == NULL) {
    return node;
  } else {
    // both of these are memoized, so O(1), and simplifies call (less params)
    IDListNode *filter_tables = collect_TablesExpr(filter);
    IDListNode *node_tables = collect_TablesFrom(node);

    if (is_setEqual_IDLists(filter_tables,
                            node_tables)) { // we've arrived to the simplest
                                            // case, and is solely  possible for
                                            // equality filters, given the way
                                            // we use this function
      if (node->node_type == FILTER_WHERE) { // we already have an a filter
                                             // there, so just append to exprs
        node->params.exprs->next_sibling = filter;
        return node;
      } else { // otherwise create filter
        return make_filterWhere(node, filter);
      }
    } else if (is_subset_IDLists(filter_tables, node_tables)) {
      IDListNode *left_names = collect_TablesFrom(node->arg);
      IDListNode *right_names = collect_TablesFrom(node->next_arg);
      /*
      if(!is_eq_filter(filter) )
      { //this might be a candidate for a possible push filter
          if(node->node_type == POSS_PUSH_FILTER)
          { //we already created the possible push filter, so just append
              node->params.exprs->next_sibling = filter;
              return node;
          }
          else if(is_join_node(node->node_type))
          { //we just reached a join node, so create the possible push here
              printf("creating a new possible push filter\n");
              return make_PossPushFilter(node, filter);
          }
          else if(node->node_type == FILTER_WHERE)
          { //we need to place it underneath this, between it and the join
              node->arg = deposit_one_filter_deeply(node->arg, filter);
              return node;
          }
          else
          {
              //do nothing, this means we had a cross product, so we let
              //these filters float down
          }
      }
      */
      if (!is_eq_filter(filter)) {
        if (node->node_type == POSS_PUSH_FILTER) { // we already have a possible
                                                   // push filter to which we
                                                   // could add
          LogicalQueryNode *join =
              node->arg; // take out the join underneath it and inspect
          IDListNode *join_left = collect_TablesFrom(join->arg);
          IDListNode *join_right = collect_TablesFrom(join->next_arg);

          if (is_setEqual_IDLists(filter_tables, join_left) ||
              is_setEqual_IDLists(filter_tables,
                                  join_right)) { // the tables we might push
                                                 // this to are just underneath
                                                 // this join, so just add to
                                                 // this push filter
            node->params.exprs->next_sibling = filter;
            return node;
          }
        }

        if (is_join_node(node->node_type) &&
            (is_setEqual_IDLists(filter_tables, left_names) ||
             is_setEqual_IDLists(filter_tables,
                                 right_names))) { // we should create a possible
                                                  // push filter here
          return make_PossPushFilter(node, filter);
        }
      }

      // other cases fall through here
      if (is_subset_IDLists(filter_tables, left_names)) {

        node->arg = deposit_one_filter_deeply(node->arg, filter);
        return node;
      } else if (is_subset_IDLists(filter_tables, right_names)) {
        node->next_arg = deposit_one_filter_deeply(node->next_arg, filter);
        return node;
      } else { // this is the first node where the necessary tables are
               // accessible
        if (node->node_type ==
            FILTER_WHERE) { // we already have a filter there, so just append
          node->params.exprs->next_sibling = filter;
          return node;
        } else { // otherwise create filter
          return make_filterWhere(node, filter);
        }
      }
    } else { // our target is not in this part of the tree, return tree intact
      return node;
    }
  }
}

// Groups filters based on tables referenced and then deposits them as units, so
// that the filters that reference a specific set of tables
// can be treated as 1 sequence of filters in a single AST node
LogicalQueryNode *deposit_filters_deeply(LogicalQueryNode *table,
                                         ExprNode *filters) {
  OPTIM_PRINT_DEBUG("depositing filters deeply");
  ExprNode *curr_filter = filters;
  ExprNode *next_filter = NULL;
  ExprNode *filters_to_apply = NULL;
  ExprNode *rem_filters = NULL;
  IDListNode *names = NULL;

  while (curr_filter != NULL) {
    next_filter = curr_filter->next_sibling;
    curr_filter->next_sibling =
        NULL; // break link to search names approrpriately
    table = deposit_one_filter_deeply(table, curr_filter); // deposit filter
    curr_filter = next_filter; // filters remaining should be iterated over
  }

  return table;
}

// returns 1 if a node is a join of any kind
int is_join_node(LogicalQueryNodeType type) {
  return type == INNER_JOIN_ON || type == FULL_OUTER_JOIN_ON ||
         type == INNER_JOIN_USING || type == FULL_OUTER_JOIN_USING ||
         type == EQUI_JOIN_ON;
}

// Returns a general list of logical query nodes that have added the
// filters possible from filters argument
// filters argument is modified to end up only with filters that weren't applied
// no need to free filters that were applied as they have been reused in the
// plan's AST
GenList *add_filters(GenList *ts, ExprNode **filters) {
  OPTIM_PRINT_DEBUG("add possible filters to list of nodes");
  IDListNode *names = NULL;
  ExprNode *applicable_filters = NULL;
  ExprNode *inapplicable_filters = NULL;
  GenList *curr_table = ts;

  if ((*filters) == NULL) { // nothing left to apply
    return ts;
  }

  while (curr_table != NULL) {
    names = collect_TablesFrom(
        curr_table->data); // extract tables referenced in the current clause
    groupExpr_OnSubsetTables(*filters, &applicable_filters,
                             &inapplicable_filters, names);

    if (applicable_filters !=
        NULL) { // We should deposit these into the appropriate node
      curr_table->data = (void *)deposit_filters_deeply(
          (LogicalQueryNode *)curr_table->data,
          applicable_filters); // deposit relevant filters
    }

    *filters = inapplicable_filters; // we'll see if some of these apply to a
                                     // different node
    applicable_filters = NULL; // reset for next iteration
    inapplicable_filters = NULL; // reset for next iteration
    curr_table = curr_table->next;
  }

  return ts;
}

// returns 1 if the join_filter has at least 1 name in common with both
// arguments to a join
int join_is_possible(IDListNode *left, IDListNode *right,
                     IDListNode *join_filter) {
  return any_in_IDList(join_filter, left) &&
         any_in_IDList(join_filter, right); // a link in common
}

// count the number of equality filters that can be applied given a list of
// table names
int Expr_count_eq_filters(ExprNode *filters, IDListNode *table_names) {
  OPTIM_PRINT_DEBUG("counting relevant equality filters");
  char *extra_name = strdup(UNKNOWN_TABLE_NM);
  IDListNode *extended_names =
      make_IDListNode(extra_name, table_names); // add the unknown table name,
                                                // otherwise something like A.c1
                                                // = x will fail, as we'll get
                                                // UNKNOWN_TABLE_NM for x
  ExprNode *curr = filters, *next = NULL;
  int count = 0;

  while (curr != NULL) {
    next = curr->next_sibling;
    curr->next_sibling =
        NULL; // break link,make sure get correct table names below

    if (is_eq_filter(curr)) {
      if (is_subset_IDLists(collect_TablesExpr(curr), extended_names)) {
        count++;
      }
    }

    curr->next_sibling = next; // restore link
    curr = next;
  }

  free(extra_name);
  free(extended_names);
  return count;
}

// Warn user if a cross is missing a join clause and returns the union
void check_warn_Join() {
  if (!silence_warnings) {
    printf("Warning: there is a missing join clause for an implicit join");
    printf("Please make sure your join clauses use table prefixes for all "
           "references\n");
    printf("and your implicit join clauses are equality selections\n");
    printf("Offending from clause starts at line:%d, col:%d\n", query_line_num,
           query_col_num);
    printf("Silence these warnings with option -s\n");
  }
}

/// Implement a join heuristic which picks which 2 nodes to join based on which
/// allows for the largest # of equality based filters to be applied
// returns 1 if we are performing a join, vs 0 if we are going to have to cross
void join_heuristic(GenList *tables, ExprNode *join_filters,
                    ExprNode *reg_filters, LogicalQueryNode **left,
                    LogicalQueryNode **right) {
  OPTIM_PRINT_DEBUG("running join heuristic");
  int filter_iters = 0; // used to count how many times we've iterated through
                        // the fitler loop, in case we have no joins filters
  int max_ct_eq_filters = -1, ct_eq_filters; // initialize max to -1 so we
                                             // always pick at least 1 join per
                                             // call of function
  GenList *try_left, *try_right;
  ExprNode *try_join_filter, *next_join_filter;
  IDListNode *try_left_names, *try_right_names, *try_join_names;

  try_join_filter = join_filters;

  while (try_join_filter != NULL ||
         filter_iters < 1) // we either have join filters, or we don't (meaning
                           // we are dealing with across, so only iterate once)
  {
    filter_iters++;
    try_left = tables;

    if (try_join_filter != NULL) {
      next_join_filter =
          try_join_filter->next_sibling; // break link before searching names
      try_join_filter->next_sibling = NULL;
      try_join_names = collect_TablesExpr(try_join_filter); // search names
      try_join_filter->next_sibling = next_join_filter; // restore link
    }

    while (try_left != NULL) {
      try_right = tables;
      try_left_names = collect_TablesFrom(try_left->data);

      while (try_right != NULL) {
        if (try_right->id >
            try_left
                ->id) { // we check ids to avoid testing A join B and B join A
          // using GenList->id as a relative position marker solves this issue
          try_right_names = collect_TablesFrom(try_right->data);

          if (try_join_filter == NULL ||
              join_is_possible(
                  try_left_names, try_right_names,
                  try_join_names)) { // try_join_filter can be null for crosses

            ct_eq_filters = Expr_count_eq_filters(
                reg_filters, unionIDList(try_left_names, try_right_names));

            if (ct_eq_filters > max_ct_eq_filters) {
              max_ct_eq_filters = ct_eq_filters;
              *left = try_left->data;
              *right = try_right->data;
            }
          }
        }

        try_right = try_right->next;
      }

      try_left = try_left->next;
    }

    if (try_join_filter != NULL) {
      try_join_filter = try_join_filter->next_sibling;
    }
  }
}

// Given a list of tables, picks a join based on heuristic
// returns new list of tables, and modifies the join_filter list to remove those
// used
// uses reg_filters in the heuristic, but doesn't modify them
GenList *choose_next_join(GenList *tables, ExprNode **join_filters_ptr,
                          ExprNode *reg_filters) {
  OPTIM_PRINT_DEBUG("choosing next join");
  int max_ct_eq_filters =
      -1; // number of equality filters resulting from best join
  int ct_eq_filters = 0; // count of equality filters resulting from curr join
  LogicalQueryNode *chosen_left = NULL, *chosen_right = NULL;
  ExprNode *chosen_join = NULL;

  if (list_length(tables) == 1) { // there is nothing to join
    return tables;
  }

  // pick best join based on heuristic
  join_heuristic(tables, *join_filters_ptr, reg_filters, &chosen_left,
                 &chosen_right);

  // remove from list nodes that will combined by join
  tables = list_remove_first(tables, chosen_left);
  tables = list_remove_first(tables, chosen_right);

  if (*join_filters_ptr == NULL) { // we're performing a cross, warn user
    check_warn_Join();
    tables = list_append(tables, make_cross(chosen_left, chosen_right));
  } else {
    // all join filters relating to the current join
    ExprNode *relevant_join_filters = NULL;
    ExprNode *other_join_filters = NULL;
    IDListNode *combined_tables = unionIDList(collect_TablesFrom(chosen_left),
                                              collect_TablesFrom(chosen_right));
    // there might be more than 1 relevant join filter, recall that our
    // implementation is simplistic and tests each filter separately
    // TODO: improve the implementation of heuristic to avoid separately testing
    // join_filters that should be treated together
    groupExpr_OnSubsetTables(*join_filters_ptr, &relevant_join_filters,
                             &other_join_filters, combined_tables);
    // create new node and add to list
    tables =
        list_append(tables, make_joinOn(EQUI_JOIN_ON, chosen_left, chosen_right,
                                        relevant_join_filters));
    // the join filters that remain are those that are irrelevant
    *join_filters_ptr = other_join_filters;
  }

  return tables;
}

int is_simple_from(LogicalQueryNode *node) {
  return node->node_type == SIMPLE_TABLE || node->node_type == ALIAS ||
    node->node_type == CONCATENATE_FUN || node->node_type == FLATTEN_FUN;
}

void optim_from(LogicalQueryNode **from, LogicalQueryNode **where) {
  if (*where == NULL) { // TODO: and if from has cross: check_warn_Join();
    return; // nothing to do
  }

  OPTIM_PRINT_DEBUG("optimizing from clause");
  ExprNode *join_filters = NULL;
  ExprNode *other_filters = NULL;
  // ExprNode *all_filters = (*where)->params.exprs->first_child;
  ExprNode *all_filters = (*where)->params.exprs;

  // free the params.expr
  // free((*where)->params.exprs);
  groupExpr_OnJoin(all_filters, &join_filters,
                   &other_filters); // separate into join and others

  ExprNode *oi_filters = NULL;
  ExprNode *od_filters = NULL;
  partExpr_OnOrder(other_filters, &oi_filters, &od_filters); // separete on
                                                             // order

  ExprNode *no_agg_filters = NULL;
  ExprNode *agg_filters = NULL;
  partExpr_OnAgg(oi_filters, &no_agg_filters,
                 &agg_filters); // separate into no-agg and agg using

  ExprNode *known_filters = NULL;
  ExprNode *unknown_filters = NULL;
  // separate into known and unknown references
  groupExpr_OnUnknownTables(no_agg_filters, &unknown_filters, &known_filters);

  // Separate known filters into equality based and other filters
  ExprNode *eq_filters = NULL;
  ExprNode *non_eq_filters = NULL;
  groupExpr_OnEqFilter(known_filters, &eq_filters, &non_eq_filters);

  GenList *sep_ts = split_from(*from); // split from into a list of tables

  while (list_length(sep_ts) != 1) {
    sep_ts =
        choose_next_join(sep_ts, &join_filters,
                         eq_filters); // choose join based on equality filters
    add_filters(sep_ts, &eq_filters); // apply equality filters prior to join
  }

  // add any equality filters that might remain (consider that the table list
  // might have already been of len = 1 if only 1 explicit join, since we don't
  // cut up user joins)
  add_filters(sep_ts, &eq_filters);

  // add non-equality filters on top of joins as possible filters to push
  // so apply after all joins have been performed
  add_filters(sep_ts, &non_eq_filters);

  LogicalQueryNode *table = sep_ts->data;

  free(sep_ts);

  // unknown no aggs, then agg_filters
  ExprNode *remaining_oi_filters = NULL;
  remaining_oi_filters = append_toExpr(remaining_oi_filters, unknown_filters);
  remaining_oi_filters = append_toExpr(remaining_oi_filters, agg_filters);

  if (remaining_oi_filters !=
      NULL) { // if we still have things to filter and we can apply, apply them
    table = make_filterWhere(table, remaining_oi_filters);
  }

  // set from clause to new reduced tables
  *from = table;

  // if we still have OD filters, make them a new where clause, otherwise set to
  // null
  if (od_filters != NULL) {
    //*where = make_filterWhere(NULL, make_exprListNode(od_filters));
    *where = make_filterWhere(NULL, od_filters);
  } else {
    *where = NULL;
  }
}

LogicalQueryNode *optim_sort_where(LogicalQueryNode *proj,
                                   LogicalQueryNode *from,
                                   LogicalQueryNode *order,
                                   LogicalQueryNode *where,
                                   LogicalQueryNode *grouphaving) {
  OPTIM_PRINT_DEBUG("optimizing where clause");
  LogicalQueryNode *order_indep_filter = NULL;
  LogicalQueryNode *remaining_filter = NULL;
  LogicalQueryNode *sort = NULL;
  ExprNode *order_indep_exprs = NULL;
  ExprNode *remaining_exprs = NULL;

  // partExpr_OnOrder(where->params.exprs->first_child, &order_indep_exprs,
  // &remaining_exprs);
  partExpr_OnOrder(where->params.exprs, &order_indep_exprs, &remaining_exprs);

  if (order_indep_exprs != NULL) { // perform any order-independent filtering up
                                   // to first order-dependent first
    OPTIM_PRINT_DEBUG("found order independent where predicates");
    // print_expr(order_indep_exprs, i, &i);
    order_indep_filter = make_filterWhere(NULL, order_indep_exprs);
  }

  remaining_filter = make_filterWhere(NULL, remaining_exprs);

  // Find all column references in projection and groupbyclauses

  OPTIM_PRINT_DEBUG("collecting where OD references");
  IDListNode *order_cols_where = collect_sortCols(remaining_exprs, 0);

  OPTIM_PRINT_DEBUG("collecting projection all col references");
  IDListNode *all_cols_proj = collect_AllColsProj(proj);

  OPTIM_PRINT_DEBUG("collecting projection all col references");
  IDListNode *all_cols_group = collect_AllColsGroupby(grouphaving);

  IDListNode *all_cols = unionIDList(all_cols_proj, all_cols_group);
  all_cols = unionIDList(all_cols, order_cols_where);

  if (in_IDList("*",
                all_cols)) { // referencing "all columns" results in a full sort
    sort = order;
  } else {
    sort = make_sortCols(order->params.order, all_cols);
  }

  // filter order independent, then sort, then filter order dependent
  where = pushdown_logical(sort, order_indep_filter);
  where = pushdown_logical(remaining_filter, where);
  return where;
}

// Operations associated with a order-dependent group-by/having
LogicalQueryNode *optim_sort_group_od(LogicalQueryNode *proj,
                                      LogicalQueryNode *from,
                                      LogicalQueryNode *order,
                                      LogicalQueryNode *where,
                                      LogicalQueryNode *grouphaving) {
  OPTIM_PRINT_DEBUG("optimizing order-dependent group-by/having");
  LogicalQueryNode *sort = NULL;

  IDListNode *all_cols_proj = collect_AllColsProj(proj);
  IDListNode *all_cols_group = collect_AllColsGroupby(grouphaving);
  IDListNode *all_cols = unionIDList(all_cols_group, all_cols_proj);

  if (in_IDList("*",
                all_cols)) { // referencing "all columns" results in a full sort
    sort = order;
  } else {
    sort = make_sortCols(order->params.order, all_cols);
  }

  return pushdown_logical(grouphaving, sort);
}

// Operations associated with a order-independent group-by/having
LogicalQueryNode *optim_sort_group_oi(LogicalQueryNode *proj,
                                      LogicalQueryNode *from,
                                      LogicalQueryNode *order,
                                      LogicalQueryNode *where,
                                      LogicalQueryNode *grouphaving) {
  OPTIM_PRINT_DEBUG("optimizing order-independent group-by/having");
  LogicalQueryNode *sort = NULL;

  // assume need sorting in proj clause unless order annihilating operators
  // applied
  IDListNode *proj_order_cols =
      collect_sortColsNamedExpr(proj->params.namedexprs);
  IDListNode *all_cols_group = collect_AllColsGroupby(grouphaving);
  IDListNode *all_cols_proj =  collect_AllColsProj(proj);
  // create copy before unioning, since unioning makes original lists useless
  IDListNode *all_cols_group_and_proj_order = unionIDList(all_cols_group, deepcopy_IDListNode(proj_order_cols));
  IDListNode *all_cols = unionIDList(deepcopy_IDListNode(all_cols_group), deepcopy_IDListNode(all_cols_proj));
  
  if (proj_order_cols == NULL) { // no order dependencies in projection
    return grouphaving;
  } else if (in_IDList("*", proj_order_cols)) { // referencing "all columns"
    // results in a full sort
    sort = order;
    return pushdown_logical(grouphaving, sort);
  } else if (any_in_IDList(all_cols_group, proj_order_cols)) {
    // if any of the columns used in grouping are order dependent in projection,
    // given that we sort prior to grouping, we need to actually sort
    // all columns used
    sort = make_sortCols(order->params.order, all_cols);
    return pushdown_logical(grouphaving, sort);
  } else { // else sort dependent columns, then group (based on experimental
           // results found in aquery/test/groupsorting.q and
           // aquery/test/grp_viz.r)
    // Note that sorting dependent columns before in turn means
    // that we also have to sort the columns used in the group-by clause
    sort = make_sortCols(order->params.order, all_cols_group_and_proj_order);
    return pushdown_logical(grouphaving, sort);
  }

  return grouphaving;
}

LogicalQueryNode *optim_sort_proj(LogicalQueryNode *proj,
                                  LogicalQueryNode *from,
                                  LogicalQueryNode *order,
                                  LogicalQueryNode *where,
                                  LogicalQueryNode *grouphaving) {
  OPTIM_PRINT_DEBUG("optimizing projection");
  // assume need sorting in proj clause unless order annihilating operators
  // applied
  IDListNode *proj_order_cols =
      collect_sortColsNamedExpr(proj->params.namedexprs);
  LogicalQueryNode *sort = NULL;

  if (in_IDList("*", proj_order_cols)) { // referencing "all columns" results in
                                         // a full sort
    sort = order;
    return pushdown_logical(proj, sort);
  } else if (proj_order_cols == NULL) { // no order dependencies
    return proj;
  } else { // only sort specific columns necessary
    sort = make_sortCols(order->params.order, proj_order_cols);
    return pushdown_logical(proj, sort);
  }
}

// TODO: modify to handle more complicated froms....
char *get_table_name(LogicalQueryNode *from) {
  if (from == NULL)
  {
    return NULL;
  }
  else if (from->node_type == SIMPLE_TABLE)
  {
    return from->params.name;
  }
  else
  {
    return get_table_name(from->arg);
  }
}

LogicalQueryNode *assemble_opt1(LogicalQueryNode *proj, LogicalQueryNode *from,
                                LogicalQueryNode *order,
                                LogicalQueryNode *where,
                                LogicalQueryNode *grouphaving) {

  if (!is_simple_from(from)) { // we have to push selections and choose joins
    optim_from(&from, &where);
  }

  if (order == NULL) {
    return assemble_base(proj, from, order, where, grouphaving);
  }

  // TODO: look up order in a way that accounts for complicated from
  if (order !=
      NULL) { // try to remove order clause first as a result of existing order
    char *tablenm = get_table_name(from);
    Symentry *entry = (tablenm == NULL) ? NULL : lookup_sym(env, tablenm);
    OrderNode *want_order = order->params.order;
    OrderNode *have_order = (entry == NULL) ? NULL : entry->order_spec;

    order->params.order = get_NeededSort(have_order, want_order);

    if (order->params.order == NULL) {
      order = NULL;
    }
  }

  if (order != NULL) {
    if (where != NULL && where->order_dep) {
      where = optim_sort_where(proj, from, order, where, grouphaving);
    } else if (grouphaving != NULL && grouphaving->order_dep) {
      grouphaving = optim_sort_group_od(proj, from, order, where, grouphaving);
    } else if (grouphaving != NULL && !grouphaving->order_dep) {
      grouphaving = optim_sort_group_oi(proj, from, order, where, grouphaving);
    } else // just check projection clauses
    {
      proj = optim_sort_proj(proj, from, order, where, grouphaving);
    }
  }

  // send things along to assemble as usual...
  return assemble_base(proj, from, NULL, where, grouphaving);
}

LogicalQueryNode *assemble_plan(LogicalQueryNode *proj, LogicalQueryNode *from,
                                LogicalQueryNode *order,
                                LogicalQueryNode *where,
                                LogicalQueryNode *grouphaving) {
  OPTIM_PRINT_DEBUG("building query");
  if (optim_level == 1) {
    return assemble_opt1(proj, from, order, where, grouphaving);
  } else {
    return assemble_base(proj, from, order, where, grouphaving);
  }
}

void print_table_dummy(GenList *t) {
  printf("-->ELEM IN LIST\n");
  int x = 0;
  print_logical_query((LogicalQueryNode *)t->data, x, &x);
}

void print_join_clause(GenList *e) {
  printf("%d ", is_JoinClause((ExprNode *)e->data));
}

#if STAND_ALONE
int main() {
  int x = 0;

  // Testing generic lists
  printf("testing generic list\n");
  GenList *list = NULL;
  char *str1 = "this";
  char *str2 = "is";
  char *str3 = "fun";
  list = list_append(list, (void *)str2);
  list = list_prepend(list, (void *)str1);
  list = list_append(list, (void *)str3);
  list_foreach(list, print_str);
  list = list_remove_first(list, str2);
  printf("removing is\n");
  list_foreach(list, print_str);
  char *other = "bla";

  printf("removing something that is not in list\n");
  list = list_remove_first(list, other);
  list_foreach(list, print_str);

  /// Testing extracting indirect order dependencies affected by sorting
  Symtable *env = init_symtable();
  ExprNode *id1 = make_id(env, strdup("c1"));
  ExprNode *id2 = make_id(env, strdup("c2"));
  ExprNode *id31 = make_id(env, strdup("c3"));
  ExprNode *id32 = make_id(env, strdup("c3"));
  ExprNode *id4 = make_id(env, strdup("c4"));

  ExprNode *op1 = make_arithNode(PLUS_EXPR, id31, id4); // c3 + c4
  ExprNode *op2 =
      make_callNode(make_builtInFunNode(env, strdup("min")), op1); // min(c3+c4)
  ExprNode *op3 = make_arithNode(PLUS_EXPR, id2, op2); // c2 + min(c3+c4)
  ExprNode *op4 = make_arithNode(MULT_EXPR, id1, op3); // c1 * (c2 + min(c3+c4))
  ExprNode *op5 = make_callNode(make_builtInFunNode(env, strdup("max")),
                                op4); // max(c1 * (c2 + min(c3 + c4)))
  ExprNode *op6 =
      make_callNode(make_builtInFunNode(env, strdup("sums")), id32); // sums(c3)
  ExprNode *comb = make_arithNode(
      MULT_EXPR, op5, op6); // max(c1 * (c2 + min(c3 + c4))) * sums(c3)

  printf("digraph {\n");
  print_expr(comb, x, &x);
  printf("}\n");

  IDListNode *deps = collect_sortCols(comb, 0);
  printf("final sorting list\n");
  print_id_list(deps, x, &x); // should be c3 and c4

  // Testing partitioning an expression list based on order dependency
  // information
  // Need to allocate mem for strings to simulate how things wokr when within
  // bison
  ExprNode *p_id1 = make_id(env, strdup("c1"));
  ExprNode *p_id2 = make_id(env, strdup("c2"));
  p_id1->order_dep = 0;
  p_id2->order_dep = 0;

  ExprNode *p_id3 = make_id(env, strdup("c3"));
  p_id3->order_dep = 1;

  ExprNode *p_id4 = make_id(env, strdup("c4"));
  p_id4->order_dep = 1;

  p_id1->next_sibling = p_id2;
  p_id2->next_sibling = p_id3;
  p_id3->next_sibling = p_id4;

  ExprNode *exprlist = make_exprListNode(p_id1);
  ExprNode *order_dep = NULL;
  ExprNode *order_indep = NULL;

  partExpr_OnOrder(exprlist->first_child, &order_indep, &order_dep);
  printf("order-independent expressions from paritioning\n");
  print_expr(order_indep, x, &x);
  printf("order-dependent expressions from paritioning\n");
  print_expr(order_dep, x, &x);

  // Testing extracting all column references...
  printf("All columns\n");
  print_id_list(collect_AllCols(comb), x, &x);
  printf("expression\n");
  print_expr(comb, x, &x);

  // Testing order dependencies in named expressions
  printf("testing order dependencies in named expressions\n");
  NamedExprNode *n1 = make_NamedExprNode(strdup("calc_1"), comb);
  ExprNode *idx = make_id(env, strdup("rand"));
  ExprNode *idx2 = make_id(env, strdup("c4"));
  ExprNode *add_em = make_arithNode(
      PLUS_EXPR, idx,
      idx2); // contaminate rand with c4, so now need to sort rand, c4, c3
  NamedExprNode *n2 = make_NamedExprNode(strdup("calc_2"), add_em);
  n1->next_sibling = n2;
  printf("looking for order dependencies in named expressions\n");
  IDListNode *found = collect_sortColsNamedExpr(n1);
  print_id_list(found, x, &x);

  // Testing deep copy of expressions
  printf("testing deep copies of expressions\n");
  printf("original\n");
  print_expr(comb, x, &x);
  printf("deep copy\n");
  ExprNode *copy_of_comb = deepcopy_ExprNode(comb);
  print_expr(copy_of_comb, x, &x);

  // Testing deep copy of named expression
  printf("testing deep copies of named expressions\n");
  printf("original\n");
  print_named_expr(n1, x, &x);
  printf("deep copy\n");
  NamedExprNode *copy_of_n1 = deepcopy_NamedExprNode(n1);
  print_named_expr(copy_of_n1, x, &x);

  // Testing extraction of join clauses
  printf("testing join clauses\n");
  ExprNode *table_A = make_id(env, "A");
  ExprNode *table_B = make_id(env, "B");
  ExprNode *table_C = make_id(env, "C");
  ExprNode *col_1 = make_id(env, "c1");
  ExprNode *col_2 = make_id(env, "c2");
  ExprNode *acc1 = make_colDotAccessNode(deepcopy_ExprNode(table_A),
                                         deepcopy_ExprNode(col_1)); // A.c1
  ExprNode *acc2 = make_colDotAccessNode(deepcopy_ExprNode(table_B),
                                         deepcopy_ExprNode(col_1)); // B.c1
  ExprNode *acc3 = make_colDotAccessNode(deepcopy_ExprNode(table_C),
                                         deepcopy_ExprNode(col_2)); // C.c2
  ExprNode *jc_1 = make_compNode(NEQ_EXPR, deepcopy_ExprNode(acc1),
                                 deepcopy_ExprNode(acc2)); // A.c1 != B.c1
  ExprNode *jc_2 = make_compNode(EQ_EXPR, deepcopy_ExprNode(acc2),
                                 deepcopy_ExprNode(acc3)); // B.c1 = C.c2
  ExprNode *reg =
      make_compNode(LT_EXPR, deepcopy_ExprNode(col_1), make_int(10)); // c1 = 10
  ExprNode *reg2 = make_compNode(NEQ_EXPR, deepcopy_ExprNode(acc1),
                                 make_id(env, "some_col"));

  ExprNode *full = deepcopy_ExprNode(jc_1);
  full->next_sibling = deepcopy_ExprNode(reg);
  full->next_sibling->next_sibling = deepcopy_ExprNode(reg2);
  full->next_sibling->next_sibling->next_sibling = deepcopy_ExprNode(jc_2);
  ExprNode *full_list = make_exprListNode(full);
  print_expr(full_list, x, &x);

  printf("testing grouping join clauses and separating from others\n");
  ExprNode *join_filters = NULL;
  ExprNode *other_filters = NULL;
  groupExpr_OnJoin(full_list->first_child, &join_filters, &other_filters);
  printf("printing join clauses\n");
  print_expr(join_filters, x, &x);
  printf("printing remaining clauses\n");
  print_expr(other_filters, x, &x);

  // Testing split from clause
  LogicalQueryNode *table1 = make_table("t1");
  LogicalQueryNode *table2 = make_table("t2");
  LogicalQueryNode *table3 = make_table("t3");
  LogicalQueryNode *table4 = make_table("t4");
  ExprNode *join_cond = deepcopy_ExprNode(jc_1);
  LogicalQueryNode *joined_tables =
      make_joinOn(INNER_JOIN_ON, table3, table4, join_cond);
  LogicalQueryNode *cross1 = make_cross(table1, table2);
  LogicalQueryNode *cross2 = make_cross(joined_tables, cross1);
  GenList *split = split_from(cross2);
  printf("PRINTING SPLIT LIST\n");
  list_foreach(split, print_table_dummy);

  // Splitting an expression based on aggregate use
  printf("testing parting expressions based on first use of aggregate\n");
  ExprNode *agg_id1 = make_id(env, "c1");
  ExprNode *agg_id2 = make_id(env, "c2");
  ExprNode *agg_id3 = make_id(env, "c3");
  ExprNode *agg_id4 = make_id(env, "c4");
  ExprNode *agg_id5 = make_id(env, "c5");
  ExprNode *agg_id6 = make_id(env, "c1");
  ExprNode *agg_e1 = make_logicOpNode(EQ_EXPR, deepcopy_ExprNode(agg_id1),
                                      deepcopy_ExprNode(agg_id2));
  ExprNode *agg_e2 = make_logicOpNode(EQ_EXPR, deepcopy_ExprNode(agg_id3),
                                      deepcopy_ExprNode(agg_id4));

  // add a fake UDF that does not use aggregates
  put_sym(env, "my_fake_fun", FUNCTION_TYPE, 0, 0);

  ExprNode *agg_e3 = make_callNode(make_udfNode(env, "my_fake_fun"),
                                   deepcopy_ExprNode(agg_id5));
  ExprNode *agg_e4 = make_logicOpNode(EQ_EXPR, deepcopy_ExprNode(agg_id5),
                                      deepcopy_ExprNode(agg_id6));
  ExprNode *agg_e5 = make_callNode(make_builtInFunNode(env, "sums"),
                                   deepcopy_ExprNode(agg_id6));
  ExprNode *agg_e6 =
      make_arithNode(PLUS_EXPR, deepcopy_ExprNode(agg_e5), make_int(10));
  agg_e1->next_sibling = agg_e2;
  agg_e2->next_sibling = agg_e3;
  agg_e3->next_sibling = agg_e4;
  agg_e4->next_sibling = agg_e6;
  ExprNode *no_agg = NULL;
  ExprNode *agg = NULL;
  partExpr_OnAgg(agg_e1, &no_agg, &agg);
  printf("pre-aggregate\n");
  print_expr(no_agg, x, &x);
  printf("post-aggregate\n");
  print_expr(agg, x, &x);

  // Testing various functions on IDList
  printf("testing IDList functions\n");
  IDListNode *l1 =
      make_IDListNode("c1", make_IDListNode("c2", make_IDListNode("c3", NULL)));
  IDListNode *subset = make_IDListNode("c2", make_IDListNode("c3", NULL));
  IDListNode *exact = deepcopy_IDListNode(l1);
  IDListNode *none = make_IDListNode("bla", NULL);

  printf("testing if any of subset in l1:%d\n", any_in_IDList(subset, l1));
  printf("testing if any of none in l1:%d\n", any_in_IDList(none, l1));
  printf("testing if subset is subset of l1:%d\n",
         is_subset_IDLists(subset, l1));
  printf("testing if none is subset of  l1:%d\n", is_subset_IDLists(none, l1));
  printf("testing if subset is set equal of l1:%d\n",
         is_setEqual_IDLists(subset, l1));
  printf("testing if exact is subset of l1:%d\n", is_subset_IDLists(exact, l1));
  printf("testinf if exact is set equal of l1:%d\n",
         is_setEqual_IDLists(exact, l1));
  printf("length of l1:%d\n", length_IDList(l1));

  // Testing collecting tables referenced in a from clause
  printf("testing collecting tables referenced in from clause\n");
  LogicalQueryNode *refs_table1 = make_alias(make_table("t1"), "other_t1");
  LogicalQueryNode *refs_table2 = make_table("t2");
  LogicalQueryNode *refs_table3 = make_table("t3");
  LogicalQueryNode *refs_table4 = make_table("t4");
  LogicalQueryNode *refs_combined = make_cross(
      refs_table1,
      make_cross(refs_table2, make_joinUsing(INNER_JOIN_USING, refs_table3,
                                             refs_table4, NULL)));
  IDListNode *refs_tables_names = collect_TablesFrom(refs_combined);
  printf("tables found:\n");
  print_id_list(refs_tables_names, x, &x);

  // Testing collecting tables referenced in expressions
  printf("testing collecting tables referenced in expression\n");
  ExprNode *refs_expr = deepcopy_ExprNode(full_list);
  IDListNode *refs_expr_names = collect_TablesExpr(full_list->first_child);
  printf("tables found:\n");
  print_id_list(refs_expr_names, x, &x);
  printf("testing breaking link between expressions for name purposes\n");
  ExprNode *next_expr = refs_expr->first_child->next_sibling;
  refs_expr->first_child->next_sibling = NULL;
  IDListNode *reduced_names = collect_TablesExpr(refs_expr->first_child);
  printf("tables found:\n");
  print_id_list(reduced_names, x, &x);

  // Testing grouping expressions based on tables referenced
  printf("testing grouping expressions based on subset of tables referenced\n");
  IDListNode *table_names =
      make_IDListNode("A", make_IDListNode("B", make_IDListNode("C", NULL)));
  // Create expression
  ExprNode *have = NULL;
  ExprNode *dont = NULL;
  ExprNode *expr1 =
      make_colDotAccessNode(make_id(env, "A"), make_id(env, "c1"));
  ExprNode *expr2 = make_id(env, "c2");
  expr1->next_sibling = expr2;
  groupExpr_OnSubsetTables(deepcopy_ExprNode(expr1), &have, &dont, table_names);
  printf("expressions that have subset:\n");
  print_expr(have, x, &x);
  printf("expressions that do not have subset:\n");
  print_expr(dont, x, &x);

  // has all
  printf("testing grouping expressions based on exact set equality of tables "
         "referenced\n");
  ExprNode *expr3 = make_arithNode(
      PLUS_EXPR,
      make_arithNode(
          MINUS_EXPR,
          make_colDotAccessNode(make_id(env, "A"), make_id(env, "c1")),
          make_colDotAccessNode(make_id(env, "B"), make_id(env, "c2"))),
      make_colDotAccessNode(make_id(env, "C"), make_id(env, "c3")));
  expr2->next_sibling = expr3;
  ExprNode *exact_refs = NULL;
  ExprNode *not_exact_refs = NULL;
  groupExpr_OnEqualTables(deepcopy_ExprNode(expr1), &exact_refs,
                          &not_exact_refs, table_names);
  printf("expressions that have exact match:\n");
  print_expr(exact_refs, x, &x);
  printf("expressions that do not have exact match:\n");
  print_expr(not_exact_refs, x, &x);

  // Testing filter depositing
  printf("testing filter depositing\n");
  LogicalQueryNode *orig = make_cross(make_table("t1"), make_table("t2"));
  ExprNode *f1 = make_compNode(
      EQ_EXPR, make_colDotAccessNode(make_id(env, "t1"), make_id(env, "c1")),
      make_int(10));
  ExprNode *f2 = make_compNode(
      NEQ_EXPR, make_colDotAccessNode(make_id(env, "t1"), make_id(env, "c2")),
      make_int(20));
  ExprNode *f3 = make_compNode(
      EQ_EXPR, make_colDotAccessNode(make_id(env, "t2"), make_id(env, "c3")),
      make_int(30));

  // Testing depositing 1 filter deeply
  printf("testing depositing 1 filter deeply");
  printf("original tree:\n");
  print_logical_query(orig, x, &x);

  ExprNode *one_filter = deepcopy_ExprNode(f1);
  LogicalQueryNode *with_one_filter =
      deposit_one_filter_deeply(deepcopy_LogicalQueryNode(orig), one_filter);
  printf("with one deeply placed filter:\n");
  print_logical_query(with_one_filter, x, &x);

  // Testing depositing various relevant filters
  printf("testing depositing various relevant filters\n");
  ExprNode *various_filters = deepcopy_ExprNode(f1);
  various_filters->next_sibling = deepcopy_ExprNode(f2);
  various_filters->next_sibling->next_sibling = deepcopy_ExprNode(f3);
  printf("original tree:\n");
  print_logical_query(orig, x, &x);
  LogicalQueryNode *with_var_filters =
      deposit_filters_deeply(deepcopy_LogicalQueryNode(orig), various_filters);
  printf("with various deeply placed filters:\n");
  print_logical_query(with_var_filters, x, &x);
  // TODO: test A X(B X C) and have filters on A, filters on (A,B) and (A,B,C)

  // Testing futher depositing of relevant filters
  printf("further tests on depositing relevant filters\n");
  LogicalQueryNode *diff =
      make_cross(make_table("A"), make_cross(make_table("B"), make_table("C")));
  ExprNode *filter_A =
      make_colDotAccessNode(make_id(env, "A"), make_id(env, "c1"));
  ExprNode *filter_AB = make_compNode(
      EQ_EXPR, make_colDotAccessNode(make_id(env, "B"), make_id(env, "c2")),
      deepcopy_ExprNode(filter_A));
  ExprNode *filter_ABC = make_arithNode(
      PLUS_EXPR, make_colDotAccessNode(make_id(env, "C"), make_id(env, "c3")),
      deepcopy_ExprNode(filter_AB));
  filter_A->next_sibling = filter_AB;
  filter_AB->next_sibling = filter_ABC;
  printf("original tree:\n");
  print_logical_query(diff, x, &x);
  LogicalQueryNode *with_diff_filters =
      deposit_filters_deeply(deepcopy_LogicalQueryNode(diff), filter_A);
  printf("with difficult various deeply placed filters:\n");
  print_logical_query(with_diff_filters, x, &x);

  // Testing depositing filters, some of which are relevant to only part of the
  // list of tables
  printf("testing add filters\n");
  LogicalQueryNode *rel_table1 = make_table("A");
  LogicalQueryNode *rel_table2 = make_cross(make_table("B"), make_table("C"));
  GenList *rel_table_list = list_prepend(NULL, rel_table1);
  rel_table_list = list_prepend(rel_table_list, rel_table2);
  ExprNode *rel_filter1 = deepcopy_ExprNode(filter_A);
  ExprNode *rel_filter2 =
      make_colDotAccessNode(make_id(env, "B"), make_id(env, "c2"));
  ExprNode *rel_filter3 =
      make_colDotAccessNode(make_id(env, "C"), make_id(env, "c3"));
  ExprNode *rel_filter4 = make_compNode(EQ_EXPR, deepcopy_ExprNode(rel_filter2),
                                        deepcopy_ExprNode(rel_filter3));
  rel_filter1->next_sibling = rel_filter2;
  rel_filter2->next_sibling = rel_filter3;
  rel_filter3->next_sibling = rel_filter4;
  printf("printing original list of trees:\n");
  list_foreach(rel_table_list, print_table_dummy);
  printf("applying filters\n");
  rel_table_list = add_filters(rel_table_list, &rel_filter1);
  printf("filters are null after all applied:%d\n", rel_filter1 == NULL);
  printf("printing modified list of trees:\n");
  list_foreach(rel_table_list, print_table_dummy);

  printf("testing if a join is possible\n");
  IDListNode *left_names = make_IDListNode("A", make_IDListNode("B", NULL));
  IDListNode *right_names = make_IDListNode("C", NULL);
  IDListNode *join_poss_names =
      make_IDListNode("C", make_IDListNode("B", NULL));
  IDListNode *join_notposs_names = make_IDListNode("C", NULL);
  printf("possible join is possible:%d\n",
         join_is_possible(left_names, right_names, join_poss_names));
  printf("not possible join is possible:%d\n",
         join_is_possible(left_names, right_names, join_notposs_names));

  printf("testing counting equality filters\n");
  ExprNode *rel_eq = make_compNode(
      EQ_EXPR, make_colDotAccessNode(make_id(env, "B"), make_id(env, "c2")),
      make_id(env, "some_id"));
  ExprNode *irrel_eq = make_compNode(
      EQ_EXPR, make_colDotAccessNode(make_id(env, "C"), make_id(env, "c3")),
      make_id(env, "other_id"));
  ExprNode *rel_neq =
      make_colDotAccessNode(make_id(env, "B"), make_id(env, "c4"));
  rel_eq->next_sibling = irrel_eq;
  irrel_eq->next_sibling = rel_neq;
  IDListNode *rel_names = make_IDListNode("B", NULL);
  printf("number of relevant:%d\n", Expr_count_eq_filters(rel_eq, rel_names));

  // Testing Join heuristic
  printf("testing join heuristic\n");
  GenList *join_tables = list_append(NULL, make_table("A"));
  join_tables = list_append(join_tables, make_table("B"));
  join_tables = list_append(join_tables, make_table("C"));
  ExprNode *jf1 = make_compNode(
      EQ_EXPR, make_colDotAccessNode(make_id(env, "A"), make_id(env, "c1")),
      make_colDotAccessNode(make_id(env, "B"), make_id(env, "c1")));
  ExprNode *jf2 = make_compNode(
      EQ_EXPR, make_colDotAccessNode(make_id(env, "B"), make_id(env, "c2")),
      make_colDotAccessNode(make_id(env, "C"), make_id(env, "c2")));
  jf1->next_sibling = jf2;
  ExprNode *regf =
      deepcopy_ExprNode(jf1); // we'll pretend this is a regular filter
  LogicalQueryNode *chosen_left = NULL, *chosen_right = NULL;
  join_heuristic(join_tables, jf1, regf, &chosen_left, &chosen_right);
  printf("chosen left:\n");
  print_logical_query(chosen_left, x, &x);
  printf("chosen right:\n");
  print_logical_query(chosen_right, x, &x);

  printf("testing choose_next_join\n");
  printf("original list of trees:\n");
  list_foreach(join_tables, print_table_dummy);
  printf("picking next join and applying\n");
  join_tables = choose_next_join(join_tables, &jf1, regf);
  printf("remaining join filter:\n");
  print_expr(jf1, x, &x);
  printf("new list of tress:\n");
  list_foreach(join_tables, print_table_dummy);

  /*
  Add to header file
  Functions to test:
  void *optim_from(LogicalQueryNode **from, LogicalQueryNode **where)

  */
}
#endif
