using System;
using Entropy.Bindings;

namespace Entropy.ECS.Components;

public struct Rotation : IComponent
{
    public float x { get; set; }
    public float y { get; set; }
    public float z { get; set; }
    public float a { get; set; }

    public Rotation(float a)
    {
        this.x = 0;
        this.y = 0;
        this.z = 0;
        this.a = a;
    }

    public void AddTo(ulong entityId)
    {
        NativeBindings.EntityAddRotation(entityId, this);
    }
}
