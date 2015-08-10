CC=$(CROSS_COMPILE)gcc
AR=$(CROSS_COMPILE)ar

ifneq ($(SYSROOT),)
	CFLAGS += --sysroot $(SYSROOT)
endif

all:
	@mkdir -p ./out
	@$(CC) $(CFLAGS) ./src/kppd.c ./src/pp.c -o ./out/kppd \
		-I./include -static && echo ./out/kppd
	@$(CC) $(CFLAGS) ./src/kcal.c ./src/pp.c -o ./out/kcal \
		-I./include -static && echo ./out/kcal

kppd:
	@mkdir -p ./out
	@$(CC) $(CFLAGS) ./src/kppd.c ./src/pp.c -o ./out/kppd \
		-I./include -static && echo ./out/kppd

kcal:
	@mkdir -p ./out
	@$(CC) $(CFLAGS) ./src/kcal.c ./src/pp.c -o ./out/kcal \
		-I./include -static && echo ./out/kcal

clean:
	-@rm ./out/kppd 2>/dev/null || true
	-@rm ./out/kcal 2>/dev/null || true
	-@rmdir ./out 2>/dev/null || true
