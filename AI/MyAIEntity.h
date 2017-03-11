/**************************************************************************************/
/* A very basic AI initialization for testing of registering multiple vision points.  */
/* Created on: 11/03/2017                                                             */
/* Created by: Ozan Ozkan                                                             */
/* https://github.com/OzanOzkan/                                                      */
/**************************************************************************************/

#pragma once

#include "Player/ISimpleActor.h"

#include <CryAISystem/IAIObjectManager.h>
#include <CryAISystem/IVisionMap.h>
#include <CryAISystem/VisionMapTypes.h>

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
	virtual void Update(SEntityUpdateContext& ctx, int updateSlot) override;
	// ~ISimpleActor

protected:
	void InitializeVisionPoints();
	void TerminateVisionPoints();
	void UpdateVisionPointLocations();
	void VisionChanged(const VisionID& observerID, const ObserverParams& observerParams, const VisionID& observableID, const ObservableParams& observableParams, bool visible);
	void Physicalize();

	VisionID m_ObserverVisionID;
	VisionID m_ObservableVisionID;
};