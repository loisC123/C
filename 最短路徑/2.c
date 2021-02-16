/*演算法
KRUSKAL處理邏輯 

begin
	begin T包含少於n-1個邊&&E非空  loop
	 	從E中選成本最小的邊 
	  	if 放入T中不會形成迴路 then
	 		 將邊加入
	 	else 
			捨棄
	end
end
 	
*完成 union find 兩個函式
*將quickSort改寫成heapSort
*/

#include<stdio.h>
#include<stdlib.h>
#define SWAP(x, y, t){ t = x; x = y; y = t;}

typedef struct {
	int	start;
	int	end;
	int	cost;
}mst;

int parent[1000];
void heapSort(mst *e, int n);
void adjust(mst *e, int root,int n);
void makeSet(int size); 
int find(int v);
int unionSet(int v, int u);

int main(int argc, char *argv[]) {
	int i, n, m, sum = 0, count = 0, set_num = 0;
	mst *e;
	mst *ptr;
	FILE* fin;
	
	if (argc < 1) { 
		fin = stdin;
	}else {
		fin = fopen(argv[1], "rt");
		if (!fin) {
			fprintf(stderr, "file %s not found\n", argv[1]);
			exit(1);
		}
	}
	fscanf(fin, "%d, %d", &n, &m); //n節點, m邊  
	printf("nodes = %3d, edges = %3d\n", n, m);
	
	e = (mst *)malloc(sizeof(mst)*(m+1));//配記憶體 
	for (i = 1; i <= m; i++) { //從1開始 方便後續 堆積排序從[1]開始儲存 留[0]刪除
		ptr = &(e[i]);  //e[i]=整個結構
		fscanf(fin, "%d, %d, %d", &(ptr->start), &(ptr->end), &(ptr->cost)); 
		printf("edge[%3d]: start = %3d, end = %3d, cost = %3d\n", i, ptr->start, ptr->end, ptr->cost); //完成輸出 (輸出A點至B點的花費)
	}
	fclose(fin); //關閉檔案

	makeSet(n); //呼叫function初始化 傳入節點
	heapSort(e, m); //排序 傳入結構e和邊
	
	puts("排序後"); 
	//印出排序後的情況
	puts("MST...成長中");
	
	for (i = 1; i <= m; i++) {
		if (count == m - 1) { //判斷如果邊達到n-1則跳出演算法
			break;
		}
		if (unionSet(e[i].start, e[i].end)) { 
			count++;//條數增加 
			sum += e[i].cost;//成本加總 
			printf("[%3d] start = %3d, end = %3d, cost = %3d\n",i,e[i].start, e[i].end, e[i].cost);//完成印出 
		}
	}
	
	for (i = 1; i < n+1; i++) {//到最後只有一個parent又是他自己 
		if (parent[i] == i) {
			set_num++;
		}
	} 
	
	if (set_num > 1) {//完成此判斷條件 
		puts("");
		puts("找不到生成樹");
	}
	else {
		printf("總成本:%d\n", sum);
	} 
	
	free(e);
	return 0;
}


void makeSet(int size){  //初始化為了之後的UNION 
	int i;
	for(i = 0;i < size;i++){
		parent[i] = i;//先使每個節點為父節點  
	} 
}

//將此排序法改寫成heap sort
void heapSort(mst *e, int n) { // n個邊 
	int i,j; //堆積排序 最大堆積 
	mst temp;
	for(i = n/2;i > 0;i--){//排最大推積  parent在e[n/2]
		adjust(e, i, n); //傳入e路徑 i節點位置 n資料數
	}
	for(i = n-1;i > 0;i--){//開始排序  最後由小到大排 
		SWAP(e[1], e[i+1], temp);   //temp=e[1] e[1]存為根; e[1]=e[i+1] e[1]變成e[i+1]; e[i+1]=temp 
		adjust(e, 1, i);//每拿一個rootkey 刪除一個節點
	}
}

void adjust(mst *e, int root,int n){//最大堆積
	int child, rootkey;
	mst temp;
	temp = e[root];//暫存點 為根結點
	rootkey = e[root].cost;  
	child = 2*root;//可以得到左孩子 root為i 左2i 右2i+1
	while(child <= n){
		if( (child < n) && (e[child].cost < e[child+1].cost) ){//左右小孩比較 
			child++; //如果右孩子比較大 孩子節點往後跟父節點比較
		}
		if(rootkey > e[child].cost){//父親ROOTKEY比小孩大那就不用換 
			break;
		}else{ //小孩比ROOTKEY大 交換
			e[child/2] = e[child];//child/2=父節點 小孩跟父親交換 
			child *= 2;// 繼續往下一個左孩子
		}
	}
	e[child/2] = temp;//原孩子變成新根結點 
}

//檢查是否為迴路(路徑壓縮) 
int find(int v) {          
	while(parent[v]!=v){     //找到parent[v]=v 也就是原始e儲存的節點
    	v=parent[v];
	}
	return v;
}

int unionSet(int v, int vv) {//e[i].start, e[i].end 
	int a =  find(v);
	int b =  find(vv);
	
	if(a != b){//如果頭不一樣 那就連接起來 
		parent[a] = b;
		return 1;		
	}else{//有迴圈  
		return 0;		
	}
}
