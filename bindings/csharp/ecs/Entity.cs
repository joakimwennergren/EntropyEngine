using System;
using System.Runtime.CompilerServices;
using Entropy.Bindings;

namespace Entropy.ECS;

public class Entity : IDisposable
{
    private ulong entityId_;

    public Entity()
    {
        entityId_ = NativeBindings.EntityCreate();
    }

    ~Entity()
    {
        Dispose();
    }

    public void Dispose()
    {
        NativeBindings.EntityDestroy(entityId_);
    }

    public void UpdateComponent(IComponent component)
    {
        component.Update(entityId_);
    }

    public void AddComponent(IComponent component)
    {
        component.AddTo(entityId_);
    }
}
