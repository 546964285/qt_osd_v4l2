KERNEL_DIR=/home/andy/RidgeRun-SDK-DM36x-Turrialba-Eval/kernel/src
CROSS_COMPILE = arm-none-linux-gnueabi-
CFLAGS = -Wall -I $(KERNELDIR)/include
CC = $(CROSS_COMPILE)gcc

all: button_test
button: button.o
	$(CC) -o button -lpthread $^	
button.o:button.c
	$(CC) $(CFLAGS) -c $^
clean:
	rm -f *.o *~ button_test