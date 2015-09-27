#pragma once

namespace
{
	//�L�[
	BYTE
		gKey[256],    //���݂̃L�[���
		gKeyOld[256]; //1�t���[���O�̃L�[���

	//�L�[�I���֐�
	bool keyOn(BYTE key)
	{
		//�L�[��������Ă��邩�ǂ�����Ԃ�
		return ((gKey[key] & 0xf0) != 0);
	}

	//�L�[�^�b�`�֐�
	bool keyTouch(BYTE key)
	{
		//���݁C������Ă���L�[���C�O��C������Ă��邩�ǂ�����Ԃ�
		return 
			((gKey[key] & 0xf0) != 0) &&  //���݁C�L�[��������Ă���
			((gKeyOld[key] & 0xf0) == 0); //�O��C�L�[��������Ă���
	}

	//�}�E�X
	DIMOUSESTATE2
		gMouseState,    //���݂̃}�E�X���
		gMouseStateOld; //1�t���[���O�̃}�E�X���

	//�}�E�X�I���֐�
	bool mouseOn(BYTE key)
	{
		//�}�E�X��������Ă��邩�ǂ�����Ԃ�
		return (!!gMouseState.rgbButtons[key]);
	}

	//�}�E�X�^�b�`�֐�
	bool mouseTouch(BYTE key)
	{
		//���݁C������Ă���}�E�X���C�O��C������Ă��邩�ǂ�����Ԃ�
		return
			(!!gMouseState.rgbButtons[key] && //���݁C�}�E�X��������Ă���
			!gMouseStateOld.rgbButtons[key]); //�O��C�}�E�X��������Ă���
	}
}