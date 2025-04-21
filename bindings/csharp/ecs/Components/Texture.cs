using System;
using System.IO;
using Entropy.Bindings;
using System.Runtime.InteropServices;

namespace Entropy.ECS.Components;

public struct Texture : IComponent
{

    public struct AtlasCoord 
    {
        public float x, y, z, w;
    }

    public IntPtr Name { get; set; }
    public IntPtr Path { get; set; }
    public float Index { get; set; } = -1.0f;
    public AtlasCoord Coords { get; set; }
    public bool isAtlas { get; set; }

    public Texture(string path)
    {
        int lastSlash = path.LastIndexOfAny(new[] { '/', '\\' });
        int lastDot = path.LastIndexOf('.');
        if (lastDot > lastSlash) 
        {
            string withoutExtension = path.Substring(0, lastDot);
            Name = Marshal.StringToHGlobalAnsi(withoutExtension);
        }
        Path = Marshal.StringToHGlobalAnsi(path);
    }

    public unsafe IntPtr ToNative()
    {
        IntPtr ptr = Marshal.AllocHGlobal(Marshal.SizeOf<Texture>());
        Marshal.StructureToPtr(this, ptr, false);
        return ptr;
    }

    public static unsafe void FreeNative(IntPtr ptr)
    {
        var native = Marshal.PtrToStructure<Texture>(ptr)!;

        Marshal.FreeHGlobal(native.Name);
        Marshal.FreeHGlobal(native.Path);
        Marshal.FreeHGlobal(ptr);
    }

    public void AddTo(ulong entityId)
    {
        IntPtr texPtr = ToNative();
        NativeBindings.EntityAddTexture(entityId, texPtr);
        //FreeNative(texPtr); //@TODO: Free this after the entity is destroyed
    }
}
