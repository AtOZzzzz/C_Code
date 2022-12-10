#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>              //test
#include <stdlib.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <linux/input.h>

#define FB0_PATH "/dev/fb0"
#define BMP1_PATH "/project/ps/Gai.bmp"
#define BMP2_PATH "/project/ps/AfterJourney.bmp"
#define BMP3_PATH "/project/ps/AR.bmp"
#define PHOTO_NUM 3

#pragma pack(1) //设置字节对齐数为1
typedef struct bitmapfileheader
{
    unsigned short bfType;      //图片文件的格式
    unsigned int bfSize;        //文件的总大小
    unsigned short bfReserved1; //保留位
    unsigned short bfReserved2; //保留位
    unsigned int bfOffset;      //文件开始到位图数据的偏移量
} bitmap_file_head;
#pragma pack() //取消设置

typedef struct tagBITMAPINFOHEADER
{

    unsigned int biSize;         // 此结构体的大小
    int biWidth;                 // 图像的宽
    int biHeight;                // 图像的高
    unsigned short biPlanes;     // 表示bmp图片的平面属，显然显示器只有一个平面，所以恒等于1
    unsigned short biBitCount;   // 一像素所占的位数，一般为24
    unsigned int biCompression;  // 说明图象数据压缩的类型，0为不压缩。
    unsigned int biSizeImage;    // 像素数据所占大小, 这个值应该等于上面文件头结构中bfSize-bfOffBits
    int biXPelsPerMeter;         // 说明水平分辨率，用象素/米表示。一般为0
    int biYPelsPerMeter;         // 说明垂直分辨率，用象素/米表示。一般为
    unsigned int biClrUsed;      // 说明位图实际使用的彩色表中的颜色索引数(设为0的话，则说明使用所有调色板项)。
    unsigned int biClrImportant; // 说明对图象显示有重要影响的颜色索引的数目，如果是0，表示都重要。
} bitmap_info_head;

typedef struct bitmap_photo
{
    bitmap_file_head file_head;
    bitmap_info_head info_head;
    char *pixel_point;
    int fd;
} photo;

typedef struct LCD
{
    int fd_fb0;
    int *fb0_mapping;
    struct fb_fix_screeninfo *ffs;
    struct fb_var_screeninfo *fvs;
} lcd;

typedef struct touch_screen
{
    int touch_screen_fd;
    struct input_event *touch_info;
} touch_screen;

void Photo_Init(char *, photo *);
photo *(*Photo_Init_Room(int, char **))[];

lcd *Fb0_Mapping_Init();

void Write_Pixel(int *, photo *);
void Write_Pixel_Room(lcd *, photo *(*p)[]);

void Change_Spotlight(lcd *);
void Free(lcd *); //我把一块结构体free掉，里面的成员地址会不会也free掉？？？



int GetEvent(touch_screen *);
touch_screen *Touch_Screen_Init();
void Change_Spotlight_Event(lcd *lcd, touch_screen *ts_p);





int main()
{
    char *PATH[PHOTO_NUM] = {BMP1_PATH, BMP2_PATH, BMP3_PATH};

    //获取bmp所有信息
    photo *(*p)[] = Photo_Init_Room(PHOTO_NUM, PATH);
    // Photo_Init(BMP1_PATH,&bmp_photo);

    //映射fb0文件   获取LCD的所有信息，并且映射lcd虚拟区。
    lcd *m = Fb0_Mapping_Init();

    //把图片的像素点写进lcd的映射内存中
    Write_Pixel_Room(m, p);
    // Write_Pixel(m.fb0_mapping,&bmp_photo);

    touch_screen *ts_p = Touch_Screen_Init();

    //切换可见区
    //Change_Spotlight(m);

    
        Change_Spotlight_Event(m,ts_p);
    

    //释放
    Free(m);

    //循环显示三张图片
    // char* arr[3] = {BMP1_PATH,BMP2_PATH,BMP3_PATH};
    // unsigned int count = 0;
    // while (1)
    // {
    //     Photo_Init(arr[count%3],&bmp_photo);
    //     Fb0_Mapping_Init(&m);
    //     Write_Pixel(m.fb0_mapping,&bmp_photo);
    //     Free(&m,&bmp_photo);

    //     count++;
    //     sleep(2);
    // }

    return 0;
}

void Free(lcd *m)
{
    close(m->fd_fb0);
    munmap(m->fb0_mapping, m->ffs->smem_len);
    free(m->ffs);
    free(m->fvs);
    free(m);

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

lcd *Fb0_Mapping_Init()
{
    lcd *lcd_p = (lcd *)malloc(sizeof(lcd));
    lcd_p->ffs = (struct fb_fix_screeninfo *)malloc(sizeof(struct fb_fix_screeninfo));
    lcd_p->fvs = (struct fb_var_screeninfo *)malloc(sizeof(struct fb_var_screeninfo));
    //打开设备文件

    lcd_p->fd_fb0 = open(FB0_PATH, O_RDWR);
    if (lcd_p->fd_fb0 == -1)
    {
        // perror("fd_fb0:");
        printf("fd_fb0 open failed:%s\n", strerror(errno));
        return;
    }

    //获取lcd_p硬件设备信息
    int io_res1 = ioctl(lcd_p->fd_fb0, FBIOGET_FSCREENINFO, lcd_p->ffs);
    int io_res2 = ioctl(lcd_p->fd_fb0, FBIOGET_VSCREENINFO, lcd_p->fvs);
    if (io_res1 == -1 || io_res2 == -1)
    {
        printf("ioctl failed:%s\n", strerror(errno));
        return;
    }
    //映射内存
    printf("显存的大小是%d\n", lcd_p->ffs->smem_len);

    lcd_p->fb0_mapping = mmap(NULL, lcd_p->ffs->smem_len,
                              PROT_READ | PROT_WRITE,
                              MAP_SHARED, lcd_p->fd_fb0, 0);

    printf("mapping出来的首地址是%p\n", lcd_p->fb0_mapping);
    printf("mapping出来的尾地址是%p\n", lcd_p->fb0_mapping + (800 * 480 * 3));

    if (lcd_p->fb0_mapping == MAP_FAILED)
    {
        printf("mapping failed:%s\n", strerror(errno));
        return;
    }

    return lcd_p;
}

void Change_Spotlight(lcd *lcd)
{
    int count = 0;
    while (1)
    {
        if (count > 2)
            count = 0;

        lcd->fvs->xoffset = 0;
        lcd->fvs->yoffset = lcd->fvs->yres * count;
        ioctl(lcd->fd_fb0, FB_ACTIVATE_NOW, lcd->fvs);
        ioctl(lcd->fd_fb0, FBIOPAN_DISPLAY, lcd->fvs);
        sleep(2);
        count++;
    }
}

void Write_Pixel(int* mapping_p, photo* photo_p)
{
    char* tmp = photo_p->pixel_point;
    // int* mapping_p_copy = mapping_p;

    printf("%d\n", photo_p->file_head.bfSize);
    printf("%d\n", photo_p->info_head.biSizeImage);
    printf("%d\n", photo_p->file_head.bfOffset);

    int alignment = photo_p->info_head.biSizeImage - //求对齐总数
                    (photo_p->info_head.biHeight * photo_p->info_head.biWidth * 3);

    printf("字节对齐的总数为%d\n", alignment);

    mapping_p += 800 * (240 - photo_p->info_head.biHeight / 2) +
                 (400 - photo_p->info_head.biWidth / 2);

    int x, y;
    for (y = photo_p->info_head.biHeight; y > 0; y--)
    {
        for (x = 0; x < photo_p->info_head.biWidth; x++)
        {
            *(mapping_p + (y - 1) * 800 + x) = *(tmp + (x * 3)) << 0 |
                                               *(tmp + (x * 3 + 1)) << 8 |
                                               *(tmp + (x * 3 + 2)) << 16;
        }
        tmp += photo_p->info_head.biWidth * 3;
        if (alignment)
        {
            tmp += alignment / photo_p->info_head.biHeight;
        }
    }

    // free(photo_p->pixel_point);
}

void Write_Pixel_Room(lcd *lcd, photo *(*p)[])
{
    printf("%d\n", lcd->fvs->xres_virtual);
    printf("%d\n", lcd->fvs->yres_virtual);
    printf("%d\n", lcd->fvs->xres);
    printf("%d\n", lcd->fvs->yres);

    // int room = (lcd->fvs->xres_virtual * lcd->fvs->yres_virtual) /
    //            (lcd->fvs->xres * lcd->fvs->yres);
    int room = 3;

    printf("room的值是%d\n", room);

    int *mapping_copy = NULL;

    int i;
    for (i = 0; i < room; i++)
    {
        mapping_copy = lcd->fb0_mapping;
        mapping_copy += (lcd->fvs->xres * lcd->fvs->yres * i);
        Write_Pixel(mapping_copy, *(*p + i));
    }

    //把图片像素数据写进去，就把图片所有有关的文件和内存释放
    for (i = 0; i < room; i++)
    {
        close((*(*p + i))->fd);
        free((*(*p + i))->pixel_point);
        free((*(*p + i)));
    }
    free(p);
}

void Photo_Init(char *address, photo *photo_p)
{
    photo_p->fd = open(address, O_RDONLY);
    if (photo_p->fd == -1)
    {
        printf("open failed:%s\n", strerror(errno));
        return;
    }

    //把图片文件头读取出来
    if (read(photo_p->fd, &(photo_p->file_head), 14) == -1)
    {
        printf("read file_head failed:%s\n", strerror(errno));
        return;
    }

    //把图片信息头读取出来
    if (read(photo_p->fd,
             &(photo_p->info_head),
             photo_p->file_head.bfOffset - sizeof(bitmap_file_head)) == -1)
    {
        printf("read info_head failed:%s\n", strerror(errno));
        return;
    }

    photo_p->pixel_point = (char *)malloc(photo_p->info_head.biSizeImage);
    if (photo_p->pixel_point == NULL)
    {
        printf("malloc:%s\n", strerror(errno));
        return;
    }

    if (read(photo_p->fd, photo_p->pixel_point, photo_p->info_head.biSizeImage) == -1)
    {
        printf("read image_info failed:%s\n", strerror(errno));
        return;
    }

    printf("%d\n", photo_p->info_head.biSizeImage);

    printf("malloc出来的地址是%p\n", photo_p->pixel_point);

    // return photo_p->fd;
    //   printf("%x\n",photo_p->file_head.bfType);
    //   printf("%d\n",photo_p->file_head.bfSize);
    //   printf("%d\n",photo_p->file_head.bfReserved1);
    //   printf("%d\n",photo_p->file_head.bfReserved2);
    //   printf("%d\n",photo_p->file_head.bfOffset);

    // printf("%d\n",photo_p->info_head.biBitCount);
    // printf("%d\n",photo_p->info_head.biHeight);
    // printf("%d\n",photo_p->info_head.biWidth);
    // printf("%d\n",photo_p->info_head.biSizeImage);
    // printf("%d\n",photo_p->info_head.biBitCount);
    // printf("%d\n",photo_p->info_head.biBitCount);
    // printf("%d\n",photo_p->info_head.biBitCount);
}

photo *(*Photo_Init_Room(int num, char **add_arr))[]
{
    photo *(*p)[num] = (photo * (*)[num]) malloc(sizeof(photo *) * num);
    if (p == NULL)
    {
        printf("(line 265)malloc failed:%s\n", strerror(errno));
    }

    //printf("malloc-----%p\n------", p);

    //printf("malloc-----%p\n---xx---", *(*p));
    int i;
    for (i = 0; i < num; i++)
    {
        *(*p + i) = (photo *)malloc(sizeof(photo));
        Photo_Init(add_arr[i], *(*p + i));
    }

    return p;
}

touch_screen *Touch_Screen_Init()
{
    touch_screen* touch_screen_p = (touch_screen*)malloc(sizeof(touch_screen));
    touch_screen_p->touch_info = (struct input_event *)malloc(sizeof(struct input_event));
    if (touch_screen_p == NULL || touch_screen_p->touch_info == NULL)
    {
        printf("touch_screen malloc failed:%s\n", strerror(errno));
        return (touch_screen *)-1;
    }

    touch_screen_p->touch_screen_fd = open("/dev/input/event0", O_RDWR);
    if (touch_screen_p->touch_screen_fd == -1)
    {
        printf("touch_screen_fd open failed:%s\n", strerror(errno));
        return (touch_screen *)-1;
    }

    //注意read这里堵塞不能写在初始化，然后这个touch_info是空指针。
    // read(touch_screen_p->touch_screen_fd,touch_screen_p->touch_info,sizeof(struct input_event));

    return touch_screen_p;
}

int GetEvent(touch_screen *ts_p)
{
    //滑动计时变量
    long long sliding_timing = 0; //记录滑动的时间（即一次按下和松开之间的时间）

    //双击计时变量
    long long now_click_timing = 2;    //记录当前点击（按下）的时间
    long long before_click_timing = 0; //记录上一次点击（按下）的时间

    //坐标变量，记录坐标信息
    int end_x = 0, end_y = 0;
    int start_x = -1, start_y = -1;

    //清空坐标信息标志位
    int flag = 0;

    while (1)
    {

        read(ts_p->touch_screen_fd, ts_p->touch_info, sizeof(struct input_event));

        //按键事件信息获取
        if (ts_p->touch_info->type == EV_KEY)
        {
            if (ts_p->touch_info->code == BTN_TOUCH)
            {
                if (ts_p->touch_info->value == 1)
                {
                    sliding_timing = ts_p->touch_info->time.tv_sec;

                    before_click_timing = now_click_timing;
                    now_click_timing = ts_p->touch_info->time.tv_sec;

                    flag = 0;
                }

                if (ts_p->touch_info->value == 0)
                {
                    sliding_timing = ts_p->touch_info->time.tv_sec - sliding_timing;

                    flag = 1;
                }
            }
        }

        //绝对坐标事件信息获取
        if (ts_p->touch_info->type == EV_ABS)
        {

            if (ts_p->touch_info->code == ABS_X)
            {
                if (start_x == -1)
                {
                    start_x = ts_p->touch_info->value * 800 / 1024;
                }
                end_x = ts_p->touch_info->value * 800 / 1024;
            }

            if (ts_p->touch_info->code == ABS_Y)
            {
                if (start_y == -1)
                {
                    start_y = ts_p->touch_info->value * 480 / 600;
                }
                end_y = ts_p->touch_info->value * 480 / 600;
            }
        }

        //事件判定
        if (sliding_timing < 2 && start_y - end_y > 150)
        {
            printf("判定为上滑\n");
            return 1;
        }

        if (sliding_timing < 2 && end_y - start_y > 150)
        {
            printf("判定为下滑\n");
            break;
        }

        if (sliding_timing < 2 && end_x - start_x > 150)
        {
            printf("判定为右滑\n");
            break;
        }

        if (sliding_timing < 2 && start_x - end_x > 150)
        {
            printf("判定为左滑\n");
            break;
        }

        if ((now_click_timing - before_click_timing) < 2)
        {
            printf("判定为双击\n");
            break;
        }

        //一松手就把坐标数据清空，重新获取
        if (flag)
        {
            start_x = -1;
            start_y = -1;

            end_x = 0;
            end_y = 0;
        }

        // printf("sliding_timing的值,即时间差:%lld\n", sliding_timing);

        // printf("start x : %d\n", start_x);
        // printf("start y : %d\n", start_y);

        // printf("==============\n");

        // printf("end x : %d\n", end_x);
        // printf("end y : %d\n", end_y);

        // printf("timeS=%d,timeUS=%d,type=%d,code=%d,value=%d\n",
        //        ts_p->touch_info->time.tv_sec, ts_p->touch_info->time.tv_usec,
        //        ts_p->touch_info->type, ts_p->touch_info->code, ts_p->touch_info->value);

        // printf("==============\n");

        // printf("now_click_timing的值:%lld\n", now_click_timing);
        // printf("==============\n");
        // printf("before_click_timing的值:%lld\n", before_click_timing);
        // printf("==============\n");
        // printf("now_click_timing - before_click_timing:%lld\n",
        // now_click_timing - before_click_timing);
    }

}

void Change_Spotlight_Event(lcd *lcd, touch_screen *ts_p)
{
    int count = 0;

    while(1)
    {
        if(GetEvent(ts_p) == 1)
        {
            if (count > 2)count = 0;
            
            lcd->fvs->xoffset = 0;
            lcd->fvs->yoffset = lcd->fvs->yres * count;
            ioctl(lcd->fd_fb0, FB_ACTIVATE_NOW, lcd->fvs);
            ioctl(lcd->fd_fb0, FBIOPAN_DISPLAY, lcd->fvs);
            count++;
        }
    }
}
