MG_PACK = packed_fs/pack
MG_PACK_OUT = include/packed_fs.h
PACKED_FS_FILES = $(wildcard packed_fs/files/*)
FLATC = ../flatbuffers/flatc
FBS_REPO_DIR = ../flatbuffers
FBS_SRC_DIR = ../flatbuffers_schemas
FBS_OUT_DIR = include
FBS_TS_OUT_DIR = packed_fs/web-dashboard/src/generated
# Find all .fbs files in the FBS_SRC_DIR
FBS_FILES = $(wildcard $(FBS_SRC_DIR)/*.fbs)
FBS_OUT_FILES = $(patsubst $(FBS_SRC_DIR)/%.fbs,$(FBS_OUT_DIR)/%_generated.h,$(FBS_FILES))
FBS_TS_OUT_FILES = $(patsubst $(FBS_SRC_DIR)/%.fbs,$(FBS_TS_OUT_DIR)/%_generated.ts,$(FBS_FILES))
FBS_DART_OUT_FILES = $(patsubst $(FBS_SRC_DIR)/%.fbs,$(FBS_OUT_DIR)/%_generated.dart,$(FBS_FILES))
DASH_SRC_DIR = packed_fs/web-dashboard/
DASH_SRC_FILES = $(wildcard $(DASH_SRC_DIR)/*)

all: web-dash $(PACKED_FS_FILES) $(MG_PACK) $(MG_PACK_OUT) $(FLATC) $(FBS_OUT_FILES) $(FBS_TS_OUT_FILES)

# Rule to build the web dashboard
# TODO this shouldn't run every time
web-dash:
	cd $(DASH_SRC_DIR) && bun install && bun run build --emptyOutDir

# Rule to build the mongoose file system packer
$(MG_PACK): packed_fs/pack.c
	cc -o ./packed_fs/pack packed_fs/pack.c

# Rule to build the packed_fs.h (read only packed file system)
$(MG_PACK_OUT): $(MG_PACK) $(PACKED_FS_FILES)
	cd packed_fs/files && find . -type f | sed 's|^\./||' | xargs ../pack > ../../include/packed_fs.h

# Rule to build flatc (flatbuffers compiler)
$(FLATC):
	cd $(FBS_REPO_DIR) && cmake -G "Unix Makefiles" && make -j

# Rule to convert .fbs to .h
$(FBS_OUT_DIR)/%_generated.h: $(FBS_SRC_DIR)/%.fbs
	$(FLATC) -o $(FBS_OUT_DIR) -c $<

# Rule to convert .fbs to .ts (TypeScript)
ts: $(FBS_TS_OUT_FILES)
$(FBS_TS_OUT_FILES): $(FBS_FILES)
	rm -rf $(FBS_TS_OUT_DIR)/*
	$(FLATC) -o $(FBS_TS_OUT_DIR) --ts $(FBS_FILES)

# Rule to convert .fbs to .dart
dart: $(FBS_DART_OUT_FILES)
$(FBS_DART_OUT_FILES): $(FBS_FILES)
	$(FLATC) -o $(FBS_OUT_DIR) --dart $<


clean:
	rm -rf $(MG_PACK) $(MG_PACK_OUT) $(FLATC) $(FBS_OUT_FILES) packed_fs/files/* $(FBS_TS_OUT_FILES) 