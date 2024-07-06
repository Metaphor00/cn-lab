#include<stdio.h>
void main()
{
    int in,out,bsize,n,bucket=0;
    printf("enter the bucket size:\n");
    scanf("%d",&bsize);

    printf("enter the no of inputs:\n");
    scanf("%d",&n);

    printf("enter the packet outgoing rate:\n");
    scanf("%d",&out);

    while(n!=0)
    {
        printf("enter the incoming packet size:\n");
        scanf("%d",&in);

        if(in<=(bsize-bucket))
        {
            bucket+=in;
            printf("Bucket status:%d out of %d\n",bucket,bsize);
        }
        else
        {
            printf("dropped packet:%d\n",in-(bsize-bucket));
            bucket=bsize;
            printf("Bucket status:%d out of %d\n",bucket,bsize);
        }
        bucket=bucket-out;
        printf("After outgoing,Bucket status:%d out of %d\n",bucket,bsize);
        n--;
    }
}