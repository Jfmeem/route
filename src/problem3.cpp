#include "../headers/problem3.h"
#include "../headers/nodesAndEdges.h"
#include "../headers/mode.h"
#include <cstdio>
#include <cmath>

// Print detailed path for Problem 3
void printProblem3Details(int path[], int pathEdges[], int pathLen, int source, int target,
                          double srcLat, double srcLon, double destLat, double destLon) {
    double totalDistance = 0.0;
    double totalCost = 0.0;
    
    printf("\n========== PROBLEM 3: CHEAPEST ROUTE (ALL MODES) ==========\n");
    printf("Source: (%.6f, %.6f)\n", srcLon, srcLat);
    printf("Destination: (%.6f, %.6f)\n", destLon, destLat);
    printf("===========================================================\n\n");
    
    // If source is not exactly on a node, show walking segment
    if (fabs(nodes[source].lat - srcLat) > 1e-6 || 
        fabs(nodes[source].lon - srcLon) > 1e-6) {
        double walkDist = haversineDistance(srcLat, srcLon,
                                           nodes[source].lat, nodes[source].lon);
        printf("Walk from Source (%.6f, %.6f) to %s (%.6f, %.6f)\n",
               srcLon, srcLat, nodes[source].name, nodes[source].lon, nodes[source].lat);
        printf("  Distance: %.3f km, Cost: Tk 0.00\n\n", walkDist);
        totalDistance += walkDist;
    }
    
    // Print each segment using the stored edge information
    for (int i = pathLen - 1; i > 0; i--) {
        int from = path[i];
        int to = path[i - 1];
        int edgeIdx = pathEdges[i - 1];
        
        double distSeg = 0;
        Mode edgeMode = MODE_CAR;
        
        if (edgeIdx >= 0 && edgeIdx < numEdges) {
            distSeg = edges[edgeIdx].distance;
            edgeMode = edges[edgeIdx].mode;
        }
        
        // Determine rate based on mode
        double rate = CAR_RATE;
        if (edgeMode == MODE_METRO) rate = METRO_RATE;
        else if (edgeMode == MODE_BIKOLPO) rate = BIKOLPO_RATE;
        else if (edgeMode == MODE_UTTARA) rate = UTTARA_RATE;
        
        double costSeg = distSeg * rate;
        totalDistance += distSeg;
        totalCost += costSeg;
        
        printf("Ride %s from %s (%.6f, %.6f) to %s (%.6f, %.6f)\n",
               getModeName(edgeMode),
               nodes[from].name, nodes[from].lon, nodes[from].lat,
               nodes[to].name, nodes[to].lon, nodes[to].lat);
        printf("  Distance: %.3f km, Cost: Tk %.2f\n\n", distSeg, costSeg);
    }
    
    // If destination is not exactly on a node, show walking segment
    if (fabs(nodes[target].lat - destLat) > 1e-6 || 
        fabs(nodes[target].lon - destLon) > 1e-6) {
        double walkDist = haversineDistance(nodes[target].lat, nodes[target].lon,
                                           destLat, destLon);
        printf("Walk from %s (%.6f, %.6f) to Destination (%.6f, %.6f)\n",
               nodes[target].name, nodes[target].lon, nodes[target].lat, destLon, destLat);
        printf("  Distance: %.3f km, Cost: Tk 0.00\n\n", walkDist);
        totalDistance += walkDist;
    }
    
    printf("===========================================================\n");
    printf("Total Distance: %.3f km\n", totalDistance);
    printf("Total Cost: Tk %.2f\n", totalCost);
    printf("===========================================================\n");
}

// Run Problem 3: Cheapest Route (All Modes)
void runProblem3() {
    double srcLat, srcLon, destLat, destLon;
    
    printf("\n--- Problem 3: Cheapest Route (All Modes) ---\n");
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
    
    printf("\nUsing nearest nodes:\n");
    printf("Source Node: %s (%.6f, %.6f)\n", nodes[source].name, 
           nodes[source].lat, nodes[source].lon);
    printf("Target Node: %s (%.6f, %.6f)\n", nodes[target].name,
           nodes[target].lat, nodes[target].lon);
    
    // Dijkstra initialization - optimizing for COST
    for (int i = 0; i < numNodes; i++) {
        dist[i] = INF;
        prev[i] = -1;
        prevEdge[i] = -1;
        visited[i] = 0;
    }
    dist[source] = 0;
    
    // Dijkstra's algorithm - optimizing for cost
    for (int count = 0; count < numNodes; count++) {
        int u = -1;
        double minCost = INF;
        
        // Find unvisited node with minimum cost
        for (int i = 0; i < numNodes; i++) {
            if (!visited[i] && dist[i] < minCost) {
                minCost = dist[i];
                u = i;
            }
        }
        
        if (u == -1 || u == target) break;
        
        visited[u] = 1;
        
        // Relax edges - considering CAR, METRO, and BUSES
        for (int i = 0; i < numEdges; i++) {
            if (edges[i].from == u) {
                // Consider CAR, METRO, BIKOLPO, and UTTARA modes
                if (edges[i].mode != MODE_CAR && edges[i].mode != MODE_METRO && 
                    edges[i].mode != MODE_BIKOLPO && edges[i].mode != MODE_UTTARA) {
                    continue;
                }
                
                int v = edges[i].to;
                
                // Determine rate based on mode
                double rate = CAR_RATE;
                if (edges[i].mode == MODE_METRO) rate = METRO_RATE;
                else if (edges[i].mode == MODE_BIKOLPO) rate = BIKOLPO_RATE;
                else if (edges[i].mode == MODE_UTTARA) rate = UTTARA_RATE;
                
                double edgeCost = edges[i].distance * rate;
                double newCost = dist[u] + edgeCost;
                
                if (newCost < dist[v]) {
                    dist[v] = newCost;
                    prev[v] = u;
                    prevEdge[v] = i;
                }
            }
        }
    }
    
    // Build path
    int path[MAX_NODES];
    int pathEdges[MAX_NODES];
    int pathLen = 0;
    for (int at = target; at != -1; at = prev[at]) {
        path[pathLen] = at;
        pathEdges[pathLen] = prevEdge[at];
        pathLen++;
    }
    
    if (pathLen == 1 || dist[target] >= INF) {
        printf("No path found between the selected nodes.\n");
        return;
    }
    
    printf("\nCheapest path found with cost: Tk %.2f\n", dist[target]);
    
    // Print detailed path
    printProblem3Details(path, pathEdges, pathLen, source, target, srcLat, srcLon, destLat, destLon);
}
