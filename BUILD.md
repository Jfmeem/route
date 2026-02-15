# Build Instructions

## For Windows (using g++)

### Build
```cmd
build.bat
```

Or manually:
```cmd
g++ -std=c++11 -Wall -Wextra -O2 -Iheaders -c main.cpp -o main.o
g++ -std=c++11 -Wall -Wextra -O2 -Iheaders -c src/mode.cpp -o src/mode.o
g++ -std=c++11 -Wall -Wextra -O2 -Iheaders -c src/nodesAndEdges.cpp -o src/nodesAndEdges.o
g++ -std=c++11 -Wall -Wextra -O2 -Iheaders -c src/csvParse.cpp -o src/csvParse.o
g++ -std=c++11 -Wall -Wextra -O2 -Iheaders -c src/problem1.cpp -o src/problem1.o
g++ main.o src/mode.o src/nodesAndEdges.o src/csvParse.o src/problem1.o -o dhaka_transport.exe
```

### Run
```cmd
dhaka_transport.exe
```

## For Linux/Mac (using make)

### Build
```bash
make
```

### Run
```bash
./dhaka_transport
```

## Clean Build
```bash
make clean  # Linux/Mac
```

Or manually delete `.o` files and executable on Windows.
