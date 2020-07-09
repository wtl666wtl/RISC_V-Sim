#include <cmath>
#include <cstdio>
#include <cstring>
#include <iostream>
#define N 1000006
using namespace std;
char s[16];
const int endline[32]={0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,1,0,0,1,1};
int pos,PC,NPC,IR[32],reg[32],A,B,C,rd,rs1,rs2,fun,fun2,Imm,Op,LMD,cond,ALUOut,Mem[N];
int getpos()
{
	int p=0;
	for(int i=1;i<=8;i++)
		if(s[i]>='A')p=p*16+s[i]-'A'+10;
		else p=p*16+s[i]-'0';
	return p;
}
int getnum()//binary
{
	int x=0,y=0,num=0;
	if(s[1]>='A')y=s[1]-'A'+10;else y=s[1]-'0';
	if(s[0]>='A')x=s[0]-'A'+10;else x=s[0]-'0';
	int p=1,t=1;
	while(y){
		if(y&p)num+=t,y-=p;
		p<<=1;t<<=1;
	}
	p=1;t=1<<4;
	while(x){
		if(x&p)num+=t,x-=p;
		p<<=1;t<<=1;
	}
	return num;
}
void preU()
{
	rd=0;
	for(int i=11;i>=7;i--)rd=rd*2+IR[i];
	Imm=0;
	for(int i=31;i>=12;i--)Imm=Imm*2+IR[i];
	Imm<<=12;
}
void preJ()
{
	rd=0;
	for(int i=11;i>=7;i--)rd=rd*2+IR[i];
	Imm=0;
	for(int i=31;i>=20;i--)Imm=Imm*2+IR[31];
	for(int i=19;i>=12;i--)Imm=Imm*2+IR[i];
	Imm=Imm*2+IR[20];
	for(int i=10;i>=1;i--)Imm=Imm*2+IR[i+20];
	Imm<<=1;
}
void preI()
{
	rd=0;
	for(int i=11;i>=7;i--)rd=rd*2+IR[i];
	fun=0;
	for(int i=14;i>=12;i--)fun=fun*10+IR[i];
	rs1=0;
	for(int i=19;i>=15;i--)rs1=rs1*2+IR[i];
	Imm=0;
	for(int i=31;i>=11;i--)Imm=Imm*2+IR[31];
	for(int i=10;i>=0;i--)Imm=Imm*2+IR[i+20];
}
void preB()
{
	fun=0;
	for(int i=14;i>=12;i--)fun=fun*10+IR[i];
	rs1=0;
	for(int i=19;i>=15;i--)rs1=rs1*2+IR[i];
	rs2=0;
	for(int i=24;i>=20;i--)rs2=rs2*2+IR[i];
	Imm=0;
	for(int i=31;i>=12;i--)Imm=Imm*2+IR[31];
	Imm=Imm*2+IR[7];
	for(int i=10;i>=5;i--)Imm=Imm*2+IR[i+20];
	for(int i=4;i>=1;i--)Imm=Imm*2+IR[i+7];
	Imm<<=1;
}
void preS()
{
	fun=0;
	for(int i=14;i>=12;i--)fun=fun*10+IR[i];
	rs1=0;
	for(int i=19;i>=15;i--)rs1=rs1*2+IR[i];
	rs2=0;
	for(int i=24;i>=20;i--)rs2=rs2*2+IR[i];
	Imm=0;
	for(int i=31;i>=11;i--)Imm=Imm*2+IR[31];
	for(int i=10;i>=5;i--)Imm=Imm*2+IR[i+20];
	for(int i=4;i>=0;i--)Imm=Imm*2+IR[i+7];
}
void preR()
{
	rd=0;
	for(int i=11;i>=7;i--)rd=rd*2+IR[i];
	fun=0;
	for(int i=14;i>=12;i--)fun=fun*10+IR[i];
	rs1=0;
	for(int i=19;i>=15;i--)rs1=rs1*2+IR[i];
	rs2=0;
	for(int i=24;i>=20;i--)rs2=rs2*2+IR[i];
	fun2=0;
	for(int i=31;i>=25;i--)fun2=fun2*10+IR[i];
}
int judge()
{
	if(Op==110111)return preU(),1;
	if(Op==10111)return preU(),2;
	if(Op==1101111)return preJ(),3;
	if(Op==1100111)return preI(),4;
	if(Op==1100011)return preB(),5;
	if(Op==11)return preI(),6;
	if(Op==100011)return preS(),7;
	if(Op==10011)return preI(),8;
	if(Op==110011)return preR(),9;
}
bool pd()
{
	if(fun==0){
		return B==C;
	}else if(fun==1){
		return B!=C;
	}else if(fun==100){
		return B<C;
	}else if(fun==101){
		return B>=C;
	}else if(fun==110){
		return (unsigned int)B<(unsigned int)C;
	}else if(fun==111){
		return (unsigned int)B>=(unsigned int)C;
	}
}
void doOp6()
{
	LMD=0;
	if(fun==0){
		for(int i=31;i>=24;i--)LMD=LMD*2+Mem[B+Imm]/(1<<7);
		int t=Mem[B+Imm];
		for(int i=1<<7;i>=1;i>>=1)LMD=LMD*2+t/i,t%=i;
	}else if(fun==1){
		for(int i=31;i>=16;i--)LMD=LMD*2+Mem[B+Imm+1]/(1<<7);
		int t=Mem[B+Imm+1];
		for(int i=1<<7;i>=1;i>>=1)LMD=LMD*2+t/i,t%=i;
		t=Mem[B+Imm];
		for(int i=1<<7;i>=1;i>>=1)LMD=LMD*2+t/i,t%=i;
	}else if(fun==10){
		int t=Mem[B+Imm+3];
		for(int i=1<<7;i>=1;i>>=1)LMD=LMD*2+t/i,t%=i;
		t=Mem[B+Imm+2];
		for(int i=1<<7;i>=1;i>>=1)LMD=LMD*2+t/i,t%=i;
		t=Mem[B+Imm+1];
		for(int i=1<<7;i>=1;i>>=1)LMD=LMD*2+t/i,t%=i;
		t=Mem[B+Imm];
		for(int i=1<<7;i>=1;i>>=1)LMD=LMD*2+t/i,t%=i;
	}else if(fun==100){
		int t=Mem[B+Imm];
		for(int i=1<<7;i>=1;i>>=1)LMD=LMD*2+t/i,t%=i;
	}else if(fun==101){
		int t=Mem[B+Imm+1];
		for(int i=1<<7;i>=1;i>>=1)LMD=LMD*2+t/i,t%=i;
		t=Mem[B+Imm];
		for(int i=1<<7;i>=1;i>>=1)LMD=LMD*2+t/i,t%=i;
	}
}
void doOp7()
{
	int t=ALUOut;
	if(fun==0){
		int tmp=(unsigned int)C%(1<<8);
		Mem[t]=tmp;
	}else if(fun==1){
		int tmp=(unsigned int)C%(1<<8);
		Mem[t]=tmp;
		tmp=(unsigned int)C%(1<<16)/(1<<8);
		Mem[t+1]=tmp;
	}else if(fun==10){
		int tmp=(unsigned int)C%(1<<8);
		Mem[t]=tmp;
		tmp=(unsigned int)C%(1<<16)/(1<<8);
		Mem[t+1]=tmp;
		tmp=(unsigned int)C%(1<<24)/(1<<16);
		Mem[t+2]=tmp;
		tmp=(unsigned int)C/(1<<24);
		Mem[t+3]=tmp;
	}
}
void doOp8()
{
	if(fun==0){
		ALUOut=B+Imm;
	}else if(fun==10){
		ALUOut=B<Imm;
	}else if(fun==11){
		ALUOut=(unsigned int)B<(unsigned int)Imm;
	}else if(fun==100){
		ALUOut=B^Imm;
	}else if(fun==110){
		ALUOut=B|Imm;
	}else if(fun==111){
		ALUOut=B&Imm;
	}else if(fun==1){
		ALUOut=B<<Imm;
	}else if(fun==101&&Imm/(1<<6)==0){
		ALUOut=B>>Imm;
	}else if(fun==101&&Imm/(1<<6)!=0){
		int t=Imm%(1<<6);
		ALUOut=B>>Imm;
		if((unsigned int)B&(1<<31)){
			for(int i=0;i<Imm;i++)ALUOut+=(1<<(31-i));
		}
	}
}
void doOp9()
{
	if(fun==0&&fun2==0){
		ALUOut=B+C;
	}else if(fun==0&&fun2!=0){
		ALUOut=B-C;
	}else if(fun==1){
		ALUOut=B<<(C%(1<<5));
	}else if(fun==10){
		ALUOut=B<C;
	}else if(fun==11){
		ALUOut=(unsigned int)B<(unsigned int)C;
	}else if(fun==100){
		ALUOut=B^C;
	}else if(fun==101&&fun2==0){
		ALUOut=B>>((unsigned int)C%(1<<5));
	}else if(fun==101&&fun2!=0){
		ALUOut=B>>((unsigned int)C%(1<<5));
		if((unsigned int)B&(1<<31)){
			for(int i=0;i<(unsigned int)C%(1<<5);i++)ALUOut+=(1<<(31-i));
		}
	}else if(fun==110){
		ALUOut=B|C;
	}else if(fun==111){
		ALUOut=B&C;
	}
}
void IF()
{
	int t=Mem[PC+3],p=24;
	memset(IR,0,sizeof(IR));
	while(t){
		if(t&1)IR[p]=1;
		t>>=1;p++;
	}
	t=Mem[PC+2],p=16;
	while(t){
		if(t&1)IR[p]=1;
		t>>=1;p++;
	}
	t=Mem[PC+1],p=8;
	while(t){
		if(t&1)IR[p]=1;
		t>>=1;p++;
	}
	t=Mem[PC],p=0;
	while(t){
		if(t&1)IR[p]=1;
		t>>=1;p++;
	}
	NPC=PC+4;
	//for(int i=31;i>=0;i--)printf("%d",IR[i]);puts("");
}
void ID()
{
	Op=0;
	for(int i=6;i>=0;i--)Op=Op*10+IR[i];
	Op=judge();
	A=reg[rd];
	B=reg[rs1];
	C=reg[rs2];
//	printf("%d %d %d  %d %d %d %d %d\n",Op,PC,rd,rs1,rs2,B,C,Imm);
}
void EX()
{
	if(Op==1){
		ALUOut=Imm;
	}else if(Op==2){
		ALUOut=PC+Imm;
	}else if(Op==3){
		ALUOut=PC+4;
		NPC=PC+Imm;
	}else if(Op==4){
		ALUOut=PC+4;
		NPC=(B+Imm)&~1;
	}else if(Op==5){
		cond=pd();
		ALUOut=PC+Imm;
	}else if(Op==6){
		//doOp6();
		//printf("#### %d %d\n",Mem[B+Imm],Mem[B+Imm+1]);
	}else if(Op==7){
		ALUOut=B+Imm;
	}else if(Op==8){
		doOp8();
	}else if(Op==9){
		doOp9();
	}
}
void MEM()
{
	PC=NPC;
	if(Op==6){
		doOp6();
	}else if(Op==5){
		if(cond)PC=ALUOut;
	}else if(Op==7){
		doOp7();
	}
}
void WB()
{
	if(Op==1||Op==2||Op==3||Op==4||Op==8||Op==9){
		reg[rd]=ALUOut;
	}else if(Op==6){
		reg[rd]=LMD;
	}
	reg[0]=0;
}
bool isend()
{
	for(int i=0;i<=31;i++)if(IR[i]!=endline[31-i])return 0;
	return 1;
}
int main()
{
//	freopen("array_test1.data","r",stdin);
	while(scanf("%s",s)!=EOF){
		if(s[0]=='@')pos=getpos();
		else Mem[pos]=getnum(),pos++;
	}
	while(1){
		IF();
		if(isend()){
			printf("%u\n",(unsigned int)(reg[10])&255u);
			break;
		}
		ID();
		EX();
		MEM();
		WB();
	//	printf("%u %u\n",(unsigned int)(reg[10]),(unsigned int)(reg[11]));
	//	printf("%u %u\n",(unsigned int)(Mem[4480]),(unsigned int)(Mem[4481]));
	}
}
