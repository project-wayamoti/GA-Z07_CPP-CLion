// �ȉ��̍s���Ȃ��ƃR���p�C�����ʂ�܂���
#pragma comment(linker, "/subsystem:windows")
#include "DxLib.h"
#include "utils.h"
#include "vector.h"
//## ���ɉ����Đݒ�
#define WIDTH 1280
#define HEIGHT 720
#define SPEED 8 // �ړ����x 60Hz=8, 300Hz=1
using namespace std;

// ���Ԃ̏����l���擾
int startTime = GetNowCount();

void gameStop(int mode) {
    switch (mode) {
        case 0:
            // �Q�[���I��
            printfDx("�Q�[���I�� : �Q�[���N���A�I\n");
            break;
        case 1:
            // �Q�[���I��
            printfDx("�Q�[���I�� : �Q�[���I�[�o�[�I\n");
            break;
    }
    WaitKey();
}

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

    //## �ŗL�̐ݒ�
    // �F�̐ݒ�
    unsigned int charaColor = GetColor(255, 255, 255);
    unsigned int enemy1Color = GetColor(255, 0, 0);
    unsigned int enemy2Color = GetColor(0, 255, 0);
    unsigned int blue = GetColor(0, 0, 255);

    // Vector2�\���̂̐錾
    // pos: ���݂̍��W, move: �ړ���, moveVec: �ړ��x�N�g��
    Vector2 pos, move, moveVec, enemy1, enemy2, enemy1Vec, enemy2Vec;

    // �ړ����̏����ݒ�
    pos.x = WIDTH / 2;  // ��ʂ̒��S�ɐݒ�
    pos.y = HEIGHT / 2;
    pos.size = 32;      // �ړ����̂̃T�C�Y
    move.x = SPEED; // �ړ����̈ړ����l
    move.y = SPEED;
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

        //##### �������ԏ���
        // ���݂̎��Ԃ�\��
        printfDx("���݂̎��� : %.0d\n", (GetNowCount() - startTime) / 1000);

        // 20�b�o�߂�����\�t�g���I��
        if (GetNowCount() - startTime > 20000) {
            gameStop(0);
            break;
        }

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
        enemy1.x = enemy1.x + enemy1Vec.x * SPEED;
        enemy1.y = enemy1.y + enemy1Vec.y * SPEED;
        enemy2.x = enemy2.x + enemy2Vec.x * SPEED;
        enemy2.y = enemy2.y + enemy2Vec.y * SPEED;

        //�ǂɓ��������甽��
        if (enemy1.x < 0 + enemy1.size || enemy1.x > WIDTH - enemy1.size)  enemy1Vec.x = -enemy1Vec.x;
        if (enemy1.y < 0 + enemy1.size || enemy1.y > HEIGHT - enemy1.size) enemy1Vec.y = -enemy1Vec.y;
        if (enemy2.x < 0 + enemy2.size || enemy2.x > WIDTH - enemy2.size)  enemy2Vec.x = -enemy2Vec.x;
        if (enemy2.y < 0 + enemy2.size || enemy2.y > HEIGHT - enemy2.size) enemy2Vec.y = -enemy2Vec.y;

        // ���΃x�N�g���v�Z
        Vector2 relativeVector1 = getRelativeVector(enemy1, enemy2);
        Vector2 relativeVector2 = getRelativeVector(enemy1, pos);
        Vector2 relativeVector3 = getRelativeVector(enemy2, enemy1);
        Vector2 relativeVector4 = getRelativeVector(enemy2, pos);

        //�O�ςƓ��ς��v�Z
        Vector2 RV1Normal = getNormalizedVector(relativeVector1);
        Vector2 RV2Normal = getNormalizedVector(relativeVector2);
        float cross = getCrossProduct(RV1Normal.x, RV1Normal.y, relativeVector2.x, relativeVector2.y);
        if(cross < 0) cross *= -1; //�O�ς̐�Βl���擾

        float dotV1 = getDotProduct(relativeVector1.x, relativeVector1.y, relativeVector2.x, relativeVector2.y);
        float dotV2 = getDotProduct(relativeVector3.x, relativeVector3.y, relativeVector4.x, relativeVector4.y);

        //�O�ς����̎��͎��v���A���̎��͔����v���
        if (cross > 0) {
            printfDx("���v���\n");
        } else if (cross < 0) {
            printfDx("�����v���\n");
        } else {
            printfDx("����\n");
        }

        //���ς����̎��͓݊p�A���̎��͉s�p
        if (dotV1 > 0) {
            printfDx("�݊p\n");
        } else if (dotV1 < 0) {
            printfDx("�s�p\n");
        } else {
            printfDx("���p\n");
        }

        if(dotV1 > 0 && dotV2 > 0 && cross < 50) {
            gameStop(1);
            break;
        }

        //##### �`�揈��

        // enemy1��enemy2�̊Ԃɐ�������
        DrawLine(enemy1.x, enemy1.y, enemy2.x, enemy2.y, blue);

        // ��ʂ̍X�V�i�K�{�j
        ScreenFlip(); // ��ʂ𔽓]�����鏈��
        clsDx();      // �f�o�b�O���O��ʂ��N���A���鏈��
    }

    DxLib_End(); // DX Library�̎g�p���I�����鏈��
    return 0;    // �\�t�g�𐳏�I��
}