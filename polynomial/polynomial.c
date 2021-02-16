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
        } else if (poly->terms[ii].degree == 0) { //�L�X0����
            if (cc == 0) { //�P�_�O�_���Ĥ@��
                printf("%f", poly->terms[ii].coef);
            } else {
                printf("%+f", poly->terms[ii].coef);
            }
        } else if (poly->terms[ii].degree == 1) { //�L�X�@����
            if (cc == 0) {
                printf("%fx", poly->terms[ii].coef);
            } else {
                printf("%+fx", poly->terms[ii].coef);
            }
        } else { //�L�X�h����
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
    poly->terms[0].degree = -1;  //�}�C�O0�� �����=-1
}

int
Polynomial_isZero(Polynomial *poly)
{
    int ii;
    assert(poly != NULL);
    for(ii=Polynomial_MaxTerm; ii>=0; ii--){
        if(poly->terms[ii].degree != 0){//�ˬd�O�_��0�h����
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
Polynomial_Attach(Polynomial *poly, float coef, int degree)  //�[�s�����i�h 
{
    int ii;
    int ss;
    assert(poly != NULL);
    assert(0<=degree);
    // find the position to attach this term  
    for (ii=0; ii<Polynomial_MaxTerm; ii++) {
        if (poly->terms[ii].degree == degree) {  //�P����
            poly->terms[ii].coef = coef+poly->terms[ii].coef;  //��Y��
		if(poly->terms[ii].coef==0) poly->terms[ii] .degree=0;
            return;
        } if (poly->terms[ii].degree < degree) {
            break;
        }
    }
    ss = ii;//�ⴡ�J��m��ss
    
    for (; ii<Polynomial_MaxTerm; ii++) {
        if (poly->terms[ii].degree == -1) {//���n���J���}�C�_�l��m
            break;
        }
    }
    ii++;//���J�_�l��m 
    assert(ii<=Polynomial_MaxTerm);
    // move [ss - ii] -> [ss+1 - ii+1]
    for (; ii>ss; ii--) {
        poly->terms[ii] = poly->terms[ii-1];//�d�Ŧ쵹�n���J����
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
            // not found �^�h�~���
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
 for(a=0;pa->terms[a].degree!=-1;a++){  //�q0��U�@�Ӧh����-1
  Polynomial_Attach(pc,pa->terms[a].coef,pa->terms[a].degree);//pc�h������Jpa���Y�Ƹ򦸤�
 }
 for(b=0;pb->terms[b].degree!=-1;b++){  //���pb��{��
  for(c=0;pc->terms[c].degree!=-1;c++){ //���pc��{��
   if(pb->terms[b].degree==pc->terms[c].degree){  //�P�˦���
    if(pb->terms[b].coef+pc->terms[c].coef==0){ // �Y�Ƭۥ[=0
     Polynomial_Remove(pc,pb->terms[b].degree); //�R���Y�Ƭ�0����
     break;
    }
    else{ //�ۥ[����0
     pc->terms[c].coef=pb->terms[b].coef+pc->terms[c].coef;
     break;
    }
   }
  }
  if(pc->terms[c].degree==-1){ //pa���ť�J��Pc������ ��Jpb
   Polynomial_Attach(pc,pb->terms[b].coef,pb->terms[b].degree);
  }
 }
}

void Polynomial_Mul(Polynomial *pa, Polynomial *pb, Polynomial *pc)
{
 int a=0,b=0,c=0,max=0;
 Polynomial temp;
 Polynomial_Zero(pc);
 /*�ۭ�*/
 for(a=0;pa->terms[a].degree!=-1;a++){ 
  for(b=0;pb->terms[b].degree!=-1;b++){
   pc->terms[c+1]=pc->terms[c]; //�W�[pc �ťX��m
   pc->terms[c].coef=pa->terms[a].coef*pb->terms[b].coef;
   pc->terms[c].degree=pa->terms[a].degree+pb->terms[b].degree;
   c++; //�U�@�Ӧ�m
  }
 }
 /*�X�ۭ֬����P����Y��*/
 for(c=0;pc->terms[c].degree!=-1;c++){   
  for(b=c+1;pc->terms[b].degree!=-1;b++){ 
      if(pc->terms[c].degree==pc->terms[b].degree&&pc->terms[b].coef!=0){
    //pc�h�������P���� �B ��̫Y�Ƥ���0
       pc->terms[c].coef=pc->terms[c].coef+pc->terms[b].coef;
    //�P���� �Y�Ƭۥ[
       pc->terms[b].coef=0;
    //�P���� �ᶵ�Y���k0
   }
  }
 }
 /*�R���Y�Ƭ�0��*/
 for(c=0;pc->terms[c].degree!=-1;c++){
  if(pc->terms[c].coef==0){ //�Y�Ƭ�0 ��mC
   for(b=c;pc->terms[b].degree!=-1;b++){
    pc->terms[b]=pc->terms[b+1]; //�ᶵ�мg
   }
  }
 }
}

