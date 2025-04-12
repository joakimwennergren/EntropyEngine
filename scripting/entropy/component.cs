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
        public static extern void Entity_AddTexture(IntPtr entity, string path);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void Entity_AddTextureAtlas(IntPtr entity, string[] paths);
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

        public Position(float x, float y)
        {
            this.x = x; this.y = y; this.z = 0.0f;
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

        public Dimension(float x, float y)
        {
            this.x = x; this.y = y; this.z = 0.0f;
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
        public string path;

        public Texture(string path)
        {
            this.path = path;
        }

        public void AddTo(IntPtr entity)
        {
            NativeBindings.Entity_AddTexture(entity, path);
        }
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct TextureAtlas : IComponent
    {
        public string[] paths;

        public TextureAtlas(string[] paths)
        {
            this.paths = paths;
        }

        public void AddTo(IntPtr entity)
        {
            NativeBindings.Entity_AddTextureAtlas(entity, paths);
        }
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct Sprite : IComponent
    {
        private string[] paths;
        private Dimension dimension;
        private Position position;


        // Main constructor
        public Sprite(string[] paths, Dimension dim, Position pos)
        {
            this.dimension = dim;
            this.position = pos;
            this.paths = paths;
        }

        /*

        // Overload with default Dimension and Position
        public Sprite(string path)
            : this(path, new Dimension(0.0f, 0.0f, 0.0f), new Position(0.0f, 0.0f, 0.0f))
        {
        }

        // Overload with default Position only
        public Sprite(strings path, Dimension dim)
            : this(path, dim, new Position(0.0f, 0.0f, 0.0f))
        {
        }
        */

        public void AddTo(IntPtr entity)
        {
            NativeBindings.Entity_AddPosition(entity, position);
            NativeBindings.Entity_AddDimension(entity, dimension);
            NativeBindings.Entity_Add2DQuad(entity);
            NativeBindings.Entity_AddTextureAtlas(entity, paths);
        }
    }
}
