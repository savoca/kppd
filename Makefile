CC=$(CROSS_COMPILE)gcc
AR=$(CROSS_COMPILE)ar

ifneq ($(SYSROOT),)
	CFLAGS += --sysroot $(SYSROOT)
endif

all:
	@mkdir -p ./out
	@$(CC) $(CFLAGS) ./src/kcal.c ./src/pcc.c ./src/pa.c ./src/pa_v2.c \
		./src/igc.c -o ./out/kcal -I./include -static && echo ./out/kcal

clean:
	-@rm ./out/kcal 2>/dev/null || true
	-@rmdir ./out 2>/dev/null || true
