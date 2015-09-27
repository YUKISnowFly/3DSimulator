// MsXmlLoad.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

//2重インクルード防止
#pragma once

#include "stdafx.h"

#import "msxml6.dll" named_guids raw_interfaces_only
#include <atlbase.h>	// CComVariant, CComBSTR

#include <vector>
#include <iostream>
#include <conio.h>

// pXMLDOMDocumentの中身を読んでrvbstrValueとして出力
bool GetXmlValue(	const IXMLDOMDocumentPtr&	pXMLDOMDocument,
					const CComBSTR&				bstrTagName,
					const CComBSTR&				bstrAttributeName,
					std::vector<CComBSTR>&		rvbstrValue )
{
	// アウトプットの初期化
	rvbstrValue.clear();

	// インプットのチェック
	if( 0 == bstrTagName.Length() )
	{
		return false;
	}

	HRESULT hResult;

	// タグ名のノードリストの取得
	IXMLDOMNodeListPtr pXMLDOMNodeList = NULL;
	hResult = pXMLDOMDocument->getElementsByTagName( bstrTagName, &pXMLDOMNodeList );
	if( FAILED(hResult) || NULL == pXMLDOMNodeList )
	{
		ASSERT( !"タグ名のノードリストの取得に失敗" );
		return false;
	}
	// ノードリストのノードの数の取得
	long lCountNode = 0;
	hResult = pXMLDOMNodeList->get_length( &lCountNode );
	if( FAILED(hResult) )
	{
		ASSERT( !"ノードリストのノードの数の取得に失敗" );
		return false;
	}
	// ノードリストのノード一つずつ処理
	for( int i = 0; i < lCountNode; ++i )
	{
		// ノードリストのうちの一つのノードの取得
		IXMLDOMNodePtr pXMLDOMNode = NULL;
		hResult = pXMLDOMNodeList->get_item( i, &pXMLDOMNode );
		if( FAILED(hResult) )
		{
			ASSERT( !"ノードリストのうちの一つのノードの取得に失敗" );
			return false; 
		}

		// ノードタイプの取得
		DOMNodeType eNodeType;
		hResult = pXMLDOMNode->get_nodeType( &eNodeType );
		if( FAILED(hResult) )
		{
			ASSERT( !"ノードタイプの取得に失敗" );
			return false; 
		}
		if( NODE_ELEMENT != eNodeType )
		{
			ASSERT( !"ノードタイプがエレメントでないのはおかしい" );
			return false;
		}
		// エレメント型への変換
		IXMLDOMElementPtr pXMLDOMElement = NULL;
		hResult = pXMLDOMNode->QueryInterface( IID_IXMLDOMElement, (void**)&pXMLDOMElement );	// スマートポインタ型を利用しているので、pXMLDOMElement = pXMLDOMNode; でも良い。
		if( FAILED(hResult) || NULL == pXMLDOMElement )
		{
			ASSERT( !"エレメント型への変換に失敗" );
			return false;
		}

		// 属性値の取得か、データ値の取得か
		if( bstrAttributeName.Length() )
		{	// 属性値の取得
			IXMLDOMAttribute* pAttributeNode = NULL;
			CComVariant varValue;
			hResult = pXMLDOMElement->getAttribute( bstrAttributeName, &varValue ); 
			if( SUCCEEDED(hResult) && VT_BSTR == varValue.vt )
			{
				rvbstrValue.push_back( varValue.bstrVal );
			}
		}
		else
		{	// データ値の取得
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

// XMLのドキュメントをpXMLDOMDocumentとして作成
bool ParseXmlFile( TCHAR* pszFileName, IXMLDOMDocumentPtr& pXMLDOMDocument)
{
	if( NULL == pszFileName )
	{
		return false;
	}

	HRESULT hResult;

	// DOMドキュメントの作成
	hResult = pXMLDOMDocument.CreateInstance(CLSID_DOMDocument);
	if( FAILED(hResult) )
	{
		ASSERT( !"DOMドキュメントの作成に失敗" );
		return false;
	}

	// XMLファイル読み込み
	pXMLDOMDocument->put_async( VARIANT_FALSE ); // load関数がロードを完了するまで待つようにする。
	VARIANT_BOOL varbResult;
	hResult = pXMLDOMDocument->load( CComVariant(pszFileName), &varbResult );
	if( FAILED(hResult) || !varbResult)
	{
		ASSERT( !"XMLファイル読み込みに失敗" );
		return false;
	}

	/*
	// 要素の値の取得

	if( !GetXmlValue(	pXMLDOMDocument,
						_T("name"),
						_T(""),
						vbstrValue ) )
	{
		ASSERT( !"「項目」要素の値の取得に失敗" );
		return false;
	}
	
	if( !GetXmlValue(	pXMLDOMDocument,
						_T("range"),
						_T(""),
						vbstrValue2 ) )
	{
		assert( !"「射程」要素の値の取得に失敗" );
		return false;
	}
	if( !GetXmlValue(	pXMLDOMDocument,
						_T("damage"),
						_T(""),
						vbstrValue3 ) )
	{
		assert( !"「ダメージ」要素の値の取得に失敗" );
		return false;
	}
	*/

	/////////////////////////////////////////////////////////////////////////////

	//for( unsigned int ui = 0; ui < vbstrValue.size(); ++ui )
	//{
		/*
		std::cout << "「項目」要素の値" << std::endl;
		std::cout << "\t" << _com_util::ConvertBSTRToString(vbstrValue1[ui]) << std::endl;
		
		std::cout << "「射程」要素の値" << std::endl;
		std::cout << "\t" << _com_util::ConvertBSTRToString(vbstrValue2[ui]) << std::endl;

		std::cout << "「ダメージ」要素の値" << std::endl;
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
