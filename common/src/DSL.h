#ifndef THISTLE_DSL_H_
#define THISTLE_DSL_H_

#define T(t) ((Token) {.type = t})
#define ET ((Token) {})

#define N(t) ((Node_data) {.type = t})

#define OPERATION(op, left, right)                                             \
    (node_ctor((Node_data) {.type = NODE_MATH_OPERATION, .operation = op},     \
               left,                                                           \
               right))
#define INTEGER(num)                                                           \
    (node_ctor((Node_data) {.type = NODE_INTEGER, .integer = num}, NULL, NULL))
#define FLOATING(num)                                                          \
    (node_ctor((Node_data) {.type = NODE_FLOAT, .floating = num}, NULL, NULL))
#define NAME(str)                                                              \
    (node_ctor((Node_data) {.type = NODE_NAME, .string = str}, NULL, NULL))
#define STRING(str)                                                            \
    (node_ctor((Node_data) {.type = NODE_STRING, .string = str}, NULL, NULL))

#endif // THISTLE_DSL_H_
