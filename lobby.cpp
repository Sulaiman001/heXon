/* heXon
// Copyright (C) 2016 LucKey Productions (luckeyproductions.nl)
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

#include "lobby.h"
#include "door.h"
#include "ship.h"
#include "splatterpillar.h"
#include "highest.h"
#include "effectmaster.h"

void Lobby::RegisterObject(Context *context)
{
    context->RegisterFactory<Lobby>();
}

Lobby::Lobby(Context* context) : LogicComponent(context)
{

}

void Lobby::OnNodeSet(Node *node)
{ (void)node;

    node_->Rotate(Quaternion(0.0f, 0.0f, 0.0f));
    Node* chamberNode{node_->CreateChild("Chamber")};
    StaticModel* chamberModel{chamberNode->CreateComponent<StaticModel>()};
    chamberModel->SetModel(MC->GetModel("Chamber"));
    chamberModel->SetMaterial(0, MC->GetMaterial("Basic"));
    chamberModel->SetMaterial(1, MC->GetMaterial("PitchBlack"));
    chamberModel->SetMaterial(4, MC->GetMaterial("Drain"));
    chamberModel->SetMaterial(2, MC->GetMaterial("GreenGlow"));
    chamberModel->SetMaterial(3, MC->GetMaterial("PurpleGlow"));
    chamberModel->SetCastShadows(true);

    //Create highest
    Node* highestNode{ node_->CreateChild("Highest") };
    highestNode->CreateComponent<Highest>();

    //Create coliders
    node_->CreateComponent<RigidBody>();
//    node_->CreateComponent<CollisionShape>()->SetTriangleMesh(MC->GetModel("Chamber_COLLISION"));

    node_->CreateComponent<CollisionShape>()->SetConvexHull(MC->GetModel("CC_Center"));
    node_->CreateComponent<CollisionShape>()->SetConvexHull(MC->GetModel("CC_CentralBox"));
    node_->CreateComponent<CollisionShape>()->SetConvexHull(MC->GetModel("CC_Edge1"));
    node_->CreateComponent<CollisionShape>()->SetConvexHull(MC->GetModel("CC_Edge2"));
    node_->CreateComponent<CollisionShape>()->SetConvexHull(MC->GetModel("CC_Side1"));
    node_->CreateComponent<CollisionShape>()->SetConvexHull(MC->GetModel("CC_Side2"));

    CollisionShape* edge1Shape{node_->CreateComponent<CollisionShape>()};
    edge1Shape->SetConvexHull(MC->GetModel("CC_Edge1"));
    edge1Shape->SetPosition(Vector3::FORWARD * 1.7f);
    edge1Shape->SetRotation(Quaternion(180.0f, Vector3::UP));
    CollisionShape* edge2Shape{node_->CreateComponent<CollisionShape>()};
    edge2Shape->SetConvexHull(MC->GetModel("CC_Edge2"));
    edge2Shape->SetRotation(Quaternion(180.0f, Vector3::UP));
    CollisionShape* side1Shape{node_->CreateComponent<CollisionShape>()};
    side1Shape->SetConvexHull(MC->GetModel("CC_Side1"));
    side1Shape->SetRotation(Quaternion(180.0f, Vector3::UP));
    CollisionShape* side2Shape{node_->CreateComponent<CollisionShape>()};
    side2Shape->SetConvexHull(MC->GetModel("CC_Side2"));
    side2Shape->SetRotation(Quaternion(180.0f, Vector3::UP));

    //Create lights
    for (int l{0}; l < 4; ++l){
        Node* pointLightNode{node_->CreateChild("PointLight")};
        pointLightNode->SetPosition(Vector3(l % 2 ? -2.3f : 2.3f, 2.3f, l < 2 ? 3.0f : -3.0f));
        Light* pointLight{ pointLightNode->CreateComponent<Light>() };
        pointLight->SetLightType(LIGHT_POINT);
        pointLight->SetColor( l % 2 ? Color(0.34f, 0.9f, 0.1f) : Color(0.42f, 0.1f, 1.0f));
        pointLight->SetRange(13.0f);
        pointLight->SetCastShadows(true);
        pointLight->SetShadowBias(BiasParameters(0.0001f, 0.1f));
    }
    //Create doors and splatterpillars
    for (float x : { -2.26494f, 2.26494f }){
        Node* doorNode{ node_->CreateChild("Door") };
        doorNode->SetPosition(Vector3( x , 0.0f, 5.21843f));
        doorNode->CreateComponent<Door>();

        Node* splatterPillarNode{ node_->CreateChild("SplatterPillar") };
        splatterPillarNode->SetPosition(Vector3( x , 0.0f, -3.91992f));
        splatterPillarNode->CreateComponent<SplatterPillar>();
    }
}

void Lobby::Update(float timeStep)
{ (void)timeStep;

    PODVector<Node*> lightNodes{};
    node_->GetChildrenWithComponent<Light>(lightNodes);
    for (Node* lightNode : lightNodes)
        lightNode->GetComponent<Light>()->SetBrightness(
                    MC->Sine(0.13f, 0.666f, 1.0f,
                    0.5f * (lightNode->GetPosition().z_ < 0.0f)));
}

void Lobby::HandlePlayTrigger(StringHash otherNode, VariantMap& eventData)
{ (void)otherNode; (void)eventData;

    ///Both players should be ready
    MC->SetGameState(GS_PLAY);
}
