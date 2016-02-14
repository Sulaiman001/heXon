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

#ifndef RAZOR_H
#define RAZOR_H

#include <Urho3D/Urho3D.h>

#include "enemy.h"

using namespace Urho3D;

class Razor : public Enemy
{
    URHO3D_OBJECT(Razor, Enemy);
public:
    Razor(Context* context, MasterControl* masterControl);
    void Hit(float damage, int ownerID);
    void Set(Vector3 position);
protected:
    float topSpeed_;
    float aimSpeed_;

    Node* topNode_;
    Node* bottomNode_;
    StaticModel* topModel_;
    StaticModel* bottomModel_;
    Vector2 textureOffset;
    Vector2 textureOffsetDelta;

    void HandleRazorUpdate(StringHash eventType, VariantMap &eventData);
};

#endif // RAZOR_H
