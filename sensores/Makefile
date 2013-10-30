TARGET  := hcsr04v2
WARN    := -W -Wall -Wstrict-prototypes -Wmissing-prototypes
INCLUDE := -isystem /home/alex/ts7250/linux24/include -I/usr/armrealtime/include
CFLAGS  := -O2 -DMODULE -D__KERNEL__ ${WARN} ${INCLUDE} 
CC      := /usr/local/opt/crosstool/arm-linux/gcc-3.3.4-glibc-2.3.2/bin/arm-linux-gcc
	
${TARGET}.o: ${TARGET}.c

.PHONY: clean

clean:
	rm -rf ${TARGET}.o
