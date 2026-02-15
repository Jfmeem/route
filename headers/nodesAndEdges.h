#ifndef NODES_AND_EDGES_H
#define NODES_AND_EDGES_H

#include "mode.h"

// Maximum limits
#define MAX_NODES 100000
#define INF 9999999999.0

// Node structure (intersection or station)
typedef struct {
    int id;
    char name[32];  // Station name if applicable
    double lat;
    double lon;
} Node;

// Edge structure (connection between nodes)
typedef struct {
    int from;
    int to;
    Mode mode;
    double distance;  // Distance in km
} Edge;

// Global arrays (simple beginner approach)
extern Node nodes[MAX_NODES];
extern Edge edges[MAX_NODES * 10];
extern int numNodes;
extern int numEdges;

// Dijkstra algorithm arrays
extern double dist[MAX_NODES];
extern int prev[MAX_NODES];
extern int visited[MAX_NODES];
extern int prevEdge[MAX_NODES];

// Function declarations
int findOrAddNode(double lat, double lon);
int findNearestNode(double lat, double lon);
void addEdge(int from, int to, Mode mode, double distance);
double haversineDistance(double lat1, double lon1, double lat2, double lon2);

#endif
