#include "mzStackA.h"

void mazeStack_init(mazeStack_t *stack)//完成此函數 
{
	stack->top=-1;      //堆疊初始化 top設為-1 
}

int mazeStack_full(mazeStack_t *stack)
{
	return (stack->top>=(MAX_MAZE_STACK_SIZE-1));   //堆疊已滿 
}

int mazeStack_empty(mazeStack_t *stack)
{
	return (stack->top < 0);    //堆疊是空的 
}  

int mazeStack_insert(mazeStack_t *stack, mazeStackElement_t *element)
{
	if (mazeStack_full(stack)) {   //如果堆疊已滿
		return 0;
	} else {
		stack->elements[++stack->top]= *element;   //將座標方向存入堆疊裡 
		return 1;
	}
}

int mazeStack_delete(mazeStack_t *stack, mazeStackElement_t *element)//完成此函數 
{
	if (mazeStack_empty(stack)) { //如果堆疊已空
		return 0;
	} else {
		*element = stack->elements[stack->top--];  //將座標方向從堆疊裡拿出來 
		return 1;
	}
}
