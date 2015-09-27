// MsXmlLoad.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

//2�d�C���N���[�h�h�~
#pragma once

#include "stdafx.h"

#import "msxml6.dll" named_guids raw_interfaces_only
#include <atlbase.h>	// CComVariant, CComBSTR

#include <vector>
#include <iostream>
#include <conio.h>

// pXMLDOMDocument�̒��g��ǂ��rvbstrValue�Ƃ��ďo��
bool GetXmlValue(	const IXMLDOMDocumentPtr&	pXMLDOMDocument,
					const CComBSTR&				bstrTagName,
					const CComBSTR&				bstrAttributeName,
					std::vector<CComBSTR>&		rvbstrValue )
{
	// �A�E�g�v�b�g�̏�����
	rvbstrValue.clear();

	// �C���v�b�g�̃`�F�b�N
	if( 0 == bstrTagName.Length() )
	{
		return false;
	}

	HRESULT hResult;

	// �^�O���̃m�[�h���X�g�̎擾
	IXMLDOMNodeListPtr pXMLDOMNodeList = NULL;
	hResult = pXMLDOMDocument->getElementsByTagName( bstrTagName, &pXMLDOMNodeList );
	if( FAILED(hResult) || NULL == pXMLDOMNodeList )
	{
		ASSERT( !"�^�O���̃m�[�h���X�g�̎擾�Ɏ��s" );
		return false;
	}
	// �m�[�h���X�g�̃m�[�h�̐��̎擾
	long lCountNode = 0;
	hResult = pXMLDOMNodeList->get_length( &lCountNode );
	if( FAILED(hResult) )
	{
		ASSERT( !"�m�[�h���X�g�̃m�[�h�̐��̎擾�Ɏ��s" );
		return false;
	}
	// �m�[�h���X�g�̃m�[�h�������
	for( int i = 0; i < lCountNode; ++i )
	{
		// �m�[�h���X�g�̂����̈�̃m�[�h�̎擾
		IXMLDOMNodePtr pXMLDOMNode = NULL;
		hResult = pXMLDOMNodeList->get_item( i, &pXMLDOMNode );
		if( FAILED(hResult) )
		{
			ASSERT( !"�m�[�h���X�g�̂����̈�̃m�[�h�̎擾�Ɏ��s" );
			return false; 
		}

		// �m�[�h�^�C�v�̎擾
		DOMNodeType eNodeType;
		hResult = pXMLDOMNode->get_nodeType( &eNodeType );
		if( FAILED(hResult) )
		{
			ASSERT( !"�m�[�h�^�C�v�̎擾�Ɏ��s" );
			return false; 
		}
		if( NODE_ELEMENT != eNodeType )
		{
			ASSERT( !"�m�[�h�^�C�v���G�������g�łȂ��̂͂�������" );
			return false;
		}
		// �G�������g�^�ւ̕ϊ�
		IXMLDOMElementPtr pXMLDOMElement = NULL;
		hResult = pXMLDOMNode->QueryInterface( IID_IXMLDOMElement, (void**)&pXMLDOMElement );	// �X�}�[�g�|�C���^�^�𗘗p���Ă���̂ŁApXMLDOMElement = pXMLDOMNode; �ł��ǂ��B
		if( FAILED(hResult) || NULL == pXMLDOMElement )
		{
			ASSERT( !"�G�������g�^�ւ̕ϊ��Ɏ��s" );
			return false;
		}

		// �����l�̎擾���A�f�[�^�l�̎擾��
		if( bstrAttributeName.Length() )
		{	// �����l�̎擾
			IXMLDOMAttribute* pAttributeNode = NULL;
			CComVariant varValue;
			hResult = pXMLDOMElement->getAttribute( bstrAttributeName, &varValue ); 
			if( SUCCEEDED(hResult) && VT_BSTR == varValue.vt )
			{
				rvbstrValue.push_back( varValue.bstrVal );
			}
		}
		else
		{	// �f�[�^�l�̎擾
			CComBSTR bstrText;
			hResult = pXMLDOMElement->get_text( &bstrText );
			if( SUCCEEDED(hResult) )
			{
				rvbstrValue.push_back( bstrText );
			}
		}
	}

	return true;
}

// XML�̃h�L�������g��pXMLDOMDocument�Ƃ��č쐬
bool ParseXmlFile( TCHAR* pszFileName, IXMLDOMDocumentPtr& pXMLDOMDocument)
{
	if( NULL == pszFileName )
	{
		return false;
	}

	HRESULT hResult;

	// DOM�h�L�������g�̍쐬
	hResult = pXMLDOMDocument.CreateInstance(CLSID_DOMDocument);
	if( FAILED(hResult) )
	{
		ASSERT( !"DOM�h�L�������g�̍쐬�Ɏ��s" );
		return false;
	}

	// XML�t�@�C���ǂݍ���
	pXMLDOMDocument->put_async( VARIANT_FALSE ); // load�֐������[�h����������܂ő҂悤�ɂ���B
	VARIANT_BOOL varbResult;
	hResult = pXMLDOMDocument->load( CComVariant(pszFileName), &varbResult );
	if( FAILED(hResult) || !varbResult)
	{
		ASSERT( !"XML�t�@�C���ǂݍ��݂Ɏ��s" );
		return false;
	}

	/*
	// �v�f�̒l�̎擾

	if( !GetXmlValue(	pXMLDOMDocument,
						_T("name"),
						_T(""),
						vbstrValue ) )
	{
		ASSERT( !"�u���ځv�v�f�̒l�̎擾�Ɏ��s" );
		return false;
	}
	
	if( !GetXmlValue(	pXMLDOMDocument,
						_T("range"),
						_T(""),
						vbstrValue2 ) )
	{
		assert( !"�u�˒��v�v�f�̒l�̎擾�Ɏ��s" );
		return false;
	}
	if( !GetXmlValue(	pXMLDOMDocument,
						_T("damage"),
						_T(""),
						vbstrValue3 ) )
	{
		assert( !"�u�_���[�W�v�v�f�̒l�̎擾�Ɏ��s" );
		return false;
	}
	*/

	/////////////////////////////////////////////////////////////////////////////

	//for( unsigned int ui = 0; ui < vbstrValue.size(); ++ui )
	//{
		/*
		std::cout << "�u���ځv�v�f�̒l" << std::endl;
		std::cout << "\t" << _com_util::ConvertBSTRToString(vbstrValue1[ui]) << std::endl;
		
		std::cout << "�u�˒��v�v�f�̒l" << std::endl;
		std::cout << "\t" << _com_util::ConvertBSTRToString(vbstrValue2[ui]) << std::endl;

		std::cout << "�u�_���[�W�v�v�f�̒l" << std::endl;
		std::cout << "\t" << _com_util::ConvertBSTRToString(vbstrValue3[ui]) << std::endl;

		std::cout << "\t" << std::endl;
		*/
	//	a[ui].range = atoi(_com_util::ConvertBSTRToString(vbstrValue2[ui]));
	//}
	
	/////////////////////////////////////////////////////////////////////////////

	//int a;
	//a = atoi(_com_util::ConvertBSTRToString(vbstrValue2[0]));

	return true;
}
