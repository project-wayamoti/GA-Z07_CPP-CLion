//
// Created by waya on 2023/07/14.
//

#ifndef UTILS_CLASS
#define UTILS_CLASS

#pragma once
#include <cmath>
#include <iostream>
#include <random>
#include "vector.h"
using namespace std;

// ##### 2つの円の中心座標の距離を計算する関数 #####
// 参考文献 : https://www.geisya.or.jp/~mwm48961/math/distance1j.htm
// 引数     : x1, y1, x2, y2
// 戻り値   : 2つの円の中心座標の距離
// 注意     : sqrt関数を使用するため、math.h or cmathをインクルードする必要がある
float getDistance(float x1, float y1, float x2, float y2) {
    // 2つの円の中心座標の差分を計算（相対ベクトル）
    float dx = x1 - x2; // 目的の地点 - 現在の地点
    float dy = y1 - y2;
    // 2つの円の中心座標の距離を計算
    return sqrt((dx * dx) + (dy * dy));
}

// ##### 外積計算関数 #####
// 参考文献 : ゲームアルゴリズム Z07 7ページ目資料
// 引数     : x1, y1, x2, y2
// 戻り値   : 外積
// 注意     : なし
float getCrossProduct(float x1, float y1, float x2, float y2) {
    return x1 * y2 - y1 * x2;
}

// ##### 内積計算関数 #####
// 参考文献 : ゲームアルゴリズム Z07 12ページ目資料
// 引数     : x1, y1, x2, y2
// 戻り値   : 内積
// 注意     : なし
float getDotProduct(float x1, float y1, float x2, float y2) {
    return x1 * x2 + y2 * y1;
}

// ##### 内積と外積を計算して点と点の間だったらtrueを返す #####
// 参考文献 : なし
// 引数     : x1, y1, x2, y2, x3, y3
// 戻り値   : 点と点の間だったらtrue
// 注意     : なし
bool isPointBetween(float x1, float y1, float x2, float y2, float x3, float y3) {
    // 点と点の間かどうかを判定する
    // 2つの円の中心座標の差分を計算（相対ベクトル）
    float dx1 = x2 - x1; // 目的の地点 - 現在の地点
    float dy1 = y2 - y1;
    float dx2 = x3 - x1; // 目的の地点 - 現在の地点
    float dy2 = y3 - y1;
    // 外積を計算
    float crossProduct = getCrossProduct(dx1, dy1, dx2, dy2);
    // 内積を計算
    float dotProduct = getDotProduct(dx1, dy1, dx2, dy2);
    // 点と点の間の幅前後100に侵入してきたらtrueを返す
    if (crossProduct >= -10 || crossProduct <= 10 && dotProduct >= 0) {
        return true;
    }
        // 内積と外積を計算して点と点の間だったらtrueを返す
    else return false;
}

// ##### ランダムな値を返却する関数 #####
// 参考文献 : http://vivi.dyndns.org/tech/cpp/random.html
// 引数     : min, max
// 戻り値   : min以上max以下のランダムな値
// 注意     : random関数を使用するため、randomをインクルードする必要がある
int getRandom(int min, int max) {
    random_device rnd;     // 非決定的な乱数生成器を生成
    mt19937 mt(rnd());     //  メルセンヌ・ツイスタの32ビット版、引数は初期シード値
    uniform_int_distribution<> rand100(min, max); // 範囲指定の乱数
    // ランダムな値を返却する
    return rand100(mt);
}

// 判定処理を共通化
Vector2 findDistance(float x1, float y1, float x2, float y2) {
    // 相対ベクトルを正規化
    Vector2 relativeVector = {
            (x1 - x2) / getDistance(x1, y1, x2, y2),
            (y1 - y2) / getDistance(x1, y1, x2, y2)
    };

    // return 時に入れ替えする方法がわからないのでPOWER IS POWER
    Vector2 tmp = { relativeVector.y, -relativeVector.x };

    // 中央物体（球体）の進行方向ベクトルを返却
    return tmp;
};

// ##### 相対ベクター計算関数 #####
// 参考文献 : ゲームアルゴリズム Z07 7ページ目資料
// 引数     : x1, y1, x2, y2
// 戻り値   : 相対ベクター
// 注意     : なし
Vector2 getRelativeVector(Vector2 V1, Vector2 V2) {
    // 2つの円の中心座標の差分を計算（相対ベクトル）
    float dx = V1.x - V2.x; // 目的の地点 - 現在の地点
    float dy = V1.y - V2.y;
    // 相対ベクトルを返却する
    return Vector2(dx, dy);
};

// ##### 相対ベクター正規化 #####
// 参考文献 : ゲームアルゴリズム Z07 7ページ目資料
// 引数     : V1
// 戻り値   : 相対ベクター
// 注意     : なし
Vector2 getNormalizedVector(Vector2 V1) {
    // 相対ベクトルを正規化する
    float length = sqrt(V1.x * V1.x + V1.y * V1.y);
    V1.x /= length;
    V1.y /= length;
    // 正規化した相対ベクトルを返却する
    return Vector2(V1.x, V1.y);
};

#endif //UTILS_H
