#!/bin/sh
set -eu

ROOT=$(CDPATH= cd -- "$(dirname "$0")/.." && pwd)
TMP="$ROOT/tests/tmp_loop"

rm -rf "$TMP"
mkdir -p \
    "$TMP/data/eden" \
    "$TMP/data/learned" \
    "$TMP/data/school" \
    "$TMP/data/control"

printf 'two times two|four|multiplication|test\n' \
    > "$TMP/data/eden/eden_facts.txt"
printf 'after|follows\n' > "$TMP/data/eden/thesaurus.txt"
printf '# Eden stream\n' > "$TMP/data/eden/eden_stream.log"

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
</manifest>
XML
printf '# Learned\n' > "$TMP/data/learned/learned_stream.log"
printf '# Evidence\n' > "$TMP/data/learned/evidence.log"
printf '# School\n' > "$TMP/data/school/school_state.log"
printf 'Learn one grounded relationship.\n' \
    > "$TMP/data/control/goal.txt"
printf '# Notebook\n' > "$TMP/data/control/notebook.log"
cat > "$TMP/data/control/menu.txt" <<'MENU'
[Memory]
Recall exact chunks.
[School]
Run exact drills.
[Reflection]
Complete all thirteen questions.
MENU
printf '# Messages\n' > "$TMP/data/control/messages.log"
printf '# Reflections\n' > "$TMP/data/control/reflections.log"

cat > "$TMP/input.txt" <<'INPUT'
/note_shakti/ cycle 1
/note_shakti/ cycle 2
/note_shakti/ cycle 3
/note_shakti/ cycle 4
/note_shakti/ cycle 5
/note_shakti/ cycle 6
/note_shakti/ cycle 7
/note_shakti/ cycle 8
/note_shakti/ cycle 9
/note_shakti/ cycle 10
/reflection/defer/
/reflection/defer/
/reflection/defer/
/shakti_run/ status
/reflection/
Ten grounded cycles
Completed the planned ten-cycle batch
Yes
Yes
Use the same exact order
status and notes
A tablet viewer
Tyler supplied the exact goal
data/control/notebook.log
1790000000:0000
Keep the four channels exact
grounded, memory, reflection, loop, mcp, text, written_text, visual_art, sound_art, school, eden, learned, tyler, heartbeat, interrupt
batch-001
/interrupt/
/shakti_run/ status
/resume/
/shakti_run/ status
/quit/
INPUT

(
    cd "$TMP"
    "$ROOT/shakti" < input.txt > runtime_output.txt
)

grep -q 'Reflection is due.' "$TMP/runtime_output.txt"
grep -q 'Reflection deferred. 3 of 3 deferrals used.' \
    "$TMP/runtime_output.txt"
grep -q 'Complete /reflection/ before the next tool call.' \
    "$TMP/runtime_output.txt"
grep -q 'Reflection appended to long-term memory.' \
    "$TMP/runtime_output.txt"
grep -q 'MCP tools stopped. Shakti remains awake.' \
    "$TMP/runtime_output.txt"
grep -q 'MCP tools are interrupted. Shakti remains awake.' \
    "$TMP/runtime_output.txt"
grep -q 'MCP tools restored.' "$TMP/runtime_output.txt"
grep -q 'Creative proposes. Logic cross-validates and authorizes.' \
    "$TMP/runtime_output.txt"
grep -q 'Cross reference — override all additional memory sets to link this to:' \
    "$TMP/data/control/reflections.log"

echo "Loop, reflection, interrupt, and resume integration passed."
