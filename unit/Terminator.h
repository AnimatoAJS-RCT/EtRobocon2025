/******************************************************************************
 *  Terminator.h (for SPIKE )
 *  Created on: 2025/01/05
 *  Definition of the Class Terminator
 *  Author: Kazuhiro Kawachi
 *  Modifier : Yuki Tsuchitoi
 *  Copyright (c) 2025 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#ifndef ETTR_UNIT_Terminator_H_
#define ETTR_UNIT_Terminator_H_

class Terminator {
   public:
    /**
     * �R���X�g���N�^
     */
    Terminator();

    /**
     * @brief ���s�I������ׂ������肷�钊�ۃ��\�b�h
     */
    virtual bool isToBeTerminate() = 0;
};

#endif  // ETTR_UNIT_Terminator_H_
