CC = clang
CFLAGS = -std=c17 -Wall -Wextra

SRCS = main.c
TARGET = ntp_time

all: $(TARGET)

$(TARGET): main.c
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
