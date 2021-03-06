#include "physics.h"
#include <exception>

namespace wx2::phys
{
	Physics::~Physics() noexcept
	{
		SafeRelease(physics_);
		
		if (pvd_)
		{
			pvd_->release();
		}

		SafeRelease(foundation_);
	}

	bool Physics::Initialize(const physx::PxU32 numThread) noexcept
	{
		try
		{
			foundation_ = PxCreateFoundation(
				PX_PHYSICS_VERSION,allocator_,errorCallback_);
			WX2_RUNTIME_ERROR_IF_FAILED(foundation_, "PxFoundationの作成に失敗しました。");

			pvd_ = PxCreatePvd(*foundation_);
			physx::PxPvdTransport* tranceport = physx::PxDefaultPvdSocketTransportCreate("localhost", 5425, 10);
			pvd_->connect(*tranceport, physx::PxPvdInstrumentationFlag::eALL);

			physics_ = PxCreatePhysics(
				PX_PHYSICS_VERSION,
				*foundation_,
				physx::PxTolerancesScale(),
				true,
				pvd_);
			WX2_RUNTIME_ERROR_IF_FAILED(physics_, "PxPhysicsの作成に失敗しました。");

			cooking_ = PxCreateCooking(
				PX_PHYSICS_VERSION, *foundation_, physx::PxCookingParams(physics_->getTolerancesScale()));
			WX2_RUNTIME_ERROR_IF_FAILED(cooking_, "PxCoolingの作成に失敗しました。");

			cpuDispacher_ = physx::PxDefaultCpuDispatcherCreate(numThread);
			WX2_RUNTIME_ERROR_IF_FAILED(cpuDispacher_, "PxDefaultCPUDispacherの作成に失敗しました。");

			//physx::PxSceneDesc sceneDesc(physics_->getTolerancesScale());
			//sceneDesc.gravity = physx::PxVec3(0.0f, -9.8f, 0.0f);
			//sceneDesc.cpuDispatcher = cpuDispacher_;
			//sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
			//sceneDesc.cudaContextManager = cudaContextManager_;
			//sceneDesc.flags |= physx::PxSceneFlag::eENABLE_GPU_DYNAMICS;
			//sceneDesc.broadPhaseType = physx::PxBroadPhaseType::eGPU;
			//scene_ = physics_->createScene(sceneDesc);
			//WX2_RUNTIME_ERROR_IF_FAILED(scene_, "PxSceneの作成に失敗しました。");
			
			bool res = scene_.Initialize(*physics_, *cpuDispacher_, *cudaContextManager_);
			WX2_RUNTIME_ERROR_IF_FAILED(res, "物理演算シーンの作成に失敗しました。");

			physx::PxPvdSceneClient* pvdClient = scene_->getScenePvdClient();
			if (pvdClient)
			{
				scene_->getScenePvdClient()->setScenePvdFlags(
					physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS |
					physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES |
					physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS);
			}
			else
			{
				WX2_LOG_INFO("PVDクライアントが起動されていません。");
			}

			return true;
		}
		catch (const RuntimeError& runtimeError)
		{
			WX2_LOG_ERROR(runtimeError.what());
			return false;
		}
	}

	physx::PxMaterial* Physics::CreateMaterial(
		const float staticFriction,
		const float dynamicFriction,
		const float bounciness) const noexcept
	{
		return physics_->createMaterial(staticFriction, dynamicFriction, bounciness);
	}

	RigidDynamic Physics::CreateDynamic(
		const physx::PxTransform& transform, 
		const physx::PxGeometry& geometry,
		physx::PxMaterial& material,
		const physx::PxReal density) const noexcept
	{
		RigidDynamic rd;
		rd.Initialize(*physics_, scene_, transform, geometry, material, density);
		return rd;
	}

	RigidStatic Physics::CreateStatic(
		const physx::PxTransform& transform,
		const physx::PxGeometry& geometry, 
		physx::PxMaterial& material) const noexcept
	{
		RigidStatic rs;
		rs.Initialize(*physics_, scene_, transform, geometry, material);
		return rs;
	}

	void Physics::Step(const float deltaTime) const noexcept
	{
		scene_->simulate(deltaTime);
		scene_->fetchResults(true);
	}
}
