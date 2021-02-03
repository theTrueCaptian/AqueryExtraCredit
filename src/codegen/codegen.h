#ifndef AQUERY_CG_H
#define AQUERY_CG_H

#include "../types/aquery_types.h"
#include "../ast/ast.h"

void native_comma_enlist();
void native_kw_enlist();

/* q utilities for aquery */
void init_aq_helpers();
void add_to_dc(char *alias, char *tbl, int modify_cols);
void init_dc();
void add_to_dt(char *alias, char *tbl);
void sort_where_clause_by_ix(char *tbl);
char *gen_table_nm();
void cols_to_Aquery(char *new, char *t, char *p);
void acct_for_computed_groupBys();

/* C utilities for aquery code gen */
void turn_on_query_global_flags();
void turn_off_query_global_flags();

/* Expressions */
void cg_Constant(ExprNode *c);
void cg_LookupCol(char *nm);
void cg_ID(ExprNode *id);
void cg_rowId();
void cg_allCols(char *table_name);
int aquery_to_q_builtin_ix(char *fun);
void cg_callNode(ExprNode *call);
void cg_Oddix();
void cg_Evenix();
void cg_Everynix(int everyn);
void cg_indexNode(ExprNode *ixnode);
void cg_Neg(ExprNode *expr);
char *aquery_to_q_op(ExprNodeType op_type);
void cg_OpNode(ExprNode *expr);
void cg_ExprList(ExprNode *expr);
void cg_ExprNode(ExprNode *expr);

/* overloads */
int is_overloaded(char *name);
int ct_exprs(ExprNode *expr);

/* function definitions */
void cg_UDFDefNode(UDFDefNode *fundef);
void cg_DefArgList(IDListNode *args);
void cg_UDFBodyNode(UDFBodyNode *body);
void cg_LocalVar(NamedExprNode *vardef);

/* query related */
char *cg_SimpleTable(LogicalQueryNode *node);
char *cg_Alias(LogicalQueryNode *a);
char *cg_Table(char *orig_name, char *prefix, int modify_cols);
char *cg_FilterWhere(LogicalQueryNode *where);
void cg_FilterWhere0(ExprNode *selection, char *from);
void cg_FilterWhereExpressions(ExprNode *selection, char *from);
char *gc_ProjectSelect(LogicalQueryNode *proj);
void cg_NamedExprTuples(char *tblnm, NamedExprNode *nexpr, int id_ctr, int infer_name);
char *cg_groupBy(LogicalQueryNode *node);
void groupby_shadow_cols(NamedExprNode *groups);
NamedExprNode *groupExpr_to_NamedGroupExpr(ExprNode *exprs);
char *cg_flatten(LogicalQueryNode *node);
char *cg_LogicalQueryNode(LogicalQueryNode *node);
char *cg_FlattenedQuery(FlatQuery *flat);
char *cg_queryPlan(LogicalQueryNode *node);

/* local and full queries */
void cg_LocalQueryNode(LocalQueryNode *local);
void cg_colList(IDListNode *cols);
void cg_colRename(IDListNode *names);
void cg_LocalQueries(LocalQueryNode *locals);
void cg_FullQuery(FullQueryNode *full_query);
char *cg_FullQuery_Embedded(FullQueryNode *full_query);

/* sorting related */
char *cg_Sort(LogicalQueryNode *node); // naive
void cg_SimpleOrder(OrderNode *ordnode); // naive
void cg_SortIx(OrderNode *ordnode); // optimized
char *cg_SortCols(LogicalQueryNode *node); // optimized

/* inner join using */
void cg_RenameColsJoinUsing(char *t);
void cg_PrepareJoinUsing(char *t1, char *t2, IDListNode *using);
char *cg_IJUsing(LogicalQueryNode *ij);
void cg_IJUsing0(LogicalQueryNode *ij, char *joined, char *t1, char *t2);
char *cg_FOJUsing(LogicalQueryNode *ij);
void cg_FOJUsing0(LogicalQueryNode *ij, char *joined, char *t1, char *t2);

/* pushing filters under joins: poss push filter */
char *cg_PossPushFilter(LogicalQueryNode *poss_push);
int pushToLeftPossPush(LogicalQueryNode *poss_push);
typedef void (*cg_join_push)(LogicalQueryNode *join, char *joined, char *t1,
                             char *t2);
cg_join_push pickJoinTypePossPush(LogicalQueryNode *join);
void cg_AttributeCheckPossPush(IDListNode *cols, char *table);
IDListNode *colsCheckPossPush(LogicalQueryNode *join);

/* joins on */
void validate_no_prohibited_joinpred(ExprNode *joinpred);
char *find_first_prohibited_joinfun(ExprNode *expr);

/* insertion */
void cg_InsertStmt(InsertNode *insert);
void cg_InsertFromValues(char *tableInsertedInto, InsertNode *insert);
void cg_ExplicitValues(LogicalQueryNode *src);
void cg_InsertFromQuery(char *tableInsertedInto, InsertNode *insert);

/* create statements */
void cg_CreateStatement(CreateNode *create);
void cg_CreateTable(CreateNode *create);
void cg_CreateTableFromQuery(CreateNode *create);
void cg_CreateTableFromSchema(CreateNode *create);
void cg_CreateSchema(SchemaNode *schema);
char get_SchemaTypeCode(char *typename);

/* updates/deletes */
void cg_Update(FlatQuery *update);
void cg_Delete(FlatQuery *delete);
void cg_flatWhere(LogicalQueryNode *where, char *source);
void cg_flatGroupBy(LogicalQueryNode *groupby, char *source);
void cg_flatBooleanVector(FlatQuery *query, char *source);
void remove_is_grouped_attr_expr(ExprNode *node);
void remove_is_grouped_attr_namedExpr(NamedExprNode *node);

/* exec arrays */
void validate_exec_arrays(LogicalQueryNode *query);
char *cg_execArrays(LogicalQueryNode *exec);

/* load data statement */
void cg_LoadStatement(LoadNode *load);

/* top level code generation */
void cg_AQUERY2Q(TopLevelNode *node);
void cg_TopLevel(TopLevelNode *node);


#endif