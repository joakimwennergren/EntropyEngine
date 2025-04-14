using System;
using System.Runtime.CompilerServices;
using Entropy.Bindings;

namespace Entropy.ECS
{
    public class Entity : IDisposable
    {
        private IntPtr entityPtr;
        public Entity()
        {
            entityPtr = NativeBindings.EntityCreate();
        }
        public void Dispose()
        {
            if (entityPtr != IntPtr.Zero)
            {
                NativeBindings.EntityDestroy(entityPtr);
                entityPtr = IntPtr.Zero;
            }
        }
    }
}
