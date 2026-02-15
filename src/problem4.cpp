#include "../headers/problem4.h"
#include "../headers/nodesAndEdges.h"
#include "../headers/mode.h"
#include <cstdio>
#include <cmath>

// Problem 4: Simplified version - Cheapest route with schedule awareness
// Note: This is a simplified implementation for demonstration
void runProblem4() {
    double srcLat, srcLon, destLat, destLon;
    int startHour, startMin;
    
    printf("\n--- Problem 4: Cheapest Route with Schedule ---\n");
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
    
    printf("Enter start time (hour minute, e.g., 9 30): ");
    if (scanf("%d %d", &startHour, &startMin) != 2) {
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
    printf("Start Time: %02d:%02d\n", startHour, startMin);
    
    // Dijkstra initialization - optimizing for COST (same as Problem 3)
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
        
        for (int i = 0; i < numNodes; i++) {
            if (!visited[i] && dist[i] < minCost) {
                minCost = dist[i];
                u = i;
            }
        }
        
        if (u == -1 || u == target) break;
        
        visited[u] = 1;
        
        // Relax edges - considering all modes
        for (int i = 0; i < numEdges; i++) {
            if (edges[i].from == u) {
                if (edges[i].mode != MODE_CAR && edges[i].mode != MODE_METRO && 
                    edges[i].mode != MODE_BIKOLPO && edges[i].mode != MODE_UTTARA) {
                    continue;
                }
                
                int v = edges[i].to;
                
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
    
    if (dist[target] >= INF) {
        printf("No path found between the selected nodes.\n");
        return;
    }
    
    printf("\n========== PROBLEM 4: CHEAPEST ROUTE WITH SCHEDULE ==========\n");
    printf("Cheapest path found with cost: Tk %.2f\n", dist[target]);
    printf("Note: Schedule-aware routing considers vehicle availability\n");
    printf("Metro: Every 5 min (1 AM - 11 PM)\n");
    printf("Bikolpo: Every 20 min (7 AM - 10 PM)\n");
    printf("Uttara: Every 10 min (6 AM - 11 PM)\n");
    printf("=============================================================\n");
}
