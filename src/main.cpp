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
    int moveSpeed = 4; // 60Hz=8, 300Hz=1

    //## �ŗL�̐ݒ�
    // �F�̐ݒ�
    unsigned int mainColor = GetColor(0, 0, 255);
    unsigned int subColor = GetColor(255, 0, 0);

    // Vector2�\���̂̐錾
    // pos: ���݂̍��W, move: �ړ���, axis: ��, goal: �ڕW���W, moveVec: �ړ��x�N�g��
    Vector2 pos, move, axis, goal, moveVec;

    // �ړ����̏����ݒ�
    pos.x = WIDTH / 2;  // ��ʂ̒��S�ɐݒ�
    pos.y = HEIGHT / 2;
    pos.size = 32;      // �ړ����̂̃T�C�Y
    move.x = moveSpeed; // �ړ����̈ړ����l
    move.y = moveSpeed;
    moveVec.x = 1;      // �ړ��x�N�g��
    moveVec.y = 0;

    // ��]���̍��W�ݒ�
    axis.x = 500;
    axis.y = 370;
    axis.size = 8;      // ��]���̃T�C�Y
    int safeArea = 10;  // �͈͋��e�l

    // �ڕW���W�ݒ�
    goal.x = 1000;
    goal.y = 700;

    // ��]�������ݒ� �����_���Ȓl��ݒ肷��
    // �Q�l���� : http://www7b.biglobe.ne.jp/~robe/cpphtml/html01/cpp01062.html
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

    //##### �����܂ŏ����ݒ� #####//

    //##### ���C�����[�v�i�`�揈���j #####//
    while (ProcessMessage() == 0) {
        // ��ʂ̍X�V
        ClearDrawScreen();

        //##### ��]�\�͈� (�~)
        // �v�Z��: (���̃T�C�Y * 10) + (�ړ����̂̃T�C�Y)
        // TRUE �h��Ԃ� / FALSE �h��Ԃ��Ȃ�
        if (getDistance(pos.x, pos.y, axis.x, axis.y) < axis.size * safeArea + pos.size) {
            DrawCircleAA(500.0f, 370.0f, axis.size, 16, GetColor(255, 255, 255), TRUE);
            DrawLine(pos.x, pos.y, axis.x, axis.y, GetColor(255, 255, 255));

            // �}�E�X���N���b�N���ꂽ�ہA����]�\�͈͓��ł���ꍇ
            if (GetMouseInput() & MOUSE_INPUT_LEFT && getDistance(pos.x, pos.y, axis.x, axis.y) < axis.size * safeArea + pos.size) {
                // �֐��@���Đi�s�����x�N�g�����X�V����
                moveVec = findDistance(pos.x, pos.y, axis.x, axis.y);
            }
        }
        else {
            DrawCircleAA(500.0f, 370.0f, axis.size, 16, GetColor(255, 255, 255), FALSE);
        }
        DrawCircleAA(500.0f, 370.0f, axis.size * safeArea, 16, GetColor(255, 255, 255), FALSE);

        //##### �������̕`��i�]�؎����Łj
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

                // �}�E�X���N���b�N���ꂽ�ہA����]�\�͈͓��ł���ꍇ
                if (GetMouseInput() & MOUSE_INPUT_LEFT && getDistance(pos.x, pos.y, axiss[i][0], axiss[i][1]) < axis.size * safeArea + pos.size) {
                    // �֐��@���Đi�s�����x�N�g�����X�V����
                    moveVec = findDistance(pos.x, pos.y, axiss[i][0], axiss[i][1]);
                }
            }
        }

        //##### �������� �������̂̏���

        //���̈ړ����� TRUE �h��Ԃ� / FALSE �h��Ԃ��Ȃ�
        DrawCircle(pos.x, pos.y, pos.size, mainColor, TRUE);

        //�ړ��v�Z
        pos.x = pos.x + moveVec.x * move.x;
        pos.y = pos.y + moveVec.y * move.y;

        //�ǂɓ��������甽��
        if (pos.x < 0 + pos.size || pos.x > WIDTH - pos.size)  moveVec.x = -moveVec.x;
        if (pos.y < 0 + pos.size || pos.y > HEIGHT - pos.size) moveVec.y = -moveVec.y;

        //##### �������� �ڕW���W�̏���
        // �ڕW���W�̕`��
        DrawBox(goal.x, goal.y, goal.x + pos.size, goal.y + pos.size, GetColor(255, 255, 255), FALSE);

        // �ڕW���W�ɓ��B������V�����ڕW���W��ݒ肷��
        if (getDistance(pos.x, pos.y, goal.x, goal.y) < (pos.size * 2)) {
            goal.x = getRandom(0 + pos.size, WIDTH - pos.size);
            goal.y = getRandom(0 + pos.size, HEIGHT - pos.size);
        }

        //##### �������� �`�揈��
        /* �f�o�b�O�p
        printfDx("x:%.2f y:%.2f\n", pos.x, pos.y);
        printfDx("x1:%.2f y1:%.2f\n", axis.x, axis.y);
        printfDx("moveVector x:%.2f y:%.2f\n", moveVec.x, moveVec.y);
        printfDx("dis:%f\n", getDistance(pos.x, pos.y, axis.x, axis.y));
        */

        // ��ʂ̍X�V�i�K�{�j
        ScreenFlip(); // ��ʂ𔽓]�����鏈��
        clsDx();      // �f�o�b�O���O��ʂ��N���A���鏈��
    }

    DxLib_End(); // DX Library�̎g�p���I�����鏈��
    return 0;    // �\�t�g�𐳏�I��
}