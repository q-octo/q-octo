MG_PACK = packed_fs/pack
MG_PACK_OUT = include/packed_fs.h
PACKED_FS_FILES = $(wildcard packed_fs/files/*)

all: $(PACKED_FS_FILES) $(MG_PACK) $(MG_PACK_OUT)

# Rule to build the mongoose file system packer
$(MG_PACK): packed_fs/pack.c
	cc -o ./packed_fs/pack packed_fs/pack.c

# Rule to build the packed_fs.h (read only packed file system)
$(MG_PACK_OUT): $(MG_PACK) $(PACKED_FS_FILES)
	cd packed_fs/files && find . -type f | sed 's|^\./||' | xargs ../pack > ../../include/packed_fs.h


clean:
	rm -rf $(MG_PACK) $(MG_PACK_OUT) packed_fs/files/*