include config.mk

OBJ_PATH             := ./obj
OBJ_PATHD            := ./objd

OUTPUTDIR            := ../output
TARGET               := libxxxxxxxxx.so

SINKDIR              := ../xx
DMRWRAPPERDIR        := ../xxx/xxxx

DMRLIBSD             := -L../input/lib -lopus -lglog -ldl -lc -lstdc++ -lpthread -lm -lasound

DMRLIBS              := $(DMRLIBSD) -lpcap



DMRWRAPPERINCDIR     := -I$(DMRWRAPPERDIR) \
						-I$(DMRWRAPPERDIR)/inc \
						-I$(DMRWRAPPERDIR)/utils \
						-I../input/inc \
                        -I../input/inc/pcap \
						-I$(SINKDIR)/utils

SINKINCDIR           := -I$(SINKDIR)/common \
                        -I$(SINKDIR)/utils

INCDIR               := $(DMRWRAPPERINCDIR)\
                        $(SINKINCDIR)

VPATH                := $(shell find $(DMRWRAPPERDIR)/ -type d)
VPATH                += $(shell find $(SINKDIR)/ -type d)
CPPSRCS              := $(foreach x, $(VPATH), $(wildcard $(addprefix $(x)/*, .cpp)))
CPPOBJS              := $(addprefix $(OBJ_PATH)/, $(addsuffix .o, $(notdir $(basename $(CPPSRCS)))))
CPPOBJSD             := $(addprefix $(OBJ_PATHD)/, $(addsuffix .o, $(notdir $(basename $(CPPSRCS)))))

.PHONY : all
all : TOUCH $(OUTPUTDIR)/$(TARGET)

CPPDS                := $(CPPOBJS:.o=.d)
CPPDSD               := $(CPPOBJSD:.o=.d)

$(OBJ_PATH)/%.d : %.cpp
	@-printf "\tMM\t%s \n" $(notdir $^)
	@set -e; rm -f $@; \
	$(CXX) $(INCDIR) -MM  $< >> $@.$$$$; \
	sed 's,/($*/)/.o[ :]*,/1.o $@ : ,g' < $@.$$$$ > $@; \
	sed -i '1s#^#$(OBJ_PATH)/&#' $@; \
	rm -f $@.$$$$

$(OBJ_PATHD)/%.d : %.cpp
	@-printf "\tMM-D\t%s \n" $(notdir $^)
	@set -e; rm -f $@; \
	$(CXX) $(INCDIR) -MM  $< >> $@.$$$$; \
	sed 's,/($*/)/.o[ :]*,/1.o $@ : ,g' < $@.$$$$ > $@; \
	sed -i '1s#^#$(OBJ_PATHD)/&#' $@; \
	rm -f $@.$$$$

ifeq "$(MAKECMDGOALS)" ""
sinclude $(CPPDS)
endif

ifeq "$(MAKECMDGOALS)" "delivery"
sinclude $(CPPDSD)
endif

TOUCH:
	@touch $(DMRWRAPPERDIR)/utils/version.cpp

$(OUTPUTDIR)/$(TARGET) : $(CPPOBJS)
	@$(CXX) -Wl,-soname,$(TARGET) -fPIC -shared -Wl,--no-undefined $(CPPOBJS) $(DMRLIBS) -o $(OUTPUTDIR)/$(TARGET)
	@-printf "\n\tLD\t %s \n" $@

$(OBJ_PATH)/%.o : %.cpp
	@-printf "\tCXX\t%s\n" $(notdir $^)
	@$(CXX) $(CXXFLAGS) $(INCDIR) -c -o $@ $<

delivery : $(CPPOBJSD)
	@$(CXX) -Wl,-soname,$(TARGET) -fPIC -shared -Wl,--no-undefined $(CPPOBJSD) $(DMRLIBSD) -o $(OUTPUTDIR)/$(TARGET)
	@-printf "\n\tLD-D\t %s \n" $(OUTPUTDIR)/$(TARGET)

$(OBJ_PATHD)/%.o : %.cpp
	@-printf "\tCXX-D\t%s\n" $(notdir $^)
	@$(CXX) $(DELIVERY_FLAGS) $(INCDIR) -c -o $@ $<

.PHONY:clean
clean:
	-rm -rf $(CPPOBJS) $(CPPOBJSD) $(OUTPUTDIR)/*.h $(OUTPUTDIR)/*.so $(CPPDS) $(CPPDSD)
