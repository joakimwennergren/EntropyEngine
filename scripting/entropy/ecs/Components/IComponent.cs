using System;

namespace Entropy.ECS
{
    public interface IComponent
    {
        void AddTo(IntPtr entity);
    }
}