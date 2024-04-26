FLATC = ../flatbuffers/flatc
FBS_REPO_DIR = ../flatbuffers
FBS_SRC_DIR = ../flatbuffers_schemas
FBS_PICO1_OUT_DIR = include
FBS_PICO2_OUT_DIR = ../src_companion/include
FBS_TS_OUT_DIR = ../src_companion/packed_fs/web-dashboard/src/generated
FBS_PYTHON_OUT_DIR = ../src_onboard

# Find all .fbs files in the FBS_SRC_DIR
FBS_FILES = $(wildcard $(FBS_SRC_DIR)/*.fbs)
FBS_PICO1_FILES = $(patsubst $(FBS_SRC_DIR)/%.fbs,$(FBS_PICO1_OUT_DIR)/%_generated.h,$(FBS_FILES))
FBS_PICO2_FILES = $(patsubst $(FBS_SRC_DIR)/%.fbs,$(FBS_PICO2_OUT_DIR)/%_generated.h,$(FBS_FILES))
FBS_TS_OUT_FILES = $(patsubst $(FBS_SRC_DIR)/%.fbs,$(FBS_TS_OUT_DIR)/%_generated.ts,$(FBS_FILES))
FBS_DART_OUT_FILES = $(patsubst $(FBS_SRC_DIR)/%.fbs,$(FBS_PICO1_OUT_DIR)/%_generated.dart,$(FBS_FILES))
FBS_PYTHON_OUT_FILES = $(patsubst $(FBS_SRC_DIR)/%.fbs,$(FBS_PYTHON_OUT_DIR)/%_generated.py,$(FBS_FILES))

all: $(FLATC) $(FBS_PICO1_FILES) $(FBS_PICO2_FILES) $(FBS_TS_OUT_FILES) $(FBS_PYTHON_OUT_FILES)

# Rule to build flatc (flatbuffers compiler)
$(FLATC):
	cd $(FBS_REPO_DIR) && cmake -G "Unix Makefiles" && make -j

# Rule to convert .fbs to .h for the main pico
$(FBS_PICO1_OUT_DIR)/%_generated.h: $(FBS_SRC_DIR)/%.fbs
	rm -rf $(FBS_PICO1_OUT_DIR)/_generated.h
	$(FLATC) --gen-object-api -o $(FBS_PICO1_OUT_DIR) --cpp $<

# Rule to convert .fbs to .h for the second pico
$(FBS_PICO2_OUT_DIR)/%_generated.h: $(FBS_SRC_DIR)/%.fbs
	rm -rf $(FBS_PICO2_OUT_DIR)/_generated.h
	$(FLATC) --gen-object-api -o $(FBS_PICO2_OUT_DIR) --cpp $<

# Rule to convert .fbs to .ts (TypeScript)
ts: $(FBS_TS_OUT_FILES)
$(FBS_TS_OUT_FILES): $(FBS_FILES)
	rm -rf $(FBS_TS_OUT_DIR)/*
	$(FLATC) --gen-object-api -o $(FBS_TS_OUT_DIR) --ts $(FBS_FILES)

# Rule to convert .fbs to python	
python: $(FBS_PYTHON_OUT_FILES)
$(FBS_PYTHON_OUT_FILES): $(FBS_FILES)
	rm -rf $(FBS_PYTHON_OUT_DIR)/*
	$(FLATC) --gen-object-api -o $(FBS_PYTHON_OUT_DIR) --python $(FBS_FILES)

# Rule to convert .fbs to .dart
dart: $(FBS_DART_OUT_FILES)
$(FBS_DART_OUT_FILES): $(FBS_FILES)
	$(FLATC) --gen-object-api -o $(FBS_PICO1_OUT_DIR) --dart $<

clean:
	rm -rf $(FLATC) $(FBS_PICO1_OUT_DIR)/*_generated.h $(FBS_PICO2_OUT_DIR)/*_generated.h $(FBS_TS_OUT_DIR)/**