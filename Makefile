NAME	:= $(shell basename $(CURDIR))

#------------------------------------------------

BUILD	:= build
SOURCE	:= .

#------------------------------------------------

SOURCES	:= $(shell find $(SOURCE) -name '*.c')
OBJECTS	:= $(addprefix $(BUILD)/,$(SOURCES:%.c=%.o))

#------------------------------------------------

CFLAGS	:= -Wall -g -lto -march=armv7-a -mfloat-abi=hard -mfpu=neon -O2 -pipe -fstack-protector-strong -fno-plt -fexceptions \
        -Wp,-D_FORTIFY_SOURCE=3 -Wformat -Werror=format-security \
        -fstack-clash-protection \
        -fno-omit-frame-pointer
LDFLAGS	:= -lc -flto

#------------------------------------------------
all: $(NAME)
#------------------------------------------------

#------------------------------------------------
$(NAME): $(OBJECTS)
#------------------------------------------------
	@echo Linking...
	@$(CC) $(LDFLAGS) $(OBJECTS) -o $(NAME)
	@echo Build done.

#------------------------------------------------
$(BUILD)/%.o: %.c
#------------------------------------------------
	@echo - $<
	@mkdir -p $(BUILD)/$(SOURCE)
	@$(CC) $(CFLAGS) -I$(dir $<) -c $< -o $@

#------------------------------------------------
clean:
#------------------------------------------------
	@rm -rf $(BUILD) $(NAME)