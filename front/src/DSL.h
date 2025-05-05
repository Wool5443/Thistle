#ifndef THISTLE_DSL_H_
#define THISTLE_DSL_H_

#define T(t) ((Token) { .type = t })
#define ET ((Token){})

#define N(t) ((Node_data) { .type = t })

#define OPERATION(op, left, right) (node_ctor((Node_data){ .type = NODE_MATH_OPERATION, .operation = op }, left, right))
#define NUMBER(num) (node_ctor((Node_data){ .type = NODE_NUMBER, .integer = num }, NULL, NULL))
#define STRING(str) (node_ctor((Node_data){ .type = NODE_STRING, .string = str }, NULL, NULL))

#endif // THISTLE_DSL_H_
