VERSION = $(shell grep -m 1 version XPS.podspec | cut -f2 -d"'")
OUTDIR  = build
SRCS    = $(wildcard libxps/*.c)
TSRCS   = $(SRCS) $(wildcard tests/*.c)
OBJS    = $(addprefix $(OUTDIR)/, $(TSRCS:.c=.o))
TARGET  = $(OUTDIR)/test
CC = clang
CFLAGS += -coverage -g -O0 \
		  -std=gnu11 -fmodules \
		  -I ./libxps
LDFLAGS += -lcheck

$(OBJS): $(OUTDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo Complie file $<
	@$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

-include $(OBJS:.o=.d)

$(TARGET): $(OBJS)
	@echo "Link project $@\n"
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

.PHONY: test clean tag

test: $(TARGET)
	@echo Start unittest:
	@rm -f $(OBJS:.o=.gcda)
	@$^
	@echo "\nTest coverage:"
	@gcov -n $(addprefix $(OUTDIR)/, $(SRCS))

clean:
	@echo Cleanup
	@rm -rf $(OUTDIR)

tag:
	@echo version ${VERSION}
	@sed -i '' 's/>[0-9]*\.[0-9]*\.[0-9]*</>${VERSION}</g' XPS-iOS/Info.plist
	@sed -i '' 's/>[0-9]*\.[0-9]*\.[0-9]*</>${VERSION}</g' XPS-macOS/Info.plist
	@sed -i '' 's/"[0-9]*\.[0-9]*\.[0-9]*"/"${VERSION}"/g' libxps/libxps.c
	@sed -i '' 's/~> [0-9]*\.[0-9]*\.[0-9]*/~> ${VERSION}/g' README.md

