
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
//struct for customer
    typedef struct customer{
        char *custname;
        int numitems;
        int linenum;
        int time;
    }customer;

//struct for node which contains pointer to a customer
    typedef struct node{
        customer *data;
        struct node *next;
    }node;

//struct for queue which contains pointers to first and last node
    typedef struct queue{
        node *front;
        node *rear;
    }queue;

//sets queue front and rear to null
    void newqueue(queue *que){
        que->front = NULL;
        que->rear = NULL;
    }

//takes in variables needed for a customer struct and a queue
//assigns variables to temp which is then transferred into the queue
    void enqueue(queue *q, char *name, int num_items, int line_num, int tim){
        node* temp = (node*)malloc(sizeof(node));
        temp->data = (customer*)malloc(sizeof(customer));
        temp->data->numitems = num_items;
        temp->data->linenum = line_num;
        temp->data->time = tim;
        temp->data->custname = strdup(name);
        temp->next = NULL;
        
        if(q->rear == NULL){
            q->front = temp;
            q->rear = temp;
            return; //DONT DELETE ILHAM PLEASE
        }
        q->rear->next = temp;
        q->rear = temp;
        
    }

//removes a node from the q
    void dequeue(queue *q){
        //print in dequeue its easier
        
        if(q->front == NULL){
            return;
        }
            
        node *temp = q->front;
        q->front = q->front->next;
        if(q->front==NULL){
            q->rear = NULL;
        }
        free(temp->data);
        free(temp);
        //DONT FREE ^^^^ WE NEED TEMP
        //needs to return customer type
            
    }

//this function figures out which customer gets into line first, it then proceeds to check them out
    int ckoutfirst(queue *q){
        int time = 0;
        queue* temp = &q[0];
        for(int i=1; i<12; i++){
            if(q[i].front==NULL)
                continue;
            if((temp->front->data->time)>(q[i].front->data->time)){
                temp = &q[i];
                time = temp->front->data->time;
            }
         }
        time = 30 + temp->front->data->time + (5)*(temp->front->data->numitems);
        printf("%s from line %d checks out at time %d \n", temp->front->data->custname, temp->front->data->linenum, time);
        dequeue(temp);
        return time;

     }

//Calculates the time the customer checks out
    int time_func(int time, int *timeentered, int *numitems){
        int timeee = 0;
        if(*timeentered>time){
            timeee = *timeentered + ((*numitems)*5) + 30;
        }
        else{
            timeee = time + ((*numitems)*5) + 30;
        }
        return timeee;
    }

//checks which customer is after the first, and checks them out
   int ckouttime(queue *q, int time){
       queue* temp = &q[0];
       for(int i=1; i<12; i++){
           if(q[i].front==NULL)
               continue;
           if(temp->front == NULL){
               temp = &q[i];
           }
           else if((temp->front->data->numitems)>(q[i].front->data->numitems)){
                temp = &q[i];
            }
           else if((temp->front->data->numitems)==(q[i].front->data->numitems)){
                if((temp->front->data->linenum)>(q[i].front->data->linenum))
                temp = &q[i];
            }
        }
       int time1 = time_func(time, &temp->front->data->time, &temp->front->data->numitems);
       printf("%s from line %d checks out at time %d \n", temp->front->data->custname, temp->front->data->linenum, time1);
       dequeue(temp);
       return time1;

    }
       
//scans all variables needed for customer struct
    queue read_customers(FILE *fp, queue *queues, int numcust){
        char Name[11];
        int numItems, lineNum, Time;
        for(int i = 0; i < numcust; i++){
            fscanf(fp,"%d", &Time);
            fscanf(fp,"%d", &lineNum);
            fscanf(fp,"%s", Name);
            fscanf(fp,"%d", &numItems);
            enqueue(&queues[lineNum-1], Name, numItems, lineNum, Time);
        }
        return *queues;
    }

    int main()
    {
        //opens file
        FILE *infile;
        infile = fopen("assignment2input.txt","r");
        if(infile != NULL) {
            printf("We successfully opened the input file.\n");
        } else {
            printf("You freaked up\n");
            return 0;
        }
        
        //creates ques and makes their front and rears=NULL
        //Creating queue
        queue queues[12];
        for(int i = 0; i<12; i++){
            newqueue(&queues[i]);
        }
        
        //scans for test case amount
        int test_cases, num_cust;
        fscanf(infile,"%d", &test_cases);
        
        //loops through all functions needed for each test case
        for(int i=0; i<test_cases; i++){
            printf("\n \n \nTest Case %d\n", i+1);
            fscanf(infile,"%d", &num_cust);
            read_customers(infile, queues, num_cust);
            int time = ckoutfirst(queues);
            //loops for every customer in test case
            for(int i = 0; i<num_cust-1; i++){
                time = ckouttime(queues, time);
            }
        }

        
        return 0;

    }

