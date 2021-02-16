/* Note.
	Stack is implemented by using an array.
*/
#ifndef	_MAZE_STACK //程序一
//當標識符已經被定義過(一般是用#define命令定義)，則對程序段1進行編譯，否則編譯程序段2。

#define _MAZE_STACK  //程序2
#ifndef	_MAZE_STACK_ 
#define	_MAZE_STACK_ 

#ifdef	__cplusplus
extern "C" {
#endif	/* __cplusplus */

#define	MAX_MAZE_STACK_SIZE	10000

typedef	struct	{ //typedef 是為struct起新名字mazeStackElement_t
	int xx;		// x-axis of the current position
	int yy;		// y-axis of the current position
	int dir;	// next search direction
}mazeStackElement_t;
/*
*   巢狀結構 堆疊儲存行走的路徑(座標以及方向) 
*/ 
typedef	struct{  //typedef 是為struct起新名字mazeStack_t
	int	top;
	mazeStackElement_t	elements[MAX_MAZE_STACK_SIZE];
}mazeStack_t;

void mazeStack_init(mazeStack_t *stack);
int	mazeStack_full(mazeStack_t *stack);
int	mazeStack_empty(mazeStack_t *stack);
int	mazeStack_insert(mazeStack_t *stack, mazeStackElement_t *element);
int	mazeStack_delete(mazeStack_t *stack, mazeStackElement_t *element);

#ifdef	__cplusplus
}
#endif	/* __cplusplus */

#endif	/* _MAZE_STACK_ */
#endif	/* _MAZE_STACK */

