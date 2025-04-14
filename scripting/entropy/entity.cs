using System;
using System.Runtime.CompilerServices;

namespace Entropy.ECS
{
    public class Entity : IDisposable
    {
        private IntPtr entityPtr;
        public Entity()
        {
            Console.WriteLine("FROM ENTITY C#");
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
        /*
        public void AddComponent(IComponent component)
        {
            component.AddTo(nativePtr);
        }
        */
    }
}
