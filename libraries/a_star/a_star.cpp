#include "a_star.h"

#include <iostream>
#include <queue>
#include <stack>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>


using namespace a_star;

namespace {
    class Compare {
      public:
        bool operator()(const Cell *a, const Cell *b) {
            return a->f > b->f;
        }
    };
}

std::pair<bool, int> AStar::FromCoordinatesToIndex(
  const std::pair<int, int> &coordinate) {
    if (IsValidCoordinate(coordinate)) {
        return std::make_pair(true, coordinate.second * columns_ + 
            coordinate.first);
    } else {
        return std::make_pair(false, -1);
    }
}

std::stack<std::pair<int, int>> AStar::GetPath(
  const std::pair<int, int> &source_coordinate, 
  const std::pair<int, int> &target_coordinate) {
    auto cell_index = FromCoordinatesToIndex(target_coordinate).second;
    std::stack<std::pair<int, int>> coordinates_path;
    coordinates_path.push(map_.at(cell_index).coordinate);
    while(map_.at(cell_index).coordinate != source_coordinate) {
        cell_index = FromCoordinatesToIndex(
          map_.at(cell_index).parent_coordinate).second;
        coordinates_path.push(map_.at(cell_index).coordinate);
    }
    return coordinates_path;
}

std::vector<int> AStar::GetCoordinateNeighborsIndexes(
  const std::pair<int, int> &coordinate) {
    std::vector<int> neighbors_indexes;
    auto neighbor = FromCoordinatesToIndex(std::make_pair(coordinate.first, 
      coordinate.second - 1));
    if (neighbor.first) {
        neighbors_indexes.push_back(neighbor.second);
    }
    neighbor = FromCoordinatesToIndex(std::make_pair(coordinate.first + 1, 
      coordinate.second - 1));
    if (neighbor.first) {
        neighbors_indexes.push_back(neighbor.second);
    }
    neighbor = FromCoordinatesToIndex(std::make_pair(coordinate.first + 1, 
      coordinate.second));
    if (neighbor.first) {
        neighbors_indexes.push_back(neighbor.second);
    }
    neighbor = FromCoordinatesToIndex(std::make_pair(coordinate.first + 1, 
      coordinate.second + 1));
    if (neighbor.first) {
        neighbors_indexes.push_back(neighbor.second);
    }
    neighbor = FromCoordinatesToIndex(std::make_pair(coordinate.first, 
      coordinate.second + 1));
    if (neighbor.first) {
        neighbors_indexes.push_back(neighbor.second);
    }
    neighbor = FromCoordinatesToIndex(std::make_pair(coordinate.first - 1, 
      coordinate.second + 1));
    if (neighbor.first) {
        neighbors_indexes.push_back(neighbor.second);
    }
    neighbor = FromCoordinatesToIndex(std::make_pair(coordinate.first - 1, 
      coordinate.second));
    if (neighbor.first) {
        neighbors_indexes.push_back(neighbor.second);
    }
    neighbor = FromCoordinatesToIndex(std::make_pair(coordinate.first - 1, 
      coordinate.second - 1));
    if (neighbor.first) {
        neighbors_indexes.push_back(neighbor.second);
    }
    return neighbors_indexes;
}

// TODO: Try different ways to calculate the G value, for example taking into
// account the angle of the robot inside the coordinate
double AStar::CalculateGValue(const std::pair<int, int> &source_coordinate, 
  const std::pair<int, int> &target_coordinate) {
    int x_difference = source_coordinate.first - target_coordinate.first; 
    int y_difference = source_coordinate.second - target_coordinate.second; 
    if (x_difference == 1 || x_difference == -1) {
        if (y_difference != 0) {
            return 1.4;
        }
    }
    return 1.0;
}

void AStar::CreateMap(
  const std::vector<std::tuple<Robot, int, int>> &blocked_coordinates, 
  const std::vector<std::pair<int, int>> &corners, int safe_zone) {
    rows_ = corners.at(2).second - corners.at(0).second + 1;
    columns_ = corners.at(1).first - corners.at(0).first + 1;
    min_x_ = corners.at(0).first;
    max_x_ = corners.at(1).first;
    min_y_ = corners.at(0).second;
    max_y_ = corners.at(2).second;
    map_.clear();
    map_.resize(columns_ * rows_);

    std::unordered_map<int, Robot> occupied_indexes;
    for (const auto &blocked_coordinate : blocked_coordinates) {
        auto occupied_index = FromCoordinatesToIndex(std::make_pair(
          std::get<1>(blocked_coordinate), std::get<2>(blocked_coordinate)));
        if (occupied_index.first) {
            occupied_indexes.insert(std::make_pair(occupied_index.second, 
              std::get<0>(blocked_coordinate)));
            std::pair<int, int> expand_safe_zone;
            expand_safe_zone = std::make_pair(std::get<1>(blocked_coordinate), 
              std::get<2>(blocked_coordinate));
            for (int j = 1; j <= safe_zone; j++) {
                expand_safe_zone.second = std::get<2>(blocked_coordinate);
                expand_safe_zone.second = expand_safe_zone.second + j;
                auto expanded_safe_zone_coordinate = FromCoordinatesToIndex(
                    expand_safe_zone);
                if (expanded_safe_zone_coordinate.first) {
                    occupied_indexes.insert(std::make_pair(
                      expanded_safe_zone_coordinate.second, 
                      std::get<0>(blocked_coordinate)));
                }
                expand_safe_zone.second = std::get<2>(blocked_coordinate);
                expand_safe_zone.second = expand_safe_zone.second - j;
                expanded_safe_zone_coordinate = FromCoordinatesToIndex(
                    expand_safe_zone);
                if (expanded_safe_zone_coordinate.first) {
                    occupied_indexes.insert(std::make_pair(
                      expanded_safe_zone_coordinate.second, 
                      std::get<0>(blocked_coordinate)));
                }
            }
            for (int i = 1; i <= safe_zone; i++) {
                expand_safe_zone = std::make_pair(
                  std::get<1>(blocked_coordinate), 
                  std::get<2>(blocked_coordinate));
                expand_safe_zone.first = expand_safe_zone.first + i;
                for (int j = 0; j <= safe_zone; j++) {
                    expand_safe_zone.second = std::get<2>(blocked_coordinate);
                    expand_safe_zone.second = expand_safe_zone.second + j;
                    auto expanded_safe_zone_coordinate = FromCoordinatesToIndex(
                      expand_safe_zone);
                    if (expanded_safe_zone_coordinate.first) {
                        occupied_indexes.insert(std::make_pair(
                          expanded_safe_zone_coordinate.second, 
                          std::get<0>(blocked_coordinate)));
                    }
                    expand_safe_zone.second = std::get<2>(blocked_coordinate);
                    expand_safe_zone.second = expand_safe_zone.second - j;
                    expanded_safe_zone_coordinate = FromCoordinatesToIndex(
                      expand_safe_zone);
                    if (expanded_safe_zone_coordinate.first) {
                        occupied_indexes.insert(std::make_pair(
                          expanded_safe_zone_coordinate.second, 
                          std::get<0>(blocked_coordinate)));
                    }
                }
                expand_safe_zone = std::make_pair(
                  std::get<1>(blocked_coordinate), 
                  std::get<2>(blocked_coordinate));
                expand_safe_zone.first = expand_safe_zone.first - i;
                for (int j = 0; j <= safe_zone; j++) {
                    expand_safe_zone.second = std::get<2>(blocked_coordinate);
                    expand_safe_zone.second = expand_safe_zone.second + j;
                    auto expanded_safe_zone_coordinate = FromCoordinatesToIndex(
                      expand_safe_zone);
                    if (expanded_safe_zone_coordinate.first) {
                        occupied_indexes.insert(std::make_pair(
                          expanded_safe_zone_coordinate.second, 
                          std::get<0>(blocked_coordinate)));
                    }
                    expand_safe_zone.second = std::get<2>(blocked_coordinate);
                    expand_safe_zone.second = expand_safe_zone.second - j;
                    expanded_safe_zone_coordinate = FromCoordinatesToIndex(
                      expand_safe_zone);
                    if (expanded_safe_zone_coordinate.first) {
                        occupied_indexes.insert(std::make_pair(
                          expanded_safe_zone_coordinate.second, 
                          std::get<0>(blocked_coordinate)));
                    }
                }
            }
        }
    }
    for (unsigned int i = 0; i < map_.size(); i++) {
        map_.at(i).coordinate = FromIndexToCoordinate(i);
        auto occupied_index = occupied_indexes.find(i);
        if (occupied_index == occupied_indexes.end()) {
            map_.at(i).blocked.is_blocked = false;
        } else {
            map_.at(i).blocked.is_blocked = true;
            map_.at(i).blocked.robot = occupied_index->second;
        }
    }
}

bool AStar::AStarSearch(const Robot &robot, 
  const std::pair<int, int> &source_coordinate, 
  const std::pair<int, int> &target_coordinate) {
    std::priority_queue<Cell*, std::vector<Cell*>, Compare> open_list_queue;
    std::unordered_set<Cell*> open_list_set;
    std::unordered_set<Cell*> closed_list;
    
    auto source_index = FromCoordinatesToIndex(source_coordinate);
    if (!source_index.first) {
        return false;  // Source coordinate not valid.
    }
    map_.at(source_index.second).f = 0;
    open_list_set.insert(&map_.at(source_index.second));
    open_list_queue.push(&map_.at(source_index.second));
    
    while (!open_list_queue.empty()) {
        auto q = open_list_queue.top();  // Node with least f
        open_list_queue.pop();
        open_list_set.erase(q);
        auto neighbors_indexes = GetCoordinateNeighborsIndexes(q->coordinate);
        for (int neighbor_index : neighbors_indexes) {
            if (IsDestination(map_.at(neighbor_index).coordinate, 
              target_coordinate)) { 
                map_.at(neighbor_index).parent_coordinate = q->coordinate;
                return true;
            } else if ((!map_.at(neighbor_index).blocked.is_blocked || 
              map_.at(neighbor_index).blocked.robot == robot) &&
              closed_list.find(&map_.at(neighbor_index)) == 
              closed_list.end()) {
                double new_g = q->g + CalculateGValue(q->coordinate,
                  map_.at(neighbor_index).coordinate);
                double new_h = CalculateHValue(
                  map_.at(neighbor_index).coordinate, target_coordinate);
                double new_f = new_g + new_h;
                auto neighbor_cell = 
                  open_list_set.find(&map_.at(neighbor_index));
                if ( neighbor_cell == open_list_set.end()) {
                    map_.at(neighbor_index).g = new_g;
                    map_.at(neighbor_index).h = new_h;
                    map_.at(neighbor_index).f = new_f;
                    map_.at(neighbor_index).parent_coordinate = q->coordinate;
                    open_list_queue.push(&map_.at(neighbor_index));
                    open_list_set.insert(&map_.at(neighbor_index));
                } else if ((*neighbor_cell)->f > new_f) {
                    open_list_set.erase((*neighbor_cell));
                    map_.at(neighbor_index).g = new_g;
                    map_.at(neighbor_index).h = new_h;
                    map_.at(neighbor_index).f = new_f;
                    map_.at(neighbor_index).parent_coordinate = q->coordinate;
                    open_list_set.insert(&map_.at(neighbor_index));
                    // Refresh queue order considering the new neighbor values
                    auto cell = open_list_queue.top();
                    open_list_queue.pop();
                    open_list_queue.push(cell);
                }
            }
        }
        closed_list.insert(q);
    }
    return false;  // Failed to find a path to the target coordinate
}

void AStar::PrintMap() {
    for (const auto &cell : map_) {
        if (cell.blocked.is_blocked) {
            std::cout << cell.blocked.robot << " ";
        } else {
            std::cout << "+ ";
        }
        if (cell.coordinate.first == columns_ - 1) {
            std::cout << std::endl;
        }
    }
}