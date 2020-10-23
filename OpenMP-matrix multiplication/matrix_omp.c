#include<stdio.h>
#include<omp.h>
#include<sys/time.h>
int main()
{
int x,y,i,j,a[20][20],b[20][20],res[20][20],k;
printf("Enter the number of rows ");
scanf("%d",&x);
printf("Enter the number of coloumns ");
scanf("%d",&y);
printf("Enter the first matrix elements\n");
for(i=0;i<x;i++)
{
for(j=0;j<y;j++)
{
scanf("%d",&a[i][j]);
}
}
printf("Enter the second matrix elements\n");
for(i=0;i<x;i++)
{
for(j=0;j<y;j++)
{
scanf("%d",&b[i][j]);
}
}
struct timeval t1,t2;
gettimeofday(&t1,0);
#pragma omp parallel for
for(i=0;i<x;i++)
{
for(j=0;j<y;j++)
{
res[i][j]=0;
for(k=0;k<y;k++)
{
res[i][j]=res[i][j]+(a[i][k]*b[k][j]);
}
}
}
gettimeofday(&t2,0);
double diff = (t2.tv_sec-t1.tv_sec)*1.0f+(t2.tv_usec-t1.tv_usec)/1000000.0f;
printf("The time required to perform the calculation is %lf\n",diff);

printf("The result is\n");
for(i=0;i<x;i++)
{
for(j=0;j<y;j++)
{
printf("%d\t",res[i][j]);
}
printf("\n");
}
}
