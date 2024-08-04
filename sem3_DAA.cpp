#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
#include <cmath>
#include <climits> // Include the <climits> header for integer limits

struct Point {
    double x, y;
};

// Function to calculate the polar angle of a point with respect to the pivot
double polarAngle(const Point& p, const Point& pivot) {
    double dx = p.x - pivot.x;
    double dy = p.y - pivot.y;
    return atan2(dy, dx);
}

// Function to check if three points make a counterclockwise turn (left turn)
int orientation(const Point& p, const Point& q, const Point& r) {
    double val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    if (val == 0) return 0;      // Collinear
    return (val > 0) ? 1 : 2;  // Clockwise or Counterclockwise
}

// Graham Scan algorithm to find the convex hull
std::vector<Point> grahamScan(std::vector<Point>& points) {
    // Find the bottommost point
    Point pivot = points[0];
    int pivotIdx = 0;

    for (int i = 1; i < points.size(); i++) {
        if (points[i].y < pivot.y || (points[i].y == pivot.y && points[i].x < pivot.x)) {
            pivot = points[i];
            pivotIdx = i;
        }
    }

    // Sort the points by polar angle with respect to the pivot
    std::sort(points.begin(), points.end(), [&](const Point& p1, const Point& p2) {
        double angle1 = polarAngle(p1, pivot);
        double angle2 = polarAngle(p2, pivot);
        if (angle1 != angle2) return angle1 < angle2;
        return (p1.x < p2.x) || (p1.x == p2.x && p1.y < p2.y);
    });

    // Initialize the convex hull with the first three points
    std::vector<Point> convexHull;
    convexHull.push_back(points[0]);
    convexHull.push_back(points[1]);
    convexHull.push_back(points[2]);

    // Process the rest of the points
    for (int i = 3; i < points.size(); i++) {
        while (convexHull.size() > 1 && orientation(convexHull[convexHull.size() - 2], convexHull.back(), points[i]) != 2) {
            convexHull.pop_back();
        }
        convexHull.push_back(points[i]);
    }

    return convexHull;
}

void printContainerPositions(const std::vector<Point>& points) {
    std::cout << "Container Positions:" << std::endl;
    for (const Point& p : points) {
        std::cout << "(" << p.x << ", " << p.y << ")" << std::endl;
    }
}

void printConvexHull(const std::vector<Point>& convexHull) {
    std::cout << "Convex Hull Points:" << std::endl;
    for (const Point& p : convexHull) {
        std::cout << "(" << p.x << ", " << p.y << ")" << std::endl;
    }
}

void visualizeContainerPlacement(const std::vector<Point>& points, const std::vector<Point>& convexHull) {
    // Create an ASCII art representation of the container placement
    const char containerChar = 'C';
    const char hullChar = '*';
    const char emptyChar = '.';
    
    // Determine the dimensions of the visualization grid
    int minX = INT_MAX;
    int minY = INT_MAX;
    int maxX = INT_MIN;
    int maxY = INT_MIN;

    for (const Point& p : points) {
        minX = std::min(minX, static_cast<int>(p.x));
        minY = std::min(minY, static_cast<int>(p.y));
        maxX = std::max(maxX, static_cast<int>(p.x));
        maxY = std::max(maxY, static_cast<int>(p.y));
    }

    // Create a 2D grid for visualization
    std::vector<std::vector<char>> grid(maxY - minY + 1, std::vector<char>(maxX - minX + 1, emptyChar));

    // Mark container positions
    for (const Point& p : points) {
        grid[p.y - minY][p.x - minX] = containerChar;
    }

    // Mark convex hull
    for (const Point& p : convexHull) {
        grid[p.y - minY][p.x - minX] = hullChar;
    }

    // Print the visualization grid
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].size(); j++) {
            std::cout << grid[i][j];
        }
        std::cout << std::endl;
    }
}

int main() {
    std::vector<Point> containerPositions;
    int numContainers;

    std::cout << "Enter the number of containers: ";
    std::cin >> numContainers;

    std::cout << "Enter the container positions (x y):" << std::endl;
    for (int i = 0; i < numContainers; i++) {
        Point p;
        std::cin >> p.x >> p.y;
        containerPositions.push_back(p);
    }

    std::vector<Point> convexHull = grahamScan(containerPositions);

    printContainerPositions(containerPositions);
    printConvexHull(convexHull);

    visualizeContainerPlacement(containerPositions, convexHull);

    return 0;
}