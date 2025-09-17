#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_SPOTS 10
#define MAX_NAME_LEN 64

typedef struct Destination {
    char name[MAX_NAME_LEN];
    int attributeMask;
} Destination;

static int code_to_bit(const char *code) {
    if (strcmp(code, "MT") == 0) return 1 << 0; // Mountainous Terrain
    if (strcmp(code, "BE") == 0) return 1 << 1; // Beach
    if (strcmp(code, "CU") == 0) return 1 << 2; // Cultural Heritage
    if (strcmp(code, "NP") == 0) return 1 << 3; // National Park
    if (strcmp(code, "UR") == 0) return 1 << 4; // Urban Area
    if (strcmp(code, "WD") == 0) return 1 << 5; // Wildlife Diversity
    if (strcmp(code, "LA") == 0) return 1 << 6; // Lake
    return 0;
}

static void trim_in_place(char *s) {
    size_t len = strlen(s);
    while (len > 0 && (s[len - 1] == '\n' || s[len - 1] == '\r' || isspace((unsigned char)s[len - 1]))) {
        s[--len] = '\0';
    }
    size_t start = 0;
    while (s[start] && isspace((unsigned char)s[start])) start++;
    if (start > 0) memmove(s, s + start, strlen(s + start) + 1);
}

static int parse_and_group_mask(const char *segment) {
    char buffer[256];
    strncpy(buffer, segment, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    trim_in_place(buffer);
    int mask = 0;
    for (char *tok = strtok(buffer, " \t"); tok != NULL; tok = strtok(NULL, " \t")) {
        mask |= code_to_bit(tok);
    }
    return mask;
}

static int destination_matches_query(const Destination *dest, const char *queryLine) {
    // OR is '+', AND is whitespace within each segment
    const char *p = queryLine;
    while (*p) {
        const char *plus = strchr(p, '+');
        size_t segLen = plus ? (size_t)(plus - p) : strlen(p);
        char segment[256];
        if (segLen >= sizeof(segment)) segLen = sizeof(segment) - 1;
        memcpy(segment, p, segLen);
        segment[segLen] = '\0';
        trim_in_place(segment);
        if (segment[0] != '\0') {
            int andMask = parse_and_group_mask(segment);
            if (andMask != 0 && (dest->attributeMask & andMask) == andMask) {
                return 1; // matches at least one AND-group
            }
        }
        if (!plus) break;
        p = plus + 1;
    }
    return 0;
}

int main(void) {
    char line[256];
    int n = 0;

    // Read number of destinations
    while (fgets(line, sizeof(line), stdin)) {
        trim_in_place(line);
        if (line[0] == '\0') continue;
        if (sscanf(line, "%d", &n) == 1) break;
    }
    if (n <= 0) return 0;

    Destination destinations[MAX_SPOTS];
    int count = 0;

    for (int i = 0; i < n; i++) {
        if (!fgets(line, sizeof(line), stdin)) return 0;
        trim_in_place(line);
        if (line[0] == '\0') { i--; continue; }

        char buffer[256];
        strncpy(buffer, line, sizeof(buffer) - 1);
        buffer[sizeof(buffer) - 1] = '\0';

        char *tok = strtok(buffer, " \t");
        if (!tok) { i--; continue; }
        Destination d;
        memset(&d, 0, sizeof(d));
        strncpy(d.name, tok, sizeof(d.name) - 1);
        d.attributeMask = 0;
        for (tok = strtok(NULL, " \t"); tok != NULL; tok = strtok(NULL, " \t")) {
            d.attributeMask |= code_to_bit(tok);
        }
        destinations[count++] = d;
    }

    int m = 0;
    // Read number of queries
    while (fgets(line, sizeof(line), stdin)) {
        trim_in_place(line);
        if (line[0] == '\0') continue;
        if (sscanf(line, "%d", &m) == 1) break;
    }
    if (m <= 0) return 0;

    for (int qi = 0; qi < m; qi++) {
        if (!fgets(line, sizeof(line), stdin)) break;
        trim_in_place(line);
        int printed = 0;
        int any = 0;
        for (int i = 0; i < count; i++) {
            if (destination_matches_query(&destinations[i], line)) {
                if (printed) printf(" ");
                printf("%s", destinations[i].name);
                printed = 1;
                any = 1;
            }
        }
        if (!any) printf("None");
        printf("\n");
    }

    return 0;
}

