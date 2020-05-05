#ifndef TYPEDEF_H
#define TYPEDEF_H

#define TD_SUB 0
#define TD_DIA 1
#define TD_SUP 2
#define TD_RHS 3
//lhs subdiagonal   = tdsys[0]
//lhs diagonal      = tdsys[1]
//lhs superdiagonal = tdsys[2]
//rhs               = tdsys[3]
typedef double* tdsys[4];

#endif
