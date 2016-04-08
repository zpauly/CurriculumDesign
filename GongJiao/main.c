#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define   m   15
#define   n   50
#define   infinity       99999

typedef struct node
{
   float distence;
   int  vexnode;
   struct node *next;

}Arcnode;

typedef struct vertexnode
{
	char name[30];
	char information[100];
	Arcnode* head;
} Line;

typedef struct  adjlist
{
	Line point[m];
	int  arcnum;
	int  vexnum;
} graph;

typedef struct queue
{
     int father;
     int son;
     struct queue *next;
}queue;

typedef struct  mst_point
{
    int father;
    int son;
    struct mst_point *next;
}mst_point;

void   init_matrix(float (*matrix) [m])
{
    for (int i = 0; i < m; ++i)
        for (int j = 0; j <m ; ++j)
             matrix[i][j]=infinity;
}


void  init_graph(graph *g)
{
    for (int i = 0; i < m; ++i)
    {
        memset(g->point[i].name,0,30*sizeof(char));
        memset(g->point[i].information,0,100*sizeof(char));
    }

    for (int i = 0; i < m; ++i)
    {
        g->point[i].name[29]='\n';
        g->point[i].information[29]='\n';
    }
}

void test(graph *g,float (*matrix)[m])
{
	Arcnode *temp;

    printf("the matrix is as follows:\n");
    printf("%d\n",g->vexnum );

    for (int i = 0; i < g->vexnum; ++i)
    {
        for (int j = 0; j < g->vexnum; ++j)
        {
            printf("%6.1f  ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    printf("%d\n", g->vexnum);
    for (int i = 0; i < g->vexnum; ++i)
    {

        temp=g->point[i].head->next;
        printf("point connected to '%s':",g->point[i].name);
        while(temp)
        {
            printf(" (%s,distence:%.1f) ", g->point[temp->vexnode].name,temp->distence);
            temp=temp->next;
        }
        printf("\n");
        printf("information:%s", g->point[i].information);
        printf("\n\n\n");
    }
}

queue * init_queue()
{
    queue* head=(queue *)malloc(sizeof(queue));
    head->next=NULL;
    return head;

}

int  empty(queue *head)
{
    if(head->next)     return 0;
    else               return -1;
}

void push(queue *head,int i,int j)
{
     queue *temp=(queue *)malloc(sizeof(queue));
     if(temp)
     {
         temp->father=i;
         temp->son=j;
         temp->next=head->next;
         head->next=temp;
     }
}

void pop(queue *head,int* i,int* j)
{
    queue * temp;
    if(!empty(head))
    {
        temp=head->next;
        (*i)=temp->father;
        (*j)=temp->son;

        head->next=temp->next;
        free(temp);
    }
}

void print_queue(queue *head)
{
    queue *temp;
    temp=head->next;
    printf("the queue :\n");
    while(temp)
    {
        printf("father:%d ,son:%d ", temp->father,temp->son);
        temp=temp->next;
    }
}

void  print_rout(graph *g,queue *head,int end)
{
    queue *temp=head->next;
    printf("the rout:\n");
    printf("%s <- ",g->point[end].name);
    while(temp)
    {
        printf("%s <- ",g->point[temp->father].name);
        temp=temp->next;
    }
    printf("\n");
}

int get_next(graph* g,int* father,int* son)
{
    Arcnode *temp=g->point[*father].head->next;
    if((*son)==-1)
    {
        if(temp)
        {
           (*son)=temp->vexnode;
            return 1;
        }
        else
            return 0;
    }
    else
    {

        while(temp->vexnode !=(*son))
           temp=temp->next;
        if(temp->next)
        {
           (*son)=temp->next->vexnode;
           return 1;
        }
        else
            return 0;
    }
}

int contin_find(graph*g,int father,int son,int start)
{
    Arcnode *temp=g->point[father].head->next;
    while(temp->vexnode !=son)
        temp=temp->next;
    if( temp->next!=NULL)
        return 1;
    else
        return 0;
}

void  all_ways_bttps(graph *g,queue *head,int start,int end)
{
    head=init_queue();
    int in_stack[m];
    int father=start;
    int son;
    int count=0;

    for (int i = 0; i < m; ++i)
        in_stack[i]=0;

    if(g->point[start].head->next)
    {
        son=g->point[start].head->next->vexnode;
        push(head,father,son);
        in_stack[start]=1;
        if(g->point[start].head->next->vexnode==end)
        {
            print_rout(g,head,end);
            count ++;
            in_stack[father]=0;
        }
        else
        {
             father=son;
             son=-1;
        }
    }
    else
    {
            printf("there is no way between the two points \n");
            return;
    }
    while((empty(head)==0)||(contin_find(g,father,son,start)))
    {
        if(get_next(g,&father,&son))
        {
            if(in_stack[father]==0)
            {
                if(son!=end)
                {
                     push(head,father,son);
                     in_stack[father]=1;
                     father=son;
                     son=-1;
                }
                else
                {
                    push(head,father,son);
                    in_stack[father]=1;
                    print_rout(g,head,son);
                    count ++;
                }
            }
        }
        else
        {
            pop(head,&father,&son);
            in_stack[father]=0;
        }
    }
    printf("in total:%d way(s)\n",count);
}

void dijkstra(graph *g,float (*matrix)[m],int start,int end,int dist[m],int path[m][m+1])
{
     int mindist;
     int t,k;
     t=1;

     for (int i = 0; i < g->vexnum; ++i)
         path[i][0]=0;
     for (int i = 0; i <g->vexnum ; ++i)
     {
         for (int j = 1; j < m+1; ++j)
         {
             path[i][j]=-1;
         }
     }

     for (int i = 0; i < g->vexnum; ++i)
     {
         dist[i]=matrix[start][i];
         if(matrix[start][i]!=infinity)
            path[i][1]=start;
     }

     path[start][0]=1;
     for (int i = 1; i < g->vexnum; ++i)     //?
     {
         mindist=infinity;
         for (int j = 0; j < g->vexnum; ++j)
         {
               if(!path[j][0]  && dist[j]<mindist)
               {
                   k=j;
                   mindist=dist[j];
               }
         }
         if(mindist==infinity)
         {
            printf("this point has no out way \n" );
            return ;
         }
         path[k][0]=1;
         for (int j = 1; j < m; ++j)
         {
             if(!path[j][0] && matrix[k][j]<infinity  && (dist[k]+matrix[k][j]<dist[j]))
             {
                dist[j]=dist[k]+matrix[k][j];
                t=1;
                while(path[k][t]!=-1)
                {
                    path[j][t]=path[k][t];
                    t++;
                }
                path[j][t]=k;
             }
         }
     }
     printf("the shortest way between the %s and %s is\n",g->point[start].name,g->point[end].name);
     t=1;
     while((k=path[end][t])!=-1)
     {
        printf("%s  ", g->point[k].name);
        t++;
     }
     printf("%s\n",g->point[end].name );
     printf("\nin length:%d\n", dist[end]);
}
/*
void test_dijs(int *dist,int path[][m+1])
{
    printf("the distence array:\n");
    for (int i = 0; i < m; ++i)
    {
        printf("%d ", dist[i]);
    }
    printf("\n");

    printf("the path array:\n");

    for (int i = 0; i < m; ++i)
    {
        for (int j = 0; j <m+1 ; ++j)
        {
            printf("%d  ", path[i][j]);
        }
        printf("\n");
    }

    printf("\n");

}
*/
mst_point * prim(graph* g,float(*matrix)[m],int start)
{
     mst_point *head=(mst_point *)malloc(sizeof(mst_point));
     mst_point* temp;
     struct
     {
        int adjvex;
        int lowcost;
     } closedge[m];

     int k ,s,min;

     head->next=NULL;

     closedge[start].lowcost=0;

     for (int i = 0; i < g->vexnum; ++i)
         if (i!=start)
         {
             closedge[i].adjvex=start;
             closedge[i].lowcost=matrix[start][i];

         }

     for (int i = 0; i < g->vexnum-1; ++i)
     {
         min=infinity;
         for (int j = 0; j < g->vexnum; ++j)
         {
             if(closedge[j].lowcost !=0 && closedge[j].lowcost<min)
                 {
                    s=j;
                    min=closedge[j].lowcost;
                 }
         }
        temp=(mst_point*)malloc(sizeof(mst_point));
        temp->father=closedge[s].adjvex;
        temp->son=s;

        temp->next=head->next;
        head->next=temp;

        closedge[s].lowcost=0;

        for (int j = 0; j < g->vexnum; ++j)
            if(j!=s && matrix[s][j] <closedge[j].lowcost)
            {
                closedge[j].lowcost=matrix[s][j];
                closedge[j].adjvex=s;

            }
     }
     return head;
}

void test_mst(mst_point *head)
{
    mst_point* temp;
    temp=head->next;
    printf("the MST tree is:\n");
    while(temp)
    {
        printf("(%d,%d)  ", temp->father,temp->son);
        temp=temp->next;
    }
    printf("\n");
}

void  read(graph* g,float (*matrix)[m],FILE *fp)
{
     Arcnode *temp;
     int vexnum;
     int  w;
     int vexnode;
     float distence;

     fscanf(fp, "%d\n",&vexnum );


     g->vexnum=vexnum;
     for (int i = 0; i < vexnum; ++i)
     {
        g->point[i].head=(Arcnode*)malloc(sizeof(Arcnode));
        g->point[i].head->next=NULL;

        //fread(g->point[i].name,30,1,fp);
        fscanf(fp,"%s",g->point[i].name);

        //fread(g->point[i].information,100,1,fp);
        fscanf(fp,"%s",g->point[i].information);
        fscanf(fp, "%d\n", &w);
        for (int j = 0; j <w; ++j)
        {
            temp=(Arcnode *)malloc(sizeof(Arcnode));
            fscanf(fp, "%d %f\n",&vexnode,&distence );


            temp->vexnode=vexnode;
            temp->distence=distence;

            temp->next=g->point[i].head->next;
            g->point[i].head->next=temp;

            matrix[i][vexnode]=distence;
        }

     }
     test(g,matrix);
}

void display(graph *g)
{
    int choice;
    printf("\n    places intrduction    \n");
    for (int i = 0; i < g->vexnum; ++i)
    {
        printf("%d : %s\n",i,g->point[i].name);
    }
    scanf("%d",&choice);
    getchar();
    printf("%s\n\n",g->point[choice].information);
}

void Select(graph *g)
{
    int point;
    for (int i = 0; i < g->vexnum; ++i)
    {
        printf("%d:%s\n",i,g->point[i].name);
    }
}

void display_mst(graph* g,mst_point *head)
{
     test_mst(head);
     mst_point* temp;
     temp=head->next;
     printf("visit way as:\n");
     while(temp)
     {
        printf("%s to %s\n", g->point[temp->father].name,g->point[temp->son].name);
        temp=temp->next;
     }
}

int main()
{
	graph g;
    queue *head;
    float matrix[m][m];
    int path[m][m+1];
    int start,end;
    int dist[m];

    mst_point *Head;

    init_matrix(matrix);

    int point;
    int choice;
    FILE *fp;
    float distence;
    while(1)
{

    system("clear");

    printf("                        BUSLINE demo                \n\n ");
    printf("                                 FUNCTIONS                            \n");
    printf("      0.basic info of map.\n");
    printf("      1.basic info of each place\n");
    printf("      2.every route of any two places \n");
    printf("      3.shortest route of any twp places\n");
    printf("      4.shortest route from any places\n");
    printf("      5.exit\n");
    printf("\n\n  input your choice: ");

    scanf("%d",&choice);
    if(choice==5)
        break;
    else
    {
        switch(choice)
        {
            case 0:
            {
                  fp=fopen("graph.txt","rt");
                  read(&g,matrix,fp);
                  fclose(fp);
                  getchar();
                  getchar();
                  break;
            }
            case 1:
            {
                fp=fopen("graph.txt","rt");
                read(&g,matrix,fp);
                fclose(fp);
                display(&g);
                getchar();
                getchar();
                break;
            }
            case 2:
            {
                fp=fopen("graph.txt","rt");
                read(&g,matrix,fp);
                fclose(fp);
                Select(&g);
                printf("input the start point and target point:");
                scanf("%d%d",&start,&end);
                getchar();
                all_ways_bttps(&g,head,start,end);
                getchar();
                getchar();
                break;
            }
            case 3:
            {
                 fp=fopen("graph.txt","rt");
                 read(&g,matrix,fp);
                 fclose(fp);
                 Select(&g);
                 printf("input the start point and target point:");
                 scanf("%d%d",&start,&end);
                 getchar();
                 dijkstra(&g,matrix,start,end,dist,path);
                 getchar();
                 getchar();
                 break;
            }
            case 4:
            {
                fp=fopen("graph.txt","rt");
                read(&g,matrix,fp);
                fclose(fp);
                Select(&g);
                printf("input the start point:\n");
                scanf("%d",&start);
                getchar();
                Head=prim(&g,matrix,start);
                display_mst(&g,Head);
                getchar();
                getchar();
                break;
            }
        }
    }
}
	return 0;
}
