#include<stdio.h>
struct node{
    unsigned dist[20];
    unsigned from[20];

}rt[10];
void main()
{
    int costmat[20][20];
    int nodes,i,j,k,count=0;
    printf("\n enter the no of nodes:");
    scanf("%d",&nodes);
    printf("enter the cost matrix:-1 for infinite cost\n");
    for(i=0;i<nodes;i++){
        for(j=0;j<nodes;j++){
            scanf("%d",&costmat[i][j]);
            costmat[i][i]=0;
            rt[i].dist[j]=costmat[i][j];
            rt[i].from[j]=j;
        }
    }
    do{
        count=0;
        for(i=0;i<nodes;i++){
            for(j=0;j<nodes;j++){
                for(k=0;k<nodes;k++){
                    if(rt[i].dist[j]>costmat[i][k]+rt[k].dist[j]){
                        rt[i].dist[j]=rt[i].dist[k]+rt[k].dist[j];
                        rt[i].from[j]=k;
                        count++;
                    }
                }
            }

        }
    }while(count!=0);
    for(i=0;i<nodes;i++){
        printf("\nrouting table for router %d \n",i+1);
        for(j=0;j<nodes;j++){
            printf("\t\nnode%d via %d distance:%d",j+1,rt[i].from[j]+1,rt[i].dist[j]);
        }
    }
    printf("\n\n");
    
}