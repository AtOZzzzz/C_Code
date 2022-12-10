#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <sys/ioctl.h>

#define FB0_PATH "/dev/fb0"
#define BMP1_PATH "./Gai.bmp"
#define BMP2_PATH "./AfterJourney.bmp"
#define BMP3_PATH "./AR.bmp"
#define PHOTO_NUM 3

#pragma pack(1) //设置字节对齐数为1
typedef struct bitmapfileheader
{
    unsigned short bfType;      //图片文件的格式
    unsigned int bfSize;        //文件的总大小
    unsigned short bfReserved1; //保留位
    unsigned short bfReserved2; //保留位
    unsigned int bfOffset;      //文件开始到位图数据的偏移量
}bitmap_file_head;
#pragma pack() //取消设置

typedef struct tagBITMAPINFOHEADER 
{

    unsigned int biSize; // 此结构体的大小
    int biWidth; // 图像的宽
    int biHeight; // 图像的高
    unsigned short biPlanes; // 表示bmp图片的平面属，显然显示器只有一个平面，所以恒等于1
    unsigned short biBitCount; // 一像素所占的位数，一般为24
    unsigned int biCompression; // 说明图象数据压缩的类型，0为不压缩。
    unsigned int biSizeImage; // 像素数据所占大小, 这个值应该等于上面文件头结构中bfSize-bfOffBits
    int biXPelsPerMeter; // 说明水平分辨率，用象素/米表示。一般为0
    int biYPelsPerMeter; // 说明垂直分辨率，用象素/米表示。一般为
    unsigned int biClrUsed; // 说明位图实际使用的彩色表中的颜色索引数(设为0的话，则说明使用所有调色板项)。
    unsigned int biClrImportant;// 说明对图象显示有重要影响的颜色索引的数目，如果是0，表示都重要。
} bitmap_info_head;


typedef struct bitmap_photo
{
    bitmap_file_head file_head;
    bitmap_info_head info_head;
    char* pixel_point;
    int fd;
}photo;

typedef struct LCD
{
    int fd_fb0;
    int* fb0_mapping;
    struct fb_fix_screeninfo ffs;      //改成指针节省空间？？？？？？？
    struct fb_var_screeninfo fvs;
}lcd;


void Photo_Init( char* , photo* );
photo* (* Photo_Init_Room(int ,char* *) )[];

lcd* Fb0_Mapping_Init();

void Write_Pixel( int* , photo* );
void Write_Pixel_Room( lcd* , photo* (*p) [] );

void Change_Spotlight( lcd* );
void Free( lcd* , photo* * ); 

int main()
{
        photo pho = {{0},{0},NULL,0};
        Photo_Init("./AR.bmp",&pho);

        lcd* m = Fb0_Mapping_Init();

        Write_Pixel(m->fb0_mapping,&pho);

        //free(m,)


        return 0;
}


void Change_Spotlight(lcd* lcd)
{
    unsigned int count = 0;
    while(1)
    {
        lcd->fvs.xoffset = 0;
        lcd->fvs.yoffset = lcd->fvs.yres * (count % 3);
        ioctl(lcd->fd_fb0,FB_ACTIVATE_NOW,&lcd->fvs);
        ioctl(lcd->fd_fb0,FBIOPAN_DISPLAY,&lcd->fvs);
        sleep(2);
        count++;
    }
}

void Free(lcd* m,photo* * arr)
{


    // close(m->fd_fb0);
    // munmap(m->fb0_mapping,m->ffs.smem_len);
    // free(m);     

    // int i;
    // for (i = 0; i < num; i++)
    // {
    //     close(arr[i]->fd);
    //     free(arr[i]->pixel_point);
    //     free(arr[i]);
    // }
    
    // free(*arr);                
    // free(*(arr+1));                
    // free(*(arr+2));                
    // free(p->pixel_point);                       
    // close(p->fd);
    // printf("free掉的地址是%p\n",p->pixel_point);
}


void Write_Pixel(int* mapping_p,photo* photo_p)
{
    char* tmp = photo_p->pixel_point;
    int* mapping_p_copy = mapping_p;


    printf("%d\n",photo_p->file_head.bfSize);
    printf("%d\n",photo_p->info_head.biSizeImage);
    printf("%d\n",photo_p->file_head.bfOffset);

    int alignment = photo_p->info_head.biSizeImage -       //求对齐总数
                    (photo_p->info_head.biHeight * photo_p->info_head.biWidth * 3);
                    
    printf("字节对齐的总数为%d\n",alignment);

    mapping_p += 800 * (300 - photo_p->info_head.biHeight / 2) + 
                        (400 - photo_p->info_head.biWidth  / 2);
                    
    int x , y; 
    for (y = photo_p->info_head.biHeight ; y > 0 ; y--)
    {                       
        for (x = 0; x < photo_p->info_head.biWidth; x++)
        {                       
            *(mapping_p + (y-1)*800  + x)  =    *(tmp + (x*3))   << 0 |
                                                *(tmp + (x*3+1)) << 8 |
                                                *(tmp + (x*3+2)) << 16;

            
        }
        tmp += photo_p->info_head.biWidth * 3;
        if(alignment)
        {
            tmp += alignment / photo_p->info_head.biHeight;
        }
    }


    //free(photo_p->pixel_point);
}


void Write_Pixel_Room(lcd* lcd,photo* (*p) [])
{
    int room =  (lcd->fvs.xres_virtual * lcd->fvs.yres_virtual) / 
                (lcd->fvs.xres * lcd->fvs.yres);  

    int* mapping_copy = lcd->fb0_mapping;

    int i;
    for (i = 0; i < room; i++)
    {
        mapping_copy += (lcd->fvs.xres * lcd->fvs.yres) * i;
        Write_Pixel(mapping_copy,*(*p+i));
    }



    //把图片像素数据写进去，就把图片所有有关的文件和内存释放
    for (i = 0; i < room; i++)
    {
        close((*(*p+i))->fd);
        free((*(*p+i))->pixel_point);
        free((*(*p+i)));
    }
    free(p);
}


lcd* Fb0_Mapping_Init()
{
    lcd* lcd_p = (lcd*)malloc(sizeof(lcd));
    if(lcd_p == NULL)
    {
        printf("lcd malloc failed:%s\n",strerror(errno));
        return ;
    }
    //打开设备文件

    lcd_p->fd_fb0 = open(FB0_PATH,O_RDWR);
    if(lcd_p->fd_fb0 == -1)
    {
        //perror("fd_fb0:");
        printf("fd_fb0 open failed:%s\n",strerror(errno));
        return ;
    }

    //获取lcd_p硬件设备信息
    /*int io_res1 = ioctl(lcd_p->fd_fb0,FBIOGET_FSCREENINFO,&lcd_p->ffs);
    int io_res2 = ioctl(lcd_p->fd_fb0,FBIOGET_VSCREENINFO,&lcd_p->fvs);
    if(io_res1 == -1 || io_res2 == -1)
    {
        printf("ioctl failed:%s\n",strerror(errno));
        return ;
    }
    //映射内存
    printf("显存的大小是%d\n",lcd_p->ffs.smem_len);*/

    lcd_p->fb0_mapping = mmap(NULL,800*600*4,
                          PROT_READ | PROT_WRITE,
                          MAP_SHARED,lcd_p->fd_fb0,0);

    printf("mapping出来的首地址是%p\n",lcd_p->fb0_mapping);
    printf("mapping出来的尾地址是%p\n",lcd_p->fb0_mapping +  (800 * 600));

    if(lcd_p->fb0_mapping == MAP_FAILED)
    {
        printf("mapping failed:%s\n",strerror(errno));
        return ;
    }

    return lcd_p;
}

//想一想怎么返回这个数组
photo*  (* Photo_Init_Room(int num,char* * add_arr) ) [] 
{
    photo* (*p) [num] = (photo* (*) [num]) malloc (sizeof(photo*) * num);
    if( p == NULL )
    {
        printf("(line 265)malloc failed:%s\n",strerror(errno));
    }

    printf("malloc-----%p\n------",p);

    printf("malloc-----%p\n---xx---",*(*p));
    int i;
    for (i = 0; i < num; i++)
    {
       *(*p+i) = (photo*)malloc(sizeof(photo));
       Photo_Init(add_arr[i],*(*p+i));
    }


    return p;
}

void Photo_Init(char* address,photo* photo_p)
{

    photo_p->fd = open(address,O_RDONLY);
    if(photo_p->fd == -1)
    {
        printf("open failed:%s\n",strerror(errno));
        return ;
    }


    //把图片文件头读取出来
    if(read(photo_p->fd,&(photo_p->file_head),14) == -1)
    {
        printf("read file_head failed:%s\n",strerror(errno));
        return ;
    }

    //把图片信息头读取出来 
    if(read(photo_p->fd,
            &(photo_p->info_head),
            photo_p->file_head.bfOffset-sizeof(bitmap_file_head)) == -1)
    {
        printf("read info_head failed:%s\n",strerror(errno));
        return ;
    }
    

    photo_p->pixel_point = (char*)malloc(photo_p->info_head.biSizeImage);
    if(photo_p->pixel_point == NULL)
    {
        printf("malloc:%s\n",strerror(errno));
        return ;
    }

    if(read(photo_p->fd,photo_p->pixel_point,photo_p->info_head.biSizeImage) == -1)
    {
        printf("read image_info failed:%s\n",strerror(errno));
        return ;
    }

    printf("%d\n",photo_p->info_head.biSizeImage);

    printf("malloc出来的地址是%p\n",photo_p->pixel_point);
     
     
    //return photo_p->fd;
    //  printf("%x\n",photo_p->file_head.bfType);
    //  printf("%d\n",photo_p->file_head.bfSize);
    //  printf("%d\n",photo_p->file_head.bfReserved1);
    //  printf("%d\n",photo_p->file_head.bfReserved2);
    //  printf("%d\n",photo_p->file_head.bfOffset);
     
    // printf("%d\n",photo_p->info_head.biBitCount);
    // printf("%d\n",photo_p->info_head.biHeight);
    // printf("%d\n",photo_p->info_head.biWidth);
    // printf("%d\n",photo_p->info_head.biSizeImage);
    // printf("%d\n",photo_p->info_head.biBitCount);
    // printf("%d\n",photo_p->info_head.biBitCount);
    // printf("%d\n",photo_p->info_head.biBitCount);
}




