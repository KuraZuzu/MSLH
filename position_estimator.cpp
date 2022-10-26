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

mslh::Position mslh::PositionEstimator::getPosition(){
    return _odometry;
}

void mslh::PositionEstimator::setPosition(float32_t x, float32_t y, float32_t rad) {
    _odometry._x = x;
    _odometry._y = y;
    _odometry._rad = rad;
}

mslh::MapPosition mslh::PositionEstimator::getMapPosition() {
    MapPosition v;
    v._x = _odometry._x / 180.0f;
    v._y = _odometry._y / 180.0f;

    const float32_t current_x = sin(_odometry._rad);
    const float32_t current_y = cos(_odometry._rad);

    const float32_t r_x = (current_x * cos(PI/4.0f)) - (current_y * sin(PI/4.0f));
    const float32_t r_y = (current_x * sin(PI/4.0f)) + (current_y * cos(PI/4.0f));

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
void mslh::PositionEstimator::updateMap(Map& _map){
    MapPosition pos = getMapPosition();
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
