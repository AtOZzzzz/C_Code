# project.out:Hardware_Init.o kernel_list.o Photo.o main.o
# 	arm-linux-gcc main.o Hardware_Init.o kernel_list.o Photo.o -o project.out
# main.o:main.c
# 	arm-linux-gcc -c main.c 
# Hardware_Init.o:Hardware_Init.c 
# 	arm-linux-gcc -c Hardware_Init.c 
# kernel_list.o:kernel_list.c 	
# 	arm-linux-gcc -c kernel_list.c 
# Photo.o:Photo.c 
# 	arm-linux-gcc -c Photo.c

project2.out:main.o thread.o kernel_list.o 
	gcc *.o -o $@ -lpthread -g
%.o:%.c 	
	gcc -c $< -o $@ -lpthread -g

clean:
	rm *.o *.out