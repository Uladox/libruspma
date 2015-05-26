SRC_FILES = \
	src/ruspma_symbol_constants.c \
	src/ruspma_default_symbols.c \
	src/ruspma_setup.c \
	src/ruspma_obj.c \
	src/hashmaps/obj_hashmap/ruspma_hashmap.c  \
	src/hashmaps/symbol_hashmap/ruspma_symbolmap.c \
	src/hashmaps/MurmurHash2.c \
	src/ruspma_symbol.c  \
	src/c_functs/ruspma_symbol_functs.c \
	src/c_functs/ruspma_eval.c \
	src/ruspma_c_funct.c

OBJ_PATH = bin
_OBJ_FILES = $(notdir $(SRC_FILES:.c=.o))
OBJ_FILES = $(patsubst %,$(OBJ_PATH)/%,$(_OBJ_FILES))

bin/libruspma.a: $(OBJ_FILES)
	ar -cvq bin/libruspma.a $(OBJ_FILES)

define make-obj
$(patsubst %.c, $(OBJ_PATH)/%.o, $(notdir $(1))): $(1)
	gcc -c $$< -o $$@
endef

$(foreach src,$(SRC_FILES),$(eval $(call make-obj,$(src))))

.PHONY : clean test
clean :
	-rm bin/libruspma.a $(OBJ_FILES) test
test : bin/libruspma.a
	gcc -o test test.c bin/libruspma.a
