using System;

namespace Entropy.ECS
{
    public interface IComponent
    {
        void AddTo(ulong entityId);
        void Update(ulong entityId);
    }
}