#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <ctype.h>
#include <math.h>
#include "string.h"
#include "iostream.h"

#define gx getmaxx()/2
#define gy getmaxy()/2
#define RAD 3.14159265358979/18

void sod(int al)//tao sound khi quay
{
sound(200*al);
delay(2);
nosound();
}
void volume()//can dieu chinh am luong
{
setcolor(WHITE);
line(2*gx-10,2*gy-70,2*gx-10,2*gy-90);
line(2*gx-110,2*gy-70,2*gx-10,2*gy-90);
line(2*gx-110,2*gy-70,2*gx-10,2*gy-70);
line(2*gx-90,2*gy-70,2*gx-90,2*gy-74);
line(2*gx-70,2*gy-70,2*gx-70,2*gy-78);
line(2*gx-50,2*gy-70,2*gx-50,2*gy-82);
line(2*gx-30,2*gy-70,2*gx-30,2*gy-86);
outtextxy(2*gx-110,2*gy-60,"- Amluong +");
}
void change_vol(int al)//change am luong
{
setfillstyle(1,RED);
for(int i=0;i<al;i++)
floodfill(2*gx-92+20*i,2*gy-72,WHITE);
setfillstyle(1,BLACK);
for(;i<5;i++)
floodfill(2*gx-92+20*i,2*gy-72,WHITE);
}
void clearn_al()
{
setfillstyle(1,BLACK);
bar(2*gx-111,2*gy-51,2*gx-9,2*gy-91);
}
void msg_hd(int mau)// huong dan
{
setcolor(mau);
outtextxy(gx-15,2*gy-40,"Bam ENTER: Quay Non Bam ESC: Thoat");
}
void msg_theend(int mau)//finish
{
setcolor(mau);
outtextxy(gx-15,2*gy-40,"Bam ENTER: CHOI LAI Bam ESC: THOAT");
outtextxy(2*gx-230,2*gy-123,"SAU 3 VONG THI");
}
void msg_hd1(int mau)//continue khi quay xong
{
setcolor(mau);
outtextxy(gx-20,2*gy-40," Bam phim bat ky de : TIEP TUC");
}
void msg_nchu(int mau)//bao nhap chu
{
setcolor(mau);
outtextxy(gx+110,2*gy-40,"NHAP CHU CAI");
}
void msg_o()//khung vien bao nhan duoc o gi,lan doan sai,diem , tong so o chu
{
setcolor(WHITE);
outtextxy(2*gx-95,2*gy-210,"QUAY VAO O");
setfillstyle(1,12);
rectangle(2*gx-100,2*gy-200,2*gx-5,2*gy-170);//msg khi quay xong
bar(2*gx-98,2*gy-198,2*gx-7,2*gy-172);
setcolor(MAGENTA);
rectangle(2*gx-100,2*gy-140,2*gx-5,2*gy-110);//msg diem
outtextxy(2*gx-100,2*gy-150,"DIEM CUA BAN");
setcolor(LIGHTRED);
rectangle(2*gx-100,2*gy-300,2*gx-5,2*gy-260);//msg LAN DOAN SAI
outtextxy(2*gx-98,2*gy-320,"SO LAN DUOC");
outtextxy(2*gx-100,2*gy-310," DOAN SAI");
setcolor(WHITE);
rectangle(2*gx-100,2*gy-330,2*gx-5,2*gy-360);//msg total o chu
outtextxy(2*gx-100,2*gy-370," TONG O CHU");
setcolor(YELLOW);
}
void DONG_CHU(int i)//ve khung hang o chu
{
setcolor(WHITE);
setfillstyle(1,LIGHTRED);
for(int j=0;j<i;j++)
{
rectangle(gx-10*i+20*j,80,gx-10*i+20*(j+1),60);
rectangle(gx-10*i-1+20*j,81,gx-10*i-1+20*(j+1),59);
floodfill(gx-10*i+2+20*j,79,WHITE);
}
}
void ve_cung(int a,int i,int mau)//ve cung chuyen doi
{
setfillstyle(1,mau);
pieslice(gx,gy,a+i,a+30+i,80);
}
void mui_ten()
{
line(gx-41*sqrt(2)-1,gy+41*sqrt(2),gx-50*sqrt(2)-1,gy+50*sqrt(2));
line(gx-41*sqrt(2),gy+41*sqrt(2),gx-50*sqrt(2),gy+50*sqrt(2));
line(gx-41*sqrt(2),gy+41*sqrt(2)+1,gx-50*sqrt(2),gy+50*sqrt(2)+1);
line(gx-41*sqrt(2),gy+41*sqrt(2),gx-41*sqrt(2)-9,gy+41*sqrt(2));
line(gx-41*sqrt(2)-3,gy+41*sqrt(2)+1,gx-41*sqrt(2)-9,gy+41*sqrt(2)+1);
line(gx-41*sqrt(2),gy+41*sqrt(2),gx-41*sqrt(2),gy+41*sqrt(2)+9);
line(gx-41*sqrt(2)-1,gy+41*sqrt(2),gx-41*sqrt(2)-1,gy+41*sqrt(2)+9);
}
void non_chay(int i) //ve cung
{
ve_cung(0,i,1);
ve_cung(30,i,2);
ve_cung(60,i,3);
ve_cung(90,i,4);
ve_cung(120,i,5);
ve_cung(150,i,6);
ve_cung(180,i,7);
ve_cung(210,i,8);
ve_cung(240,i,9);
ve_cung(270,i,10);
ve_cung(300,i,11);
ve_cung(330,i,12);
}
void msg_kq(char ch[])//thong bao ket qua
{
outtextxy(2*gx-83,2*gy-190,ch);
}
void clearn_msgkq()
{
setfillstyle(1,12);//xoa truoc khi hien (msg quay vao o nao do)
bar(2*gx-98,2*gy-198,2*gx-7,2*gy-172);
}
int bao_o(int i)
{
switch(i)
{
case 12:msg_kq(" 100") ;return(100);break;
case 1 :msg_kq(" 200") ;return(200);break;
case 2 :msg_kq(" 300") ;return(300);break;
case 11:msg_kq(" 400") ;return(400);break;
case 3 :msg_kq(" 500") ;return(500);break;
case 10:msg_kq(" 600") ;return(600);break;
case 4 :msg_kq(" 700") ;return(700);break;
case 9 :msg_kq(" 800") ;return(800);break;
case 5 :msg_kq(" 900") ;return(900);break;
case 8 :msg_kq("MAT DIEM") ;return(0) ;break;
case 7 :msg_kq("NHAN DOI") ;return(0) ;break;
case 6 :msg_kq("CHIA DOI") ;return(0) ;break;
}
}
void HIEN_VAN_BAN()//huong dan cach choi
{
int i=1,j=0;
char vanban[76],tg[]=" ";
FILE *doc;
setcolor(WHITE);
doc=fopen("C:\\NON_K_D\\default\\huongdan.nt","r") ;
while(fgets(vanban,75,doc)!=NULL)
{
while(j<strlen(vanban)-1)
{
tg[0]=vanban[j];
outtextxy(8+8*j,15*i,tg);
j++;
}
j=0;
i++;
}
fclose(doc);
}
void BAO_DIEM(int DIEM)
{
gotoxy(69,23);
cout<<" ";//xoa diem
gotoxy(73,23);
cout<<DIEM;//bao diem
}
int ve_chiec_non_quay(void *tranh,char oc1[],int DS,int &al)
{
int i=0,j,DL,DIEM=0,HT,DDung=0,str,o_trung;
char ch,nphim,tg[]=" ";
mui_ten();
non_chay(i);
str=strlen(oc1);
DONG_CHU(str-1);
BAO_DIEM(DIEM);//bao diem la 0 khi bat dau choi
gotoxy(74,9);
cout<<str-1;
randomize();
do{
nt:int LS_1_0=0;//tao label khi quay vao o MAT DIEM
o_trung=0;//so o trung nhau =0
DL=random(12);
j=random(36);
gotoxy(74,13);
cout<<DS;//lan duoc doan sai
BAO_DIEM(DIEM);//bao diem khi bat dau choi hoac khi quay vao o mat diem
volume();//dieu chinh am luong
change_vol(al);//change can volume
while(!kbhit())
{
delay(100);
msg_hd(RED);
delay(100);
setcolor(RED);
rectangle(2*gx-100,2*gy-300,2*gx-5,2*gy-260);//frame msg LAN DOAN SAI
delay(100);
msg_hd(YELLOW);
delay(100);
setcolor(CYAN);
rectangle(2*gx-100,2*gy-300,2*gx-5,2*gy-260);//frame msg LAN DOAN SAI
}
ch= getch();
if(ch=='+'&&al<5 ) al++;
if(ch=='-'&&al>0 ) al--;
if(ch==27) exit(1);//thoat neu nhan ESC
if(ch==13) //chi quay non khi bam ENTER
{
clearn_al();//xoa khu vuc am luong
msg_hd(BLACK);//giau msg hd khi quay o
while(DL+j<150)//cham dan deu
{
setcolor(14);//vien mau vang
non_chay(i);
delay(DL+j);
i+=30;j++; if(i==360)i=0;
sod(al);
}
bao_o(getpixel(gx+60,gy-3));//msg quay vao o...
if(getpixel(gx+60,gy-3)==8)//o mat diem
{
delay(1000);
DIEM=0;
clearn_msgkq();//clearn kq truoc khi hien new (khi quay vao o nao do)
goto nt;
}
while(!kbhit())
{
delay(500);
msg_nchu(CYAN);//bao nhap chu
setcolor(BLACK);
rectangle(2*gx-100,2*gy-200,2*gx-5,2*gy-170);//msg khi quay xong
delay(500);
msg_nchu(BLACK);//bao nhap chu
setcolor(WHITE);
rectangle(2*gx-100,2*gy-200,2*gx-5,2*gy-170);//msg khi quay xong
}
ch=getch();
msg_nchu(BLACK);//giau msg nhap chu khi quay
HT=toascii(ch);
if(96<HT&&HT<123) HT-=32;
for(int i=0;i<str-1;i++)
{
if(HT==toascii(oc1[i]))
{
putimage (gx-10*(str-1)+20*i,60,tranh,XOR_PUT);
setcolor(YELLOW);
tg[0]=oc1[i];
outtextxy(gx-10*(str-1)+7+20*i,66,tg);
oc1[i]='\0';
LS_1_0=1;//true or false
DDung++;
o_trung++;//neu nhu tu 2 o tro len
}
} //////////////
if(getpixel(gx+60,gy-3)==7&&o_trung) DIEM*=2; ///X2 or /2///
else if(getpixel(gx+60,gy-3)==6&&o_trung) DIEM/=2; ////o khac////
else DIEM+=o_trung*bao_o(getpixel(gx+60,gy-3)); //////////////
if(LS_1_0==0) DS--;//doan sai thi -
}//the end of prees ENTER
clearn_msgkq();//clearn kq truoc khi hien new
BAO_DIEM(DIEM);
delay(500);
}while(DS!=0&&DDung<str-1||ch=='-'||ch=='+');
setcolor(YELLOW);
outtextxy(gx-10*str-80,66,"O CHU LA");
if(DS==0)
{
BAO_DIEM(0);
gotoxy(74,13);
cout<<DS;//lan duoc doan sai
for(i=0;i<str-1;i++)
{
if(toascii(oc1[i])!=0)
{
tg[0]=oc1[i];
putimage (gx-10*(str-1)+20*i,60,tranh,XOR_PUT);
outtextxy(gx-10*(str-1)+7+20*i,66,tg);
}
}
}
while(!kbhit())//bao continue
{
msg_hd1(RED);
delay(6);
msg_hd1(WHITE);
}
msg_hd1(BLACK);
outtextxy(gx-10*str-80,66,"O CHU LA");
getch();// please while ...
return(DIEM);
}
void NEN_VAN_BAN()
{
setcolor(RED);
setfillstyle(9,BLUE);
rectangle(0,0,2*gx,2*gy);
floodfill(1,1,RED);
setcolor(WHITE);
}
void LAP_CAU_HOI()
{
outtextxy(gx,2*gy-20," PHAN NHAP CAU HOI _ CHU Y HUONG DAN");
FILE *ghi;
char vanban[80];
char *h_tl[]={"?1:","?2:","?3:","da1:","da2:","da3:","het:"} ;
ghi=fopen("C:\\NON_K_D\\cauhoi\\cauhoi.nt","w");
int i=1;
gotoxy(5,5+i);
outtextxy(1,70+i*16,h_tl[i-1]);
while(strlen(gets(vanban))>0)
{
i++;
outtextxy(1,70+i*16,h_tl[i-1]);
gotoxy(5,5+i);
fputs(vanban,ghi);
fputs("\n",ghi);
}
fclose(ghi);
}
main()
{
NT: clrscr();
int mh=DETECT , k , a,b,lanchoi=1,al=5,DIEM=0;
void *tranh;
initgraph(&mh,&k,"c:\\tc\\bgi");
NEN_VAN_BAN();
HIEN_VAN_BAN();//msg gioi thieu cach choi
char cho=NULL;
while(cho!='1'&&cho!='2')//cho kich hoat phim 1 or 2
cho= getch();
if(cho=='1') {cleardevice();NEN_VAN_BAN(); LAP_CAU_HOI();}
cleardevice();//xoa ...
NEN_VAN_BAN();//tao nen xanh va kieu to cheo
int LS=0;
while(LS<1||LS>9){
gotoxy(10,15);
cout<<"BAN HAY NHAP 0 < SO LAN DUOC DOAN SAI TOI DA < 10 LA: ";
cin>>LS;
}
setfillstyle(1,WHITE);
bar(0,0,19,20);
tranh=malloc(imagesize(0,0,19,20));
getimage(0,0,19,20,tranh);
do{
cleardevice();
FILE *doc;
int T=0;
char cau_hoi[80];
char tra_loi[80];
if(cho=='2') doc=fopen("C:\\NON_K_D\\default\\cauhoi.nt","r");
else doc=fopen("C:\\NON_K_D\\cauhoi\\CAUHOI.NT","r");
while(T<lanchoi &&fgets(cau_hoi,79,doc)!=NULL) T++;
while(T<lanchoi+3&&fgets(tra_loi,79,doc)!=NULL) T++;
setcolor(RED);
rectangle(0,0,2*gx,2*gy);//vien bao
line(0,25,2*gx,25);//msg cau hoi
floodfill(1,2,RED);
setfillstyle(9,BLUE);
floodfill(1,2,RED);
setcolor(WHITE);
char tg[]=" ";//trung gian
int j=0;
while(j<strlen(cau_hoi)-1)
{
tg[0]=cau_hoi[j];
outtextxy(gx-(strlen(cau_hoi)/2-j)*8,10,tg);
j++;
}
setcolor(RED);
rectangle(gx-30,2*gy-50,2*gx,2*gy-25);//msg huong dan
rectangle(0,2*gy-50,2*gx,2*gy-50);//introduction
setfillstyle(9,RED);
floodfill(1,2*gy-49,RED);
setcolor(WHITE);
outtextxy(10,2*gy-40," CHIEC NON KY DIEU");
outtextxy(10,2*gy-20,"DESIGN BY NHUAN _ K46 CNTT");
msg_o();
DIEM+= ve_chiec_non_quay(tranh,tra_loi,LS,al);
lanchoi++;
}while(lanchoi!=4);
BAO_DIEM(DIEM);
while(!kbhit())
{
msg_theend(RED);
delay(10);
msg_theend(CYAN);
}
char kt;//dieu kien ket thuc hoac quay lai
while(kt!=27)
{
kt=getch();
if(kt==13) goto NT;
}
closegraph();
}