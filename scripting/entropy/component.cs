using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace Entropy.ECS
{
    internal static class NativeBindings
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void Entity_AddPosition(IntPtr entity, Position position);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void Entity_AddDimension(IntPtr entity, Dimension dimension);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void Entity_Add2DQuad(IntPtr entity);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void Entity_AddTexture(IntPtr entity, int textureId);
    }

    public interface IComponent
    {
        void AddTo(IntPtr entity);
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct Position : IComponent
    {
        public float x, y, z;
        public Position(float x, float y, float z)
        {
            this.x = x; this.y = y; this.z = z;
        }

        public void AddTo(IntPtr entity)
        {
            NativeBindings.Entity_AddPosition(entity, this);
        }
    }


    [StructLayout(LayoutKind.Sequential)]
    public struct Dimension : IComponent
    {
        public float x, y, z;
        public Dimension(float x, float y, float z)
        {
            this.x = x; this.y = y; this.z = z;
        }

        public void AddTo(IntPtr entity)
        {
            NativeBindings.Entity_AddDimension(entity, this);
        }
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct TwoDQuad : IComponent
    {
        public void AddTo(IntPtr entity)
        {
            NativeBindings.Entity_Add2DQuad(entity);
        }
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct Texture : IComponent
    {
        public int textureId;

        public Texture(int id)
        {
            this.textureId = id;
        }

        public void AddTo(IntPtr entity)
        {
            NativeBindings.Entity_AddTexture(entity, textureId);
        }
    }
}
