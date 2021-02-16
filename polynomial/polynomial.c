#include<stdio.h>
#include<assert.h>
#include"polynomial.h"

int
Polynomial_Print(Polynomial *poly)
{
    int cc = 0;
    int ii;
    if (poly == NULL) return cc;
    for (ii=0; ii<Polynomial_MaxTerm; ii++) {
        if (poly->terms[ii].degree == -1) {
            break;
        } else if (poly->terms[ii].degree == 0) { //印出0次方
            if (cc == 0) { //判斷是否為第一項
                printf("%f", poly->terms[ii].coef);
            } else {
                printf("%+f", poly->terms[ii].coef);
            }
        } else if (poly->terms[ii].degree == 1) { //印出一次方
            if (cc == 0) {
                printf("%fx", poly->terms[ii].coef);
            } else {
                printf("%+fx", poly->terms[ii].coef);
            }
        } else { //印出多次方
            if (cc == 0) {
                printf("%fx^%d", poly->terms[ii].coef, poly->terms[ii].degree);
            } else {
                printf("%+fx^%d", poly->terms[ii].coef, poly->terms[ii].degree);
            }
        }
        cc++;
    }
    if (cc == 0) {
        /* There is no non-zero term. This is a zero polynomial. */
        printf("0");
    }
    return cc;
}

void
Polynomial_Zero(Polynomial *poly)
{
    assert(poly != NULL);
    // set stop term at position zero
    poly->terms[0].degree = -1;  //陣列是0時 次方值=-1
}

int
Polynomial_isZero(Polynomial *poly)
{
    int ii;
    assert(poly != NULL);
    for(ii=Polynomial_MaxTerm; ii>=0; ii--){
        if(poly->terms[ii].degree != 0){//檢查是否為0多項式
            return NO;
        }else return YES;
    }
}

int
Polynomial_Degree(Polynomial *poly)
{
    assert(poly != NULL);
    return poly->terms[0].degree;
}

float
Polynomial_Coef(Polynomial *poly, int degree)
{
    int ii;
    assert(poly != NULL);
    assert(0<=degree);
    for (ii=0; ii<Polynomial_MaxTerm; ii++) {
        if (poly->terms[ii].degree == degree) {
            return poly->terms[ii].coef;
        } else if (poly->terms[ii].degree < degree) {
            break;
        }
    }
    return 0;
}

void
Polynomial_Attach(Polynomial *poly, float coef, int degree)  //加新的項進去 
{
    int ii;
    int ss;
    assert(poly != NULL);
    assert(0<=degree);
    // find the position to attach this term  
    for (ii=0; ii<Polynomial_MaxTerm; ii++) {
        if (poly->terms[ii].degree == degree) {  //同次方
            poly->terms[ii].coef = coef+poly->terms[ii].coef;  //改係數
		if(poly->terms[ii].coef==0) poly->terms[ii] .degree=0;
            return;
        } if (poly->terms[ii].degree < degree) {
            break;
        }
    }
    ss = ii;//把插入位置給ss
    
    for (; ii<Polynomial_MaxTerm; ii++) {
        if (poly->terms[ii].degree == -1) {//找到要插入的陣列起始位置
            break;
        }
    }
    ii++;//插入起始位置 
    assert(ii<=Polynomial_MaxTerm);
    // move [ss - ii] -> [ss+1 - ii+1]
    for (; ii>ss; ii--) {
        poly->terms[ii] = poly->terms[ii-1];//留空位給要插入的項
    }
    poly->terms[ii].degree = degree;
    poly->terms[ii].coef = coef;
}

void
Polynomial_Remove(Polynomial *poly, int degree)
{
    int ii;
    assert(poly != NULL);
    assert(0<=degree);
    // find the position of the specified term
    for (ii=0; ii<Polynomial_MaxTerm; ii++) {
        if (poly->terms[ii].degree == degree) {
            break;
        } else if (poly->terms[ii].degree < degree) {
            // not found 回去繼續找
            return;
        }
    }
    for (; ii<Polynomial_MaxTerm; ii++) {
        poly->terms[ii] = poly->terms[ii+1];
        if (poly->terms[ii].degree == -1) {
            return;
        }
    }
}

void
Polynomial_Add(Polynomial *pa, Polynomial *pb, Polynomial *pc)
{
 int a=0,b=0,c=0;
 Polynomial_Zero(pc);
 for(a=0;pa->terms[a].degree!=-1;a++){  //從0到下一個多項式-1
  Polynomial_Attach(pc,pa->terms[a].coef,pa->terms[a].degree);//pc多項式放入pa的係數跟次方
 }
 for(b=0;pb->terms[b].degree!=-1;b++){  //整個pb方程式
  for(c=0;pc->terms[c].degree!=-1;c++){ //整個pc方程式
   if(pb->terms[b].degree==pc->terms[c].degree){  //同樣次方
    if(pb->terms[b].coef+pc->terms[c].coef==0){ // 係數相加=0
     Polynomial_Remove(pc,pb->terms[b].degree); //刪除係數為0的項
     break;
    }
    else{ //相加不為0
     pc->terms[c].coef=pb->terms[b].coef+pc->terms[c].coef;
     break;
    }
   }
  }
  if(pc->terms[c].degree==-1){ //pa為空丟入後Pc仍為空 放入pb
   Polynomial_Attach(pc,pb->terms[b].coef,pb->terms[b].degree);
  }
 }
}

void Polynomial_Mul(Polynomial *pa, Polynomial *pb, Polynomial *pc)
{
 int a=0,b=0,c=0,max=0;
 Polynomial temp;
 Polynomial_Zero(pc);
 /*相乘*/
 for(a=0;pa->terms[a].degree!=-1;a++){ 
  for(b=0;pb->terms[b].degree!=-1;b++){
   pc->terms[c+1]=pc->terms[c]; //增加pc 空出位置
   pc->terms[c].coef=pa->terms[a].coef*pb->terms[b].coef;
   pc->terms[c].degree=pa->terms[a].degree+pb->terms[b].degree;
   c++; //下一個位置
  }
 }
 /*合併相乘後找同次方係數*/
 for(c=0;pc->terms[c].degree!=-1;c++){   
  for(b=c+1;pc->terms[b].degree!=-1;b++){ 
      if(pc->terms[c].degree==pc->terms[b].degree&&pc->terms[b].coef!=0){
    //pc多項式內同次方 且 後者係數不為0
       pc->terms[c].coef=pc->terms[c].coef+pc->terms[b].coef;
    //同次方 係數相加
       pc->terms[b].coef=0;
    //同次方 後項係數歸0
   }
  }
 }
 /*刪掉係數為0項*/
 for(c=0;pc->terms[c].degree!=-1;c++){
  if(pc->terms[c].coef==0){ //係數為0 位置C
   for(b=c;pc->terms[b].degree!=-1;b++){
    pc->terms[b]=pc->terms[b+1]; //後項覆寫
   }
  }
 }
}

