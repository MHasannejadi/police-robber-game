#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <windows.h>
#include <conio.h>
int tx,ty; //position of thief
int p[100][100]; //global array for positions with first initial 0
char visual[500][500];  //global array for show game
int m,n,tc; //global dimensions and office numbers
int C[100]; //global number of police in each office
int xp[50][50],yp[50][50];//first[]=number of office , second[]=number of police in each office
int np; //number of police
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
int checkend()
{
    int i,j;
    for(i=0; i<m; i++)
    {
        for(j=0; j<n; j++)
        {
            if(p[i][j]>2)  //sum in consiously_move
            {
                return 1;
            }
        }
    }
    return 0;
}
int agir(int i)        //warning the a office for thief in police area
{
    int j;
    for(j=0; j<C[i]; j++)
    {
        if((fabs(xp[i][j]-tx)<3)&&(fabs(yp[i][j]-ty)<3))
        {
            return 1;
        }
    }
    return 0;
}
void tm(void)       //thief motion
{
    static int f=0;
//    static int tx,savtx;
//    static int ty,savty;

    if(f==0)  //first allocate thief
    {
        tx=rand()%m;
        ty=rand()%n;
        p[tx][ty]=2;
        f++;
    }
    else   //allocate thief after first time
    {
        int stx=tx,sty=ty;
        p[tx][ty]=0;
        tx=stx+(rand()%3-1);
        ty=sty+(rand()%3-1);
        while(!(tx<m&&tx>=0&&ty<n&&ty>=0))
        {
            tx=stx+(rand()%3-1);
            ty=sty+(rand()%3-1);
        }
        p[tx][ty]+=2;
    }
}
void rap(int *x,int *y)   //random allocate police
{
    static int px,py;
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
void consiously_move(int *x,int *y)
{
    int px=*x,py=*y;
    if((px==tx)&&(ty>py))
    {
        if(p[px][py+1]!=1)
        {
            *y=py+1;

        }
        else if(p[px+1][py+1]!=1)
        {
            *x=px+1;
            *y=py+1;

        }
        else if(p[px-1][py+1]!=1)
        {
            *y=py+1;
            *x=px-1;

        }

    }
    else if((px==tx)&&(ty<py))
    {
        if(p[px][py-1]!=1)
        {
            *y=py-1;

        }
        else if(p[px+1][py-1]!=1)
        {
            *x=px+1;
            *y=py-1;

        }
        else if(p[px-1][py-1]!=1)
        {
            *y=py-1;
            *x=px-1;

        }
    }
    else if((py==ty)&&(tx<px))
    {
        if(p[px-1][py]!=1)
        {
            *x=px-1;

        }
        else if(p[px-1][py-1]!=1)
        {
            *x=px-1;
            *y=py-1;

        }
        else if(p[px-1][py+1]!=1)
        {
            *y=py+1;
            *x=px-1;

        }

    }
    else if((py==ty)&&(tx>px))
    {
        if(p[px+1][py]!=1)
        {
            *x=px+1;

        }
        else if(p[px+1][py-1]!=1)
        {
            *x=px+1;
            *y=py-1;

        }
        else if(p[px+1][py+1]!=1)
        {
            *y=py+1;
            *x=px-1;

        }

    }
    else if((ty<py)&&(tx<px))
    {
        if(p[px-1][py-1]!=1)
        {
            *x=px-1;
            *y=py-1;
        }
        else if(p[px][py-1]!=1)
        {

            *y=py-1;
        }
        else if(p[px-1][py]!=1)
        {

            *x=px-1;

        }

    }
    else if((ty>py)&&(tx>px))
    {
        if(p[px+1][py+1]!=1)
        {
            *x=px+1;
            *y=py+1;
        }
        else if(p[px][py+1]!=1)
        {

            *y=py+1;
        }
        else if(p[px+1][py]!=1)
        {

            *x=px+1;

        }

    }
    else if((ty<py)&&(tx>px))
    {
        if(p[px+1][py-1]!=1)
        {
            *x=px+1;
            *y=py-1;
        }
        else if(p[px][py-1]!=1)
        {

            *y=py-1;
        }
        else if(p[px+1][py]!=1)
        {

            *x=px+1;

        }

    }
    else if((ty>py)&&(tx<px))
    {
        if(p[px-1][py+1]!=1)
        {
            *x=px-1;
            *y=py+1;
        }
        else if(p[px][py+1]!=1)
        {

            *y=py+1;
        }
        else if(p[px-1][py]!=1)
        {

            *x=px-1;

        }

    }
    p[*x][*y]+=1;
}
void pm()     //police motion
{
    int i,j; //xp[tc][100],yp[tc][100];
    static int f;
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
                    consiously_move(&xp[i][j],&yp[i][j]);
                }
                else
                {
                    nap(&xp[i][j],&yp[i][j]);
                }

            }
        }
    }
}
void pjadval(void)
{

    int i,j,t;
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
    int i,j;
    visual[tx*5+2][ty*3+2]='T';

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
    int c=0;
    srand(time(NULL));
    int t=0;
    int i,j=1;
    SetColor(10);
    fscan();
    if(np+1>m*n)
    {
        printf("warning:number of police is more than map positions\nplease try again with less police");
        return 0;
    }
    while(1)
    {
        tm();
        if(checkend()==1)
        {
            pjadval();
            add_player();
            SetColor(13);
            show();
            printf("fool robbber :(\nthe robber went to previous position of police in X=%d  Y=%d",tx+1,ty+1);
            return 0;
        }
        pm();
        if(checkend()==1)
        {
            pjadval();
            add_player();
            SetColor(13);
            show();
            printf("intelligent police :)\nclever police capture the fool robber in X=%d  Y=%d",tx+1,ty+1);
            return 0;
        }
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


        pjadval();
        add_player();
        //system("cls");



        show();
        printf("time=%d\n",t);
        t++;
        j++;
        // getch();
        Sleep(500);
        system("cls");
    }



    return 0;


}

