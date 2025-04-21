using System;
using Entropy.Bindings;

namespace Entropy.ECS.Components;

public struct Color : IComponent
{
    public float r { get; set; }
    public float g { get; set; }
    public float b { get; set; }
    public float a { get; set; }
    
    public Color(float r, float g, float b, float a)
    {
        this.r = r;
        this.g = g;
        this.b = b;
        this.a = a;
    }

    public void AddTo(ulong entityId)
    {
        NativeBindings.EntityAddColor(entityId, this);
    }
}
