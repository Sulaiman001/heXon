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

#ifndef SHIP_H
#define SHIP_H

#include <Urho3D/Urho3D.h>
#include "TailGenerator.h"

#include "controllable.h"

class Ship : public Controllable
{
    URHO3D_OBJECT(Ship, LogicComponent);
public:
    Ship(Context* context);
    static void RegisterObject(Context* context);
    virtual void OnNodeSet(Node* node);
    virtual void Update(float timeStep);
    void EnterPlay();
    void HandleNodeCollisionStart(StringHash eventType, VariantMap& eventData);
private:
    int playerId_;
//    StaticModel* model_;
    ParticleEmitter* particleEmitter_;
    void CreateTails();
    Vector<TailGenerator*> tailGens_;
};

#endif // SHIP_H
