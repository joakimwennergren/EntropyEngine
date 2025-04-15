using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using Entropy.ECS.Components;

namespace Entropy.Bindings;

internal static partial class NativeBindings
{
    [LibraryImport("Entropy", EntryPoint = "Entity_Create")]
    public static partial ulong EntityCreate();

    [LibraryImport("Entropy", EntryPoint = "Entity_Destroy")]
    public static partial void EntityDestroy(ulong entityId);

    [LibraryImport("Entropy", EntryPoint = "Entity_AddPosition")]
    public static partial void EntityAddPosition(ulong entityId, Position position);

    [LibraryImport("Entropy", EntryPoint = "Entity_AddDimension")]
    public static partial void EntityAddDimension(ulong entityId, Dimension dimension);

    [LibraryImport("Entropy", EntryPoint = "Entity_AddRotation")]
    public static partial void EntityAddRotation(ulong entityId, Rotation rotation);
    
    /*
    [UnmanagedCallersOnly(EntryPoint = "Entity_AddAsset")]
    //[MethodImplAttribute(MethodImplOptions.InternalCall)]
    public static extern void Entity_AddAsset(IntPtr entity, AssetHandle handle);
    */
}

