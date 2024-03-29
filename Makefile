GCC=g++
LD=g++
ASM_SRC =$(sort $(wildcard *.S)) 
C_SRC   =$(sort $(wildcard *.cpp)) $(sort $(wildcard src/*.cpp))
OBJ     =$(patsubst %.S,%.o,$(ASM_SRC)) $(patsubst %.cpp,%.o,$(C_SRC)) # $(patsubst pattern, replacement, text) 查找text中的单词（单词以“空格”、“Tab”或“回车”“换行”分隔）是否符合模式pattern，如果匹配的话，则以replacement替换
OUT		=run.out
CFLAG	=

all: $(OUT)
		${MAKE} -C src all

run: all
		${MAKE} -C src all
		@echo Launch the calculater ......
		@./run.out

run.out: ${OBJ}
		${MAKE} -C src all
		g++ $^ ${CFLAG} -o $@

%.o:%.S
		${GCC}  ${CFLAG} -c $<

%.o:%.c
		${GCC}  ${CFLAG} -c $<

clean:
		${MAKE} -C src clean
		$(shell rm *.o 2>/dev/null)
		$(shell rm *.out 2>/dev/null)