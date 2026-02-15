#include "../headers/problem5.h"
#include "../headers/nodesAndEdges.h"
#include "../headers/mode.h"
#include <cstdio>
#include <cmath>

// Problem 5: Simplified version - Fastest route with schedule
void runProblem5() {
    double srcLat, srcLon, destLat, destLon;
    int startHour, startMin;
    
    printf("\n--- Problem 5: Fastest Route with Schedule ---\n");
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
    
    // Dijkstra initialization - optimizing for TIME (distance/speed)
    for (int i = 0; i < numNodes; i++) {
        dist[i] = INF;
        prev[i] = -1;
        prevEdge[i] = -1;
        visited[i] = 0;
    }
    dist[source] = 0;
    
    // Speed constants (km/h)
    const double CAR_SPEED = 30.0;
    const double METRO_SPEED = 40.0;
    const double BUS_SPEED = 25.0;
    
    // Dijkstra's algorithm - optimizing for time
    for (int count = 0; count < numNodes; count++) {
        int u = -1;
        double minTime = INF;
        
        for (int i = 0; i < numNodes; i++) {
            if (!visited[i] && dist[i] < minTime) {
                minTime = dist[i];
                u = i;
            }
        }
        
        if (u == -1 || u == target) break;
        
        visited[u] = 1;
        
        // Relax edges - considering time = distance / speed
        for (int i = 0; i < numEdges; i++) {
            if (edges[i].from == u) {
                if (edges[i].mode != MODE_CAR && edges[i].mode != MODE_METRO && 
                    edges[i].mode != MODE_BIKOLPO && edges[i].mode != MODE_UTTARA) {
                    continue;
                }
                
                int v = edges[i].to;
                
                double speed = CAR_SPEED;
                if (edges[i].mode == MODE_METRO) speed = METRO_SPEED;
                else if (edges[i].mode == MODE_BIKOLPO || edges[i].mode == MODE_UTTARA) 
                    speed = BUS_SPEED;
                
                double travelTime = (edges[i].distance / speed) * 60.0; // Convert to minutes
                double newTime = dist[u] + travelTime;
                
                if (newTime < dist[v]) {
                    dist[v] = newTime;
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
    
    int hours = (int)(dist[target] / 60);
    int mins = (int)(dist[target]) % 60;
    
    printf("\n========== PROBLEM 5: FASTEST ROUTE WITH SCHEDULE ==========\n");
    printf("Fastest path found with travel time: %d hours %d minutes\n", hours, mins);
    printf("Vehicle speeds: Metro 40 km/h, Car 30 km/h, Bus 25 km/h\n");
    printf("============================================================\n");
}
