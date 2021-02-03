%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "symtable.h"  /* manages the symbol table, to enable meta data lookup */	
#include "../ast/ast.h"       /* builds ast for parser */
#include "../ast/ast_print.h" /* provides dot printing of ast */
#include "../optimizer/optimizer.h" /* optimizing for query plans */
#include "../codegen/codegen.h" /* generates q code */
	
#define YYDEBUG 1
#define ERRORCOLOR  "\x1B[32m"
#define SAFE_ORDER_DEP(x) ((x) != NULL && (x)->order_dep);

extern int yyleng;
extern FILE *yyin;
extern int yylex();
extern int yyparse();


//for debugging purposes, defined in flex
extern int line_num; 
extern int col_num;

//For reporting query issues
int query_line_num;
int query_col_num;

//Symbol table
Symtable *env;	/* global symbol table: stack of hash tables */
Symentry *entry; /* place holder for entry pointers to perform modifications ad-hoc */

//AST
TopLevelNode* ast; /* holds any asts created during compilation */
LogicalQueryNode *curr_order;

//Code generation
FILE *DEST_FILE;
int GEN_CODE = 0;


void yyerror(const char *);

//Command line options
int optim_level = 0; //level of optimization in ast
int silence_warnings = 0;

%}

 /* 
  ********************
  *Token definitions *
  ********************
  */
  
 /* SQL: select query key words */
 
%token WITH UC_AS SELECT FROM ASSUMING ASC DESC WHERE GROUP BY HAVING LC_AS

 /* SQL: indexing */
%token ROWID ODD EVEN EVERY

 /* SQL: table operations */
%token FLATTEN CONCATENATE

 /* SQL: joins */
%token JOIN INNER OUTER FULL ON USING 
 
 /* SQL: views/tables */
%token CREATE TABLE VIEW

 /* SQL: update/insert/delete statements */
%token UPDATE SET INSERT INTO VALUES DELETE

/* SQL: extract arrays as named variables */
%token EXEC ARRAYS

/* SQL: show the table in tabular form */
%token SHOW

 /* SQL: load data from file */
%token LOAD DATA INFILE FIELDS TERMINATED

/* SQL: save data to file */
%token OUTFILE

 /* SQL: search conditions */
%token AND OR  
%token <str> IS NOT BETWEEN IN LIKE NULL_KEYWORD OVERLAPS

 /* SQL: case statement */
%token CASE END WHEN THEN ELSE

 /* SQL: type names */
%token <str> TYPE_INT TYPE_FLOAT TYPE_STRING TYPE_DATE TYPE_TIMESTAMP TYPE_BOOLEAN TYPE_HEX
 
 /* SQL: user defined functions */
%token FUNCTION LOCAL_ASSIGN
  
 /* built-in functions */
 /* non-moving variants */
%token <str> ABS AVG COUNT DISTINCT DROP FILL FIRST LAST MAX MIN MOD NEXT PREV PRD REV SUM SQRT STDDEV 
 /* moving variants */
%token <str> AVGS DELTAS MAXS MINS PRDS SUMS RATIOS
%token <str> MAKENULL
 
 /* literals and identifiers, and assocating storage in the yyval union */ 
%token <floatval> FLOAT
%token <intval> INT TRUE FALSE 
%token <str> DATE TIMESTAMP HEX ID STRING
 
 /* Math operators */
%token EXP_OP TIMES_OP DIV_OP PLUS_OP MINUS_OP LE_OP GE_OP L_OP G_OP EQ_OP NEQ_OP AND_OP OR_OP

 /* VERBATIM Q CODE MARKER */
%token <str> VERBATIM_Q_CODE
  
 
/* Additional Abstract Syntax Tree Types */

/* types for expressions */
%type <exprnode> constant truth_value table_constant 
%type <exprnode> case_expression case_clause when_clauses when_clause when_clauses_tail else_clause
%type <exprnode> call built_in_fun 
%type <exprnode> unary_neg
%type <exprnode> main_expression
%type <exprnode> indexing
%type <exprnode> exp_expression mult_expression add_expression rel_expression eq_expression 
%type <exprnode> and_expression or_expression value_expression
%type <exprnode> comma_value_expression_list comma_value_expression_list_tail


/* search condition */
%type <exprnode> search_condition boolean_factor boolean_primary predicate
%type <exprnode> postfix_predicate overlaps_predicate between_predicate in_predicate null_predicate like_predicate is_predicate
%type <exprnode> in_pred_spec range_value_expression
%type <exprnode> and_search_condition and_search_condition_tail

/* UDF related */
%type <namedexpr> function_local_var_def;
%type <udfbody> function_body function_body_tail function_body_elem 
%type <idlist> def_arg_list def_arg_list_tail	
%type <udfdef> user_function_definition

/* logical query plan */
%type <plan> table_expression_main table_expression joined_table table_expressions table_expressions_tail
%type <idlist> using_clause
%type <exprnode> on_clause
%type <idlist> comma_identifier_list comma_identifier_list_tail

%type <plan> from_clause select_clause where_clause order_clause
%type <plan> groupby_clause having_clause groupby_with_having groupby_ex_having
%type <plan> base_query query_or_exec base_query_ops
%type <order> order_spec order_specs order_specs_tail

%type <namedexpr> select_elem select_clause_tail groupby_elem groupby_tail
%type <exprnode> column_name


/* update, insert, delete */
%type <plan> update_statement
%type <namedexpr> set_clause set_clauses set_clauses_tail
%type <fullquery> insert_source
%type <idlist> insert_modifier
%type <insert> insert_statement
%type <plan> delete_statement
%type <load> load_statement
%type <dump> dump_statement


/* create statements */
%type <create> create_table_or_view
%type <createsrc> create_spec
%type <schema> schema schema_element schema_tail
%type <str> type_name


/* full queries */
%type <fullquery> full_query
%type <localquery> local_queries local_queries_tail local_query
%type <idlist> col_aliases

%type <fullquery> global_query
%type <top> top_level program


%start program

%union {
  int   intval;
  float floatval;
  char* str;
  struct ExprNode *exprnode;
  struct UDFDefNode *udfdef;
  struct IDListNode *idlist;
  struct LocalVarDefNode *localvardef;
  struct UDFBodyNode *udfbody;
  struct FullQueryNode *fullquery; 
  struct LocalQueryNode *localquery;
  struct LogicalQueryNode *plan;
  struct OrderNode *order;
  struct NamedExprNode *namedexpr;
  struct InsertNode *insert;
  struct CreateNode *create;
  struct CreateSourceNode *createsrc;
  struct SchemaNode *schema;
  struct LoadNode *load;
  struct DumpNode *dump;
  struct TopLevelNode *top;
}


%%

 /* 
 ****************
 *Aquery Grammar*
 ****************
 
 For additional information, 
 please refer to the pdf document Aquery to Q Compiler: Parser Grammar
 
 */
 
 /******* 2.1: Top level program definition *******/


program: top_level						    { $$ = $1; ast = $$; }												

top_level: global_query top_level           { $$ = make_Top_GlobalQuery($1, $2); }
	|	create_table_or_view top_level      { $$ = make_Top_Create($1, $2); }
	|	insert_statement top_level          { $$ = make_Top_Insert($1, $2); }
	|	update_statement top_level          { $$ = make_Top_UpdateDelete($1, $2); }
	|	delete_statement top_level          { $$ = make_Top_UpdateDelete($1, $2); }
	| load_statement top_level            { $$ = make_Top_Load($1, $2); }
	| dump_statement top_level            { $$ = make_Top_Dump($1, $2); }
	|	user_function_definition top_level  { $$ = make_Top_UDF($1, $2); }
	| VERBATIM_Q_CODE top_level           { $$ = make_Top_VerbatimQ($1, $2); }
	| /* epsilon */	                      { $$ = NULL; }
	;


 /*[% %]

 /******* 2.2: Local and global queries *******/
 
global_query: full_query                     { $$ = $1; }
	;

full_query:                     { env = push_env(env); } /* create a new scope to handle local query identifiers */
			local_queries 		
			query_or_exec 			{ env = pop_env(env); $$ = make_FullQueryNode($2, $3); } /* pop off to remove local queries and create AST node */
			;

query_or_exec: base_query_ops {$$ = $1; }
			 | EXEC ARRAYS base_query_ops {$$ = make_execArrays($3); }
			 ;


local_queries: WITH local_query local_queries_tail  { $2->next_sibling = $3; $$ = $2; } /* local queries are chained as siblings */
	|	/* epsilon */                               { $$ = NULL; }
	;
	
local_queries_tail: local_query local_queries_tail  { $1->next_sibling = $2; $$ = $1; }
	| /* epsilon */                                 { $$ = NULL; }
	;

local_query: ID                        
             col_aliases 				
			 UC_AS '(' base_query_ops ')'  { put_sym(env, $1, TABLE_TYPE, 0, 0); $$ = make_LocalQueryNode($1, $2, $5); add_order(env, $1, curr_order); } /* place local query info in sym table */
			 ; 

col_aliases: '(' comma_identifier_list ')' 			{ $$ = $2; }
	| /* epsilon */									{ $$ = NULL; }
	;

comma_identifier_list: ID comma_identifier_list_tail    { $$ = make_IDListNode($1, $2); } /* id lists are handled as linked lists in the ast, with $1 as head and $2 as tail */
	; 

comma_identifier_list_tail: ',' ID comma_identifier_list_tail	{ $$ = make_IDListNode($2, $3);}
	|	/* epsilon */											{ $$ = NULL; }
	;

 /******* 2.3: Base query *******/
 //TODO: make this better.....

base_query_ops: base_query { $$ = $1; }
  | SHOW base_query        { $$ = make_showOp($2); }
  ;

 /* assemble plan calls a different function depending on optimizer level, store order info for current query to use in symtable */
base_query: select_clause from_clause order_clause where_clause groupby_clause  { $$ = assemble_plan($1, $2, $3, $4, $5); curr_order = $3; if($5 != NULL) { annotate_groupedNamedExpr($1->params.namedexprs); } }
	; 
 
select_clause: SELECT select_elem select_clause_tail { $2->next_sibling = $3; $$ = make_project(PROJECT_SELECT, NULL, $2); } /* select_elems are linked, and put into projection */
	;

select_elem: value_expression LC_AS ID  { $$ = make_NamedExprNode($3, $1); }
	|	value_expression				{ $$ = make_NamedExprNode(NULL, $1); }
	;
	
select_clause_tail: ',' select_elem select_clause_tail		{ $2->next_sibling = $3; $$ = $2; }
	| /* epislon */											{ $$ = NULL; }
	;
		
from_clause: FROM {query_line_num = line_num; query_col_num = col_num; } table_expressions { $$ = $3; }
	;

order_clause: ASSUMING order_specs	{ $$ = make_sort(NULL, $2);}
	| /* epsilon */					{ $$ = NULL; }
	;
	
order_specs: order_spec order_specs_tail  { $1->next = $2; $$ = $1; }
		;
	
order_spec: ASC column_name							{ $$ = make_OrderNode(ASC_SORT, $2); }
	 | DESC column_name								{ $$ = make_OrderNode(DESC_SORT, $2); }
	 ;

column_name: ID 									{ $$ = make_id(env, $1); }
	| ID '.' ID  									{ $$ = make_colDotAccessNode(make_id(env, $1), make_id(env, $3)); }
	;	 
	 
order_specs_tail: ',' order_spec order_specs_tail 		{ $2->next = $3; $$ = $2; }
	| /* epsilon */										{ $$ = NULL; }
	;

where_clause: WHERE and_search_condition 		{ $$ = make_filterWhere(NULL, $2); }
	| /* epsilon */							    { $$ = NULL; }	;


groupby_clause: groupby_with_having { $$ = $1;   }
  | groupby_ex_having               { $$ = $1;   }
  | /*  epsilon */                  { $$ = NULL; }
  ;

// we break out groupbys into those that use having and those that don't
// as it allows us to easily include group-by-having into delete statements. group-by withou having would not be allowed
// as it doesn't make any semantic sense
groupby_with_having: GROUP BY groupby_elem groupby_tail having_clause     { $3->next_sibling = $4; $$ = pushdown_logical($5, make_groupby(NULL, $3)); }
  ;

groupby_ex_having: GROUP BY groupby_elem groupby_tail     { $3->next_sibling = $4; $$ = make_groupby(NULL, $3); }
  ;

groupby_tail: ',' groupby_elem groupby_tail                { $2->next_sibling = $3; $$ = $2; }
  | /* epsilon */                                          { $$ = NULL; }
  ;

// An option of anonymous group bys, that if included in selection results
// in re-computation of groups
groupby_elem: value_expression LC_AS ID  { $$ = make_NamedExprNode($3, $1); }
  |	value_expression				{ $$ = make_NamedExprNode(NULL, $1); }
  ;

having_clause: HAVING and_search_condition 						    { annotate_groupedExpr($2); $$ = make_filterHaving(NULL, $2); }
	;


 /******* 2.3.1: search condition *******/
//search_condition: boolean_term				{ $$ = $1; } 
	//| search_condition OR boolean_term		{ $$ = make_logicOpNode(WHERE_OR_EXPR, $1, $3); }
//	;
	
//boolean_term: boolean_factor				{ $$ = $1; }
	//| boolean_term AND boolean_factor		{ $$ = make_logicOpNode(WHERE_AND_EXPR, $1, $3); }
//	;


//and_search_condition: search_condition and_search_condition_tail { $1->next_sibling = $2; $$ = make_exprListNode($1); }
and_search_condition: search_condition and_search_condition_tail { $1->next_sibling = $2; $$ = $1; }
    ;
    
and_search_condition_tail : AND search_condition and_search_condition_tail { $2->next_sibling = $3; $$ = $2; } //don't make another exprListNode here..we want flat structure
    | /* epsilon */                                                        { $$ = NULL;                      } 
    ;
	
search_condition: boolean_factor				    { $$ = $1; } 
	| search_condition OR boolean_factor		    { $$ = make_logicOpNode(WHERE_OR_EXPR, $1, $3); }
	;    
    
boolean_factor: boolean_primary				{ $$ = $1; }
	| NOT boolean_primary					{ $$ = make_callNode(make_predNode($1), $2); }
	;
		
boolean_primary: predicate 					{ $$ = $1; }
		| '(' search_condition ')' 			{ $$ = $2; }
		;


predicate: value_expression 						{ $$ = $1; }
	| postfix_predicate								{ $$ = $1; }
	| overlaps_predicate							{ $$ = $1; }
	;


postfix_predicate:  between_predicate			{ $$ = $1; }
	|  in_predicate								{ $$ = $1; }
	|  null_predicate							{ $$ = $1; }
	|  like_predicate							{ $$ = $1; }
	|  is_predicate								{ $$ = $1; }
	;
	
	
 /* most postfix predicates are treated simply as a function call, with all arguments as a list of arguments so a between b and c becomes between(a,b,c)  effectively */
 /* in turn negation of a predicate simply is a call to not(x) where x is a call to another predicate and so forth */   
between_predicate: value_expression BETWEEN value_expression AND value_expression { $3->next_sibling = $5; $1->next_sibling = make_exprListNode($3); $$ = make_callNode(make_predNode($2), make_exprListNode($1)); }
	|  value_expression NOT BETWEEN value_expression AND value_expression		  { $4->next_sibling = $6; $1->next_sibling = make_exprListNode($4); $$ = make_callNode(make_predNode($2), make_exprListNode(make_callNode(make_predNode($3), make_exprListNode($1)))); }
	;
	
in_predicate: value_expression IN in_pred_spec  { $1->next_sibling = $3; $$ = make_callNode(make_predNode($2), make_exprListNode($1)); }
	| value_expression NOT IN in_pred_spec	    { $1->next_sibling = $4; $$ = make_callNode(make_predNode($2), make_exprListNode(make_callNode(make_predNode($3), make_exprListNode($1)))); }
	;
	
in_pred_spec: '(' comma_value_expression_list ')'  { $$ = $2; }
	| value_expression                             { $$ = $1; }
	;
	
like_predicate: value_expression LIKE value_expression  { $1->next_sibling = $3;  $$ = make_callNode(make_predNode($2), make_exprListNode($1)); }
	| value_expression NOT LIKE value_expression		{ $1->next_sibling = $4;  $$ = make_callNode(make_predNode($2), make_exprListNode(make_callNode(make_predNode($3), make_exprListNode($1)))); }
	;
	
null_predicate: value_expression IS NOT NULL_KEYWORD	{ $$ = make_callNode(make_predNode($3), make_exprListNode(make_callNode(make_predNode($4), make_exprListNode($1)))); }
	| value_expression IS NULL_KEYWORD					{ $$ = make_callNode(make_predNode($3), make_exprListNode($1)); }
	;
	
is_predicate: value_expression IS truth_value			{ $1->next_sibling = $3;  $$ = make_callNode(make_predNode($2), make_exprListNode($1)); }
	| value_expression IS NOT truth_value				{ $1->next_sibling = $4;  $$ = make_callNode(make_predNode($3),  make_exprListNode(make_callNode(make_predNode($2), make_exprListNode($1)))); }
	;

truth_value: TRUE 				{ $$ = make_bool(1); }
			| FALSE 			{ $$ = make_bool(0); }
			;	

overlaps_predicate: range_value_expression OVERLAPS range_value_expression 	{ $1->next_sibling = $3; $$ = make_callNode(make_predNode($2), make_exprListNode($1)); }
	;
	
range_value_expression: '(' value_expression ',' value_expression ')' 		{ $2->next_sibling = $4; $$ = make_exprListNode($2); }
	;
	


/******* 2.3.2: table expressions *******/

joined_table: table_expression 										{ $$ = $1;                                                }
	| table_expression INNER JOIN joined_table on_clause			{ $$ = make_joinOn(INNER_JOIN_ON, $1, $4, $5);            }	
	| table_expression INNER JOIN joined_table using_clause			{ $$ = make_joinUsing(INNER_JOIN_USING, $1, $4, $5);      } 
	| table_expression FULL OUTER JOIN joined_table on_clause		{ $$ = make_joinOn(FULL_OUTER_JOIN_ON, $1, $5, $6);       }
	| table_expression FULL OUTER JOIN joined_table using_clause	{ $$ = make_joinUsing(FULL_OUTER_JOIN_USING, $1, $5, $6); } 
	;


on_clause: ON and_search_condition								{ $$ = $2; } 
	;

using_clause: USING '(' comma_identifier_list ')' 				{ $$ = $3; }
    | USING ID                                                  { $$ = make_IDListNode($2, NULL); }
	;
	
table_expression: table_expression_main							{ $$ = $1;               } 
	| FLATTEN '(' table_expression_main ')'						{ $$ = make_flatten($3); }
	| CONCATENATE '('  comma_identifier_list  ')'     { $$ = make_concatenate($3); }
	;
		
table_expression_main: ID ID 									{ $$ = make_alias(make_table($1), $2); }
	| ID UC_AS ID 												{ $$ = make_alias(make_table($1), $3); }
	| ID														{ $$ = make_table($1);                 }
	| '(' joined_table ')' 										{ $$ = $2;                             }
	;
	
//built_in_table_fun: FLATTEN ; //for now we are just gonna merge this with table expressions since we don't have other table functions....

table_expressions: joined_table table_expressions_tail 				{ if($2 == NULL){ $$ = $1; } else { $$ = make_cross($1, $2); } }
	;

table_expressions_tail: ',' joined_table table_expressions_tail	 	 { if($3 == NULL){ $$ = $2; } else { $$ = make_cross($2, $3); } }
	| /* epsilon */ 											     { $$ = NULL;                                                   }   
	;

/******* 2.5: table and view creation *******/	 
create_table_or_view: CREATE TABLE ID create_spec             { put_sym(env, $3, TABLE_TYPE, 0, 0); $$ = make_createNode(CREATE_TABLE, $3, $4); }
	| CREATE VIEW ID create_spec                              { put_sym(env, $3, VIEW_TYPE, 0, 0);  $$ = make_createNode(CREATE_VIEW, $3, $4);  }
	;

create_spec: UC_AS full_query		{ $$ = make_querySource($2);  }
	|	'(' schema ')'				{ $$ = make_schemaSource($2); }
 	;
	
schema: schema_element schema_tail { $1->next_sibling = $2; $$ = $1; }
	;

schema_element: ID type_name 	 { $$ = make_schemaNode($1, $2); }
	; 

schema_tail: ',' schema_element schema_tail      { $2->next_sibling = $3; $$ = $2; }
	| /* epsilon */								 { $$ = NULL;                      }
	;
	
type_name: TYPE_INT 	{$$ = $1; }
	| TYPE_FLOAT 		{$$ = $1; }
	| TYPE_STRING 		{$$ = $1; }
	| TYPE_DATE 		{$$ = $1; }
	| TYPE_TIMESTAMP 		{$$ = $1; }
	| TYPE_BOOLEAN 		{$$ = $1; }
	| TYPE_HEX 			{$$ = $1; }
	;	


/******* 2.6: update, insert, delete statements *******/
update_statement: UPDATE ID SET set_clauses order_clause where_clause groupby_clause {
																									if($7 != NULL){ annotate_groupedNamedExpr($4); }
                                                  $$  = assemble_flat(make_project(PROJECT_UPDATE, NULL, $4), make_table($2), $5, $6, $7);
                                                  }
	;

set_clauses: set_clause set_clauses_tail { $1->next_sibling = $2; $$ = $1; }
	;

set_clauses_tail: ',' set_clause set_clauses_tail	{ $2->next_sibling = $3; $$ = $2; }
	| /* epsilon */									{ $$ = NULL; }
	;

set_clause: ID EQ_OP value_expression 			{ $$ = make_NamedExprNode($1, $3); }
	;

insert_statement: INSERT INTO ID order_clause insert_modifier insert_source { $$ = make_insert(assemble_base(NULL, make_table($3), $4, NULL, NULL), $5, $6); }
	;

insert_modifier: '(' comma_identifier_list ')'					{  $$ = $2; }
	| /* epsilon */												{  $$ = NULL; }
	;
	
insert_source: full_query								{ $$ = $1; }
	| VALUES '(' comma_value_expression_list ')'		{ $$ = make_FullQueryNode(NULL, make_values($3)); }
	;
	
delete_statement: DELETE FROM ID order_clause where_clause					{ $$ = assemble_flat(make_delete(NULL, NULL), make_table($3), $4, $5, NULL); }
  | DELETE FROM ID order_clause where_clause groupby_with_having    { $$ = assemble_flat(make_delete(NULL, NULL), make_table($3), $4, $5, $6);   }
	| DELETE comma_identifier_list FROM ID 									          { $$ = assemble_flat(make_delete(NULL, $2), make_table($4), NULL, NULL, NULL);   }
	;

/****** Load statements ****////
load_statement: LOAD DATA INFILE STRING INTO TABLE ID FIELDS TERMINATED BY STRING { $$ = make_loadNode($4, $11, $7); }

/****** Dump statements ****////
dump_statement: full_query INTO OUTFILE STRING FIELDS TERMINATED BY STRING { $$ = make_dumpNode($1, $8, $4); }


/******* 2.7: user defined functions *******/
//TODO: we need to infer the properties of the function based on the function_body
user_function_definition: FUNCTION ID         { put_sym(env, $2, FUNCTION_TYPE, 0, 0); env = push_env(env);  } /* place function in symtable, assuming order-independent  and create new scope */
                        '(' 
						    def_arg_list 	  
					     ')' 
						'{' 
						    function_body 	 
						'}'                   { $$ =  make_UDFDefNode($2, $5, $8); 
                                                entry = lookup_sym(env, $2); 
                                                entry->order_dep = $$->order_dep; 
                                                entry->uses_agg = $$->uses_agg; 
                                                entry->is_odx = $$->is_odx;
                                                env = pop_env(env);
                                                } /* update order dependence infor for function */ 
						; 

def_arg_list: ID  def_arg_list_tail		{ put_sym(env, $1, UNKNOWN_TYPE, 0, 0); $$ = make_IDListNode($1, $2); }			 
	| /* epsilon */						{ $$ = NULL; }	
	;
	
def_arg_list_tail: ',' ID def_arg_list_tail { put_sym(env, $2, UNKNOWN_TYPE, 0, 0); $$ = make_IDListNode($2, $3); }
	| /* epsilon */							 {$$ = NULL; } 

	;

function_body: function_body_elem function_body_tail	{ $1->next_sibling = $2; $$ = $1;}
	| /* epsilon */										{ $$ = NULL; }
	;
	
function_body_tail: ';' function_body_elem function_body_tail { $2->next_sibling = $3; $$ = $2; }
	| /* epsilon */	                                          { $$= NULL; }
	;
	
function_body_elem: value_expression		{$$ = make_UDFExpr($1);   }
	| function_local_var_def				{$$ = make_UDFVardef($1); } 
	| full_query							{$$ = make_UDFQuery($1);  }
	;
	
function_local_var_def: ID LOCAL_ASSIGN value_expression   { put_sym(env, $1, UNKNOWN_TYPE, 0, 0);
                                                            entry = lookup_sym(env, $1); 
                                                            entry->is_odx = $3->is_odx;
                                                            $$ = make_NamedExprNode($1, $3);  
                                                             } ;


/******* 2.8: value expressions *******/
constant: INT 				{ $$ = make_int($1);    }
		| FLOAT 			{ $$ = make_float($1);  }
		| DATE 				{ $$ = make_date($1);   }
		| TIMESTAMP   { $$ = make_timestamp($1);   }
		| STRING 			{ $$ = make_string($1); }
		| HEX 				{ $$ = make_hex($1);    }
		| truth_value 		{ $$ = $1;              }
		;

table_constant:  ROWID 					{ $$ = make_rowId();                                              }
				| ID '.' ID 			{ $$ = make_colDotAccessNode(make_id(env, $1), make_id(env, $3)); }
				| TIMES_OP 				{ $$ = make_allColsNode();                                        }
				;


case_expression: CASE case_clause when_clauses else_clause END 					{ $$ = make_caseNode($2, $3, $4); }   
				  ;

case_clause: value_expression	 { $$ = make_caseClauseNode($1);   }
	| /* epsilon */				 { $$ = make_caseClauseNode(NULL); }
	;
	
when_clauses: when_clause when_clauses_tail { $$ = make_whenClausesNode($1, $2); }
	;

when_clauses_tail: when_clause when_clauses_tail { $1->next_sibling = $2; $$ = $1; }
	| /* epsilon */	                             { $$ = NULL; }
	;
	
when_clause: WHEN search_condition THEN value_expression  { $$ = make_caseWhenNode($2, $4); }
	;

else_clause: ELSE value_expression  { $$ = make_elseClauseNode($2); }
	;
	
main_expression: constant 					{ $$ = $1; }
			| table_constant 				{ $$ = $1; } 
			| ID 							{ $$ = make_id(env, $1); }
			| '(' value_expression ')'		{ $$ = $2; }
			| case_expression 				{ $$ = $1; }
			; 

call: main_expression						              { $$ = $1;                                         }
	| main_expression '[' indexing ']' 		              { $$ = make_indexNode($1, $3);                     }
	| built_in_fun '(' comma_value_expression_list ')'    { $$ = make_callNode($1, $3);                      } 
	| built_in_fun '(' ')' 								  { $$ = make_callNode($1, NULL);                    }
	| ID '(' comma_value_expression_list ')'              { $$ = make_callNode(make_udfNode(env, $1), $3);   }
	| ID '(' ')' 							              { $$ = make_callNode(make_udfNode(env, $1), NULL); }
	;


indexing: ODD 				{ $$ = make_oddix();      }
		| EVEN 				{ $$ = make_evenix();     }
		| EVERY INT 		{ $$ = make_everynix($2); }
		;

built_in_fun: ABS 				{ $$ = make_builtInFunNode(env, $1); }
	| AVG 						{ $$ = make_builtInFunNode(env, $1); }
	| AVGS 						{ $$ = make_builtInFunNode(env, $1); }
	| COUNT 				    { $$ = make_builtInFunNode(env, $1); }
	| DELTAS 			        { $$ = make_builtInFunNode(env, $1); }
	| DISTINCT 			        { $$ = make_builtInFunNode(env, $1); }
	| DROP 	                    { $$ = make_builtInFunNode(env, $1); }
	| FILL 	                    { $$ = make_builtInFunNode(env, $1); }
	| FIRST 	                { $$ = make_builtInFunNode(env, $1); }
	| LAST 	                    { $$ = make_builtInFunNode(env, $1); }
	| MAX 	                    { $$ = make_builtInFunNode(env, $1); }
	| MAXS 	                    { $$ = make_builtInFunNode(env, $1); }
	| MIN	                    { $$ = make_builtInFunNode(env, $1); }
	| MINS 	                    { $$ = make_builtInFunNode(env, $1); }
	| MOD 	                    { $$ = make_builtInFunNode(env, $1); }
	| NEXT 		                { $$ = make_builtInFunNode(env, $1); }
	| PREV 	                    { $$ = make_builtInFunNode(env, $1); }
	| PRD 	                    { $$ = make_builtInFunNode(env, $1); }
	| PRDS 	                    { $$ = make_builtInFunNode(env, $1); }
	| REV 	                    { $$ = make_builtInFunNode(env, $1); }
	| SUM 	                    { $$ = make_builtInFunNode(env, $1); }
	| SUMS 		                { $$ = make_builtInFunNode(env, $1); }
  | SQRT 		                { $$ = make_builtInFunNode(env, $1); }
	| STDDEV 	                { $$ = make_builtInFunNode(env, $1); }
	| MAKENULL	                { $$ = make_builtInFunNode(env, $1); }
	| RATIOS                    { $$ = make_builtInFunNode(env, $1); }
	;

unary_neg: call { $$ = $1; }
    |     MINUS_OP call { $$ = make_neg($2); }
    ;
	
exp_expression: unary_neg 				{ $$ = $1;                               }
	| unary_neg EXP_OP exp_expression	{ $$ = make_arithNode(POW_EXPR, $1, $3); }
	;

mult_expression: exp_expression					    { $$ = $1;                                }
	| mult_expression TIMES_OP exp_expression		{ $$ = make_arithNode(MULT_EXPR, $1, $3); }
	| mult_expression DIV_OP exp_expression			{ $$ = make_arithNode(DIV_EXPR, $1, $3);  }
	;
	

add_expression: mult_expression						{ $$ = $1;                                 }   
 	| add_expression PLUS_OP mult_expression		{ $$ = make_arithNode(PLUS_EXPR, $1, $3);  }
	| add_expression MINUS_OP mult_expression		{ $$ = make_arithNode(MINUS_EXPR, $1, $3); }
	;
	
rel_expression: add_expression						{ $$ = $1;                             }
	| rel_expression L_OP add_expression			{ $$ = make_compNode(LT_EXPR, $1, $3); }
	| rel_expression G_OP add_expression			{ $$ = make_compNode(GT_EXPR, $1, $3); }
	| rel_expression LE_OP add_expression			{ $$ = make_compNode(LE_EXPR, $1, $3); }
	| rel_expression GE_OP add_expression			{ $$ = make_compNode(GE_EXPR, $1, $3); }
	;

eq_expression: rel_expression						{ $$ = $1;                              } 
	| eq_expression EQ_OP rel_expression			{ $$ = make_compNode(EQ_EXPR, $1, $3);  }
	| eq_expression NEQ_OP rel_expression			{ $$ = make_compNode(NEQ_EXPR, $1, $3); }
	;

and_expression: eq_expression						{ $$ = $1;                                  }
	| and_expression AND_OP eq_expression			{ $$ = make_logicOpNode(LAND_EXPR, $1, $3); }
	;

or_expression: and_expression						{ $$ = $1;                                 }
	| or_expression OR_OP and_expression			{ $$ = make_logicOpNode(LOR_EXPR, $1, $3); }
	;
	
value_expression: or_expression 					{ $$ = $1; } 
	;

 
comma_value_expression_list: value_expression comma_value_expression_list_tail 	        { $1->next_sibling = $2; $$ = make_exprListNode($1); }
		;

comma_value_expression_list_tail: ',' value_expression comma_value_expression_list_tail { $2->next_sibling = $3; $$ = $2; }
	| /* epsilon */																		{ $$ = NULL; }
	;
	



%%

void yyerror(const char *s) 
{
	//TODO: rewrite with more robust fgets wrapper
	//rewind file and find error
	rewind(yyin);
	int lines = line_num, cols = col_num;
	int max_size = 500;
	
	char *line = malloc(sizeof(char) * (max_size - 1));
	char *marker = malloc(sizeof(char) * (col_num + 2));
	memset(marker, ' ', col_num + 1);
	marker[col_num - yyleng] = '^';
	marker[col_num + 1] = '\0';
	
	while(lines-- > 0) 
	{ //find line with error
		fgets(line, max_size, yyin);
	}
	
	printf("%s at line %d, column %d\n%s" ERRORCOLOR "%s\n", s, line_num, col_num - yyleng, line, marker);
	fclose(yyin);
	free(line); free(marker);
	exit(1);
}

void help()
{
	printf("Usage: ./a2q [-p|-c][-o output_q_file][-a <#>] aquery_file\n");
	printf("-p  print dot file AST to stdout\n");
	printf("-a  optimization level [0-1]\n");
    printf("-s  silence warnings\n");
    printf("-c  generate code\n");
    printf("-o  code output file\n");
    printf("-c/-p are mutually exclusive\n");
}



int main(int argc, char *argv[]) {
	yydebug = 0;
	
	/* Aquery compiler flags */
	int print_ast_flag = 0;
	optim_level = 0;
	int max_optim_level = 1;
	
	/* getopt values */
	int op;
	
    /* where to save the code generated */
    DEST_FILE = stdout;
    
	
	while((op = getopt(argc, argv, "cspha:o:")) != -1)
	{
		switch(op)
		{
            case 's':
                silence_warnings = 1;
                break;
			case 'p':
				print_ast_flag = 1;
				break;
            case 'c':
                GEN_CODE = 1;
                break;
			case 'h':
				help();
				exit(0);
				break;
			case 'a':
				if(!isdigit(optarg[0]))
				{
					printf("-%c requires numeric arg\n", optopt);
					help();
					exit(1);
				}
				
				optim_level = atoi(optarg);
				
				if(optim_level > max_optim_level)
				{
					printf("Defaulting to highest optimizing level:%d\n", max_optim_level);
					optim_level = max_optim_level;
				}
				break;
            case 'o':
                DEST_FILE = fopen(optarg, "w");
                if(DEST_FILE == NULL)
                {
            		printf("Unable to open %s for writing\n", optarg);
            		exit(1);
                }
                break;
			case '?':
				if(optopt == 'a')
				{
					printf("Option -%c requires an option\n", optopt);
					exit(1);
				}
				break;
			default:
 				exit(1);
		}
	}
	
    //can not generate code and print out ast...creates mess in stdout and we manipulate tree in code generation...
    if(print_ast_flag && GEN_CODE)
    {
        help();
        exit(1);
    }
    
	FILE *to_parse;
    
	if(1 > argc - optind) 
	{ //Did we get a file to analyze?
		//help();
		//exit(1);
        to_parse = stdin;
	}
    else
    {
        to_parse = fopen(argv[optind], "r");
    }
	
	if(to_parse == NULL)
	{
		printf("Unable to open %s for reading\n", argv[optind]);
		exit(1);
	}
	else
	{
		yyin = to_parse;
	}
	
	env = init_symtable(); //create the global environment
	
	do 
	{
		yyparse();
	} 
	while(!feof(yyin));
	
	if(print_ast_flag)
	{
		print_ast(ast);
	}
    else if(GEN_CODE)
    {
        cg_AQUERY2Q(ast); 
        fclose(DEST_FILE);
    }
    else
    {
        printf("Input file fits Aquery grammar, call with -c[code] or -p[ast viz]\n");
    }		
		
    return 0;
}

