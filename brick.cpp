/* heXon
// Copyright (C) 2017 LucKey Productions (luckeyproductions.nl)
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include "arena.h"
#include "TailGenerator.h"
#include "hitfx.h"
#include "chaomine.h"
#include "ship.h"
#include "spawnmaster.h"

#include "brick.h"

void Brick::RegisterObject(Context* context)
{
    context->RegisterFactory<Brick>();
}

Brick::Brick(Context* context) : SceneObject(context),
    tailGen_{},
    damage_{4.2f}
{

}

void Brick::OnNodeSet(Node* node)
{ if (!node) return;

    SceneObject::OnNodeSet(node);

    node_->SetName("Brick");

    rigidBody_ = node_->CreateComponent<RigidBody>();
    rigidBody_->SetMass(2.3f);
    rigidBody_->SetLinearDamping(0.23f);
    rigidBody_->SetTrigger(true);
    rigidBody_->SetLinearFactor(Vector3::ONE - Vector3::UP);

    CollisionShape* trigger{ node_->CreateComponent<CollisionShape>() };
    trigger->SetSphere(1.0f);

    ParticleEmitter* particleEmitter{ node_->CreateComponent<ParticleEmitter>() };
    particleEmitter->SetEffect(CACHE->GetResource<ParticleEffect>("Particles/Seeker.xml"));

    AddTail();

    Light* light{ node_->CreateComponent<Light>() };
    light->SetRange(4.2f);
    light->SetBrightness(3.4f);
    light->SetColor(Color(1.0f, 1.0f, 1.0f));

//    sample_ = MC->GetSample("Seeker");
}

void Brick::Update(float timeStep)
{
}

void Brick::Set(Vector3 position, Vector3 direction)
{
    SceneObject::Set(position);
    rigidBody_->ResetForces();
    rigidBody_->SetLinearVelocity(Vector3::ZERO);
    MC->arena_->AddToAffectors(WeakPtr<Node>(node_), WeakPtr<RigidBody>(rigidBody_));
    AddTail();
//    PlaySample(sample_, 0.666f);

    node_->LookAt(position + direction);
    rigidBody_->ApplyImpulse(direction * 123.0f);

    SubscribeToEvent(node_, E_NODECOLLISIONSTART, URHO3D_HANDLER(Brick, HandleTriggerStart));
}

void Brick::HandleTriggerStart(StringHash eventType, VariantMap &eventData)
{ (void)eventType; (void)eventData;

    if (!node_->IsEnabled())
        return;

    PODVector<RigidBody*> collidingBodies{};
    rigidBody_->GetCollidingBodies(collidingBodies);

    for (unsigned i{0}; i < collidingBodies.Size(); ++i) {
        RigidBody* collider{ collidingBodies[i] };
        if (collider->GetNode()->HasComponent<Ship>()) {
            Ship* hitShip{ collider->GetNode()->GetComponent<Ship>() };

            hitShip->Hit(damage_, false);

            GetSubsystem<SpawnMaster>()->Create<HitFX>()
                    ->Set(node_->GetPosition(), 0, false);
            collider->ApplyImpulse(rigidBody_->GetLinearVelocity() * 0.5f);
            Disable();
        }
        else if (collider->GetNode()->HasComponent<ChaoMine>()){
            collider->GetNode()->GetComponent<ChaoMine>()->Hit(damage_, 0);
        }
    }
}

void Brick::Disable()
{
//    RemoveTail();
    SceneObject::Disable();
}

void Brick::AddTail()
{
    RemoveTail();

    tailGen_ = node_->CreateComponent<TailGenerator>();
    tailGen_->SetWidthScale(0.666f);
    tailGen_->SetTailLength(0.13f);
    tailGen_->SetNumTails(7);
    tailGen_->SetColorForHead(Color(0.5f, 0.23f, 0.666f, 0.42f));
    tailGen_->SetColorForTip(Color(0.0f, 0.1f, 0.23f, 0.0f));
}
void Brick::RemoveTail()
{
    if (tailGen_){
        tailGen_->Remove();
        tailGen_ = nullptr;
    }
}
