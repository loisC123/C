#include<stdlib.h>
#include<stdio.h>

void Merge(int a[],int b[],int l,int m ,int r){  //合併兩個有序子段
    int i=l,j=m+1,k=l; //i=l為a陣列開始合併位置 , j為第二段開始位置，
    while(i<=m&&j<=r){  //m為第一段結束位置，r為第二段結束位置,兩段都還沒結束
        if(a[i]<a[j])b[k++]=a[i++]; //如果第二段值大於第一段 放第二段入b陣列
        else b[k++]=a[j++];    //如果第一段較大 放第一段進b陣列
    }
    while(i<=m)b[k++]=a[i++]; //第一段未排序完但第二段已放完或未開始 一段剩下的先放進B陣列
    while(j<=r)b[k++]=a[j++]; //第二段未排序完但第一段已放完或未開始 二段剩下的放進B陣列
}
void MergePass(int x[],int y[],int s,int n){ //對於長為n陣列進行歸併，n為串列中元素個樹  s為每一個區段大小
    int i=0;          
    while(i<=n-2*s){//未合併的超過2*s長度的元素=剩下元素可以湊成一個長為s子段還能兩兩一組當
        Merge(x,y,i,i+s-1,i+2*s-1); // i+s和i+2s都是下一個序列的起始點，所以一個序列的終點就是這兩個值-1
        i=i+2*s;  //下一個元素的起始點
    }
    //未合併的元素長度大於s，表示還能構成一個長度為s的序列和一個以歸併小於s的序列，就是最後兩個序列，再執行一次歸併
    if(i+s<n)Merge(x,y,i,i+s-1,n-1);//剩下的長度小於2*s大於s
    else{//剩下的長度小於s 只剩一個序列 無法歸併，直接把元素加到y陣列後端
        for(int j=i;j<=n-1;j++){
            y[j]=x[j];
        }
    }
}
void MergeSort(int a[],int n){  //合併排序法 
    int b[10]; //臨時陣列
    int s=1;  //每次S個元素歸併 設初始質為1   
    while(s<n){
        MergePass(a,b,s,n);//把a合併到b中 排好的在B
        s+=s; //累加
        MergePass(b,a,s,n);//把排序過的b合併到原陣列a中
        s+=s;
    }
}

int main(){
    int a[10]={2,5,1,3,4,9,7,8,6,0};
    int n=10;
    MergeSort(a,n);
    for(int i=0;i<n;i++){
        printf("%d ",a[i]);
    }
    printf("\n");
    return 0;
}
