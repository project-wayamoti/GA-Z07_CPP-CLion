//
// Created by waya on 2023/06/10.
//

#pragma once
#include <cmath>
#include <iostream>
#include <random>
using namespace std;

// ##### 2�̉~�̒��S���W�̋������v�Z����֐� #####
// �Q�l���� : https://www.geisya.or.jp/~mwm48961/math/distance1j.htm
// ����     : x1, y1, x2, y2
// �߂�l   : 2�̉~�̒��S���W�̋���
// ����     : sqrt�֐����g�p���邽�߁Amath.h or cmath���C���N���[�h����K�v������
float getDistance(float x1, float y1, float x2, float y2) {
    // 2�̉~�̒��S���W�̍������v�Z�i���΃x�N�g���j
    float dx = x1 - x2; // �ړI�̒n�_ - ���݂̒n�_
    float dy = y1 - y2;
    // 2�̉~�̒��S���W�̋������v�Z
    return sqrt((dx * dx) + (dy * dy));
}

// ##### �����_���Ȓl��ԋp����֐� #####
// �Q�l���� : http://vivi.dyndns.org/tech/cpp/random.html
// ����     : min, max
// �߂�l   : min�ȏ�max�ȉ��̃����_���Ȓl
// ����     : random�֐����g�p���邽�߁Arandom���C���N���[�h����K�v������
int getRandom(int min, int max) {
    random_device rnd;     // �񌈒�I�ȗ���������𐶐�
    mt19937 mt(rnd());     //  �����Z���k�E�c�C�X�^��32�r�b�g�ŁA�����͏����V�[�h�l
    uniform_int_distribution<> rand100(min, max); // �͈͎w��̗���
    // �����_���Ȓl��ԋp����
    return rand100(mt);
}

// Vector2�\����
// �y�ł��邱�Ɓz
// �EVector2�\���̂̃C���X�^���X���쐬
// �EVector2�\���̂̃C���X�^���X��x, y�̒l��ݒ�
// �EVector2�\���̂̃C���X�^���X��x, y�̒l���擾
// �E2�̉~�̒��S���W�̋������v�Z
class Vector2 {
public:
    // x, y�̒l��ێ�����ϐ�
    float x, y;
    float size;

    // �R���X�g���N�^
    Vector2(float x = 0, float y = 0, float size = 0) : x(x), y(y), size(size) {
        cout << "calling constructor." << endl;
    }
    ~Vector2() {
        cout << "calling destructor." << endl;
    }

    // x, y�̒l��ݒ肷��֐�
    inline float X() const {
        return x;
    }
    inline float X(float x) {
        return this->x = x;
    }
    inline float Y() const {
        return y;
    }
    inline float Y(float y) {
        return this->y = y;
    }
    inline float Size() const {
        return size;
    }
    inline float Size(float size) {
        return this->size = size;
    }
    // Q. ����͂Ȃ�ł����H
    // A. ����́A�C�����C���֐��ƌĂ΂����̂ł��B
    //    ���̊֐��́A�֐��̒��g���Ăяo�����ɓW�J���Ă����֐��ł��B
    //    �܂�A���̊֐����Ăяo���ƁA
    //    �ureturn x;�v�Ɓureturn this->x = x;�v���Ăяo�����ɓW�J����܂��B
    //    ���̊֐��́A�֐��̌Ăяo���ɔ����I�[�o�[�w�b�h���팸���邽�߂Ɏg�p����܂��B
    //    �܂��A�֐��̌Ăяo���ɔ����I�[�o�[�w�b�h���팸���邱�ƂŁA
    //    �v���O�����̎��s���x�����コ���邱�Ƃ��ł��܂��B
    //    �Ȃ��A�C�����C���֐��́A�֐��̒��g���Z���ꍇ�Ɏg�p���邱�Ƃ���������܂��B
    //    �Ȃ��Ȃ�A�֐��̒��g�������ꍇ�A
    //    �C�����C���֐����g�p���邱�ƂŁA�v���O�����̎��s���x���ቺ����\�������邩��ł��B
    //    �Ȃ��A�C�����C���֐��́A�֐��̑O�Ɂuinline�v��t���邱�ƂŁA
    //    �C�����C���֐��Ƃ��Ē�`���邱�Ƃ��ł��܂��B
};