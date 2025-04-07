using System;
using System.Runtime.CompilerServices;

namespace Entropy.ECS
{
    class Entity
    {
        private IntPtr nativePtr;

        // The method that will be called from native code
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern IntPtr Entity_Create();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void Entity_Destroy(IntPtr ptr);

        public Entity()
        {
            nativePtr = Entity_Create();
        }

        ~Entity()
        {
            Entity_Destroy(nativePtr);
        }

        public void AddComponent(IComponent component)
        {
            component.AddTo(nativePtr);
        }


    }
}
