// �ȉ��̍s���Ȃ��ƃR���p�C�����ʂ�܂���
#pragma comment(linker, "/subsystem:windows")
#include "DxLib.h"
#include "option.h"
#define WIDTH 1280
#define HEIGHT 720
using namespace std;

// ���菈�������ʉ�
Vector2 findDistance(float x1, float y1, float x2, float y2) {
    // ���΃x�N�g���𐳋K��
    Vector2 relativeVector = {
            (x1 - x2) / getDistance(x1, y1, x2, y2),
            (y1 - y2) / getDistance(x1, y1, x2, y2)
    };

    // return ���ɓ���ւ�������@���킩��Ȃ��̂�POWER IS POWER
    Vector2 tmp = { relativeVector.y, -relativeVector.x };

    // �������́i���́j�̐i�s�����x�N�g����ԋp
    return tmp;
};

// �v���O�����̍ŏ���WinMain�Ŏn�߂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    //##### �����ݒ� #####//
    // �E�B���h�E���[�h�ݒ�
    ChangeWindowMode(TRUE);

    // ��ʃT�C�Y�ݒ�
    SetGraphMode(WIDTH, HEIGHT, 32);

    // �t���[�����[�g�̐ݒ�
    SetWaitVSyncFlag(TRUE); // ����������L���ɂ���

    // DX���C�u��������������
    if (DxLib_Init() == -1) {
        return -1; // �G���[���N�����璼���ɏI��
    }

    //## ���ɉ����Đݒ�
    // �ړ����x
    int moveSpeed = 1; // 60Hz=8, 300Hz=1

    //## �ŗL�̐ݒ�
    // �F�̐ݒ�
    unsigned int charaColor = GetColor(255, 255, 255);
    unsigned int enemy1Color = GetColor(255, 0, 0);
    unsigned int enemy2Color = GetColor(0, 255, 0);

    // Vector2�\���̂̐錾
    // pos: ���݂̍��W, move: �ړ���, moveVec: �ړ��x�N�g��
    Vector2 pos, move, moveVec, enemy1, enemy2, enemy1Vec, enemy2Vec;

    // �ړ����̏����ݒ�
    pos.x = WIDTH / 2;  // ��ʂ̒��S�ɐݒ�
    pos.y = HEIGHT / 2;
    pos.size = 32;      // �ړ����̂̃T�C�Y
    move.x = moveSpeed; // �ړ����̈ړ����l
    move.y = moveSpeed;
    moveVec.x = 1;      // �ړ��x�N�g��
    moveVec.y = 0;

    // �G���̂̏����ݒ�
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

    //##### ���C�����[�v�i�`�揈���j #####//
    while (ProcessMessage() == 0) {
        // ��ʂ̍X�V
        ClearDrawScreen();

        //##### �������̂̏���
        //���̈ړ����� TRUE �h��Ԃ� / FALSE �h��Ԃ��Ȃ�
        DrawCircle(pos.x, pos.y, pos.size, charaColor, TRUE);
        // WASD�L�[��pos���ړ�������
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

        //##### �G���̂̏���

        // �G���̈ړ�����
        DrawCircle(enemy1.x, enemy1.y, enemy1.size, enemy1Color, TRUE);
        DrawCircle(enemy2.x, enemy2.y, enemy2.size, enemy2Color, TRUE);

        //�ړ��v�Z
        enemy1.x = enemy1.x + enemy1Vec.x * moveSpeed;
        enemy1.y = enemy1.y + enemy1Vec.y * moveSpeed;
        enemy2.x = enemy2.x + enemy2Vec.x * moveSpeed;
        enemy2.y = enemy2.y + enemy2Vec.y * moveSpeed;

        //�ǂɓ��������甽��
        if (enemy1.x < 0 + enemy1.size || enemy1.x > WIDTH - enemy1.size)  enemy1Vec.x = -enemy1Vec.x;
        if (enemy1.y < 0 + enemy1.size || enemy1.y > HEIGHT - enemy1.size) enemy1Vec.y = -enemy1Vec.y;
        if (enemy2.x < 0 + enemy2.size || enemy2.x > WIDTH - enemy2.size)  enemy2Vec.x = -enemy2Vec.x;
        if (enemy2.y < 0 + enemy2.size || enemy2.y > HEIGHT - enemy2.size) enemy2Vec.y = -enemy2Vec.y;

        //##### �`�揈��
        /* �f�o�b�O�p
        printfDx();
        */

        // ��ʂ̍X�V�i�K�{�j
        ScreenFlip(); // ��ʂ𔽓]�����鏈��
        clsDx();      // �f�o�b�O���O��ʂ��N���A���鏈��
    }

    DxLib_End(); // DX Library�̎g�p���I�����鏈��
    return 0;    // �\�t�g�𐳏�I��
}