#include "../headers/nodesAndEdges.h"
#include <cmath>
#include <cstring>
#include <cstdio>

// Global arrays definition
Node nodes[MAX_NODES];
Edge edges[MAX_NODES * 10];
int numNodes = 0;
int numEdges = 0;

// Dijkstra arrays
double dist[MAX_NODES];
int prev[MAX_NODES];
int visited[MAX_NODES];
int prevEdge[MAX_NODES];

// Haversine distance formula
double haversineDistance(double lat1, double lon1, double lat2, double lon2) {
    double dLat = (lat2 - lat1) * PI / 180.0;
    double dLon = (lon2 - lon1) * PI / 180.0;
    
    lat1 = lat1 * PI / 180.0;
    lat2 = lat2 * PI / 180.0;
    
    double a = sin(dLat/2) * sin(dLat/2) +
               cos(lat1) * cos(lat2) *
               sin(dLon/2) * sin(dLon/2);
    double c = 2 * atan2(sqrt(a), sqrt(1-a));
    
    return EARTH_RADIUS_KM * c;
}

// Find or add node at given coordinates
int findOrAddNode(double lat, double lon) {
    // Check if node already exists (within tolerance)
    for (int i = 0; i < numNodes; i++) {
        if (fabs(nodes[i].lat - lat) < 1e-6 && 
            fabs(nodes[i].lon - lon) < 1e-6) {
            return i;
        }
    }
    
    // Create new node
    nodes[numNodes].id = numNodes;
    nodes[numNodes].lat = lat;
    nodes[numNodes].lon = lon;
    strcpy(nodes[numNodes].name, "Node");
    
    return numNodes++;
}

// Find nearest node to given coordinates
int findNearestNode(double lat, double lon) {
    if (numNodes == 0) return -1;
    
    int nearest = 0;
    double minDist = haversineDistance(lat, lon, nodes[0].lat, nodes[0].lon);
    
    for (int i = 1; i < numNodes; i++) {
        double d = haversineDistance(lat, lon, nodes[i].lat, nodes[i].lon);
        if (d < minDist) {
            minDist = d;
            nearest = i;
        }
    }
    
    return nearest;
}

// Add edge to graph
void addEdge(int from, int to, Mode mode, double distance) {
    if (numEdges >= MAX_NODES * 10) {
        printf("Warning: Maximum edges reached!\n");
        return;
    }
    
    edges[numEdges].from = from;
    edges[numEdges].to = to;
    edges[numEdges].mode = mode;
    edges[numEdges].distance = distance;
    numEdges++;
}
