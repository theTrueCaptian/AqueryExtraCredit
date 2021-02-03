/* performs certain type checking for the ast */
#include <stdio.h>
#include <stdlib.h>
#include "aquery_types.h"

int is_int(DataType x) { return x == INT_TYPE; }

int is_float(DataType x) { return x == FLOAT_TYPE; }

int is_bool(DataType x) { return x == BOOLEAN_TYPE; }

int is_numeric(DataType x) { return is_int(x) || is_float(x) || is_bool(x); }

int is_error(DataType x) { return x == ERROR_TYPE; }

int is_fun(DataType x) { return x == FUNCTION_TYPE; }

int is_timestamp(DataType x) { return x == TIMESTAMP_TYPE; }

// unification for situations where we have a guarantee on the resulting type,
// assuming
// operands result in correct type resolution versus wrong type
DataType unif_override(DataType poss, DataType override, DataType x,
                       DataType y) {
  // we know results have to be x type, only label as error if neither operand
  // had errors
  // and we unified with error rather than unknown
  return (is_error(poss) && !is_error(x) && !is_error(y)) ? ERROR_TYPE
                                                          : override;
}

DataType unif_numeric(DataType x, DataType y) {
  if (x == UNKNOWN_TYPE || y == UNKNOWN_TYPE) {
    return UNKNOWN_TYPE;
  } else if (!is_numeric(x) || !is_numeric(y)) {
    return ERROR_TYPE;
  } else if (x == FLOAT_TYPE || y == FLOAT_TYPE) {
    return FLOAT_TYPE; // highest promotion is to float
  } else if (x == INT_TYPE || y == INT_TYPE) {
    return INT_TYPE;
  } else {
    return BOOLEAN_TYPE;
  }
}

// unify comparisons
DataType unif_comp(DataType x, DataType y) {
  if (x == UNKNOWN_TYPE || y == UNKNOWN_TYPE) {
    return UNKNOWN_TYPE;
  } else if (x == ERROR_TYPE || y == ERROR_TYPE) {
    return ERROR_TYPE;
  } else if (is_numeric(x) && is_numeric(y)) {
    return BOOLEAN_TYPE;
  } else if (x == y) {
    return BOOLEAN_TYPE;
  } else {
    return ERROR_TYPE;
  }
}

// unify logical operations
DataType unif_logic(DataType x, DataType y) {
  if (x == UNKNOWN_TYPE || y == UNKNOWN_TYPE) {
    return UNKNOWN_TYPE;
  } else if (x == ERROR_TYPE || y == ERROR_TYPE) {
    return ERROR_TYPE;
  } else if (is_bool(x) && is_bool(y)) {
    return BOOLEAN_TYPE;
  } else {
    return ERROR_TYPE;
  }
}
