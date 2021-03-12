CROSS_COMPILE ?= aarch64-linux-gnu-

AS = $(CROSS_COMPILE)as
LD = $(CROSS_COMPILE)ld

ASFLAGS = -g
LDFLAGS = -g -static

SRCS = prog.s
OBJS = $(SRCS:.s=.o)

EXE = prog

all: $(SRCS) $(EXE)

clean:
    rm -rf $(EXE) $(OBJS)

$(EXE): $(OBJS)
    $(LD) $(LDFLAGS) $(OBJS) -o $@

.s.o:
    $(AS) $(ASFLAGS) $< -o $@