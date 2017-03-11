/**************************************************************************************/
/* A very basic AI initialization for testing of registering multiple vision points.  */
/* Created on: 11/03/2017                                                             */
/* Created by: Ozan Ozkan                                                             */
/* https://github.com/OzanOzkan/                                                      */
/**************************************************************************************/


#include "StdAfx.h"
#include "GamePlugin.h"

#include "MyAIEntity.h"

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
	pGameObject->GetEntity()->LoadGeometry(0, "Objects/box.cgf");
	return pGameObject->BindToNetwork();
}

void CMyAIEntity::PostInit(IGameObject* pGameObject)
{
	AIObjectParams params(AIOBJECT_TARGET, NULL, pGameObject->GetEntityId());
	pGameObject->GetEntity()->RegisterInAISystem(params);

	gEnv->pGameFramework->GetIActorSystem()->AddActor(pGameObject->GetEntityId(), this);

	Physicalize();

	InitializeVisionPoints();
}

void CMyAIEntity::InitializeVisionPoints()
{
	if (IAIObject* pAIObject = GetEntity()->GetAI())
	{
		IVisionMap* pVisionMap = gEnv->pAISystem->GetVisionMap();

		string ObservableName = GetEntity()->GetName();
		ObservableName += ".ObservableVision";
		m_ObservableVisionID = pVisionMap->CreateVisionID(ObservableName);
		ObservableParams oParams;
		oParams.observablePositionsCount = 3;
		oParams.observablePositions[0] = GetEntity()->GetPos() + Vec3(0, 0, 0);
		oParams.observablePositions[1] = GetEntity()->GetPos() + Vec3(0, 1, 0);
		oParams.observablePositions[2] = GetEntity()->GetPos() + Vec3(0, -1, 0);
		oParams.typeMask = Player;
		oParams.faction = IFactionMap::Hostile;
		pVisionMap->RegisterObservable(m_ObservableVisionID, oParams);

		string ObserverName = GetEntity()->GetName();
		ObserverName += ".ObserverVision";
		m_ObserverVisionID = pVisionMap->CreateVisionID(ObserverName);
		ObserverParams obParams;
		obParams.callback = functor(*this, &CMyAIEntity::VisionChanged);
		obParams.entityId = GetEntityId();
		obParams.factionsToObserveMask = ~0u;
		obParams.fovCos = cosf(DEG2RAD(50));
		obParams.typesToObserveMask = Player;
		obParams.eyePosition = GetEntity()->GetWorldPos() + Vec3(0, 0, 1);
		obParams.eyeDirection = GetEntity()->GetForwardDir().GetNormalized();
		obParams.sightRange = 10.f;
		pVisionMap->RegisterObserver(m_ObserverVisionID, obParams);

		pAIObject->SetFactionID(IFactionMap::Hostile);
		pAIObject->SetGroupId(1);
		pAIObject->SetPos(GetEntity()->GetWorldPos(), GetEntity()->GetForwardDir());

		pAIObject->SetObservable(true);
	}
}

void CMyAIEntity::TerminateVisionPoints()
{
	IVisionMap* pVisionMap = gEnv->pAISystem->GetVisionMap();
	pVisionMap->UnregisterObservable(m_ObservableVisionID);
	pVisionMap->UnregisterObserver(m_ObserverVisionID);

	GetEntity()->GetAI()->SetObservable(false);
}

void CMyAIEntity::ProcessEvent(SEntityEvent& event)
{
	switch (event.event)
	{
		case ENTITY_EVENT_RESET:
		{
			TerminateVisionPoints();
			InitializeVisionPoints();
		}
		break;
		case ENTITY_EVENT_XFORM:
		{
			UpdateVisionPointLocations();
		}
		break;
	}
}

void CMyAIEntity::Update(SEntityUpdateContext& ctx, int updateSlot)
{

}

void CMyAIEntity::VisionChanged(const VisionID& observerID, const ObserverParams& observerParams, const VisionID& observableID, const ObservableParams& observableParams, bool visible)
{
	CryLogAlways("[MyAIEntity]: Vision changed.");
}

void CMyAIEntity::Physicalize()
{
	SEntityPhysicalizeParams physicsParams;

	physicsParams.nSlot = 0;
	physicsParams.mass = 1;
	physicsParams.density = 0;
	physicsParams.type = PE_LIVING;

	GetEntity()->Physicalize(physicsParams);
}

void CMyAIEntity::UpdateVisionPointLocations()
{
	if (IAIObject* pAIObject = GetEntity()->GetAI())
	{
		pAIObject->SetPos(GetEntity()->GetWorldTM().TransformPoint(Vec3(0,0,1)), GetEntity()->GetForwardDir());

		ObserverParams observerParams;
		observerParams.eyePosition = GetEntity()->GetWorldPos() + Vec3(0, 0, 1);
		observerParams.eyeDirection = GetEntity()->GetForwardDir().GetNormalized();
		gEnv->pAISystem->GetVisionMap()->ObserverChanged(m_ObserverVisionID, observerParams, eChangedPosition | eChangedOrientation);

		ObservableParams observableParams;
		observableParams.observablePositionsCount = 3;
		observableParams.observablePositions[0] = GetEntity()->GetPos() + Vec3(0, 0, 0);
		observableParams.observablePositions[1] = GetEntity()->GetPos() + Vec3(0, 1, 0);
		observableParams.observablePositions[2] = GetEntity()->GetPos() + Vec3(0, -1, 0);
		gEnv->pAISystem->GetVisionMap()->ObservableChanged(m_ObservableVisionID, observableParams, eChangedPosition | eChangedOrientation);
	}
}