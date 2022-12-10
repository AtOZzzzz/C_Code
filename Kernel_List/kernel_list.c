#include "kernel_list.h"

//内核链表（双向循环带头）

//创建单个宿主节点
host_node* Creat_Host()
{
    host_node* host = (host_node*)malloc(sizeof(host_node));
    if(host == NULL)
    {
        perror("host_head malloc failed");
        return (host_node*)-1;
    }

    host->name = NULL;
    host->path = NULL;

    INIT_LIST_HEAD(&(host->para));  //自己指向自己

    return host;

}

//头插（可以实现任意的位置之后的插入，把x的当成头（host_head）传参进去）
void Head_Inser(host_node* host_head,char* name,char* path)
{
    
    assert(host_head != NULL);

    host_node* new_host = Creat_Host();
    if(new_host == (host_node*)-1)
    {
        perror("creat new host failed");
        return ;
    }

    new_host->name = name;
    new_host->path = path;

    list_add( &(new_host->para) , &(host_head->para));

}

//尾插（可以实现任意的位置之前的插入，把x的当成头（host_head）传参进去）
void Tail_Inser(host_node* host_head,char* name,char* path)
{

    assert(host_head != NULL);

    host_node* new_host = Creat_Host();
    if(new_host == (host_node*)-1)
    {
        perror("creat new host failed");
        return ;
    }

    new_host->name = name;
    new_host->path = path;

    list_add_tail( &(new_host->para) , &(host_head->para));
}

//删除单个宿主节点
void Del_Host(host_node* host_cur)
{
    assert(host_cur != NULL);

    host_cur->name = NULL;
    host_cur->path = NULL;
    list_del(&(host_cur->para));//交换双手，自断双手，但是不free
    free(host_cur);
}


//查找数据
host_node* Sea_Host(host_node* host_head,char* name)
{

    assert(host_head != NULL);

    if(list_empty(&(host_head->para)) == 1)
    {
        printf("空链表,无数据查找\n");
        return (host_node*)-1;
    }


    host_node* temp = NULL;
    list_for_each_entry(temp , &(host_head->para) , para)
    {
        if(strcmp(temp->name,name) == 0)
        {
            printf("找到数据，返回地址:%p\n",temp);
            return temp;
        }
    }

    printf("链表中没有此数据\n");
    return (host_node*)0;
}

//遍历
void Tra_Data(host_node* host_head)
{
    assert(host_head != NULL);

    if(list_empty(&(host_head->para)) == 1)
    {
        printf("空链表,无数据查找\n");
        return ;
    }

    host_node* temp = NULL;
    list_for_each_entry(temp , &(host_head->para) , para)
    {
        printf("%s\n",temp->path);
    }

    // bug: 
    // list_for_each_entry(host_head , &(host_head->para) , para)
    // {
    //     printf("%s\n",host_head->name);
    // }


    // for (pos = list_first_entry(head, typeof(*pos), member);	\
	//      &pos->member != (head);					\
	//      pos = list_next_entry(pos, member))
    //                 list_entry((pos)->member.next, typeof(*(pos)), member)
}       

//销毁链表
void Ruin_List(host_node* host_head)
{

    host_node* temp = NULL;
    struct list_head* temp1 = NULL;
    // list_for_each_entry(temp , &(host_head->para) , para)
    // {
    //     temp1 = temp->para.next;
    //     Del_Host(temp);
    //     temp = list_entry(temp1,host_node,para);
    // }
    temp = list_first_entry(&(host_head->para),typeof(*temp),para);
    while(&(temp->para) != (&(host_head->para)))
    {
        temp1 = temp->para.next;
        Del_Host(temp);
        temp = list_entry(temp1,host_node,para);
    }
    Del_Host(temp);
}
