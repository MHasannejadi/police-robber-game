#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <windows.h>

int p[100][100]; //global array for positions with first initial 0
char visual[500][500];  //global array for show game
int m,n,tc; //global dimensions and office numbers
int C[100]; //global number of police in each office
int xp[50][50],yp[50][50];//first[]=number of office , second[]=number of police in each office
int xt[50],yt[50];// []=number of thief
int np; //number of police
int nt; //number of thieves
void fscan(void)
{
    printf("please enter dimensions\n");
    scanf("%d%d",&m,&n);
    printf("please enter offices number\n");
    scanf("%d",&tc);//tedad calantari
    printf("please enter police number for each office\n");
    int i,j;
    for (i = 0; i < tc; i++)
    {
        int tp; //tedad police
        scanf("%d",&tp);
        C[i]=tp; //tedad police dar har calantari
        np+=tp;
    }
    printf("please enter  number of robbers\n");
    scanf("%d",&nt);
}
void SetColor(int ForgC)
{
    WORD wColor;

    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    //We use csbi for the wAttributes word.
    if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
    {
        //Mask out all but the background attribute, and add in the forgournd     color
        wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
        SetConsoleTextAttribute(hStdOut, wColor);
    }
    return;
}
void textcolor (int color)
{
    static int __BACKGROUND;

    HANDLE h = GetStdHandle ( STD_OUTPUT_HANDLE );
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;


    GetConsoleScreenBufferInfo(h, &csbiInfo);

    SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE),
                             color + (__BACKGROUND << 2));
}
int checkend(void)
{
    int i,j;
    for(i=0; i<n; i++)
    {
        for(j=0; j<m; j++)
        {
            if(p[j][i]>2)  //sum in consiously_move
            {
                return 1;
            }
        }
    }
    return 0;
}
int agir(int i)        //warning a office for thief in police area
{
    int j,t;
    for(j=0; j<C[i]; j++)
    {
        for(t=0; t<nt; t++)
        {
            if(fabs(xt[t]-xp[i][j])<2&&fabs(yt[t]-yp[i][j])<2)    //adjacent points have first priority
            {
                return 1;
            }
            if((fabs(xt[t]-xp[i][j])==2&&fabs(yt[t]-yp[i][j])<=2)||(fabs(xt[t]-xp[i][j])<=2&&fabs(yt[t]-yp[i][j])==2))    //2nd priority for margin points of police area
            {
                return 2;
            }
        }

    }
    return 0;
}
int spy_thief(int j)  //ok
{
    int i,t;
    for(i=yt[j]-2; i<=yt[j]+2; i++)
    {
        for(t=xt[j]-2; t<=xt[j]+2; t++)
        {
            if((i>=0)&&(t>=0)&&(i<n)&&(t<m)){
            if(p[t][i]==1)
            {
                return 1;
            }
          }
        }
    }
    return 0;
}
void rat(int *x,int *y)    //random allocate thief       --> ok
{
    int tx,ty;
    while(1)
    {
        tx=rand()%m;
        ty=rand()%n;
        if((p[tx][ty]==0))
        {
            break;
        }
    }
    p[tx][ty]=2;
    *x=tx;
    *y=ty;
}
void nat(int *x,int *y)   //next allocate thief
{
    int tx=*x,ty=*y;
    int stx=tx,sty=ty;
    p[tx][ty]=0;

    while(1)
    {

        tx=stx+(rand()%3)-1;
        ty=sty+(rand()%3)-1;
        if((tx<m)&&(tx>0)&&(ty<n)&&(ty>0)&&(p[tx][ty]==0))
        {
            break;
        }

    }
    *x=tx;
    *y=ty;
    p[tx][ty]=2;

}
void consiously_move_thief(int *x,int *y)
{
    int i,j;
    int smovex=0,smovey=0,tx=*x,ty=*y;
    p[tx][ty]=0;
    for(i=ty-2; i<=ty+2; i++)
    {
        for(j=tx-2; j<=tx+2; j++)
        {
            if(p[j][i]==1)
            {
                if(tx-j==2)
                {
                    smovex+=1;
                }
                else if(tx-j==-2)
                {
                    smovex-=1;
                }
                else if(tx-j==1)
                {
                    smovex+=1;
                }
                else if(tx-j==-1)
                {
                    smovex-=1;
                }
                else{}
                if(ty-i==2)
                {
                    smovey+=1;
                }
                else if(ty-i==-2)
                {
                    smovey-=1;
                }
                else if(ty-i==1)
                {
                    smovey+=1;
                }
                else if(ty-i==-1)
                {
                    smovey-=1;
                }
                else{}
            }
        }
    }
    if(smovex>0&&tx<m-1)
    {
        *x+=1;
    }
    if(smovex<0&&tx>0)
    {
        *x-=1;
    }
    if(smovey>0&&ty<n-1)
    {
        *y+=1;
    }
    if(smovey<0&&ty>0)
    {
        *y-=1;
    }
    p[*x][*y]+=2;
}
void tm(void)       //thief motion
{
    int i,j;
    static int f=0;
    if(f==0)
    {
        f++;
        for(i=0; i<nt; i++)
        {
            rat(&xt[i],&yt[i]);
        }
    }
    if(f>0)
    {
        for(j=0; j<nt; j++)
        {
            if(spy_thief(j)==1)
            {
                consiously_move_thief(&xt[j],&yt[j]);
            }
            else
            {
                nat(&xt[j],&yt[j]);
            }
        }
    }
}
void rap(int *x,int *y)   //random allocate police        --> ok
{
    int px,py;
    px=rand()%m;
    py=rand()%n;
    while((p[px][py]==1)||(p[px][py]==2))
    {
        px=rand()%m;
        py=rand()%n;
    }
    p[px][py]=1;
    *x=px;
    *y=py;
}
void nap(int *x,int *y)    // next allocate police random
{
    int px=*x,py=*y;
    int spx=px,spy=py;
    p[px][py]=0;
    px=spx+(rand()%3)-1;
    py=spy+(rand()%3)-1;
    while(1)
    {
        if((px<m)&&(px>0)&&(py<n)&&(py>0)&&(p[px][py]!=1))
        {
            break;
        }
        px=spx+(rand()%3)-1;
        py=spy+(rand()%3)-1;

    }
    *x=px;
    *y=py;
    p[px][py]=1;

}
void consiously_move_police1(int *x,int *y)
{
    int i,j,px=*x,py=*y;
    p[px][py]=0;
    for(i=0; i<nt; i++)
    {
        if(fabs(xt[i]-px)<2&&fabs(yt[i]-py)<2&&p[xt[i]][yt[i]]!=3)
        {
            p[*x][*y]=0;
            *x=xt[i];
            *y=yt[i];
            break;
        }
    }
    p[*x][*y]+=1;

}
void consiously_move_police2(int *x,int *y)
{
    int i,j,px=*x,py=*y;
    for(i=0; i<nt; i++)
    {
        if((fabs(xt[i]-px)==2||fabs(yt[i]-py)==2)&&p[px+((xt[i]-px)/2)][py+((yt[i]-py)/2)]!=1)
        {
            p[px][py]=0;
            *x=px+((xt[i]-px)/2);
            *y=py+((yt[i]-py)/2);
            break;
        }
    }
    p[*x][*y]+=1;
}
void pm(void)     //police motion
{
    int i,j; //xp[tc][100],yp[tc][100];
    static int f=0;
    if(f==0)   //first allocate police
    {
        for(i=0; i<tc; i++)
        {
            for(j=0; j<C[i]; j++)
            {
                rap(&xp[i][j],&yp[i][j]);
            }
        }
        f++;
    }
    else   //next allocate police
    {
        for(i=0; i<tc; i++)
        {
            for(j=0; j<C[i]; j++)
            {
                if(agir(i)==1)
                {
                    consiously_move_police1(&xp[i][j],&yp[i][j]);
                }
                else if(agir(i)==2)
                {

                    consiously_move_police2(&xp[i][j],&yp[i][j]);
                }
                else
                {
                    nap(&xp[i][j],&yp[i][j]);
                }

            }
        }
    }
}
void jadval(void)
{

    int i,j;
    for(i=0; i<3*n+1; i++)
    {
        for(j=0; j<5*m+1; j++)
        {
            if((i%3==0)&&((j%5)!=0))
            {
                visual[j][i]='_';

            }

            else if((i!=0)&&(j%5==0))
            {
                visual[j][i]='|';

            }
            else
            {
                visual[j][i]=' ';

            }

        }
    }
}
void add_player(void)
{
    int i,j,c;
    for(c=0; c<nt; c++)
    {
        char l=c+49;
        visual[xt[c]*5+2][yt[c]*3+2]='T';
        visual[xt[c]*5+3][yt[c]*3+2]=l;
    }

    for(i=0; i<tc; i++)
    {
        for(j=0; j<C[i]; j++)
        {
            char k=(i+49);
            visual[xp[i][j]*5+2][yp[i][j]*3+2]='D';
            visual[xp[i][j]*5+3][yp[i][j]*3+2]=k;

        }
    }

}
void show(void)
{
    int i,j;
    for(i=0; i<3*n+1; i++)
    {
        for(j=0; j<5*m+1; j++)
        {
            printf("%c",visual[j][i]);
        }
        printf("\n");
    }
    printf("\n");
    printf("\n");
    //printf("\n");
}
int main()
{
    srand(time(NULL));
    int t=0,c=0;
    int i,j=1;
    SetColor(10);
    fscan();
    if(np+nt>m*n)   //ok
    {
        printf("warning:number of police is more than map positions\nplease try again with less police");
        return 0;
    }
    while(1){

        tm();
        pm();
        if(checkend()==1)
        {
            jadval();
            add_player();
            SetColor(13);
            show();
            printf("intelligent police :)\nclever police capture the robber");
             //printf("time=%d\nxthief=%d\nythief=%d\nxpol=%d\nypol=%d\n",t,xt[0],yt[0],xp[0][0],yp[0][0]);
            return 0;
        }


        jadval();
        add_player();

        for(i=0;i<tc;i++)
        {
            if(agir(i)==1)
            {
                if(c%2==0){
                textcolor(12);
                c++;
                break;
            }
            else{
                textcolor(9);
                c++;
                break;
            }
            }
            else
            {
                textcolor(10);

            }
        }

        show();
        printf("time=%d\n",t);

        t++;
        //j++;
        //getch();
        Sleep(500);
        system("cls");

    }



    return 0;


}
// jump police
// going out robbers
//mistake in red time for agir function
// dont call other police from a office




