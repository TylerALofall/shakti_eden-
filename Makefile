CC ?= cc
CFLAGS ?= -std=c99 -Wall -Wextra -Wpedantic -Werror -O2
CPPFLAGS ?= -Iinclude

SOURCES = \
	src/main.c \
	src/shakti_time.c \
	src/shakti_receptor.c \
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
	src/shakti_report.c \
	src/shakti_loader.c

OBJECTS = $(SOURCES:.c=.o)
TARGET = shakti
BUILDER = build_xml
LEDGER = build_ledger
SEED_BUILDER = build_seed_curriculum

TEST_SOURCES = \
	tests/test_shakti.c \
	src/shakti_time.c \
	src/shakti_receptor.c \
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
	src/shakti_report.c \
	src/shakti_loader.c

.PHONY: all clean test run builder eyes

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

test: $(TARGET) tests/test_shakti tests/test_integration tests/test_roundtrip
	./tests/test_shakti
	./tests/test_integration
	./tests/test_roundtrip

tests/test_roundtrip: tests/test_roundtrip.c eyes/eyes.c eyes/eyes.h
	$(CC) $(CFLAGS) -Ieyes tests/test_roundtrip.c \
		eyes/eyes.c -o tests/test_roundtrip

tests/test_shakti: $(TEST_SOURCES)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(TEST_SOURCES) -o tests/test_shakti

tests/test_integration: tests/test_integration.c src/main.c $(TEST_SOURCES) \
		tools/build_xml.c tools/build_ledger.c tools/build_seed_curriculum.c \
		tests/make_wav_fixture.c
	$(CC) $(CPPFLAGS) -Itools -Itests -DSHAKTI_APP_NO_MAIN \
		-DSHAKTI_TOOL_NO_MAIN $(CFLAGS) \
		tests/test_integration.c src/main.c \
		src/shakti_time.c src/shakti_log.c src/shakti_memory.c \
		src/shakti_reason.c src/shakti_school.c src/shakti_loop.c \
		src/shakti_handwriting.c src/shakti_asset.c src/shakti_artifact.c \
		src/shakti_tablet.c src/shakti_manifest.c src/shakti_score.c \
		src/shakti_report.c src/shakti_loader.c \
		tools/build_xml.c tools/build_ledger.c tools/build_seed_curriculum.c \
		tests/make_wav_fixture.c -o tests/test_integration

tests/make_wav_fixture: tests/make_wav_fixture.c
	$(CC) $(CFLAGS) tests/make_wav_fixture.c -o tests/make_wav_fixture

run: $(TARGET)
	./$(TARGET)

builder: $(BUILDER)

# eyes: self-contained document collector + reconstruction harness.
# Runs from the repository root; writes artifacts into eyes/output/.
eyes/eyes_map: eyes/eyes_map.c eyes/eyes.c eyes/eyes.h
	$(CC) $(CFLAGS) -Ieyes eyes/eyes_map.c eyes/eyes.c -o eyes/eyes_map

eyes: eyes/eyes_map
	./eyes/eyes_map

clean:
	rm -f $(OBJECTS) $(TARGET) $(BUILDER) $(LEDGER) $(SEED_BUILDER) \
	tests/test_shakti tests/test_integration tests/make_wav_fixture \
	tests/test_roundtrip
	rm -rf tests/tmp_builder tests/tmp_loop tests/tmp_seed tests/tmp_mvp
	rm -f tests/test_facts.txt tests/test_thesaurus.txt
	rm -f tests/test_evidence.log tests/test_stream.log tests/test_school.log
	rm -f tests/test_goal.txt tests/test_notebook.log tests/test_menu.txt
	rm -f tests/test_long_term.log tests/test_loader_fixture.txt

.PHONY: eyes-xml-collect eyes-xml-rebuild eyes-xml

eyes/eyes_xml_collect: eyes/eyes_xml_collect.c eyes/eyes_xml.h eyes/eyes.h eyes/eyes.c
	$(CC) $(CFLAGS) -Ieyes eyes/eyes_xml_collect.c eyes/eyes.c -o eyes/eyes_xml_collect

eyes/eyes_xml_rebuild: eyes/eyes_xml_rebuild.c eyes/eyes_xml.h eyes/eyes.h eyes/eyes.c
	$(CC) $(CFLAGS) -Ieyes eyes/eyes_xml_rebuild.c eyes/eyes.c -o eyes/eyes_xml_rebuild

eyes-xml-collect: eyes/eyes_xml_collect
	./eyes/eyes_xml_collect

eyes-xml-rebuild: eyes/eyes_xml_rebuild
	./eyes/eyes_xml_rebuild

eyes-xml: eyes/eyes_xml_collect eyes/eyes_xml_rebuild
	./eyes/eyes_xml_collect
	./eyes/eyes_xml_rebuild

.PHONY: eyes-loop

eyes/eyes_loop_rebuild: eyes/eyes_loop_rebuild.c eyes/eyes_xml.h eyes/eyes.h eyes/eyes.c
	$(CC) $(CFLAGS) -Ieyes eyes/eyes_loop_rebuild.c eyes/eyes.c -o eyes/eyes_loop_rebuild

eyes-loop: eyes/eyes_loop_rebuild
	./eyes/eyes_loop_rebuild

.PHONY: binary-deposit

# binary: the pixel deposit. Reads binary/page1_picture.txt and
# binary/page2_text.txt, deposits 5 binary marks per pixel location into
# dated per-page files, rebuilds from the deposit, writes the dated PDF.
binary/binary_deposit: binary/binary_deposit.c eyes/eyes.h eyes/eyes.c
	$(CC) $(CFLAGS) -Ieyes binary/binary_deposit.c eyes/eyes.c -o binary/binary_deposit

binary-deposit: binary/binary_deposit
	./binary/binary_deposit
