/*!
* @file position_estimator.cpp v1.0
* @Copyright © 2018 Kazushi Kurasawa
* @date 2018.11.16
*
* Released under the MIT license.
* see https://opensource.org/licenses/MIT
*/

#include "position_estimator.h"
#include "defines.h"

mslh::PositionEstimator::PositionEstimator(Position& odometry, DistanceSensor &sensor)
: _odometry(odometry), _sensor(sensor) {}

mslh::Position mslh::PositionEstimator::get_position(){
    return _odometry;
}

void mslh::PositionEstimator::set_position(double_t x, double_t y, double_t rad) {
    _odometry._x = x;
    _odometry._y = y;
    _odometry._rad = rad;
}

mslh::MapPosition mslh::PositionEstimator::get_map_position() {
    MapPosition v;
    v._x = _odometry._x / 180.0;
    v._y = _odometry._y / 180.0;

    const double_t  current_x = sin(_odometry._rad);
    const double_t  current_y = cos(_odometry._rad);

    const double_t r_x = (current_x * cos(PI/4)) - (current_y * sin(PI/4));
    const double_t r_y = (current_x * sin(PI/4)) + (current_y * cos(PI/4));

    if ((0. < r_x) && (0. < r_y)) {

        v._direction = WEST_MASK;

    } else if ((0. < r_x) && (r_y < 0.)) {

        v._direction = SOUTH_MASK;

    } else if ((r_x < 0.) && (r_y < 0.)) {

        v._direction = EAST_MASK;

    } else {

        v._direction = NORTH_MASK;

    }

    return v;
}


/* 各方向ごとに、壁があれば壁情報設置として更新 */
void mslh::PositionEstimator::update_map(Map& _map){
    MapPosition pos = get_map_position();
    Block b;
    uint8_t wall = 0;
    switch (pos._direction){  //機体の向いてる方向に対して分岐

        case NORTH_MASK: // 北方向を向いてる時
            wall |= (!_sensor.isOpenedLeftWall())? WEST_MASK:0;
            wall |= (!_sensor.isOpenedFrontWall())? NORTH_MASK:0;
            wall |= (!_sensor.isOpenedRightWall())? EAST_MASK:0;
            break;

        case EAST_MASK:  // 東方向を向いてる時
            wall |= (!_sensor.isOpenedLeftWall())? NORTH_MASK:0;
            wall |= (!_sensor.isOpenedFrontWall())? EAST_MASK:0;
            wall |= (!_sensor.isOpenedRightWall())? SOUTH_MASK:0;
            break;

        case SOUTH_MASK: // 南方向を向いてる時
            wall |= (!_sensor.isOpenedLeftWall())? EAST_MASK:0;
            wall |= (!_sensor.isOpenedFrontWall())? SOUTH_MASK:0;
            wall |= (!_sensor.isOpenedRightWall())? WEST_MASK:0;
            break;

        case WEST_MASK:  // 西方向を向いてる時
            wall |= (!_sensor.isOpenedLeftWall())? SOUTH_MASK:0;
            wall |= (!_sensor.isOpenedFrontWall())? WEST_MASK:0;
            wall |= (!_sensor.isOpenedRightWall())? NORTH_MASK:0;
            break;
    }

    b.setWall(wall); //壁情報をセットする
    _map.setBlock(b, pos);
}
