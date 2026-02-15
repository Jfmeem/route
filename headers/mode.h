#ifndef MODE_H
#define MODE_H

// Earth radius for Haversine formula
#define EARTH_RADIUS_KM 6371.0
#define PI 3.14159265358979323846

// Transport modes
typedef enum {
    MODE_WALK,
    MODE_CAR,
    MODE_METRO,
    MODE_BIKOLPO,  // Bikolpo Bus
    MODE_UTTARA    // Uttara Bus
} Mode;

// Cost rates (Taka per km)
#define CAR_RATE 20.0
#define METRO_RATE 5.0
#define BIKOLPO_RATE 7.0
#define UTTARA_RATE 10.0

// Function to get mode name as string
const char* getModeName(Mode mode);

#endif
