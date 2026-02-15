#include "../headers/problem1.h"
#include "../headers/nodesAndEdges.h"
#include "../headers/mode.h"
#include <cstdio>
#include <cmath>

// Print detailed path for Problem 1
void printProblem1Details(int path[], int pathLen, int source, int target,
                          double srcLat, double srcLon, double destLat, double destLon) {
    double totalDistance = 0.0;
    double totalCost = 0.0;
    
    printf("\n========== PROBLEM 1: SHORTEST CAR ROUTE ==========\n");
    printf("Source: (%.6f, %.6f)\n", srcLon, srcLat);
    printf("Destination: (%.6f, %.6f)\n", destLon, destLat);
    printf("===================================================\n\n");
    
    // If source is not exactly on a node, show walking segment
    if (fabs(nodes[source].lat - srcLat) > 1e-6 || 
        fabs(nodes[source].lon - srcLon) > 1e-6) {
        double walkDist = haversineDistance(srcLat, srcLon,
                                           nodes[source].lat, nodes[source].lon);
        printf("Walk from Source (%.6f, %.6f) to (%.6f, %.6f)\n",
               srcLon, srcLat, nodes[source].lon, nodes[source].lat);
        printf("  Distance: %.3f km, Cost: Tk 0.00\n\n", walkDist);
        totalDistance += walkDist;
    }
    
    // Print each car segment
    for (int i = pathLen - 1; i > 0; i--) {
        int from = path[i];
        int to = path[i - 1];
        
        // Find the edge distance
        double distSeg = 0;
        for (int j = 0; j < numEdges; j++) {
            if (edges[j].from == from && edges[j].to == to && edges[j].mode == MODE_CAR) {
                distSeg = edges[j].distance;
                break;
            }
        }
        
        double costSeg = distSeg * CAR_RATE;
        totalDistance += distSeg;
        totalCost += costSeg;
        
        printf("Ride Car from (%.6f, %.6f) to (%.6f, %.6f)\n",
               nodes[from].lon, nodes[from].lat,
               nodes[to].lon, nodes[to].lat);
        printf("  Distance: %.3f km, Cost: Tk %.2f\n\n", distSeg, costSeg);
    }
    
    // If destination is not exactly on a node, show walking segment
    if (fabs(nodes[target].lat - destLat) > 1e-6 || 
        fabs(nodes[target].lon - destLon) > 1e-6) {
        double walkDist = haversineDistance(nodes[target].lat, nodes[target].lon,
                                           destLat, destLon);
        printf("Walk from (%.6f, %.6f) to Destination (%.6f, %.6f)\n",
               nodes[target].lon, nodes[target].lat, destLon, destLat);
        printf("  Distance: %.3f km, Cost: Tk 0.00\n\n", walkDist);
        totalDistance += walkDist;
    }
    
    printf("===================================================\n");
    printf("Total Distance: %.3f km\n", totalDistance);
    printf("Total Cost: Tk %.2f\n", totalCost);
    printf("===================================================\n");
}

// Run Problem 1: Shortest Car Route
void runProblem1() {
    double srcLat, srcLon, destLat, destLon;
    
    printf("\n--- Problem 1: Shortest Car Route ---\n");
    printf("Enter source latitude and longitude: ");
    if (scanf("%lf %lf", &srcLat, &srcLon) != 2) {
        printf("Invalid input\n");
        return;
    }
    
    printf("Enter destination latitude and longitude: ");
    if (scanf("%lf %lf", &destLat, &destLon) != 2) {
        printf("Invalid input\n");
        return;
    }
    
    // Find nearest nodes
    int source = findNearestNode(srcLat, srcLon);
    int target = findNearestNode(destLat, destLon);
    
    if (source == -1 || target == -1) {
        printf("Error: Could not find nodes\n");
        return;
    }
    
    printf("\nUsing nearest roadmap nodes:\n");
    printf("Source Node: %s (%.6f, %.6f)\n", nodes[source].name, 
           nodes[source].lat, nodes[source].lon);
    printf("Target Node: %s (%.6f, %.6f)\n", nodes[target].name,
           nodes[target].lat, nodes[target].lon);
    
    // Dijkstra initialization
    for (int i = 0; i < numNodes; i++) {
        dist[i] = INF;
        prev[i] = -1;
        visited[i] = 0;
    }
    dist[source] = 0;
    
    // Dijkstra's algorithm
    for (int count = 0; count < numNodes; count++) {
        int u = -1;
        double minDist = INF;
        
        // Find unvisited node with minimum distance
        for (int i = 0; i < numNodes; i++) {
            if (!visited[i] && dist[i] < minDist) {
                minDist = dist[i];
                u = i;
            }
        }
        
        if (u == -1 || u == target) break;
        
        visited[u] = 1;
        
        // Relax edges (only consider CAR mode for Problem 1)
        for (int i = 0; i < numEdges; i++) {
            if (edges[i].from == u && edges[i].mode == MODE_CAR) {
                int v = edges[i].to;
                double newDist = dist[u] + edges[i].distance;
                if (newDist < dist[v]) {
                    dist[v] = newDist;
                    prev[v] = u;
                }
            }
        }
    }
    
    // Build path
    int path[MAX_NODES];
    int pathLen = 0;
    for (int at = target; at != -1; at = prev[at]) {
        path[pathLen++] = at;
    }
    
    if (pathLen == 1 || dist[target] >= INF) {
        printf("No path found between the selected nodes.\n");
        return;
    }
    
    printf("\nShortest path found with distance: %.3f km\n", dist[target]);
    
    // Print detailed path
    printProblem1Details(path, pathLen, source, target, srcLat, srcLon, destLat, destLon);
}
