#!/bin/sh
set -eu

ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
TMP="$ROOT/tests/tmp_seed"

rm -rf "$TMP"
mkdir -p \
    "$TMP/data/eden/XML_text" \
    "$TMP/eden_out/Visual_text" \
    "$TMP/eden_out/Visual_art" \
    "$TMP/eden_out/Sound_art"

"$ROOT/build_seed_curriculum" \
    "$TMP/data/eden" \
    "$TMP/eden_out"

grep -q '<stone_count>95</stone_count>' \
    "$TMP/data/eden/XML_text/00_ascii_32_126_solo.xml"
grep -q '<stone_count>11</stone_count>' \
    "$TMP/data/eden/XML_text/01_counting_zero_to_ten_solo.xml"
grep -q '<duration_ms>10000</duration_ms>' \
    "$TMP/data/eden/XML_text/01_counting_zero_to_ten_timed_light.xml"

grep -q '<stone_count>26</stone_count>' \
    "$TMP/data/eden/XML_text/02_alphabet_uppercase_solo.xml"
grep -q '<stone_count>26</stone_count>' \
    "$TMP/data/eden/XML_text/02_alphabet_lowercase_solo.xml"
grep -q '<stone_count>36</stone_count>' \
    "$TMP/data/eden/XML_text/02_alphabet_case_pairs_and_counting.xml"
grep -q '<stone_count>77</stone_count>' \
    "$TMP/data/eden/XML_text/02_alphabet_growing_sequences.xml"

grep -q '<text>Aa</text>' \
    "$TMP/data/eden/XML_text/02_alphabet_case_pairs_and_counting.xml"
grep -q '<sound_art>A.wav|a.wav</sound_art>' \
    "$TMP/data/eden/XML_text/02_alphabet_case_pairs_and_counting.xml"
grep -q '<text>10 J j</text>' \
    "$TMP/data/eden/XML_text/02_alphabet_case_pairs_and_counting.xml"
grep -q '<sound_art>ten.wav|J.wav|j.wav</sound_art>' \
    "$TMP/data/eden/XML_text/02_alphabet_case_pairs_and_counting.xml"
grep -q '<sequence_kind>uppercase_prefix</sequence_kind>' \
    "$TMP/data/eden/XML_text/02_alphabet_growing_sequences.xml"
grep -q '<sequence_kind>capitalized_prefix</sequence_kind>' \
    "$TMP/data/eden/XML_text/02_alphabet_growing_sequences.xml"
grep -q '<sequence_kind>case_pair_prefix</sequence_kind>' \
    "$TMP/data/eden/XML_text/02_alphabet_growing_sequences.xml"

test "$(find "$TMP/eden_out/Visual_text" -type f | wc -l | tr -d ' ')" -ge 217
test "$(find "$TMP/eden_out/Visual_art" -type f | wc -l | tr -d ' ')" -ge 217

for letter in \
    A B C D E F G H I J K L M N O P Q R S T U V W X Y Z \
    a b c d e f g h i j k l m n o p q r s t u v w x y z
do
    "$ROOT/tests/make_wav_fixture" \
        "$TMP/eden_out/Sound_art/$letter.wav"
done

for word in one two three four five six seven eight nine ten
do
    "$ROOT/tests/make_wav_fixture" \
        "$TMP/eden_out/Sound_art/$word.wav"
done

cat > "$TMP/data/eden/manifest.xml" <<'EOF'
<?xml version="1.0" encoding="UTF-8"?>
<manifest schema="SHAKTI_EDEN_MANIFEST_V1">
  <status>OPEN</status>
  <artifact_root>../../eden_out</artifact_root>
  <level order="0">
    <id>foundation_ascii</id>
    <title>Foundation</title>
    <requires>none</requires>
  </level>
  <level order="1">
    <id>counting</id>
    <title>Counting</title>
    <requires>foundation_ascii</requires>
  </level>
  <level order="2">
    <id>alphabet</id>
    <title>Alphabet</title>
    <requires>foundation_ascii,counting</requires>
  </level>
  <tablet order="1">
    <level>alphabet</level>
    <lesson>alphabet_uppercase_solo</lesson>
    <mode>solo</mode>
    <requires>foundation_ascii,counting</requires>
    <state>staged</state>
    <path>XML_text/02_alphabet_uppercase_solo.xml</path>
  </tablet>
  <tablet order="2">
    <level>alphabet</level>
    <lesson>alphabet_lowercase_solo</lesson>
    <mode>solo</mode>
    <requires>foundation_ascii,counting</requires>
    <state>staged</state>
    <path>XML_text/02_alphabet_lowercase_solo.xml</path>
  </tablet>
  <tablet order="3">
    <level>alphabet</level>
    <lesson>alphabet_case_pairs_and_counting</lesson>
    <mode>bridge</mode>
    <requires>foundation_ascii,counting</requires>
    <state>staged</state>
    <path>XML_text/02_alphabet_case_pairs_and_counting.xml</path>
  </tablet>
  <tablet order="4">
    <level>alphabet</level>
    <lesson>alphabet_growing_sequences</lesson>
    <mode>elastic</mode>
    <requires>foundation_ascii,counting</requires>
    <state>staged</state>
    <path>XML_text/02_alphabet_growing_sequences.xml</path>
  </tablet>
</manifest>
EOF

"$ROOT/build_ledger" \
    "$TMP/data/eden/manifest.xml" \
    "$TMP/data/eden/manifest_ledger.tsv" >/dev/null

test "$(grep -c 'TABLET_SUMMARY.*100_PERCENT_VERIFIED' \
    "$TMP/data/eden/manifest_ledger.tsv")" -eq 4

echo "Foundation, counting, and alphabet seed generation passed."
