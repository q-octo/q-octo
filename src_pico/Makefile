MG_PACK = packed_fs/pack
MG_PACK_OUT = include/packed_fs.h
FLATC = ../flatbuffers/flatc
FBS_REPO_DIR = ../flatbuffers
FBS_SRC_DIR = ../flatbuffers_schemas
FBS_OUT_DIR = include
# Find all .fbs files in the FBS_SRC_DIR
FBS_FILES = $(wildcard $(FBS_SRC_DIR)/*.fbs)
FBS_OUT_FILES = $(patsubst $(FBS_SRC_DIR)/%.fbs,$(FBS_OUT_DIR)/%_generated.h,$(FBS_FILES))

all: $(MG_PACK) $(MG_PACK_OUT) $(FLATC) $(FBS_OUT_FILES) 

# Rule to build the mongoose file system packer
$(MG_PACK): packed_fs/pack.c
	cc -o ./packed_fs/pack packed_fs/pack.c

# Rule to build the packed_fs.h (read only packed file system)
$(MG_PACK_OUT): packed_fs/pack packed_fs/files/*
	cd packed_fs/files && ../pack * > ../../include/packed_fs.h && cd ../..

# Rule to build flatc (flatbuffers compiler)
$(FLATC):
	cd $(FBS_REPO_DIR) && cmake -G "Unix Makefiles" && make -j

# Rule to convert .fbs to .h
$(FBS_OUT_DIR)/%_generated.h: $(FBS_SRC_DIR)/%.fbs
	$(FLATC) -o $(FBS_OUT_DIR) -c $<

clean: 
	rm -f packed_fs/pack include/packed_fs.h $(FBS_OUT_FILES)