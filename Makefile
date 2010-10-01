timer-accuracy: main.c
	gcc $< -o $@ -lrt -Wall -Wextra -O2 -g

clean:
	rm -f timer-accuracy
