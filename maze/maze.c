#include<stdio.h>
#include "mzStackA.h"

// choose one to enable/disable debug information
//#define	JWD(x)	printf x
#define	JWD(x)

/*
 * Data Structures and Interface of MAZE
 */

struct	{                                       //宣告結構內有整數型態座標x y 
	int off_x;                                  //x座標  
	int off_y;                                  //y座標 
} offsets[4] = {             //自訂結構陣列大小為4 儲存四個方向 
	{ -1,  0 },	// N, North                
	{  0,  1 },	// E, East
	{  1,  0 },	// S, South
	{  0, -1 }	// W, West
};

/* offsets內有2個int 1個int佔4bytes 4陣列 總共2*4*4=32bytes 
   offstes[0]=2*4*1=8bytes 所以DIRS=32/8=4陣列 */ 
#define	DIRS	(sizeof(offsets)/sizeof(offsets[0]))//0(^),1(>),2(v),3(<)
#define	ENDPT	(DIRS+0)       //ENDPT=4+0=4  (終點)  display[4] *
#define	SPACE	(DIRS+1)       //SPACE=4+1=5  (迷宮中可以走的地方) display[5] 
#define	WALL	(DIRS+2)       //WALL=4+2=6   (牆壁)  display[6] H

/*先以數字方式儲存迷宮的牆壁、路線、終點等等...配合的字元陣列display作為其索引值印出
字元陣列display 儲存路線指示圖示、終點圖示、空格圖示、牆壁圖示*/ 
char *display[] = {"*", "^", ">", "v", "<",  " ", "H" };

#define	VISITED	'Y'                //以 VISITED表示已走過此地方 
#define	NON_VISITED	'N'                //以 NON_VISITED表示尚未走過此地方 
#define	MAX_MAZE_SIZE	100        //迷宮大小最大限制為100 
#define	MAX_ARRAY_SIZE	(MAX_MAZE_SIZE + 2)   

int	mm;                                       //迷宮長 
int	nn;                                       //迷宮寬
int	maze[MAX_MAZE_SIZE][MAX_MAZE_SIZE];       //全域陣列maze最大為100*100(儲存迷宮狀態) 
char mark[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE]; //全域陣列mark最大為102*102(儲存是否走過此地方) 

void mazeInit();                              //宣告函數
int mazeIsSpace(int xx, int yy);              
int mazeIsVisited(int xx, int yy);            
int mazeMarkVisited(int xx, int yy);          
int mazeMarkPath(int xx, int yy, int dir);    

/*
 * Data Structure and Interface of STACK
 */

mazeStack_t	myStack;   //宣告巢狀結構mazeStack_t的myStack(堆疊) 

#define	stack_init()	mazeStack_init(&myStack)   //定義stack_init()會呼叫函數mazeStack_init並將myStack傳入 
#define	stack_full()	mazeStack_full(&myStack)   //定義stack_full()會呼叫函數mazeStack_full並將myStack傳入 
#define	stack_empty()	mazeStack_empty(&myStack)  //定義stack_empty()會呼叫函數mazeStack_empty並將myStack傳入
#define	push(element)	mazeStack_insert(&myStack, (element)) //定義push(element)會呼叫函數mazeStack_insert並將myStack以及element傳入 
#define	pop(element)	mazeStack_delete(&myStack, (element)) //定義pop(element)會呼叫函數mazeStack_delete並將myStack以及lelment傳入 

void ShowMaze();       //宣告函數
int	ReadFile(FILE *file); 
int	FindPath();      
int	Rollback();   

/*
 *	Main Function
 */
int main(int argc, char *argv[])//argv[1]=data 1\2\3.txt
{
	FILE *input = NULL;        //宣告FILE指標input準備指向檔案 
	if (argc < 2) {           //以參數方式讀檔 放在argv陣列裡 argc可以判斷陣列裡是否有放東西 
		input = stdin;			//stdin=0 鍵盤輸入
	}else {
		input = fopen(argv[1], "rt");     //開啟檔案來提供輸入(Read Text)
		if (input == NULL) {        //找不到
			printf("file [%s] not found\n", argv[1]);
			return 1;
		}
	}
	mazeInit();                      //把迷宮預設為全部都是牆壁 且都沒走過 
	if (ReadFile(input) == 0) {      //判斷檔案裡是否有資料被讀取 
		printf("read file [%s] error\n", argv[1]);
		return 1;
	}
	ShowMaze();                      //輸出迷宮初始樣子 
	printf("search a path from (1,1) to (%d,%d)...\n", mm, nn);
	if (FindPath() == 0) {           //判斷是否有找到終點 
		printf("path is not found\n\n");
		return 1;
	}
	printf("path is found\n\n");
	printf("MAZW Size: %dx%d\n",mm,nn);
	ShowMaze();                      //輸出迷宮從起點到終點的路徑 
	return 0;
}
int FindPath()
{
	mazeStackElement_t now, next;           //宣告結構mazeStackElement_t的now與next 儲存座標與方向 
	/*設定起始位置、起始檢索值*/ 
	now.xx=1;                               //初始座標x為1 
	now.yy=1;                               //初始座標y為1 
	now.dir=0;                              //設定初始方向 為 0(向北走) 
	stack_init();                           //把mystack的top設為-1 
	push(&now);/*完成此呼叫*/                //將當前位置存進堆疊 
	while (! stack_empty()) {               //當堆疊不是空的時候運行內部程式碼  
		pop(&now);/*完成此呼叫*/              //呼叫pop函數 回到前一個位置 
		JWD(("back to (%d, %d)\n", now.xx, now.yy));
		for (;now.dir<4; now.dir++) {//繼續下一個方向  以now.dir作為索引 測試可以行走的方向 
			/*找到下一個要走的方向*/
			next.xx=now.xx+offsets[now.dir].off_x;       //將往該方向行走後的座標x給next的x座標 
			next.yy=now.yy+offsets[now.dir].off_y;       //將往該方向行走後的座標y給next的y座標 
			JWD(("test (%d,%d)\n", next.xx, next.yy));
			if (mazeIsSpace(next.xx, next.yy)&&(!mazeIsVisited(next.xx, next.yy)) ) 
            {   //判斷欲行走的座標是否已經走過 (不能走過) 
				mazeMarkVisited(next.xx, next.yy);                 //將該座標標示已經走過了 
				push(&now);                                        //將當前座標存入堆疊 
				JWD(("move to (%d, %d)\n", next.xx, next.yy));      
				// 下一個座標 
				now=next;                                    //將行走後的位置作為現在的位置 
				now.dir = -1;                                 //現在的方向設為-1 重新尋找可以走的方向 
				if ((now.xx == mm) && (now.yy == nn)) {     //判斷現在的位置是不是終點 
					mazeMarkPath(now.xx, now.yy,ENDPT);      //該點為 終點 存入迷宮maze 
					Rollback();              //將終點回到起點的路徑從堆疊裡輸入迷宮maze 
					return 1;
				}
			}
		}
	}
	return 0;
}
int Rollback()                                                     //從終點走回原點 
{
	mazeStackElement_t stop;            //宣告 mazeStackElement_t結構的 stop  
  
	while (!stack_empty()){                 //堆疊不為空的情況下運行(回到原點前都要運行) 
		pop(&stop);                   //呼叫pop函數 當前位置會存在stop裡 並回到前一個位置 
		JWD(("(%d, %d, %d)\n", stop.xx, stop.yy, stop.dir));
		mazeMarkPath(stop.xx, stop.yy, stop.dir);    //呼叫 mazeMarkPath函數 轉換陣列maze裡的迷宮路徑如何走 
	}
	return 1;
}
/*
 * MAZE relation operations
 */
int ReadFile(FILE *file)      //讀檔 
{
	int ii, jj;
	if (fscanf(file, "%d,%d", &mm, &nn) < 2) { 
		//從檔案裡讀取一開始的長X寬 存mm與nn裡 包括牆壁要大於2
		return 0;
	}
	for (ii=1;ii<=mm;ii++) {    //最外圍預設為牆壁 從(1,1)開始
		for (jj=1; jj<=nn; jj++) {
			int digit;
			if(fscanf(file,"%d", &digit)<1) { //判斷迷宮是牆壁1還是路0
				return 0;
			}
			if (digit== 0) { //迷宮初始皆為牆壁 所以遇到0時才更改迷宮狀態 設為SPACE表示可以走 
				maze[ii][jj] = SPACE;  //更改迷宮狀態 從牆壁改為可以行走 
			}
		}
	}
	return 1;
}
void ShowMaze()//完成此函數 參考結果圖、配合此程式定義印出迷宮。 
{
	int ii,jj;
	for(ii=0;ii<=mm+1;ii++){              //長 包含外圍的牆壁所以從0開始直到mm+1 
		for(jj=0;jj<=nn+1;jj++){          //寬 包含外圍的牆壁所以從0開始直到nn+1 
			printf("%s",display[maze[ii][jj]]);  //迷宮以數值狀態儲存於陣列maze 以display把數值轉換成字元 
		}
		printf("\n");
	}
}
void mazeInit()             //將陣列初始化(全為牆壁而且都未走過) 
{
	int ii, jj;
	for (ii=0; ii<MAX_ARRAY_SIZE; ii++) {         
		for (jj=0; jj<MAX_ARRAY_SIZE; jj++) {
			maze[ii][jj] = WALL;                 //全為牆壁 
			mark[ii][jj] = NON_VISITED;          //全部未走過 
		}
	}
}
int mazeIsSpace(int xx, int yy)//判斷能不能繼續走
{
	if(maze[xx][yy]==SPACE)         //判斷該座標是否為空格可以行走 
	return 1;
	else 
	return 0;
}
int mazeIsVisited(int xx, int yy)
{
	if(mark[xx][yy]!=NON_VISITED)              //判斷該座標是否有走過 
	return 1;
	else
	return 0;
}
int mazeMarkVisited(int xx, int yy)	//走過標記 
{
	mark[xx][yy]=VISITED;           //該座標走過 所以改為已走過 
}
int mazeMarkPath(int xx, int yy, int dir)/*完成本函數*/
{
	maze[xx][yy]=dir;          //轉換迷宮路徑 
}