%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aquery.tab.h"

int line_num = 1;
int col_num = 0;

void count(); 
void consume_multi_line_comment();
void consume_single_line_comment();
void consume_verbatim_q();



%}

%option noyywrap

%%
 /* comments */
 
"/*"			 { count(); consume_multi_line_comment();  }
"//"			 { count(); consume_single_line_comment(); }
"<q>"            { count(); consume_verbatim_q(); return VERBATIM_Q_CODE; }

 /* SQL: select query key words */
("WITH")|("with")           { count(); return WITH;      }
("ASC")|("asc")             { count(); return ASC;       }
("DESC")|("desc")			{ count(); return DESC;      }
"AS"			            { count(); return UC_AS;     } 
"as"			            { count(); return LC_AS;     }
("SELECT")|("select")		{ count(); return SELECT;    }
("FROM")|("from")	    	{ count(); return FROM;      }
("ASSUMING")|("assuming")   { count(); return ASSUMING;  }
("WHERE")|("where")			{ count(); return WHERE;     }
("GROUP")|("group")			{ count(); return GROUP;     }
("BY")|("by")   			{ count(); return BY;        }
("HAVING")|("having")		{ count(); return HAVING;    }


 /* SQL: indexing */
"ROWID"			{ count(); return ROWID;     }
"ODD"			{ count(); return ODD;       }
"EVEN"			{ count(); return EVEN;      }
"EVERY"			{ count(); return EVERY;     }

 /* SQL: table operations */
"FLATTEN"		{ count(); yylval.str = strdup(yytext); return FLATTEN;	 }
"CONCATENATE" { count(); yylval.str = strdup(yytext); return CONCATENATE;	 }

 /* SQL: joins */
"JOIN"			{ count(); return JOIN;     }
"INNER"			{ count(); return INNER;    }	
"OUTER"			{ count(); return OUTER;    }
"FULL"			{ count(); return FULL;     }
"ON"			{ count(); return ON;       }
"USING"			{ count(); return USING;    }

 /* SQL: Views/Tables */
"CREATE"	{ count(); return CREATE;   }
"TABLE"		{ count(); return TABLE;    }
"VIEW"		{ count(); return VIEW;     }

 /* SQL: update/insert/delete statements */
("UPDATE")|("update")	{ count(); return UPDATE;   }
("SET")|("set")		    { count(); return SET;      }
"INSERT"	{ count(); return INSERT;   }
"INTO"		{ count(); return INTO;     }
"VALUES"	{ count(); return VALUES;   }
"DELETE"	{ count(); return DELETE;   }
("EXEC")|("exec") { count(); return EXEC;   }
("ARRAYS")|("arrays") { count(); return ARRAYS;   }
("SHOW")|("show")   { count(); return SHOW;   }

 /* SQL: load data */
("LOAD")|("load") { count(); return LOAD;   }
("DATA")|("data") { count(); return DATA;   }
("FIELDS")|("fields") { count(); return FIELDS;   }
("TERMINATED")|("terminated") { count(); return TERMINATED;   }
("INFILE")|("infile") { count(); return INFILE;   }
("OUTFILE")|("outfile") { count(); return OUTFILE;   }

 /* SQL: search conditions */
("AND")|("and")		      { count(); return AND;          }
("OR")|("or")		      { count(); return OR;           }
("IS")|("is")		      { count(); yylval.str = strdup("is"); return IS;             }
("NOT")|("not")		      { count(); yylval.str = strdup("not"); return NOT;           }
("BETWEEN")|("between")	  { count(); yylval.str = strdup("between"); return BETWEEN;   }
("IN")|("in")		      { count(); yylval.str = strdup("in"); return IN;             }
("LIKE")|("like")		  { count(); yylval.str = strdup("like"); return LIKE;         }
("NULL")|("null")		  { count(); yylval.str = strdup("null"); return NULL_KEYWORD; }
("OVERLAPS")|("overlaps") { count(); yylval.str = strdup("overlaps"); return OVERLAPS; }

 /* SQL: case statements */
("CASE")|("case")	{ count(); return CASE; }
("END")|("end")	    { count(); return END;  }
("WHEN")|("when")	{ count(); return WHEN; }
("THEN")|("then")	{ count(); return THEN; }
("ELSE")|("else")	{ count(); return ELSE; }
 
 /* SQL: type names */
"INT"		{ count(); yylval.str = strdup(yytext); return TYPE_INT;     }
"FLOAT"		{ count(); yylval.str = strdup(yytext); return TYPE_FLOAT;   }
"STRING"	{ count(); yylval.str = strdup(yytext); return TYPE_STRING;  }
"DATE"		{ count(); yylval.str = strdup(yytext); return TYPE_DATE;    }
"TIMESTAMP" { count(); yylval.str = strdup(yytext); return TYPE_TIMESTAMP;    }
"BOOLEAN"	{ count(); yylval.str = strdup(yytext); return TYPE_BOOLEAN; }
"HEX"		{ count(); yylval.str = strdup(yytext); return TYPE_HEX;     }

 /* SQL: user defined functions */
("FUNCTION"|"function")	{ count(); return FUNCTION;     }
":="		            { count(); return LOCAL_ASSIGN; } 
 
 /* Built-in functions from q and k */
"abs"			{ count(); yylval.str = strdup(yytext); return ABS;      }
"avg"			{ count(); yylval.str = strdup(yytext); return AVG;      }
"avgs"			{ count(); yylval.str = strdup(yytext); return AVGS;     }
"count"			{ count(); yylval.str = strdup(yytext); return COUNT;    }
"deltas"		{ count(); yylval.str = strdup(yytext); return DELTAS;   }
"distinct"		{ count(); yylval.str = strdup(yytext); return DISTINCT; }
"drop"			{ count(); yylval.str = strdup(yytext); return DROP;     }
"fill"			{ count(); yylval.str = strdup(yytext); return FILL;     }
"first"			{ count(); yylval.str = strdup(yytext); return FIRST;    }
"last"			{ count(); yylval.str = strdup(yytext); return LAST;     }
"max"			{ count(); yylval.str = strdup(yytext); return MAX;      }
"maxs"			{ count(); yylval.str = strdup(yytext); return MAXS;     }
"min"			{ count(); yylval.str = strdup(yytext); return MIN;      }
"mins"			{ count(); yylval.str = strdup(yytext); return MINS;     }
"mod"			{ count(); yylval.str = strdup(yytext); return MOD;      }
"next"			{ count(); yylval.str = strdup(yytext); return NEXT;     }
"prev"			{ count(); yylval.str = strdup(yytext); return PREV;     }
"prd"			{ count(); yylval.str = strdup(yytext); return PRD;      }
"prds"			{ count(); yylval.str = strdup(yytext); return PRDS;     }
"reverse"		{ count(); yylval.str = strdup(yytext); return REV;      }
"sum"			{ count(); yylval.str = strdup(yytext); return SUM;      }
"sums"			{ count(); yylval.str = strdup(yytext); return SUMS;     }
"sqrt"			{ count(); yylval.str = strdup(yytext); return SQRT;     }
"stddev"		{ count(); yylval.str = strdup(yytext); return STDDEV;   }
"make_null"		{ count(); yylval.str = strdup(yytext); return MAKENULL; }
"ratios"      { count(); yylval.str = strdup(yytext); return RATIOS; }
		


 /* Space definitions */
[ \t\n]    						{ count(); 	} 


 /* Literals */
[0-9]+                        { count(); yylval.intval = atoi(yytext); return INT;       }	
[0-9]+\.[0-9]+                { count(); yylval.floatval = atof(yytext); return FLOAT;   } 
\"(\\.|[^"])*\"                 { count(); yylval.str = strdup(yytext); return STRING;     } 
\'(([0][1-9])|([1][0-2]))\/(([0][1-9])|([1-2][0-9])|([3][0-1]))\/[0-9]{4}\'   { count(); yylval.str = strdup(yytext); return DATE; }
\'(([0][1-9])|([1][0-2]))\/(([0][1-9])|([1-2][0-9])|([3][0-1]))\/[0-9]{4}"D"(([0][0-9])|([1][0-9])|([2][0-3]))":"([0-5][0-9])":"([0-5][0-9])("."[0-9]+)?\'  { count(); yylval.str = strdup(yytext); return TIMESTAMP; }
("TRUE")|("true")               { count(); yylval.intval = 1; return TRUE;	   }
("FALSE")|("false")              { count(); yylval.intval = 0; return FALSE;   }
"0x"[0-9a-fA-F]+                { count(); yylval.str = strdup(yytext); return HEX;	   }

 /* Variables */
[a-zA-Z]+[a-zA-Z0-9_]*	{ count(); yylval.str = strdup(yytext); return ID; }	 


 /* Standard mathematical operators */
"^"         { count(); return EXP_OP;   }
"*"         { count(); return TIMES_OP; }
"/"         { count(); return DIV_OP;   }
"+"         { count(); return PLUS_OP;  }	
"-"         { count(); return MINUS_OP; }
"<="        { count(); return LE_OP;    }
">="        { count(); return GE_OP;    }
"<"         { count(); return L_OP;     }
">"         { count(); return G_OP;     }	
"="         { count(); return EQ_OP;    }
"!="        { count(); return NEQ_OP;   }
"&"         { count(); return AND_OP;   }
"|"         { count(); return OR_OP;   }

 /* miscellaneous */
 "("		{ count(); return '('; }
 ")"		{ count(); return ')'; }
 "."		{ count(); return '.'; }
 ","		{ count(); return ','; }
 ";"		{ count(); return ';'; }
 "{"		{ count(); return '{'; } 
 "}"		{ count(); return '}'; }
 "["		{ count(); return '['; }
 "]"		{ count(); return ']'; }

 /* TODO: add better error handling/reporting */
.		     { printf("Unidentified token:%s, line:%d, column:%d\n", yytext, line_num, col_num); exit(1); }

%%
void move_col_line(char c)
{
	if(c == '\n')
	{
		col_num = 0;
		line_num++;
	}
	else if(c == '\t')
	{
		col_num += 4;
	}
	else
	{
		col_num++;
	}
}


void count()
{ /* inspired from  http://www.lysator.liu.se/c/ANSI-C-grammar-l.html */
	int i = 0;
	char c;
	while((c = yytext[i++]) != '\0')
	{
		move_col_line(c);
	}
}

void consume_single_line_comment() {
	char c;
	
	while((c = input()) != '\n')
	{
		//consume comment
	}
	col_num = 0;
	line_num++;
}

void consume_multi_line_comment() {
	char c = input(), c1;
	int comment_end = 0;
	
	do
	{
		/* consume comments */
		if(c == '\0')
		{
			comment_end = 1;
		}
		else
		{	
			c1 = input();
			move_col_line(c);
			comment_end = (c == '*') && (c1 == '/');
			c = c1;
			
			
		}
	}
	while(!comment_end);
	
	
}

void consume_verbatim_q() {
    char c1 = input(), c2, c3, c4;
    int q_code_end = 0;
    int limit = 200; //roughly 2 lines of text...should be good enough for most purposes
    int charct = 0;  //track how many chars we used up
    int ix = 0;  // track index for next char
    char *qcode = malloc(sizeof(char) * (limit + 1));  //pointer to hold our q code
    char *swp; //pointer to expand string if beyond the limit

    	do
    	{
    		/* consume q code */
    		if(c1 == '\0')
    		{
    		    printf("missing matching </q>\n");
    		    exit(1);
    		}
    		else
    		{
    			  move_col_line(c1);
    			  if (c1 == '<')
    			  {
                c2 = input();
                c3 = input();
                c4 = input();
                q_code_end = (c2 == '/') && (c3 == 'q') && (c4 == '>');

                if (!q_code_end)
                {
                  unput(c2);
                  unput(c3);
                  unput(c4);
                }
    			  }

    			if (!q_code_end)
    			{
                    // add character to q code stream
                    if(charct < limit)
                    { //we still have space in current pointer
                        qcode[ix++] = c1;
                        charct++;
                    }
                    else
                    { // we ran out of space, create a pointer long enough for current contents
                      // plus another iteration fo limit
                        swp = malloc(sizeof(char) * (ix + limit + 2)); //(ix + 1) counts elems
                                                                       // limit + 1 for null char
                        qcode[ix] = '\0'; //null terminate before copying
                        swp = strcpy(swp, qcode); //copy over the current code
                        free(qcode); //free up the previous pointer and swap pointers
                        qcode = swp;
                        qcode[ix++] = c1; //add new char
                        charct = 1; //restart character count
                    }

                    c1 = input();
    			}
          else
          {
              move_col_line(c2);
              move_col_line(c3);
              move_col_line(c4);
          }
    		}
    	}
    	while(!q_code_end);

    	qcode[ix] = '\0'; // add null char
    	yylval.str = qcode; // set bison string pointer to q code
}





