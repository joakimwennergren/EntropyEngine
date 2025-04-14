using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace Entropy.ECS
{
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
            //NativeBindings.Entity_AddDimension(entity, this);
        }
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct Rotation : IComponent
    {
        public float x, y, z, a;
        public Rotation(float x, float y, float z, float a)
        {
            this.x = x; this.y = y; this.z = z; this.a = a;
        }

        public Rotation(float a)
        {
            this.x = 0.0f; this.y = 0.0f; this.z = 0.0f; this.a = a;
        }

        public void AddTo(IntPtr entity)
        {
            //NativeBindings.Entity_AddRotation(entity, this);
        }
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct Sprite : IComponent
    {
        public TextureAtlas atlas;
        private Dimension dimension;
        private Position position;
        private Rotation rotation;
        private string sprite;

        // Main constructor
        public Sprite(TextureAtlas atlas, string sprite, Dimension dim, Position pos, Rotation rot)
        {
            this.atlas = atlas;
            this.sprite = sprite;
            this.dimension = dim;
            this.position = pos;
            this.rotation = rot;
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
            //NativeBindings.Entity_AddPosition(entity, position);
            //NativeBindings.Entity_AddDimension(entity, dimension);
            //NativeBindings.Entity_AddRotation(entity, rotation);
            //atlas.asset_handle.path = Marshal.StringToHGlobalAnsi(this.sprite);
            //NativeBindings.Entity_AddAsset(entity, atlas.asset_handle);
        }
    }
}
