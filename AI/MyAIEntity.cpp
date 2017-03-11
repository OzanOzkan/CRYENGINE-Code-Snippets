/**************************************************************************************/
/* A very basic AI initialization for testing of registering multiple vision points.  */
/* Created on: 11/03/2017                                                             */
/* Created by: Ozan Ozkan                                                             */
/* https://github.com/OzanOzkan/                                                      */
/**************************************************************************************/


#include "StdAfx.h"
#include "GamePlugin.h"

#include "MyAIEntity.h"

#include <CryAISystem/IAIObjectManager.h>
#include <CryAISystem/IVisionMap.h>
#include <CryAISystem/VisionMapTypes.h>

class CMyAIEntityRegistrator : public IEntityRegistrator
{
	virtual void Register() override
	{
		REGISTER_FACTORY(gEnv->pGameFramework, "MyAIEntity", CMyAIEntity, true);
	}

	virtual void Unregister() override {}
};

CMyAIEntityRegistrator g_MyAIEntityRegistrator;

CMyAIEntity::CMyAIEntity()
{
}

CMyAIEntity::~CMyAIEntity()
{
	gEnv->pGameFramework->GetIActorSystem()->RemoveActor(GetEntity()->GetId());
}

bool CMyAIEntity::Init(IGameObject* pGameObject)
{
	pGameObject->GetEntity()->LoadGeometry(0, "Objects/sphere.cgf");
	return pGameObject->BindToNetwork();
}

void CMyAIEntity::PostInit(IGameObject* pGameObject)
{
	AIObjectParams params(AIOBJECT_TARGET, NULL, pGameObject->GetEntityId());
	pGameObject->GetEntity()->RegisterInAISystem(params);

	gEnv->pGameFramework->GetIActorSystem()->AddActor(pGameObject->GetEntityId(), this);

	if (IAIObject* pAIObject = pGameObject->GetEntity()->GetAI())
	{
		IVisionMap* pVisionMap = gEnv->pAISystem->GetVisionMap();
		VisionID vis1ID = pVisionMap->CreateVisionID("Vision1");

		ObservableParams oParams;
		oParams.observablePositionsCount = 3;
		oParams.observablePositions[0] = GetEntity()->GetPos() + Vec3(0, 0, 0);
		oParams.observablePositions[1] = GetEntity()->GetPos() + Vec3(0, 1, 0);
		oParams.observablePositions[2] = GetEntity()->GetPos() + Vec3(0, -1, 0);
		oParams.typeMask = Player;
		oParams.faction = IFactionMap::Hostile;

		pVisionMap->RegisterObservable(vis1ID, oParams);

		pAIObject->SetFactionID(IFactionMap::Hostile);
		pAIObject->SetGroupId(1);
		pAIObject->SetPos(pGameObject->GetEntity()->GetWorldPos(), pGameObject->GetEntity()->GetForwardDir());

	}
}

void CMyAIEntity::ProcessEvent(SEntityEvent& event)
{

}