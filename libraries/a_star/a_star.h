#ifndef A_STAR
#define A_STAR

#include <iostream>
#include <limits>
#include <math.h>
#include <utility>
#include <stack>
#include <vector>

namespace a_star {

enum Robot {
    GOALIE,
    DEFENDER,
    MID_FIELDER,
    UNDEFINED
};

struct Cell {
    struct Blocked {
        bool is_blocked;
        // Robot that is blocking the cell if any.
        Robot robot = Robot::UNDEFINED;   
    };
    Blocked blocked;
    std::pair<int, int> coordinate;
    std::pair<int, int> parent_coordinate;
    double g;  // Cost from the starting point to this cell
    double h;  // Estimated cost from target to this cell
    double f;  // f = g + h
    friend std::ostream & operator << (std::ostream &out, const Cell &cell) {
        out << "Cell Coordinates: " << cell.coordinate.first << " " <<
          cell.coordinate.second << "\nParent Coordinates: " << 
          cell.parent_coordinate.first <<  "" << " " << 
          cell.parent_coordinate.second << "\ng: " << 
          cell.g << "\nh: " << cell.h << "\nf: " << cell.f << "\nBlocked: " << 
          cell.blocked.is_blocked << " robot: "  << cell.blocked.robot << 
          std::endl << std::endl;
        return out;
    }
};

class AStar {
  private:
    int rows_;
    int columns_;
    int min_x_;
    int max_x_;
    int min_y_;
    int max_y_;
    std::vector<Cell> map_;

    bool IsValidCoordinate(const std::pair<int, int> &coordinate) { 
        return (coordinate.first >= min_x_) && (coordinate.first <= max_x_) && 
          (coordinate.second >= min_y_) && (coordinate.second <= max_y_); 
    }

    bool IsDestination(const std::pair<int, int> &actual_coordinate, 
      const std::pair<int, int> &target_coordinate) { 
        return actual_coordinate == target_coordinate;
    }

    double CalculateHValue(const std::pair<int, int> &actual_coordinate, 
      const std::pair<int, int> &target_coordinate) { 
        return sqrt(pow((actual_coordinate.first - target_coordinate.first), 2)
          + pow((actual_coordinate.second - target_coordinate.second), 2));
    } 

    double CalculateGValue(const std::pair<int, int> &source_coordinate, 
      const std::pair<int, int> &target_coordinate);

    std::pair<bool, int> FromCoordinatesToIndex(
      const std::pair<int, int> &coordinate);

    std::pair<int, int> FromIndexToCoordinate(int index) {
        return std::make_pair(
          index % columns_ + min_x_, index / columns_ + min_y_);
    }

    std::vector<int> GetCoordinateNeighborsIndexes(
      const std::pair<int, int> &coordinate);
      
  public:
    void CreateMap(
      const std::vector<std::tuple<Robot, int, int>> &blocked_coordinates, 
      const std::vector<std::pair<int, int>> &corners, int safe_zone);

    bool AStarSearch(const Robot &robot, 
      const std::pair<int, int> &source_coordinate, 
      const std::pair<int, int> &target_coordinate);

    std::stack<std::pair<int, int>> GetPath(
      const std::pair<int, int> &source_coordinate, 
      const std::pair<int, int> &target_coordinate);
    
    void PrintCell(const std::pair<int, int> &coordinate) {
        auto cell_index = FromCoordinatesToIndex(coordinate);
        if (cell_index.first) {
            std::cout << map_.at(cell_index.second);
        } else {
            std::cout << "No valid coordinate" << std::endl;
        }
    }

    void PrintMap();
};  

}  // namespace a_star

#endif  // A_STAR