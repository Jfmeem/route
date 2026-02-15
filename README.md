# Dhaka Transportation Route Finder

A C++ implementation of a transportation route finding system for Dhaka, solving 6 different optimization problems using graph algorithms.

## Project Structure

```
.
├── headers/             # Header files
│   ├── mode.h           # Transport mode definitions
│   ├── nodesAndEdges.h  # Graph data structures
│   ├── csvParse.h       # CSV parsing functions
│   └── problem1.h       # Problem 1 header
├── src/                 # Source files
│   ├── mode.cpp
│   ├── nodesAndEdges.cpp
│   ├── csvParse.cpp
│   └── problem1.cpp
├── Dataset/             # CSV data files
│   ├── Roadmap-Dhaka.csv
│   ├── Routemap-DhakaMetroRail.csv
│   ├── Routemap-BikolpoBus.csv
│   └── Routemap-UttaraBus.csv
├── main.cpp             # Main program
└── Makefile             # Build configuration
```

## Problems

### Implemented
- **Problem 1**: Shortest Car Route - Find shortest distance using only roads
- **Problem 2**: Cheapest Route (Car + Metro) - Find cheapest route using car and metro
- **Problem 3**: Cheapest Route (All Modes) - Find cheapest route using all transport modes
- **Problem 4**: Cheapest Route with Schedule - Find cheapest route considering vehicle schedules
- **Problem 5**: Fastest Route with Schedule - Find fastest route based on vehicle speeds
- **Problem 6**: Cheapest Route with Deadline - Find cheapest route meeting a time deadline

**All 6 problems are fully functional!**

For detailed explanations of each module and algorithm, see [guide.md](guide.md).

## Building

```bash
make
```

## Running

```bash
make run
# or
./dhaka_transport
```

## Usage

1. Select a problem from the menu
2. Enter source coordinates (latitude longitude)
3. Enter destination coordinates (latitude longitude)
4. View the optimal route and cost

## Example Input

For Problem 1 (Shortest Car Route):
```
Source: 23.8103 90.4125
Destination: 23.7465 90.3763
```

## Algorithms Used

- **Dijkstra's Algorithm**: For finding shortest/cheapest paths
- **Haversine Formula**: For calculating distances between coordinates

## Cost Rates

- Car: Tk 20/km
- Metro: Tk 5/km
- Bikolpo Bus: Tk 7/km
- Uttara Bus: Tk 10/km
