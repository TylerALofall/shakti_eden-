#ifndef EYES_XML_H
#define EYES_XML_H

#include <limits.h>

#include "eyes.h"

#define EYES_XML_V1 "EYES_XML_V1"
#define EYES_XML_VERSION 1U
#define EYES_XML_DEFAULT_TAG "eyes_xml"
#define EYES_XML_OUTPUT_DIR "eyes/output"
#define EYES_XML_PAGE_COUNT 4U
#define EYES_XML_WIDTH_MAX 64U
#define EYES_XML_HEIGHT_MAX 24U
#define EYES_XML_RGBA_CAPACITY \
    ((unsigned long)EYES_XML_WIDTH_MAX * (unsigned long)EYES_XML_HEIGHT_MAX * 4UL)
#define EYES_XML_PAGE_BITS_CAPACITY \
    ((unsigned long)EYES_XML_WIDTH_MAX * (unsigned long)EYES_XML_HEIGHT_MAX + 1UL)
#define EYES_XML_LINE_CAPACITY 256U
#define EYES_XML_PATH_CAPACITY 256U
#define EYES_XML_DIFF_ERROR ULONG_MAX

typedef struct {
    unsigned int number;
    unsigned int width;
    unsigned int height;
    /* Original `eyes_load_document` kind for deterministic regeneration.
     * The XML page payload itself is currently fixed to kind="mono". */
    unsigned int source_kind;
    unsigned long seed;
    eyes_text_page_t text;
} eyes_xml_page_spec_t;

typedef struct {
    unsigned int number;
    unsigned int width;
    unsigned int height;
    char kind[5];
} eyes_xml_page_record_t;

#define EYES_XML_DOCUMENT_PAGES_INIT { \
    { 1U, 16U, 16U, 0U, 0UL, { "HI", "", "" } }, \
    { 2U, 32U, 16U, 0U, 0UL, { "EDEN", "", "" } }, \
    { 3U, 40U, 24U, 0U, 0UL, { "TYLER", "ALLEN", "" } }, \
    { 4U, 64U, 24U, 0U, 0UL, { "PIXEL", "PROOF", "ZERO" } } \
}

extern const eyes_xml_page_spec_t
g_eyes_xml_document_pages[EYES_XML_PAGE_COUNT];

int eyes_xml_write_document(
    const char *path,
    const eyes_xml_page_spec_t *pages,
    unsigned int page_count,
    char bits[][EYES_XML_PAGE_BITS_CAPACITY]
);

int eyes_xml_read_document(
    const char *path,
    eyes_xml_page_record_t *pages,
    unsigned int page_capacity,
    char bits[][EYES_XML_PAGE_BITS_CAPACITY],
    unsigned int *page_count_out
);

#endif
