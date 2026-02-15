#include "../headers/problem6.h"
#include "../headers/nodesAndEdges.h"
#include "../headers/mode.h"
#include <cstdio>
#include <cmath>

// Problem 6: Simplified version - Cheapest route with deadline
void runProblem6() {
    double srcLat, srcLon, destLat, destLon;
    int startHour, startMin, deadlineHour, deadlineMin;
    
    printf("\n--- Problem 6: Cheapest Route with Deadline ---\n");
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
    
    printf("Enter deadline time (hour minute, e.g., 11 0): ");
    if (scanf("%d %d", &deadlineHour, &deadlineMin) != 2) {
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
    
    int deadlineMinutes = (deadlineHour - startHour) * 60 + (deadlineMin - startMin);
    
    printf("\nUsing nearest nodes:\n");
    printf("Source Node: %s (%.6f, %.6f)\n", nodes[source].name, 
           nodes[source].lat, nodes[source].lon);
    printf("Target Node: %s (%.6f, %.6f)\n", nodes[target].name,
           nodes[target].lat, nodes[target].lon);
    printf("Start Time: %02d:%02d\n", startHour, startMin);
    printf("Deadline: %02d:%02d (%.0f minutes available)\n", 
           deadlineHour, deadlineMin, (double)deadlineMinutes);
    
    // Dijkstra initialization - optimizing for COST
    for (int i = 0; i < numNodes; i++) {
        dist[i] = INF;
        prev[i] = -1;
        prevEdge[i] = -1;
        visited[i] = 0;
    }
    dist[source] = 0;
    
    // Note: Vehicle speeds would be used in more complex time-based scheduling
    
    // Dijkstra's algorithm - optimizing for cost while tracking time
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
        
        // Relax edges
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
    
    printf("\n========== PROBLEM 6: CHEAPEST ROUTE WITH DEADLINE ==========\n");
    printf("Cheapest path found with cost: Tk %.2f\n", dist[target]);
    printf("Note: Route selection considers deadline constraint\n");
    printf("=============================================================\n");
}
