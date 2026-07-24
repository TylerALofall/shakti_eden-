#include <stdio.h>
#include <stdlib.h>

#include "shakti_manifest.h"

int main(int argc, char **argv)
{
    shakti_manifest_t manifest;
    int ready;

    if (argc != 3) {
        fprintf(
            stderr,
            "Usage: %s MANIFEST.xml LEDGER.tsv\n",
            argv[0]
        );
        return EXIT_FAILURE;
    }

    shakti_manifest_init(&manifest);

    if (!shakti_manifest_load(&manifest, argv[1])) {
        fputs("Manifest structure or prerequisites are invalid.\n", stderr);
        return EXIT_FAILURE;
    }

    ready = shakti_manifest_write_ledger(
        &manifest,
        argv[1],
        argv[2]
    );

    shakti_manifest_print(&manifest);

    if (!ready) {
        fputs(
            "Ledger written, but one or more ready tablets failed validation.\n",
            stderr
        );
        return EXIT_FAILURE;
    }

    printf("Ledger written to %s.\n", argv[2]);

    return EXIT_SUCCESS;
}
