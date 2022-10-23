/*!
* @file position_estimator.h v1.0
* @Copyright © 2018 Kazushi Kurasawa
* @date 2018.11.16
*
* Released under the MIT license.
* see https://opensource.org/licenses/MIT
*/

#ifndef NEWZUZUMOUSE_POSITIONESTIMATER_H
#define NEWZUZUMOUSE_POSITIONESTIMATER_H

#include "motor.h"
#include "point.h"
#include "distance_sensor.h"
#include "map.h"
#include "block.h"

namespace mslh {


class PositionEstimator {

public:

    PositionEstimator(Position& odometry, DistanceSensor &sensor);

    /**
     * @fn x,y,rad を連続の値としてdouble型でセットする
     */
    void set_position(double_t x, double_t y, double_t rad);

    /**
     * @fn x,y,rad を連続の値としてdouble型で取得する
     */
    Position get_position();

    /**
     * @fnx,y,rad をブロック単位で取得する
     */
    MapPosition get_map_position(); //マップ上のどこのブロックにいるか

    /**
     * @fn マップ情報をアップデート
     */
    void update_map(Map &_map);


private:
    Position &_odometry;
    DistanceSensor &_sensor;

};

}
#endif //NEWZUZUMOUSE_POSITIONESTIMATER_H
