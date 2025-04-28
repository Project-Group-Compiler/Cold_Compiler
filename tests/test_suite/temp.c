
// int main(){
//     int a = 10;
//     int *ptr = &a;
//     int d = *ptr;
//     int b = 0;
//     int c = 11;
//     c = d + 0;
//     printf("%d\n",c);
//     c = d - 0;
//     printf("%d\n",c);
//     c = d * 1;
//     printf("%d\n",c);
//     c = d ^ 0;
//     printf("%d\n",c);
//     c = d | 0;
//     printf("%d\n",c);
// }

// int main(){
//     int x = 5;
//     int *ptr = &x;
//     int *ptr2 = &ptr;
//     ptr = *ptr2;

// }

// struct a{
//     int y;
//     int *b;
// };

// int main(){
//     struct a p;
//     int x = 5;
//     int y = 11;
//     // printf("%p\n",p.b);
//     printf("%p\n",&x);
//     p.b = &x;
//     p.y = 111;
//     printf("%p\n",p.b);
//     printf("%d\n",p.y);
    
//     *(p.b) = 10;
//     y = *(p.b);
//     printf("%d\n",x);
//     printf("%d\n",y);
//     printf("%d\n",*(p.b));
// }


// struct a{
//     int y;
//     int *b;
// };

// int main(){
//     struct a* p = (struct a*)malloc(sizeof(struct a));
//     int x = 5;
//     int y = 11;
//     // printf("%p\n",p.b);
//     printf("%p\n",&x);
//     p->b = &x;
//     p->y = 101;
//     printf("%p\n",p->b);
//     printf("%d\n",p->y);
    
//     *(p->b) = 10;
//     y = *(p->b) ;
//     printf("%d\n",x);
//     printf("%d\n",y);
//     printf("%d\n",*(p->b) );
// }


// struct Node
// {
//     int data;
//     struct Node *next;
// };
// int main()
// {
//     struct Node *temp;
//     struct Node *temp2;
//     struct Node t1, t2;
//     int x;
//     struct Node *first = &t1;
//     struct Node *second = &t2;
//     (*first).data = 10;
//     (*second).data = 20;
//     (*first).next = second; // This will create: 10 -> 20
//     printf("%p %p\n", (*first).next, second);
//     printf("%d\n", (*second).data);


//     x = (*((*first).next)).data;
//     printf("%d\n", x); // wrong
//     printf("%d\n", first->next->data);
//     printf("%d\n", (*first).next->data);

//     temp = first;
//     for (x = 0; x < 2; x++)
//     {
//         printf("%d ", temp->data); // same wrong answer
//         temp2 = temp->next; // cause of error
//         temp = temp2;
//     }
//     printf("\n");
//     return 0;
// }

// struct Node
// {
//     int data;
//     struct Node *next;
// };
// int main()
// {
//     struct Node *temp;
//     struct Node *temp2;
//     int x;
//     struct Node *first = (struct Node *)malloc(sizeof(struct Node));
//     struct Node *second = (struct Node *)malloc(sizeof(struct Node));
//     (*first).data = 10;
//     (*second).data = 20;
//     (*first).next = second; // This will create: 10 -> 20
//     temp = first;
//     for (x = 0; x < 2; x++)
//     {
//         printf("%d ", (*temp).data);
//         temp2 = (*temp).next; // cause of error
//         temp = temp2;
//     }
//     printf("\n");
//     return 0;
// }


// struct node {
//     int a;
//     int b;
//     char* c;
//   };
  
//   int main(){
//     int a = 10;
//     struct node d;
//     int b,c,j;
//     printf("value of a\n");
//     scanf("%d",&d.a);
//     printf("value of b\n");
//     scanf("%d",&d.b);
//     j = d.a;
//     printf("value of a - %d b - %d",d.a,d.b);
//     return 1;
//   }

// not working
// union Data {
//     int i;
//     char ch; // TODO: FIX
//     float f; // TODO: FIX
// };

// int test_union() {
//     union Data data;

//     data.i = 10;
//     printf("[test_union] After setting i = 10, data.i = %d\n", data.i);
//     data.ch = 'j';
//     printf("[test_union] After setting ch = 'j', data.ch = %c\n", data.ch);
//     printf("[test_union] After setting i = 10, data.i = %c\n", data.i);
//     data.f = 3.14;
//     printf("[test_union] After setting f = 3.14, data.f = %f\n", data.f);
//     printf("[test_union] After setting i = 10, data.i = %f\n", data.i);
//     return 0;
// }

// int main() {

//     printf("\n[main] Testing union...\n");
//     test_union();

//     return 0;
// }



// class a{
//     public{
//         int y;
//         int *b;
//     };
// };

// int main(){
//     class a p;
//     int x = 5;
//     int y = 11;
//     // printf("%p\n",p.b);
//     printf("%p\n",&x);
//     p.b = &x;
//     p.y = 111;
//     printf("%p\n",p.b);
//     printf("%d\n",p.y);
    
//     *(p.b) = 10;
//     y = *(p.b);
//     printf("%d\n",x);
//     printf("%d\n",y);
//     printf("%d\n",*(p.b));
// }


// struct a{//not working
//     int y;
//     int b[3];
// };

// int main(){
//     struct a p;
//     int x = 5;
//     int y = 11;

//     p.b[0] = x;
//     y = p.b[0];
//     printf("%d\n",y);
//     printf("%d\n",p.b[0]);
// }

// struct node {
//     int a;
//     int b;
//     int c;
//     // char* c;
// };
  
// void f(){
//     struct node d1;
//     struct node d2;
//     struct node *d3 = &d1;
//     struct node *d4 = &d2;

//     // struct node dar[2] = {d1,d2};

//     d1.a = 10, d1.b = 20, d1.c = 30;
//     printf("%d\n", d1.a);
//     printf("%d\n", d1.b);
//     printf("%d\n", d1.c);
//     // d2 = d1;
    
//     // d2 = *d3;
//     // printf("%d\n", d2.a);
//     // printf("%d\n", d2.b);
//     // printf("%d\n", d2.c);

//     *d4 = d1;
//     printf("%d\n", d2.a);
//     printf("%d\n", d2.b);
//     printf("%d\n", d2.c);

//     *d4 = *d3;
//     // printf("%d\n", (*d3).a);
//     // printf("%d\n", (*d3).b);
//     // printf("%d\n", (*d3).c);
//     printf("%d\n", d2.a);
//     printf("%d\n", d2.b);
//     printf("%d\n", d2.c);
// }

// void f3(){
//     struct node d1;
//     struct node d2;
//     // struct node *d3 = &d1;

//     struct node dar[2];
//     d1.a = 10;
//     d1.b = 20;
//     d1.c = 30;
    
//     dar[1] = d1;
//     printf("%d\n",dar[1].a);
//     printf("%d\n",dar[1].b);
//     printf("%d\n",dar[1].c);
//     dar[0] = dar[1];
//     dar[0].a = 100;
//     printf("%d\n",dar[0].a);
//     printf("%d\n",dar[0].b);
//     printf("%d\n",dar[0].c);

// }

// int main(){
//     // int b,c,j;
//     // printf("value of a\n");
//     // scanf("%d",&d.a);
//     // printf("value of b\n");
//     // scanf("%d",&d.b);
//     // j = d.a;
//     // printf("value of a - %d b - %d",d.a,d.b);
//     return 1;
// }




struct Node
{
    int data;
    struct Node *next;
};

int main()
{
    struct Node *head;
    struct Node *current;
    int i;
    int cnt = 9;
    for(i = 0; i < cnt; i++)
    {
        struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
        newNode->data = i;
        newNode->next = head;
        head = newNode;
    }
    current = head;
    for(i = 0; i < cnt; i++)
    {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
    return 0;
}
