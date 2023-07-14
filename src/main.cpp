// 以下の行がないとコンパイルが通りません
#pragma comment(linker, "/subsystem:windows")
#include "DxLib.h"
#include "utils.h"
#include "vector.h"
//## 環境に応じて設定
#define WIDTH 1280
#define HEIGHT 720
#define SPEED 8 // 移動速度 60Hz=8, 300Hz=1
using namespace std;

// 時間の初期値を取得
int startTime = GetNowCount();

void gameStop(int mode) {
    switch (mode) {
        case 0:
            // ゲーム終了
            printfDx("ゲーム終了 : ゲームクリア！\n");
            break;
        case 1:
            // ゲーム終了
            printfDx("ゲーム終了 : ゲームオーバー！\n");
            break;
    }
    WaitKey();
}

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

    //## 固有の設定
    // 色の設定
    unsigned int charaColor = GetColor(255, 255, 255);
    unsigned int enemy1Color = GetColor(255, 0, 0);
    unsigned int enemy2Color = GetColor(0, 255, 0);
    unsigned int blue = GetColor(0, 0, 255);

    // Vector2構造体の宣言
    // pos: 現在の座標, move: 移動量, moveVec: 移動ベクトル
    Vector2 pos, move, moveVec, enemy1, enemy2, enemy1Vec, enemy2Vec;

    // 移動物体初期設定
    pos.x = WIDTH / 2;  // 画面の中心に設定
    pos.y = HEIGHT / 2;
    pos.size = 32;      // 移動物体のサイズ
    move.x = SPEED; // 移動物体移動数値
    move.y = SPEED;
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

        //##### 制限時間処理
        // 現在の時間を表示
        printfDx("現在の時間 : %.0d\n", (GetNowCount() - startTime) / 1000);

        // 20秒経過したらソフトを終了
        if (GetNowCount() - startTime > 20000) {
            gameStop(0);
            break;
        }

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
        enemy1.x = enemy1.x + enemy1Vec.x * SPEED;
        enemy1.y = enemy1.y + enemy1Vec.y * SPEED;
        enemy2.x = enemy2.x + enemy2Vec.x * SPEED;
        enemy2.y = enemy2.y + enemy2Vec.y * SPEED;

        //壁に当たったら反射
        if (enemy1.x < 0 + enemy1.size || enemy1.x > WIDTH - enemy1.size)  enemy1Vec.x = -enemy1Vec.x;
        if (enemy1.y < 0 + enemy1.size || enemy1.y > HEIGHT - enemy1.size) enemy1Vec.y = -enemy1Vec.y;
        if (enemy2.x < 0 + enemy2.size || enemy2.x > WIDTH - enemy2.size)  enemy2Vec.x = -enemy2Vec.x;
        if (enemy2.y < 0 + enemy2.size || enemy2.y > HEIGHT - enemy2.size) enemy2Vec.y = -enemy2Vec.y;

        // 相対ベクトル計算
        Vector2 relativeVector1 = getRelativeVector(enemy1, enemy2);
        Vector2 relativeVector2 = getRelativeVector(enemy1, pos);
        Vector2 relativeVector3 = getRelativeVector(enemy2, enemy1);
        Vector2 relativeVector4 = getRelativeVector(enemy2, pos);

        //外積と内積を計算
        Vector2 RV1Normal = getNormalizedVector(relativeVector1);
        Vector2 RV2Normal = getNormalizedVector(relativeVector2);
        float cross = getCrossProduct(RV1Normal.x, RV1Normal.y, relativeVector2.x, relativeVector2.y);
        if(cross < 0) cross *= -1; //外積の絶対値を取得

        float dotV1 = getDotProduct(relativeVector1.x, relativeVector1.y, relativeVector2.x, relativeVector2.y);
        float dotV2 = getDotProduct(relativeVector3.x, relativeVector3.y, relativeVector4.x, relativeVector4.y);

        //外積が正の時は時計回り、負の時は反時計回り
        if (cross > 0) {
            printfDx("時計回り\n");
        } else if (cross < 0) {
            printfDx("反時計回り\n");
        } else {
            printfDx("直線\n");
        }

        //内積が正の時は鈍角、負の時は鋭角
        if (dotV1 > 0) {
            printfDx("鈍角\n");
        } else if (dotV1 < 0) {
            printfDx("鋭角\n");
        } else {
            printfDx("直角\n");
        }

        if(dotV1 > 0 && dotV2 > 0 && cross < 50) {
            gameStop(1);
            break;
        }

        //##### 描画処理

        // enemy1とenemy2の間に線を引く
        DrawLine(enemy1.x, enemy1.y, enemy2.x, enemy2.y, blue);

        // 画面の更新（必須）
        ScreenFlip(); // 画面を反転させる処理
        clsDx();      // デバッグログ画面をクリアする処理
    }

    DxLib_End(); // DX Libraryの使用を終了する処理
    return 0;    // ソフトを正常終了
}