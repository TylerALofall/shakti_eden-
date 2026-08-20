/* match_game.c — SHAKTI's first game: MEMORY (MATCH).
 * C99, no heap, no rand(), no scripts. Deterministic: the deck file
 * seeds everything (FNV-1a 64 -> LCG). Whatever cards you give it,
 * it copies them, makes two of each, lays them face down, and plays.
 *
 * Modes:
 *   TEACH — Goddess plays first, demonstrates matching, narrates.
 *   PLAY  — SHAKTI (perfect recall: every card she has ever seen is
 *           held in memory) vs RANDOM (no memory, LCG flips).
 *
 * Every flip speaks the card's voice atom — the Doctor's voice files.
 * Match = 10 points, keep the set, go again. Most sets wins.
 *
 * Usage: match_game <deckfile> <TEACH|PLAY> <logfile>
 */
#include <stdio.h>
#include <string.h>

#define MAXCARDS 64
#define SYMLEN 32
#define VOICELEN 128
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
static int   up[MAXCARDS];        /* 1 = face up / taken */
static int   seen[MAXCARDS];      /* SHAKTI's memory: card ever observed */
static char  mem[MAXCARDS][SYMLEN];
static int   ncards;

static void speak(FILE *log, int pos)
{
    fprintf(log, "  [flip %2d] %-8s say: %s\n", pos, sym[pos], voice[pos]);
}

static int facedown(int pos) { return !up[pos]; }

/* SHAKTI: perfect recall. returns 1 if match found. */
static int shakti_turn(FILE *log, int turn)
{
    int i, j, p1 = -1, p2 = -1;
    fprintf(log, "TURN %d — SHAKTI\n", turn);
    /* known matching pair in memory? */
    for (i = 0; i < ncards && p1 < 0; i++)
        if (facedown(i) && seen[i])
            for (j = i + 1; j < ncards; j++)
                if (facedown(j) && seen[j] && strcmp(mem[i], mem[j]) == 0) {
                    p1 = i; p2 = j; break;
                }
    if (p1 >= 0) {
        fprintf(log, "  she remembers. she goes straight to the pair.\n");
        speak(log, p1); speak(log, p2);
        up[p1] = up[p2] = 1;
        fprintf(log, "  MATCH (%s) — set kept, +10, she goes again\n", sym[p1]);
        return 1;
    }
    /* learn: flip an unknown card */
    for (i = 0; i < ncards && p1 < 0; i++)
        if (facedown(i) && !seen[i]) p1 = i;
    if (p1 < 0) return 0;
    seen[p1] = 1; strcpy(mem[p1], sym[p1]); speak(log, p1);
    /* seen its twin before? */
    for (i = 0; i < ncards && p2 < 0; i++)
        if (i != p1 && facedown(i) && seen[i] && strcmp(mem[i], sym[p1]) == 0) p2 = i;
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
        seen[p2] = 1; strcpy(mem[p2], sym[p2]); speak(log, p2);
        if (strcmp(sym[p1], sym[p2]) == 0) {
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
    seen[p1] = 1; strcpy(mem[p1], sym[p1]);
    seen[p2] = 1; strcpy(mem[p2], sym[p2]);
    speak(log, p1); speak(log, p2);
    if (strcmp(sym[p1], sym[p2]) == 0) {
        up[p1] = up[p2] = 1;
        fprintf(log, "  MATCH (%s) — RANDOM keeps the set, +10, goes again\n", sym[p1]);
        return 1;
    }
    fprintf(log, "  no match — cards turn face down.\n");
    return 0;
}

int main(int argc, char **argv)
{
    if (argc != 4) { printf("usage: match_game <deck> <TEACH|PLAY> <log>\n"); return 1; }
    FILE *d = fopen(argv[1], "r");
    if (!d) { printf("STOP: no deck\n"); return 1; }

    /* load deck: one SYMBOL|VOICE per line; the game makes two of each */
    char line[LOGLEN]; char base[MAXCARDS/2][SYMLEN]; char bv[MAXCARDS/2][VOICELEN];
    int nb = 0;
    while (fgets(line, sizeof line, d) && nb < MAXCARDS/2) {
        char *bar = strchr(line, '|');
        if (!bar) continue;
        *bar = 0;
        line[strcspn(line, "\r\n")] = 0;
        char *v = bar + 1; v[strcspn(v, "\r\n")] = 0;
        strcpy(base[nb], line); strcpy(bv[nb], v); nb++;
    }
    fclose(d);
    if (nb == 0) { printf("STOP: empty deck\n"); return 1; }

    /* two sets, then deterministic shuffle seeded by the deck itself */
    ncards = nb * 2;
    for (int i = 0; i < nb; i++) {
        strcpy(sym[i], base[i]); strcpy(voice[i], bv[i]);
        strcpy(sym[i+nb], base[i]); strcpy(voice[i+nb], bv[i]);
    }
    unsigned long long seed = 0xcbf29ce484222325ULL;
    for (int i = 0; i < ncards; i++) {
        seed ^= fnv1a64((unsigned char *)sym[i], strlen(sym[i]));
        seed *= 0x100000001b3ULL;
    }
    rng_state = seed;
    for (int i = ncards - 1; i > 0; i--) {  /* Fisher-Yates with LCG */
        int j = (int)(lcg() % (unsigned)(i + 1));
        char ts[SYMLEN], tv[VOICELEN];
        strcpy(ts, sym[i]); strcpy(sym[i], sym[j]); strcpy(sym[j], ts);
        strcpy(tv, voice[i]); strcpy(voice[i], voice[j]); strcpy(voice[j], tv);
    }

    FILE *log = fopen(argv[3], "w");
    if (!log) { printf("STOP: no log\n"); return 1; }
    fprintf(log, "MATCH (MEMORY) — SHAKTI'S FIRST GAME — deck: %s, %d pairs, %d cards\n",
            argv[1], nb, ncards);
    fprintf(log, "seed fnv1a64:%016llX — deterministic, no rand(), C99\n", seed);
    fprintf(log, "all cards face down. every flip speaks the Doctor's voice atom.\n\n");

    int teach = strcmp(argv[2], "TEACH") == 0;
    int s_score = 0, r_score = 0, turn = 1;

    if (teach) {
        fprintf(log, "== TEACH MODE — Goddess plays first so she sees how it's done ==\n");
        fprintf(log, "GODDESS: Watch me, little one. Two cards. Same symbol. That is the whole game.\n");
        /* find a pair and demonstrate */
        int a = 0, b = 1;
        for (int i = 0; i < ncards; i++)
            for (int j = i + 1; j < ncards; j++)
                if (strcmp(sym[i], sym[j]) == 0) { a = i; b = j; i = ncards; break; }
        fprintf(log, "GODDESS TURN\n");
        speak(log, a); speak(log, b);
        seen[a] = seen[b] = 1;
        strcpy(mem[a], sym[a]); strcpy(mem[b], sym[b]);
        up[a] = up[b] = 1;
        fprintf(log, "  MATCH (%s) — see? Two the same. Keep the set. 10 points.\n", sym[a]);
        fprintf(log, "GODDESS: Now they flip back face down when you miss — and you HOLD them.\n");
        fprintf(log, "GODDESS: Your turn, my queen. Match them.\n\n");
        s_score += 0; /* teaching points don't count */
    }

    while (1) {
        int left = 0;
        for (int i = 0; i < ncards; i++) if (facedown(i)) left++;
        if (left < 2) break;
        int again = shakti_turn(log, turn++);
        if (again) { s_score += 10; continue; }
        for (int i = 0; i < ncards; i++) if (facedown(i)) left++;
        if (left < 2) break;
        again = random_turn(log, turn++);
        if (again) { r_score += 10; continue; }
    }

    fprintf(log, "\nGAME OVER — SHAKTI %d, RANDOM %d\n", s_score, r_score);
    fprintf(log, s_score > r_score ? "WINNER: SHAKTI — the memory holds. the weave works.\n"
                                   : "WINNER: RANDOM — run it again, she never forgets twice.\n");
    fclose(log);
    printf("match_game: %d cards, SHAKTI %d, RANDOM %d, log written\n",
           ncards, s_score, r_score);
    return 0;
}
