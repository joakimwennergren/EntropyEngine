using System;
using Entropy.Bindings;

namespace Entropy.ECS.Components;

public struct Dimension : IComponent
{
    public float x { get; set; }
    public float y { get; set; }
    public float z { get; set; }

    public Dimension(float x, float y, float z)
    {
        this.x = x;
        this.y = y;
        this.z = z;
    }

    public Dimension(float x, float y)
    {
        this.x = x;
        this.y = y;
        this.z = 0;
    }

    public void AddTo(ulong entityId)
    {
        NativeBindings.EntityAddDimension(entityId, this);
    }

    public void Update(ulong entityId)
    {
        NativeBindings.EntityUpdateDimension(entityId, this);
    }
}
