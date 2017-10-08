#include <stdio.h>

#include "util.h"
#include "slp.h"
#include "prog1.h"

int maxargs(A_stm);
void interp(A_stm prog);
Table_ interpStm(A_stm s, Table_ t);
struct IntAndTable interpExp(A_exp e, Table_ t);
Table_ interPrint(A_expList exps, Table_ t);
int max(int a, int b) {
	return (a > b) ? a : b;
}
int maxargsExp(A_exp exp) {
	if (exp -> kind == A_idExp || exp -> kind == A_numExp)
	{
		return 0;
	}
	else if (exp -> kind == A_opExp)
	{
		return max(maxargsExp(exp -> u.op.left), maxargsExp(exp -> u.op.right));
	}
	else {
		return max(maxargs(exp -> u.eseq.stm), maxargsExp(exp -> u.eseq.exp));
	}
}
int maxargsPrint(A_expList expList) {
	if (expList->kind == A_lastExpList) 
		return 1;
  	if (expList->kind == A_pairExpList) 
  		return 1 + maxargsPrint(expList->u.pair.tail);
}

int maxargs(A_stm prog){
	if (prog -> kind == A_compoundStm)
	{
        return max(maxargs(prog -> u.compound.stm1), maxargs(prog -> u.compound.stm2));
	}
	else if (prog -> kind == A_assignStm)
	{
		return maxargsExp(prog -> u.assign.exp);
	}
	else if (prog -> kind == A_printStm)
	{
		return maxargsPrint(prog -> u.print.exps);
	}
	else {
		printf("Error: invalid prog kind\n");
		return 0;
	}
	return 0;
}

Table_ interpStm(A_stm s, Table_ t) {
  	if (s -> kind == A_compoundStm)
	{
		return interpStm(s -> u.compound.stm2, interpStm(s -> u.compound.stm1, t));
	}
	else if (s -> kind == A_assignStm)
	{
		struct IntAndTable table = interpExp(s -> u.assign.exp, t);
		return update(table.t, s -> u.assign.id, table.i);
	}
	else if (s -> kind == A_printStm)
	{
		return interPrint(s -> u.print.exps, t);
	}
	else {
		printf("Error: invalid prog kind\n");
		return 0;
	}
}

struct IntAndTable interpExp(A_exp e, Table_ t) {
	if (e -> kind == A_idExp)
	{
		struct IntAndTable table;
        table.i = lookup(t, e -> u.id);
        table.t = t;
        return table;
	}
	else if (e -> kind == A_numExp) {
		struct IntAndTable table;
        table.i = e -> u.num;
        table.t = t;
        return table;
	}
	else if (e -> kind == A_opExp)
	{
		struct IntAndTable table;
        struct IntAndTable left = interpExp(e -> u.op.left, t);
        struct IntAndTable right = interpExp(e -> u.op.right, t);
        if (e -> u.op.oper == A_plus)
        {
        	table.i = left.i + right.i;
        }
        else if (e -> u.op.oper == A_minus)
        {
        	table.i = left.i - right.i;
        }
        else if (e -> u.op.oper == A_times)
        {
        	table.i = left.i * right.i;
        }
        else if (e -> u.op.oper == A_div)
        {
        	table.i = left.i / right.i;
        }
        else {
			assert(0);
        }
        table.t = t;
        return table;
	}
	else {
        return interpExp(e -> u.eseq.exp, interpStm(e -> u.eseq.stm, t));
	}
}

Table_ interPrint(A_expList exps, Table_ t) {
	if (exps -> kind == A_lastExpList) 
	{
    	struct IntAndTable table = interpExp(exps -> u.last, t);
    	printf("%d\n", table.i);
    	return table.t;
  	} 
  	else {
    	struct IntAndTable table = interpExp(exps -> u.pair.head, t);
    	printf("%d ", table.i);
    	return interPrint(exps -> u.pair.tail, table.t);
  	}
}

void interp(A_stm prog){
	Table_ t = 0;
  	t = interpStm(prog, t);
}

// DO NOT CHANGE!
int main(){
	printf(">> Right Prog Section:\n");
	A_stm rp = right_prog();
	printf("the maximum number of arguments of any print statement is %d\n",maxargs(rp));
	interp(rp);

	printf(">> Error Prog Section:\n");
	A_stm ep = error_prog();
	printf("the maximum number of arguments of any print statement is %d\n",maxargs(ep));
	interp(ep);	
	return 0;
}

