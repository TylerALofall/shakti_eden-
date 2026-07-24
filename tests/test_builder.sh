#!/bin/sh
set -eu

ROOT=$(CDPATH= cd -- "$(dirname "$0")/.." && pwd)
TMP="$ROOT/tests/tmp_builder"

rm -rf "$TMP"
mkdir -p \
    "$TMP/eden_out/Visual_text" \
    "$TMP/eden_out/Visual_art" \
    "$TMP/eden_out/Sound_art" \
    "$TMP/data/eden/XML_text" \
    "$TMP/data/learned" \
    "$TMP/data/school" \
    "$TMP/data/control"

printf 'A\na\nWelcome to Eden\n' > "$TMP/list.txt"

for key in A a Welcome_to_Eden
do
    printf '<svg xmlns="http://www.w3.org/2000/svg"></svg>\n' \
        > "$TMP/eden_out/Visual_art/$key.svg"
    "$ROOT/tests/make_wav_fixture" \
        "$TMP/eden_out/Sound_art/$key.wav"
done

"$ROOT/build_xml" \
    "$TMP/list.txt" \
    "$TMP/data/eden/XML_text/alphabet_solo.xml" \
    "alphabet" \
    "alphabet_solo" \
    "$TMP/eden_out"

grep -q '<tablet schema="SHAKTI_TABLET_4S_V2">' \
    "$TMP/data/eden/XML_text/alphabet_solo.xml"
grep -q '<stone order="1">' \
    "$TMP/data/eden/XML_text/alphabet_solo.xml"
grep -q '<text>A</text>' \
    "$TMP/data/eden/XML_text/alphabet_solo.xml"
grep -q '<written_text>A.8x8.txt</written_text>' \
    "$TMP/data/eden/XML_text/alphabet_solo.xml"
grep -q '<sound_art>A.wav</sound_art>' \
    "$TMP/data/eden/XML_text/alphabet_solo.xml"
grep -q '<visual_art>A.svg</visual_art>' \
    "$TMP/data/eden/XML_text/alphabet_solo.xml"
grep -q '<text>Welcome to Eden</text>' \
    "$TMP/data/eden/XML_text/alphabet_solo.xml"
grep -q '<written_text>Welcome_to_Eden.8x8.txt</written_text>' \
    "$TMP/data/eden/XML_text/alphabet_solo.xml"
grep -q '^TEXT=A$' \
    "$TMP/eden_out/Visual_text/A.8x8.txt"
grep -q '^TEXT=Welcome to Eden$' \
    "$TMP/eden_out/Visual_text/Welcome_to_Eden.8x8.txt"

cat > "$TMP/data/eden/manifest.xml" <<'XML'
<?xml version="1.0" encoding="UTF-8"?>
<manifest schema="SHAKTI_EDEN_MANIFEST_V1">
  <status>OPEN</status>
  <artifact_root>../../eden_out</artifact_root>
  <level order="0">
    <id>foundation_ascii</id>
    <title>ASCII and handwriting</title>
    <requires>none</requires>
  </level>
  <level order="2">
    <id>alphabet</id>
    <title>Alphabet identity and elasticity</title>
    <requires>foundation_ascii</requires>
  </level>
  <tablet order="1">
    <level>alphabet</level>
    <lesson>alphabet_solo</lesson>
    <mode>solo</mode>
    <requires>foundation_ascii</requires>
    <state>ready</state>
    <path>XML_text/alphabet_solo.xml</path>
  </tablet>
</manifest>
XML

"$ROOT/build_ledger" \
    "$TMP/data/eden/manifest.xml" \
    "$TMP/data/eden/manifest_ledger.tsv"

grep -q 'alphabet_solo.*Welcome to Eden.*Welcome_to_Eden.*100.*YES.*VERIFIED' \
    "$TMP/data/eden/manifest_ledger.tsv"

cat > "$TMP/data/eden/invalid_manifest.xml" <<'XML'
<?xml version="1.0" encoding="UTF-8"?>
<manifest schema="SHAKTI_EDEN_MANIFEST_V1">
  <status>OPEN</status>
  <artifact_root>../../eden_out</artifact_root>
  <level order="1">
    <id>counting</id>
    <title>Counting</title>
    <requires>missing_foundation</requires>
  </level>
</manifest>
XML

if "$ROOT/build_ledger" \
    "$TMP/data/eden/invalid_manifest.xml" \
    "$TMP/data/eden/invalid_ledger.tsv" >/dev/null 2>&1
then
    echo "Invalid prerequisites were accepted." >&2
    exit 1
fi

printf 'two times two|four|multiplication|test\n' \
    > "$TMP/data/eden/eden_facts.txt"
printf 'after|follows\n' > "$TMP/data/eden/thesaurus.txt"
printf '# Eden stream\n' > "$TMP/data/eden/eden_stream.log"
printf '# Learned\n' > "$TMP/data/learned/learned_stream.log"
printf '# Evidence\n' > "$TMP/data/learned/evidence.log"
printf '# School\n' > "$TMP/data/school/school_state.log"
printf 'Learn one grounded relationship.\n' \
    > "$TMP/data/control/goal.txt"
printf '# Notebook\n' > "$TMP/data/control/notebook.log"
cat > "$TMP/data/control/menu.txt" <<'MENU'
[Tablet]
Load and validate a four-channel tablet.
[Manifest]
Load the Eden manifest and generate its ledger.
[Reflection]
Run the full reflection questions.
MENU
printf '# Messages\n' > "$TMP/data/control/messages.log"
printf '# Reflections\n' > "$TMP/data/control/reflections.log"

(
    cd "$TMP"
    printf '/shakti_run/ manifest data/eden/manifest.xml data/eden/runtime_ledger.tsv\n/shakti_run/ tablet data/eden/XML_text/alphabet_solo.xml eden_out\n/quit/\n' \
        | "$ROOT/shakti" > runtime_output.txt
)

grep -q 'Eden manifest status OPEN. Levels 2. Tablets 1.' \
    "$TMP/runtime_output.txt"
grep -q 'Tablet level alphabet, lesson alphabet_solo, stones 3.' \
    "$TMP/runtime_output.txt"
grep -q 'written_text=A.8x8.txt' "$TMP/runtime_output.txt"
grep -q 'Shakti sleeping.' "$TMP/runtime_output.txt"

echo "Manifest, ledger, builder, and runtime integration passed."
