/**************************************************************************************/
/* A very basic AI initialization for testing of registering multiple vision points.  */
/* Created on: 11/03/2017                                                             */
/* Created by: Ozan Ozkan                                                             */
/* https://github.com/OzanOzkan/                                                      */
/**************************************************************************************/

#pragma once

#include "Player/ISimpleActor.h"

class CMyAIEntity
	: public CGameObjectExtensionHelper<CMyAIEntity, ISimpleActor>
{
public:
	CMyAIEntity();
	virtual ~CMyAIEntity();

	// ISimpleActor
	virtual bool Init(IGameObject* pGameObject) override;
	virtual void PostInit(IGameObject* pGameObject) override;
	virtual void ProcessEvent(SEntityEvent& event) override;
	// ~ISimpleActor
};