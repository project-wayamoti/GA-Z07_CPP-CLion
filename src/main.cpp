// 以下の行がないとコンパイルが通りません
#pragma comment(linker, "/subsystem:windows")

#include "DxLib.h"
#include "option.h"
#define WIDTH 1280
#define HEIGHT 720
using namespace std;

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

// プログラムの最初はWinMainで始める
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    //##### 初期設定 #####//
    // ウィンドウモード設定
    ChangeWindowMode(TRUE);

    // 画面サイズ設定
    SetGraphMode(WIDTH, HEIGHT, 32);

    // フレームレートの設定
    SetWaitVSyncFlag(TRUE); // 垂直同期を有効にする

    // DXライブラリ初期化処理
    if (DxLib_Init() == -1) {
        return -1; // エラーが起きたら直ちに終了
    }

    //## 環境に応じて設定
    // 移動速度
    int moveSpeed = 4; // 60Hz=8, 300Hz=1

    //## 固有の設定
    // 色の設定
    unsigned int mainColor = GetColor(0, 0, 255);
    unsigned int subColor = GetColor(255, 0, 0);

    // Vector2構造体の宣言
    // pos: 現在の座標, move: 移動量, axis: 軸, goal: 目標座標, moveVec: 移動ベクトル
    Vector2 pos, move, axis, goal, moveVec;

    // 移動物体初期設定
    pos.x = WIDTH / 2;  // 画面の中心に設定
    pos.y = HEIGHT / 2;
    pos.size = 32;      // 移動物体のサイズ
    move.x = moveSpeed; // 移動物体移動数値
    move.y = moveSpeed;
    moveVec.x = 1;      // 移動ベクトル
    moveVec.y = 0;

    // 回転軸の座標設定
    axis.x = 500;
    axis.y = 370;
    axis.size = 8;      // 回転軸のサイズ
    int safeArea = 10;  // 範囲許容値

    // 目標座標設定
    goal.x = 1000;
    goal.y = 700;

    // 回転軸複数設定 ランダムな値を設定する
    // 参考文献 : http://www7b.biglobe.ne.jp/~robe/cpphtml/html01/cpp01062.html
    float axiss[3][2] = {
            {
                    static_cast<float>(getRandom(150, 450)), static_cast<float>(getRandom(150, HEIGHT - 150))
            },
            {
                    static_cast<float>(getRandom(550, 850)), static_cast<float>(getRandom(150, HEIGHT - 150))
            },
            {
                    static_cast<float>(getRandom(950, 1250)), static_cast<float>(getRandom(150, HEIGHT - 150))
            }
    };

    //##### ここまで初期設定 #####//

    //##### メインループ（描画処理） #####//
    while (ProcessMessage() == 0) {
        // 画面の更新
        ClearDrawScreen();

        //##### 回転可能範囲 (円)
        // 計算式: (軸のサイズ * 10) + (移動物体のサイズ)
        // TRUE 塗りつぶし / FALSE 塗りつぶさない
        if (getDistance(pos.x, pos.y, axis.x, axis.y) < axis.size * safeArea + pos.size) {
            DrawCircleAA(500.0f, 370.0f, axis.size, 16, GetColor(255, 255, 255), TRUE);
            DrawLine(pos.x, pos.y, axis.x, axis.y, GetColor(255, 255, 255));

            // マウス左クリックされた際、かつ回転可能範囲内である場合
            if (GetMouseInput() & MOUSE_INPUT_LEFT && getDistance(pos.x, pos.y, axis.x, axis.y) < axis.size * safeArea + pos.size) {
                // 関数叩いて進行方向ベクトルを更新する
                moveVec = findDistance(pos.x, pos.y, axis.x, axis.y);
            }
        }
        else {
            DrawCircleAA(500.0f, 370.0f, axis.size, 16, GetColor(255, 255, 255), FALSE);
        }
        DrawCircleAA(500.0f, 370.0f, axis.size * safeArea, 16, GetColor(255, 255, 255), FALSE);

        //##### 複数軸の描画（脳筋実装版）
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 2; j++) {
                if (getDistance(pos.x, pos.y, axiss[i][0], axiss[i][1]) < axis.size * safeArea + pos.size) {
                    DrawCircleAA(axiss[i][0], axiss[i][1], axis.size, 16, GetColor(255, 255, 255), TRUE);
                    DrawLine(pos.x, pos.y, axiss[i][0], axiss[i][1], GetColor(255, 255, 255));
                }
                else {
                    DrawCircleAA(axiss[i][0], axiss[i][1], axis.size, 16, GetColor(255, 255, 255), FALSE);
                }
                DrawCircleAA(axiss[i][0], axiss[i][1], axis.size * safeArea, 16, GetColor(255, 255, 255), FALSE);

                // マウス左クリックされた際、かつ回転可能範囲内である場合
                if (GetMouseInput() & MOUSE_INPUT_LEFT && getDistance(pos.x, pos.y, axiss[i][0], axiss[i][1]) < axis.size * safeArea + pos.size) {
                    // 関数叩いて進行方向ベクトルを更新する
                    moveVec = findDistance(pos.x, pos.y, axiss[i][0], axiss[i][1]);
                }
            }
        }

        //##### ここから 中央物体の処理

        //球体移動処理 TRUE 塗りつぶし / FALSE 塗りつぶさない
        DrawCircle(pos.x, pos.y, pos.size, mainColor, TRUE);

        //移動計算
        pos.x = pos.x + moveVec.x * move.x;
        pos.y = pos.y + moveVec.y * move.y;

        //壁に当たったら反射
        if (pos.x < 0 + pos.size || pos.x > WIDTH - pos.size)  moveVec.x = -moveVec.x;
        if (pos.y < 0 + pos.size || pos.y > HEIGHT - pos.size) moveVec.y = -moveVec.y;

        //##### ここから 目標座標の処理
        // 目標座標の描画
        DrawBox(goal.x, goal.y, goal.x + pos.size, goal.y + pos.size, GetColor(255, 255, 255), FALSE);

        // 目標座標に到達したら新しい目標座標を設定する
        if (getDistance(pos.x, pos.y, goal.x, goal.y) < (pos.size * 2)) {
            goal.x = getRandom(0 + pos.size, WIDTH - pos.size);
            goal.y = getRandom(0 + pos.size, HEIGHT - pos.size);
        }

        //##### ここから 描画処理
        /* デバッグ用
        printfDx("x:%.2f y:%.2f\n", pos.x, pos.y);
        printfDx("x1:%.2f y1:%.2f\n", axis.x, axis.y);
        printfDx("moveVector x:%.2f y:%.2f\n", moveVec.x, moveVec.y);
        printfDx("dis:%f\n", getDistance(pos.x, pos.y, axis.x, axis.y));
        */

        // 画面の更新（必須）
        ScreenFlip(); // 画面を反転させる処理
        clsDx();      // デバッグログ画面をクリアする処理
    }

    DxLib_End(); // DX Libraryの使用を終了する処理
    return 0;    // ソフトを正常終了
}