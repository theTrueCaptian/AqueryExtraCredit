#ifndef AQUERY_OPTIMIZER_H
#define AQUERY_OPTIMIZER_H

#include "../ast/ast.h"
#include "../types/aquery_types.h"

// Implementing a generic single linked list for use in the optimizer
typedef struct GenList {
  int id; // useful sometimes to compare positions in list
  void *data;
  struct GenList *next;
} GenList;

// Operations on generic linked list
GenList *list_alloc(void *data);
GenList *list_append(GenList *list, void *data);
GenList *list_prepend(GenList *list, void *data);
GenList *list_remove_first(GenList *list, void *data);
void list_foreach(GenList *list, void (*fun)(GenList *));
void list_foreach_warg(GenList *list, void (*fun)(GenList *, void *),
                       void *arg);
int list_length(GenList *list);

// Deep-copies of parts of AST
LogicalQueryNode *deepcopy_LogicalQueryNode(LogicalQueryNode *node);
NamedExprNode *deepcopy_NamedExprNode(NamedExprNode *node);
IDListNode *deepcopy_IDListNode(IDListNode *node);
OrderNode *deepcopy_OrderNode(OrderNode *node);
ExprNode *deepcopy_ExprNode(ExprNode *node);

// Mapping Lis_subset_IDListsists of TableNames to expressions
typedef struct TablesToExprsMap {
  IDListNode *tables;
  ExprNode *exprs;
  struct TablesToExprsMap *next_tuple;
} TablesToExprsMap;

TablesToExprsMap *make_TablesToExprsMap(IDListNode *tables, ExprNode *exprs,
                                        TablesToExprsMap *map);

int is_eq_filter(ExprNode *expr);
int is_join_node(LogicalQueryNodeType type);

// Manipulating exprnodes as lists and splitting
void partExpr_atFirst(ExprNode *orig, ExprNode **before, ExprNode **after,
                      int (*pred)(ExprNode *));
void partExpr_OnOrder(ExprNode *expr, ExprNode **order_indep,
                      ExprNode **order_dep);
void groupExpr_OnEqFilter(ExprNode *expr, ExprNode **eq_filters,
                          ExprNode **other_filters);

void groupExpr_onUnaryPred(ExprNode *orig, ExprNode * * true, ExprNode * *false,
                           int (*pred)(ExprNode *));
void groupExpr_onBinaryPred(ExprNode *orig, ExprNode * * true,
                            ExprNode * *false, int (*pred)(ExprNode *, void *),
                            void *data);

// ID lists utilities
IDListNode *unionIDList(IDListNode *x, IDListNode *y);
int in_IDList(const char *name, IDListNode *list);
int any_in_IDList(IDListNode *l1, IDListNode *l2);
int length_IDList(IDListNode *l);
int is_subset_IDLists(IDListNode *l1, IDListNode *l2);
int is_setEqual_IDLists(IDListNode *l1, IDListNode *l2);

IDListNode *add_interactionsToSort(GenList *interact, IDListNode *need_sort);
IDListNode *collect_sortCols(ExprNode *od_expr, int add_from_start);
IDListNode *collect_sortColsNamedExpr(NamedExprNode *nexprs);
IDListNode *collect_sortCols0(ExprNode *node, int add_flag,
                              IDListNode **need_sort, GenList **potential,
                              int in_proj);

ExprNode *append_toExpr(ExprNode *list, ExprNode *add);

IDListNode *collect_AllCols(ExprNode *node);
IDListNode *collect_AllColsProj(LogicalQueryNode *node);
IDListNode *collect_AllColsGroupby(LogicalQueryNode *groupby);

// Optimization 1: sorting only necessary columns
OrderNode *get_NeededSort(OrderNode *have, OrderNode *want);
char *get_table_name(LogicalQueryNode *from);
LogicalQueryNode *make_specCols(IDListNode *cols);
LogicalQueryNode *make_sortCols(OrderNode *order, IDListNode *cols);
LogicalQueryNode *make_sortEachCols(OrderNode *order, IDListNode *cols);
void partExpr_OnOrder(ExprNode *expr, ExprNode **order_indep,
                      ExprNode **remaining);

LogicalQueryNode *optim_sort_where(LogicalQueryNode *proj,
                                   LogicalQueryNode *from,
                                   LogicalQueryNode *order,
                                   LogicalQueryNode *where,
                                   LogicalQueryNode *grouphaving);
LogicalQueryNode *optim_sort_group_od(LogicalQueryNode *proj,
                                      LogicalQueryNode *from,
                                      LogicalQueryNode *order,
                                      LogicalQueryNode *where,
                                      LogicalQueryNode *grouphaving);
LogicalQueryNode *optim_sort_group_oi(LogicalQueryNode *proj,
                                      LogicalQueryNode *from,
                                      LogicalQueryNode *order,
                                      LogicalQueryNode *where,
                                      LogicalQueryNode *grouphaving);
LogicalQueryNode *optim_sort_proj(LogicalQueryNode *proj,
                                  LogicalQueryNode *from,
                                  LogicalQueryNode *order,
                                  LogicalQueryNode *where,
                                  LogicalQueryNode *grouphaving);

LogicalQueryNode *assemble_opt1(LogicalQueryNode *proj, LogicalQueryNode *from,
                                LogicalQueryNode *order,
                                LogicalQueryNode *where,
                                LogicalQueryNode *grouphaving);
LogicalQueryNode *assemble_plan(LogicalQueryNode *proj, LogicalQueryNode *from,
                                LogicalQueryNode *order,
                                LogicalQueryNode *where,
                                LogicalQueryNode *grouphaving);

// Optimization 2: separating order-dependent from order-independent expressions
// in a list

// Optimizations relating to join-based from clauses
GenList *split_from(LogicalQueryNode *node);
void partExpr_OnAgg(ExprNode *expr, ExprNode **no_agg, ExprNode **agg);
char *get_table_src(ExprNode *expr);
IDListNode *collect_TablesExpr(ExprNode *exp);
IDListNode *collect_TablesFrom(LogicalQueryNode *ts);
int is_JoinClause(ExprNode *expr);
void groupExpr_OnJoin(ExprNode *expr, ExprNode **join_filters,
                      ExprNode **other_filters);
int Expr_has_subset_tables(ExprNode *expr, void *names);
void groupExpr_OnSubsetTables(ExprNode *expr, ExprNode **have, ExprNode **dont,
                              IDListNode *names);
int Expr_has_exact_tables(ExprNode *expr, void *names);
void groupExpr_OnEqualTables(ExprNode *expr, ExprNode **match, ExprNode **dont,
                             IDListNode *names);
int Expr_has_unknown_table(ExprNode *expr);
void groupExpr_OnUnknownTables(ExprNode *expr, ExprNode **have,
                               ExprNode **dont);
void check_warn_Join();
LogicalQueryNode *deposit_one_filter_deeply(LogicalQueryNode *node,
                                            ExprNode *filter);
LogicalQueryNode *deposit_filters_deeply(LogicalQueryNode *table,
                                         ExprNode *filters);
GenList *add_filters(GenList *ts, ExprNode **filters);
int join_is_possible(IDListNode *left, IDListNode *right,
                     IDListNode *join_filter);
int Expr_count_eq_filters(ExprNode *filters, IDListNode *table_names);
void join_heuristic(GenList *tables, ExprNode *join_filters,
                    ExprNode *reg_filters, LogicalQueryNode **left,
                    LogicalQueryNode **right);
GenList *choose_next_join(GenList *tables, ExprNode **join_filter_ptr,
                          ExprNode *reg_filters);
int is_simple_from(LogicalQueryNode *node);
void optim_from(LogicalQueryNode **from, LogicalQueryNode **where);
void print_table_dummy(GenList *t);

#endif