#ifndef AQUERY_TYPES_H
#define AQUERY_TYPES_H
// Datatypes used for symbol table, and AST storage
// UNKNOWN_TYPE represents dynamic type info unavailable to us at compile time
typedef enum DataTypes {
  INT_TYPE,
  FLOAT_TYPE,
  DATE_TYPE,
  BOOLEAN_TYPE,
  STRING_TYPE,
  HEX_TYPE,
  TABLE_TYPE,
  VIEW_TYPE,
  FUNCTION_TYPE,
  CALL_TYPE,
  UNKNOWN_TYPE, // unknown dynamic types (column names, external tables,
                // function calls etc)
  ERROR_TYPE, // we create a type error that we can use to point out issues
              // later on
  TIMESTAMP_TYPE
} DataType;

// Type checking operations
int is_int(DataType x);
int is_float(DataType x);
int is_bool(DataType x);
int is_numeric(DataType x);
int is_error(DataType x);
int is_fun(DataType x);
int is_timestamp(DataType x);
DataType unif_numeric(DataType x, DataType y);
DataType unif_override(DataType poss, DataType override, DataType x,
                       DataType y);
DataType unif_comp(DataType x, DataType y);
DataType unif_logic(DataType x, DataType y);

#endif
