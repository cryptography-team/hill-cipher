empty :=
space := $(empty) $(empty)

SRCDIR := ./src
INCDIR := ./include
OBJDIR := ./build/obj
DEPDIR := ./build/deps
BINDIR := .

TARGET := $(BINDIR)/$(notdir $(subst $(space),_,$(realpath .)))_
ifeq ($(RELEASE), 1)
	TARGET := $(TARGET)Release
else
	TARGET := $(TARGET)Debug
endif
TARGET := $(TARGET).exe

MY_PATHS := $(BINDIR) $(INCDIR)
MY_FLAGS := 

###### extra variables #######
MY_PATHS += $(file <.my_paths)
MY_FLAGS += $(file <.my_flags)

###### complier set-up ######
CC = gcc
CFLAGS = $(MY_FLAGS) -Wextra -Wno-unused-result
CXX = g++
CXXFLAGS = $(CFLAGS) -std=c++17
LD = g++
LDFLAGS = $(CXXFLAGS)
DEBUGGER = gdb

ifeq ($(RELEASE), 1)
	maketype := RELEASE
	CFLAGS += -O2 -ftree-vectorize -fomit-frame-pointer -march=native
	# Link Time Optimization
	CFLAGS += -flto
else
	maketype := DEBUG
	CFLAGS += -Og -ggdb2 -DDEBUG=1
	# Overflow protection
	CFLAGS += -D_FORTIFY_SOURCE=2 -fstack-protector-strong -fstack-clash-protection -fcf-protection
	CFLAGS += -Wl,-z,defs -Wl,-z,now -Wl,-z,relro
	CXXFLAGS += -D_GLIBCXX_ASSERTIONS
endif

CFLAGS += -MMD -MP -I$(SRCDIR) $(foreach i,$(MY_PATHS),-I$(i))

SRCS := $(wildcard $(SRCDIR)/*.cpp)
SRCS += $(wildcard $(SRCDIR)/*.c)

DEPS := $(patsubst $(SRCDIR)/%,$(DEPDIR)/%.d,$(SRCS))
OBJS := $(patsubst $(SRCDIR)/%,$(OBJDIR)/%.$(maketype).o,$(SRCS))

.PHONY: all
all : $(TARGET)

.PHONY: getTarget
getTarget :
	@echo $(TARGET)

.PHONY: run
run : $(TARGET)
	@echo --------------------------------------------------
	@\time -f "\n--------------------------------------------------\nElapsed Time: %e sec\nCPU Percentage: %P\n"\
		$(TARGET) $(ARGUMENTS)

.PHONY: init
init :
	-@rm -rf build $(wildcard *.exe)
	@mkdir -p $(SRCDIR) $(INCDIR) $(OBJDIR) $(DEPDIR)
	-@for i in $(wildcard *.cpp) $(wildcard *.c) $(wildcard *.tpp); do mv ./$$i $(SRCDIR)/$$i; done
	-@for i in $(wildcard *.h) $(wildcard *.hpp); do mv ./$$i $(INCDIR)/$$i; done
	-@$(file >$(SRCDIR)/.clang_complete)\
		$(foreach i,$(MY_PATHS),\
			$(file >>$(SRCDIR)/.clang_complete,-I$(i))\
			$(file >>$(SRCDIR)/.clang_complete,-I../$(i)))

$(TARGET) : $(OBJS)
	-@echo LD $(maketype) "$(<D)/*.o" "->" $@ && \
		$(LD) -o $@ $(OBJS) $(LDFLAGS)

$(OBJDIR)/%.cpp.$(maketype).o : $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR) $(DEPDIR)
	-@echo CXX $(maketype) $< "->" $@ && \
		$(CXX) -c $< -o $@ -MF $(DEPDIR)/$(<F).d $(CXXFLAGS)

$(OBJDIR)/%.c.$(maketype).o : $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR) $(DEPDIR)
	-@echo CC $(maketype) $< "->" $@ && \
		$(CC) -c $< -o $@ -MF $(DEPDIR)/$(<F).d $(CFLAGS)

.PHONY: clean
clean : 
	-$(RM) $(OBJS) $(DEPS) $(TARGET)

.PHONY: debug
debug : $(TARGET)
	$(DEBUGGER) $(TARGET)

-include $(DEPS)
