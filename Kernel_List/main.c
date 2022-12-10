#include "kernel_list.h"

int main()
{
    host_node* head = Creat_Host();
    Head_Inser(head,"张三","18");
    Head_Inser(head,"李四","28");
    Head_Inser(head,"王五","38");
    Head_Inser(head,"田七","48");
    Tra_Data(head);
    printf("===============================\n");
    Sea_Host(head,"哈哈");
    host_node* temp = Sea_Host(head,"张三");
    Del_Host(temp);
    Tra_Data(head);
    printf("===============================\n");
    Tail_Inser(head,"刘翔","58");
    Tail_Inser(head,"无1","68");
    Tail_Inser(head,"无2","78");
    Tail_Inser(head,"无3","88");
    Tra_Data(head);
    printf("======================");
    temp = Sea_Host(head,"刘翔");
    Head_Inser(temp,"刘翔之后","99");
    Tra_Data(head);

    Ruin_List(head);

    //Tra_Data(head);

    return 0;
}