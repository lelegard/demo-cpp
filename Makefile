default: execs

SRCDIR     = src
BINDIR     = bin
LIBFILE    = $(BINDIR)/libdemo.a
SYSTEM     = $(shell uname -s)
SOURCES   := $(notdir $(basename $(wildcard $(SRCDIR)/*.cpp)))
HEADERS   := $(notdir $(basename $(wildcard $(SRCDIR)/*.h)))
LIBOBJS   := $(patsubst %,$(BINDIR)/%.o,$(filter $(HEADERS),$(SOURCES)))
EXECS     := $(patsubst %,$(BINDIR)/%,$(filter-out $(HEADERS),$(SOURCES)))
CXXFLAGS  += -std=c++17 -Werror -Wall -Wextra -Wno-unused-parameter
CPPFLAGS  += $(addprefix -I,$(wildcard /opt/homebrew/include /usr/local/include))
ARFLAGS    = rc$(if $(findstring Darwin,$(SYSTEM)),,U)
LDFLAGS   += $(addprefix -L,$(wildcard /opt/homebrew/lib /usr/local/lib))
LDLIBS    += -lm
MAKEFLAGS += --no-print-directory

# Old gcc versions.
LDLIBS += $(if $(wildcard /usr/lib/gcc/*/*/libstdc++fs.*),-lstdc++fs)

# Define DEBUG to compile in debug mode.
CXXFLAGS += $(if $(DEBUG),-g,-O2)
LDFLAGS  += $(if $(DEBUG),-g)

execs: $(EXECS)
	@true
$(LIBFILE): $(LIBOBJS)
	$(AR) $(ARFLAGS) $@ $^
$(BINDIR)/%: $(BINDIR)/%.o $(LIBFILE)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@
$(BINDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<
clean:
	rm -rf bin .vs *.tmp *.log __pycache__
clang:
	@$(MAKE) CPP=clang++ CXX=clang++

# Regenerate implicit dependencies.
ifneq ($(if $(MAKECMDGOALS),$(filter-out clean listvars clang cxxmacros,$(MAKECMDGOALS)),true),)
    -include $(patsubst %,$(BINDIR)/%.d,$(SOURCES))
endif
$(BINDIR)/%.d: $(SRCDIR)/%.cpp
	@mkdir -p $(BINDIR)
	$(CXX) -MM $(CPPFLAGS) -MT $(BINDIR)/$*.o -MT $@ $< >$@ || rm -f $@

# Display make variables for debug purposes.
listvars:
	@true
	$(foreach v, \
	  $(sort $(filter-out .% ^% @% _% *% \%% <% +% ?% BASH% LS_COLORS SSH% VTE% XDG% F_%,$(.VARIABLES))), \
	  $(info $(v) = "$($(v))"))

# Display C++ predefined macros for debug purposes.
cxxmacros:
	@$(CPP) $(CXXFLAGS) -x c++ -dM /dev/null | sort
