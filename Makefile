CC ?= cc
CFLAGS ?= -std=c99 -Wall -Wextra -Wpedantic -Werror -O2
CPPFLAGS ?= -Iinclude

SOURCES = \
	src/main.c \
	src/shakti_time.c \
	src/shakti_log.c \
	src/shakti_memory.c \
	src/shakti_reason.c \
	src/shakti_school.c \
	src/shakti_loop.c \
	src/shakti_handwriting.c \
	src/shakti_asset.c \
	src/shakti_artifact.c \
	src/shakti_tablet.c \
	src/shakti_manifest.c \
	src/shakti_score.c \
	src/shakti_report.c

OBJECTS = $(SOURCES:.c=.o)
TARGET = shakti
BUILDER = build_xml
LEDGER = build_ledger
SEED_BUILDER = build_seed_curriculum

TEST_SOURCES = \
	tests/test_shakti.c \
	src/shakti_time.c \
	src/shakti_log.c \
	src/shakti_memory.c \
	src/shakti_reason.c \
	src/shakti_school.c \
	src/shakti_loop.c \
	src/shakti_handwriting.c \
	src/shakti_asset.c \
	src/shakti_artifact.c \
	src/shakti_tablet.c \
	src/shakti_manifest.c \
	src/shakti_score.c \
	src/shakti_report.c

.PHONY: all clean test run builder

all: $(TARGET) $(BUILDER) $(LEDGER) $(SEED_BUILDER)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET)

$(BUILDER): tools/build_xml.c src/shakti_handwriting.c \
		src/shakti_asset.c src/shakti_artifact.c
	$(CC) $(CPPFLAGS) $(CFLAGS) tools/build_xml.c \
		src/shakti_handwriting.c src/shakti_asset.c \
		src/shakti_artifact.c -o $(BUILDER)

$(LEDGER): tools/build_ledger.c src/shakti_manifest.c \
		src/shakti_tablet.c src/shakti_asset.c src/shakti_artifact.c src/shakti_score.c
	$(CC) $(CPPFLAGS) $(CFLAGS) tools/build_ledger.c \
		src/shakti_manifest.c src/shakti_tablet.c \
		src/shakti_asset.c src/shakti_artifact.c src/shakti_score.c -o $(LEDGER)


$(SEED_BUILDER): tools/build_seed_curriculum.c \
		src/shakti_handwriting.c src/shakti_asset.c src/shakti_artifact.c
	$(CC) $(CPPFLAGS) $(CFLAGS) tools/build_seed_curriculum.c \
		src/shakti_handwriting.c src/shakti_asset.c \
		src/shakti_artifact.c -o $(SEED_BUILDER)

src/%.o: src/%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

test: $(TARGET) tests/test_shakti $(BUILDER) $(LEDGER) $(SEED_BUILDER) tests/make_wav_fixture
	./tests/test_shakti
	sh ./tests/test_builder.sh
	sh ./tests/test_loop.sh
	sh ./tests/test_seed.sh
	sh ./tests/test_mvp.sh

tests/test_shakti: $(TEST_SOURCES)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(TEST_SOURCES) -o tests/test_shakti

tests/make_wav_fixture: tests/make_wav_fixture.c
	$(CC) $(CFLAGS) tests/make_wav_fixture.c -o tests/make_wav_fixture

run: $(TARGET)
	./$(TARGET)

builder: $(BUILDER)

clean:
	rm -f $(OBJECTS) $(TARGET) $(BUILDER) $(LEDGER) $(SEED_BUILDER) \
	tests/test_shakti tests/make_wav_fixture
	rm -rf tests/tmp_builder tests/tmp_loop tests/tmp_seed tests/tmp_mvp
	rm -f tests/test_facts.txt tests/test_thesaurus.txt
	rm -f tests/test_evidence.log tests/test_stream.log tests/test_school.log
	rm -f tests/test_goal.txt tests/test_notebook.log tests/test_menu.txt
	rm -f tests/test_long_term.log
