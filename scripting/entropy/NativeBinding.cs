using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using Entropy.ECS;

namespace Entropy
{
    internal static partial class NativeBindings
    {
        [LibraryImport("Entropy", EntryPoint = "Entity_Create")]
        public static partial IntPtr EntityCreate();

        [LibraryImport("Entropy", EntryPoint = "Entity_Destroy")]
        public static partial void EntityDestroy(IntPtr entity);

        //[LibraryImport("Entropy", EntryPoint = "Entity_AddPosition")]
        //public static partial void Entity_AddPosition(IntPtr entity, Position position);

        /*
        [UnmanagedCallersOnly(EntryPoint = "Entity_AddDimension")]
        //[MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void Entity_AddDimension(IntPtr entity, Dimension dimension);

        [UnmanagedCallersOnly(EntryPoint = "Entity_AddRotation")]
        //[MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void Entity_AddRotation(IntPtr entity, Rotation rotation);

        [UnmanagedCallersOnly(EntryPoint = "Entity_AddAsset")]
        //[MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void Entity_AddAsset(IntPtr entity, AssetHandle handle);
        */
    }
}
