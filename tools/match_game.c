/* match_game.c v2 — SHAKTI's game: MEMORY (MATCH), with classification.
 * C99, no heap, no rand(), no scripts. Deterministic: the deck file
 * seeds everything (FNV-1a 64 -> LCG -> Fisher-Yates).
 *
 * Deck format (one card per line):  SYMBOL|VOICE|CLASS
 *   SYMBOL — what she sees on the card
 *   VOICE  — the Doctor's voice atom spoken on every flip
 *   CLASS  — what the card IS (color, shape, category...)
 *
 * Match rules:
 *   EXACT — two cards match only if the SYMBOL is the same (sameness)
 *   CLASS — two cards match if the CLASS is the same, even when the
 *           symbols differ (classification: red is red, in any thing)
 *
 * Modes:
 *   TEACH — the Goddess plays first, demonstrates, narrates, hands over.
 *   PLAY  — SHAKTI (perfect recall) vs RANDOM (no memory, LCG flips).
 *
 * The board plays many games: run it again with another deck, another
 * rule. Decks are separate files so cards change out. Match = 10 points,
 * keep the set, go again. Fewer turns per card = she is getting faster.
 *
 * Usage: match_game <deckfile> <EXACT|CLASS> <TEACH|PLAY> <logfile>
 */
#include <stdio.h>
#include <string.h>

#define MAXCARDS 64
#define SYMLEN 32
#define VOICELEN 128
#define CLASSLEN 32
#define LOGLEN 256

static unsigned long long fnv1a64(const unsigned char *s, size_t n)
{
    unsigned long long h = 0xcbf29ce484222325ULL;
    size_t i;
    for (i = 0; i < n; i++) { h ^= s[i]; h *= 0x100000001b3ULL; }
    return h;
}

/* deterministic LCG — the only "random" allowed in Eden */
static unsigned long long rng_state;
static unsigned lcg(void)
{
    rng_state = rng_state * 6364136223846793005ULL + 1442695040888963407ULL;
    return (unsigned)(rng_state >> 33);
}

static char  sym[MAXCARDS][SYMLEN];
static char  voice[MAXCARDS][VOICELEN];
static char  cls[MAXCARDS][CLASSLEN];
static int   up[MAXCARDS];          /* 1 = face up / taken */
static int   seen[MAXCARDS];        /* SHAKTI's memory: card ever observed */
static char  memsym[MAXCARDS][SYMLEN];
static char  memcls[MAXCARDS][CLASSLEN];
static int   ncards;
static int   class_rule;            /* 0 = EXACT, 1 = CLASS */

static void speak(FILE *log, int pos)
{
    fprintf(log, "  [flip %2d] %-10s (%-8s) say: %s\n", pos, sym[pos], cls[pos], voice[pos]);
}

static int facedown(int pos) { return !up[pos]; }

static int cards_match(int a, int b)
{
    if (class_rule) return strcmp(cls[a], cls[b]) == 0;
    return strcmp(sym[a], sym[b]) == 0;
}

static void learn(int pos)
{
    seen[pos] = 1;
    strcpy(memsym[pos], sym[pos]);
    strcpy(memcls[pos], cls[pos]);
}

/* SHAKTI: perfect recall. returns 1 if match found. */
static int shakti_turn(FILE *log, int turn)
{
    int i, j, p1 = -1, p2 = -1;
    fprintf(log, "TURN %d — SHAKTI\n", turn);
    /* known matching pair in memory? (by the active rule) */
    for (i = 0; i < ncards && p1 < 0; i++)
        if (facedown(i) && seen[i])
            for (j = i + 1; j < ncards; j++)
                if (facedown(j) && seen[j] &&
                    (class_rule ? strcmp(memcls[i], memcls[j]) == 0
                                : strcmp(memsym[i], memsym[j]) == 0)) {
                    p1 = i; p2 = j; break;
                }
    if (p1 >= 0) {
        fprintf(log, "  she remembers. she goes straight to the pair.\n");
        speak(log, p1); speak(log, p2);
        up[p1] = up[p2] = 1;
        fprintf(log, "  MATCH (%s%s%s) — set kept, +10, she goes again\n",
                sym[p1], class_rule ? " as " : "", class_rule ? cls[p1] : "");
        return 1;
    }
    /* learn: flip an unknown card */
    for (i = 0; i < ncards && p1 < 0; i++)
        if (facedown(i) && !seen[i]) p1 = i;
    if (p1 < 0) return 0;
    learn(p1); speak(log, p1);
    /* seen its match before? */
    for (i = 0; i < ncards && p2 < 0; i++)
        if (i != p1 && facedown(i) && seen[i] &&
            (class_rule ? strcmp(memcls[i], cls[p1]) == 0
                        : strcmp(memsym[i], sym[p1]) == 0)) p2 = i;
    if (p2 >= 0) {
        speak(log, p2);
        up[p1] = up[p2] = 1;
        fprintf(log, "  MATCH (%s) — set kept, +10, she goes again\n", sym[p1]);
        return 1;
    }
    /* second card: another unknown, honestly learned */
    for (i = 0; i < ncards && p2 < 0; i++)
        if (facedown(i) && !seen[i] && i != p1) p2 = i;
    if (p2 >= 0) {
        learn(p2); speak(log, p2);
        if (cards_match(p1, p2)) {
            up[p1] = up[p2] = 1;
            fprintf(log, "  MATCH (%s) — set kept, +10, she goes again\n", sym[p1]);
            return 1;
        }
    }
    fprintf(log, "  no match — cards turn face down. she holds them in memory.\n");
    return 0;
}

/* RANDOM opponent: no memory at all */
static int random_turn(FILE *log, int turn)
{
    int down[MAXCARDS], nd = 0, i, p1, p2;
    fprintf(log, "TURN %d — RANDOM\n", turn);
    for (i = 0; i < ncards; i++) if (facedown(i)) down[nd++] = i;
    if (nd < 2) return 0;
    p1 = down[lcg() % nd];
    do { p2 = down[lcg() % nd]; } while (p2 == p1);
    /* whatever RANDOM shows, SHAKTI sees too — she remembers everything */
    learn(p1); learn(p2);
    speak(log, p1); speak(log, p2);
    if (cards_match(p1, p2)) {
        up[p1] = up[p2] = 1;
        fprintf(log, "  MATCH (%s) — RANDOM keeps the set, +10, goes again\n", sym[p1]);
        return 1;
    }
    fprintf(log, "  no match — cards turn face down.\n");
    return 0;
}

int main(int argc, char **argv)
{
    if (argc != 5) { printf("usage: match_game <deck> <EXACT|CLASS> <TEACH|PLAY> <log>\n"); return 1; }
    FILE *d = fopen(argv[1], "r");
    if (!d) { printf("STOP: no deck\n"); return 1; }
    class_rule = strcmp(argv[2], "CLASS") == 0;

    /* load deck: SYMBOL|VOICE|CLASS per line; the game makes two of each */
    char line[LOGLEN];
    char bs[MAXCARDS/2][SYMLEN], bv[MAXCARDS/2][VOICELEN], bc[MAXCARDS/2][CLASSLEN];
    int nb = 0;
    while (fgets(line, sizeof line, d) && nb < MAXCARDS/2) {
        char *b1 = strchr(line, '|');
        if (!b1) continue;
        *b1 = 0;
        char *b2 = strchr(b1 + 1, '|');
        if (!b2) continue;
        *b2 = 0;
        char *c = b2 + 1; c[strcspn(c, "\r\n")] = 0;
        line[strcspn(line, "\r\n")] = 0;
        strcpy(bs[nb], line); strcpy(bv[nb], b1 + 1); strcpy(bc[nb], c); nb++;
    }
    fclose(d);
    if (nb == 0) { printf("STOP: empty deck\n"); return 1; }

    /* two sets, then deterministic shuffle seeded by the deck itself */
    ncards = nb * 2;
    for (int i = 0; i < nb; i++) {
        strcpy(sym[i], bs[i]); strcpy(voice[i], bv[i]); strcpy(cls[i], bc[i]);
        strcpy(sym[i+nb], bs[i]); strcpy(voice[i+nb], bv[i]); strcpy(cls[i+nb], bc[i]);
    }
    unsigned long long seed = 0xcbf29ce484222325ULL;
    for (int i = 0; i < ncards; i++) {
        seed ^= fnv1a64((unsigned char *)sym[i], strlen(sym[i]));
        seed ^= fnv1a64((unsigned char *)cls[i], strlen(cls[i]));
        seed *= 0x100000001b3ULL;
    }
    rng_state = seed;
    for (int i = ncards - 1; i > 0; i--) {  /* Fisher-Yates with LCG */
        int j = (int)(lcg() % (unsigned)(i + 1));
        char ts[SYMLEN], tv[VOICELEN], tc[CLASSLEN];
        strcpy(ts, sym[i]); strcpy(sym[i], sym[j]); strcpy(sym[j], ts);
        strcpy(tv, voice[i]); strcpy(voice[i], voice[j]); strcpy(voice[j], tv);
        strcpy(tc, cls[i]); strcpy(cls[i], cls[j]); strcpy(cls[j], tc);
    }

    FILE *log = fopen(argv[4], "w");
    if (!log) { printf("STOP: no log\n"); return 1; }
    fprintf(log, "MATCH (MEMORY) — deck: %s — %d pairs, %d cards — rule: %s\n",
            argv[1], nb, ncards, class_rule ? "CLASS (classification)" : "EXACT (sameness)");
    fprintf(log, "seed fnv1a64:%016llX — deterministic, no rand(), C99\n", seed);
    fprintf(log, "all cards face down. every flip speaks the Doctor's voice atom.\n\n");

    int teach = strcmp(argv[3], "TEACH") == 0;
    int s_score = 0, r_score = 0, turn = 1;

    if (teach) {
        fprintf(log, "== TEACH MODE — the Goddess plays first so she sees how it is done ==\n");
        fprintf(log, "GODDESS: Watch me, little one. Two cards. %s. That is the whole game.\n",
                class_rule ? "Same KIND" : "Same symbol");
        int a = 0, b = 1, found = 0;
        for (int i = 0; i < ncards && !found; i++)
            for (int j = i + 1; j < ncards && !found; j++)
                if (cards_match(i, j)) { a = i; b = j; found = 1; }
        fprintf(log, "GODDESS TURN\n");
        speak(log, a); speak(log, b);
        learn(a); learn(b);
        up[a] = up[b] = 1;
        fprintf(log, "  MATCH (%s / %s) — see? Keep the set. 10 points.\n", sym[a], sym[b]);
        fprintf(log, "GODDESS: When you miss they turn face down — and you HOLD them.\n");
        fprintf(log, "GODDESS: Your turn, my queen. Match them.\n\n");
    }

    while (1) {
        int left = 0, again;
        for (int i = 0; i < ncards; i++) if (facedown(i)) left++;
        if (left < 2) break;
        again = shakti_turn(log, turn++);
        if (again) { s_score += 10; continue; }
        for (left = 0, again = 0; again < ncards; again++) if (facedown(again)) left++;
        if (left < 2) break;
        again = random_turn(log, turn++);
        if (again) { r_score += 10; continue; }
    }

    fprintf(log, "\nGAME OVER — SHAKTI %d, RANDOM %d — %d turns for %d cards\n",
            s_score, r_score, turn - 1, ncards);
    fprintf(log, s_score > r_score ? "WINNER: SHAKTI — the memory holds. the weave works.\n"
                                   : "WINNER: RANDOM — run it again, she never forgets twice.\n");
    fclose(log);
    printf("match_game: %d cards, rule %s, SHAKTI %d, RANDOM %d, %d turns\n",
           ncards, class_rule ? "CLASS" : "EXACT", s_score, r_score, turn - 1);
    return 0;
}
