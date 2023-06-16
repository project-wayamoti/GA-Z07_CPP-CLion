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
    int moveSpeed = 1; // 60Hz=8, 300Hz=1

    //## 固有の設定
    // 色の設定
    unsigned int charaColor = GetColor(255, 255, 255);
    unsigned int enemy1Color = GetColor(255, 0, 0);
    unsigned int enemy2Color = GetColor(0, 255, 0);

    // Vector2構造体の宣言
    // pos: 現在の座標, move: 移動量, moveVec: 移動ベクトル
    Vector2 pos, move, moveVec, enemy1, enemy2, enemy1Vec, enemy2Vec;

    // 移動物体初期設定
    pos.x = WIDTH / 2;  // 画面の中心に設定
    pos.y = HEIGHT / 2;
    pos.size = 32;      // 移動物体のサイズ
    move.x = moveSpeed; // 移動物体移動数値
    move.y = moveSpeed;
    moveVec.x = 1;      // 移動ベクトル
    moveVec.y = 0;

    // 敵物体の初期設定
    enemy1.x = 100;
    enemy1.y = 100;
    enemy1.size = 32;
    enemy1Vec.x = 1.0f;
    enemy1Vec.y = 0.15f;
    enemy2.x = 800;
    enemy2.y = 600;
    enemy2.size = 32;
    enemy2Vec.x = 0.15f;
    enemy2Vec.y = 1.0f;

    //##### メインループ（描画処理） #####//
    while (ProcessMessage() == 0) {
        // 画面の更新
        ClearDrawScreen();

        //##### 中央物体の処理
        //球体移動処理 TRUE 塗りつぶし / FALSE 塗りつぶさない
        DrawCircle(pos.x, pos.y, pos.size, charaColor, TRUE);
        // WASDキーでposを移動させる
        if (CheckHitKey(KEY_INPUT_W)) {
            pos.y -= move.y;
        }
        if (CheckHitKey(KEY_INPUT_S)) {
            pos.y += move.y;
        }
        if (CheckHitKey(KEY_INPUT_A)) {
            pos.x -= move.x;
        }
        if (CheckHitKey(KEY_INPUT_D)) {
            pos.x += move.x;
        }

        //##### 敵物体の処理

        // 敵物体移動処理
        DrawCircle(enemy1.x, enemy1.y, enemy1.size, enemy1Color, TRUE);
        DrawCircle(enemy2.x, enemy2.y, enemy2.size, enemy2Color, TRUE);

        //移動計算
        enemy1.x = enemy1.x + enemy1Vec.x * moveSpeed;
        enemy1.y = enemy1.y + enemy1Vec.y * moveSpeed;
        enemy2.x = enemy2.x + enemy2Vec.x * moveSpeed;
        enemy2.y = enemy2.y + enemy2Vec.y * moveSpeed;

        //壁に当たったら反射
        if (enemy1.x < 0 + enemy1.size || enemy1.x > WIDTH - enemy1.size)  enemy1Vec.x = -enemy1Vec.x;
        if (enemy1.y < 0 + enemy1.size || enemy1.y > HEIGHT - enemy1.size) enemy1Vec.y = -enemy1Vec.y;
        if (enemy2.x < 0 + enemy2.size || enemy2.x > WIDTH - enemy2.size)  enemy2Vec.x = -enemy2Vec.x;
        if (enemy2.y < 0 + enemy2.size || enemy2.y > HEIGHT - enemy2.size) enemy2Vec.y = -enemy2Vec.y;

        //##### 描画処理
        /* デバッグ用
        printfDx();
        */

        // 画面の更新（必須）
        ScreenFlip(); // 画面を反転させる処理
        clsDx();      // デバッグログ画面をクリアする処理
    }

    DxLib_End(); // DX Libraryの使用を終了する処理
    return 0;    // ソフトを正常終了
}