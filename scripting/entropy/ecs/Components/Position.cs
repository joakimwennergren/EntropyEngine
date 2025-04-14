using System;
using Entropy.Bindings;

namespace Entropy.ECS.Components;

public struct Position : IComponent
{
    public float x { get; set; }
    public float y { get; set; }
    public float z { get; set; }

    public Position(float x, float y, float z)
    {
        this.x = x;
        this.y = y;
        this.z = z;
    }

    public Position(float x, float y)
    {
        this.x = x;
        this.y = y;
        this.z = 0;
    }

    public void AddTo(IntPtr entity)
    {
        NativeBindings.EntityAddPosition(entity, this);
    }
}
