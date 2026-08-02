#!/bin/sh
set -eu

ROOT=$(CDPATH= cd -- "$(dirname "$0")/.." && pwd)
TMP="$ROOT/tests/tmp_mvp"

prepare_sandbox()
{
    rm -rf "$TMP"
    mkdir -p "$TMP"
    cp -R "$ROOT/data" "$TMP/data"
    cp -R "$ROOT/eden_out" "$TMP/eden_out"

    mkdir -p \
        "$TMP/data/school" \
        "$TMP/data/learned" \
        "$TMP/data/control"

    cat > "$TMP/data/school/school_state.log" <<'EOF'
# SCH1|epoch:frame|event|symbol|pass|streak
EOF

    cat > "$TMP/data/learned/evidence.log" <<'EOF'
# EV1|epoch:frame|ADD-or-REJECT|source|question|answer
EOF

    cat > "$TMP/data/learned/learned_stream.log" <<'EOF'
# LRN1 readable append-only life stream.
EOF

    cat > "$TMP/data/control/messages.log" <<'EOF'
# Append-only Shakti and Tyler messages.
EOF

    cat > "$TMP/data/control/reflections.log" <<'EOF'
# Append-only reflections.
EOF
}

prepare_sandbox

(
    cd "$TMP"
    "$ROOT/shakti" --check > check_output.txt
    "$ROOT/shakti" --demo > demo_output.txt
)

grep -q 'Runnable beginning: YES.' "$TMP/check_output.txt"
grep -q 'School: pass 1, symbol none, streak 0/10' \
    "$TMP/check_output.txt"
grep -q 'PASS 1' "$TMP/demo_output.txt"
grep -q 'Demo complete. School mastery remains unchanged.' \
    "$TMP/demo_output.txt"

prepare_sandbox

cat > "$TMP/input.txt" <<'INPUT'
/shakti_run/ pass 4
/shakti_run/ school +
/status/
/help/
/resume/
/interrupt/
/status/
/resume/
/shakti_run/ school +
+
+
+
+
+
+
+
+
+
+
/quit/
INPUT

(
    cd "$TMP"
    "$ROOT/shakti" < input.txt > drill_output.txt
)

grep -q 'The active drill stopped without scoring the command.' \
    "$TMP/drill_output.txt"
grep -q 'MCP tools stopped. Shakti remains awake.' \
    "$TMP/drill_output.txt"
grep -q 'MCP tools restored.' "$TMP/drill_output.txt"
grep -q 'Drill controls:' "$TMP/drill_output.txt"
grep -q '10/10 — 100% MASTERY' "$TMP/drill_output.txt"
grep -q 'History preserved: 10 correct, 0 errors, 10 attempts.' \
    "$TMP/drill_output.txt"

RIGHT_COUNT=$(grep -c '|RIGHT|+|4|' "$TMP/data/school/school_state.log")
WRONG_COUNT=$(grep -c '|WRONG|' "$TMP/data/school/school_state.log" || true)
test "$RIGHT_COUNT" -eq 10
test "$WRONG_COUNT" -eq 0

prepare_sandbox

cat > "$TMP/stop_input.txt" <<'INPUT'
/shakti_run/ pass 4
/shakti_run/ school +
/stop/
/status/
/quit/
INPUT

(
    cd "$TMP"
    "$ROOT/shakti" < stop_input.txt > stop_output.txt
)

grep -q 'Drill stopped without changing the School pass.' \
    "$TMP/stop_output.txt"
grep -q 'Progress: 0 of 10.' "$TMP/stop_output.txt"
STOP_WRONG_COUNT=$(
    grep -c '|WRONG|' "$TMP/data/school/school_state.log" || true
)
test "$STOP_WRONG_COUNT" -eq 0

prepare_sandbox

cat > "$TMP/quit_input.txt" <<'INPUT'
/shakti_run/ pass 4
/shakti_run/ school +
/quit/
INPUT

(
    cd "$TMP"
    "$ROOT/shakti" < quit_input.txt > quit_output.txt
)

grep -q 'Quit requested. The active drill stopped safely.' \
    "$TMP/quit_output.txt"
grep -q 'Shakti sleeping. Readable memory remains.' \
    "$TMP/quit_output.txt"
QUIT_WRONG_COUNT=$(
    grep -c '|WRONG|' "$TMP/data/school/school_state.log" || true
)
test "$QUIT_WRONG_COUNT" -eq 0

echo "Runnable startup, isolated state, drill controls, and mastery passed."
