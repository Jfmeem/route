#include <cstdio>
#include <cstdlib>
#include "headers/mode.h"
#include "headers/nodesAndEdges.h"
#include "headers/csvParse.h"
#include "headers/problem1.h"
#include "headers/problem2.h"
#include "headers/problem3.h"
#include "headers/problem4.h"
#include "headers/problem5.h"
#include "headers/problem6.h"

int main() {
    printf("========================================\n");
    printf("   Dhaka Transportation Route Finder\n");
    printf("========================================\n\n");
    
    // Load all CSV files
    printf("Loading transportation network...\n");
    parseRoadmapCSV("Dataset/Roadmap-Dhaka.csv");
    parseMetroCSV("Dataset/Routemap-DhakaMetroRail.csv");
    parseBusCSV("Dataset/Routemap-BikolpoBus.csv", MODE_BIKOLPO);
    parseBusCSV("Dataset/Routemap-UttaraBus.csv", MODE_UTTARA);
    
    if (numNodes == 0) {
        printf("No nodes loaded. Please check your CSV files.\n");
        return 1;
    }
    
    printf("\nGraph loaded successfully!\n");
    printf("Total nodes: %d\n", numNodes);
    printf("Total edges: %d\n\n", numEdges);
    
    // Main menu loop
    while (true) {
        printf("\n========================================\n");
        printf("          Mr Efficient Menu\n");
        printf("========================================\n");
        printf("[1] Shortest Car Route [Problem 1]\n");
        printf("[2] Cheapest Route (Car and Metro) [Problem 2]\n");
        printf("[3] Cheapest Route (All Modes) [Problem 3]\n");
        printf("[4] Cheapest Route with Schedule [Problem 4]\n");
        printf("[5] Fastest Route with Schedule [Problem 5]\n");
        printf("[6] Cheapest Route with Deadline [Problem 6]\n");
        printf("[7] Quit\n");
        printf("========================================\n");
        
        int choice;
        printf("Enter Choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please try again.\n");
            while (getchar() != '\n'); // Clear input buffer
            continue;
        }
        
        if (choice == 7) {
            printf("\nThank you for using Mr Efficient!\n");
            break;
        }
        
        switch (choice) {
            case 1:
                runProblem1();
                break;
            case 2:
                runProblem2();
                break;
            case 3:
                runProblem3();
                break;
            case 4:
                runProblem4();
                break;
            case 5:
                runProblem5();
                break;
            case 6:
                runProblem6();
                break;
            default:
                printf("\nInvalid choice. Please try again.\n");
                break;
        }
    }
    
    return 0;
}
