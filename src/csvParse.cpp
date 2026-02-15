#include "../headers/csvParse.h"
#include "../headers/nodesAndEdges.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cctype>

// Helper function to trim whitespace
void trimInPlace(char *s) {
    if (!s) return;
    
    // Trim leading whitespace
    char *p = s;
    while (*p && isspace((unsigned char)*p)) p++;
    if (p != s) memmove(s, p, strlen(p) + 1);
    
    // Trim trailing whitespace
    size_t n = strlen(s);
    while (n > 0 && isspace((unsigned char)s[n-1])) {
        s[--n] = '\0';
    }
}

// Split CSV line by comma
int splitCSV(char *line, char **tokens, int maxTokens) {
    int count = 0;
    char *save = NULL;
    
    for (char *tok = strtok_r(line, ",", &save);
         tok && count < maxTokens;
         tok = strtok_r(NULL, ",", &save)) {
        trimInPlace(tok);
        tokens[count++] = tok;
    }
    
    return count;
}

// Check if token is a number
int isNumberToken(const char *s) {
    if (!s) return 0;
    while (*s && isspace((unsigned char)*s)) s++;
    if (*s == '\0') return 0;
    
    char *end = NULL;
    (void)strtod(s, &end);
    while (end && *end && isspace((unsigned char)*end)) end++;
    
    return end && *end == '\0';
}

// Parse Roadmap-Dhaka.csv
void parseRoadmapCSV(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        printf("Error: Could not open %s\n", filename);
        return;
    }
    
    char line[MAX_LINE];
    char *tokens[MAX_TOKENS];
    int roadCount = 0;
    
    printf("Parsing roadmap CSV...\n");
    
    while (fgets(line, sizeof(line), f)) {
        // Remove newline
        line[strcspn(line, "\r\n")] = 0;
        if (strlen(line) == 0) continue;
        
        int count = splitCSV(line, tokens, MAX_TOKENS);
        if (count < 6) continue;  // Need at least: type, 2 coords, altitude, length
        
        // Last 2 tokens should be altitude and distance
        const char *altTok = tokens[count - 2];
        const char *lenTok = tokens[count - 1];
        if (!isNumberToken(altTok) || !isNumberToken(lenTok)) continue;
        
        // Coordinates are from tokens[1] to tokens[count-3]
        int coordCount = count - 3;
        if (coordCount < 4 || coordCount % 2 != 0) continue;
        
        roadCount++;
        
        // Process consecutive lon-lat pairs
        // Format: DhakaStreet, lon1, lat1, lon2, lat2, ..., altitude, distance
        for (int i = 1; i + 3 <= count - 2; i += 2) {
            double lon1 = atof(tokens[i]);
            double lat1 = atof(tokens[i+1]);
            double lon2 = atof(tokens[i+2]);
            double lat2 = atof(tokens[i+3]);
            
            int from = findOrAddNode(lat1, lon1);
            int to = findOrAddNode(lat2, lon2);
            
            // Calculate segment distance using Haversine
            double segmentDist = haversineDistance(lat1, lon1, lat2, lon2);
            
            // Add bidirectional edges (roads go both ways)
            addEdge(from, to, MODE_CAR, segmentDist);
            addEdge(to, from, MODE_CAR, segmentDist);
        }
    }
    
    fclose(f);
    printf("Parsed %d road segments\n", roadCount);
}

// Parse Metro CSV
void parseMetroCSV(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        printf("Error: Could not open %s\n", filename);
        return;
    }
    
    char line[MAX_LINE];
    char *tokens[MAX_TOKENS];
    int routeCount = 0;
    
    printf("Parsing metro CSV...\n");
    
    while (fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\r\n")] = 0;
        if (strlen(line) == 0) continue;
        
        int count = splitCSV(line, tokens, MAX_TOKENS);
        if (count < 5) continue;
        
        // Last 2 tokens should be station names (non-numeric)
        const char *startStation = tokens[count - 2];
        const char *endStation = tokens[count - 1];
        
        if (isNumberToken(startStation) || isNumberToken(endStation)) {
            continue;
        }
        
        // Coordinates are from tokens[1] to tokens[count-3]
        int coordCount = count - 3;
        if (coordCount < 4 || coordCount % 2 != 0) {
            continue;
        }
        
        routeCount++;
        
        // Process consecutive lon-lat pairs
        for (int i = 1; i + 3 <= count - 2; i += 2) {
            double lon1 = atof(tokens[i]);
            double lat1 = atof(tokens[i+1]);
            double lon2 = atof(tokens[i+2]);
            double lat2 = atof(tokens[i+3]);
            
            int from = findOrAddNode(lat1, lon1);
            int to = findOrAddNode(lat2, lon2);
            
            double segmentDist = haversineDistance(lat1, lon1, lat2, lon2);
            
            // Add bidirectional metro edges
            addEdge(from, to, MODE_METRO, segmentDist);
            addEdge(to, from, MODE_METRO, segmentDist);
            
            // Set station names for first and last stops
            if (i == 1) {
                strncpy(nodes[from].name, startStation, sizeof(nodes[from].name) - 1);
                nodes[from].name[sizeof(nodes[from].name) - 1] = '\0';
            }
            if (i + 4 > count - 2) {  // Last pair
                strncpy(nodes[to].name, endStation, sizeof(nodes[to].name) - 1);
                nodes[to].name[sizeof(nodes[to].name) - 1] = '\0';
            }
        }
    }
    
    fclose(f);
    printf("Parsed %d metro routes\n", routeCount);
}

// Parse Bus CSV
void parseBusCSV(const char *filename, Mode busMode) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        printf("Error: Could not open %s\n", filename);
        return;
    }
    
    char line[MAX_LINE];
    char *tokens[MAX_TOKENS];
    int routeCount = 0;
    
    printf("Parsing bus CSV (%s)...\n", getModeName(busMode));
    
    while (fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\r\n")] = 0;
        if (strlen(line) == 0) continue;
        
        int count = splitCSV(line, tokens, MAX_TOKENS);
        if (count < 5) continue;
        
        // Last 2 tokens should be station names (non-numeric)
        const char *startStation = tokens[count - 2];
        const char *endStation = tokens[count - 1];
        
        if (isNumberToken(startStation) || isNumberToken(endStation)) {
            continue;
        }
        
        // Coordinates are from tokens[1] to tokens[count-3]
        int coordCount = count - 3;
        if (coordCount < 4 || coordCount % 2 != 0) {
            continue;
        }
        
        routeCount++;
        
        // Process consecutive lon-lat pairs
        for (int i = 1; i + 3 <= count - 2; i += 2) {
            double lon1 = atof(tokens[i]);
            double lat1 = atof(tokens[i+1]);
            double lon2 = atof(tokens[i+2]);
            double lat2 = atof(tokens[i+3]);
            
            int from = findOrAddNode(lat1, lon1);
            int to = findOrAddNode(lat2, lon2);
            
            double segmentDist = haversineDistance(lat1, lon1, lat2, lon2);
            
            // Add bidirectional bus edges
            addEdge(from, to, busMode, segmentDist);
            addEdge(to, from, busMode, segmentDist);
            
            // Set station names for first and last stops
            if (i == 1) {
                strncpy(nodes[from].name, startStation, sizeof(nodes[from].name) - 1);
                nodes[from].name[sizeof(nodes[from].name) - 1] = '\0';
            }
            if (i + 4 > count - 2) {  // Last pair
                strncpy(nodes[to].name, endStation, sizeof(nodes[to].name) - 1);
                nodes[to].name[sizeof(nodes[to].name) - 1] = '\0';
            }
        }
    }
    
    fclose(f);
    printf("Parsed %d bus routes\n", routeCount);
}
