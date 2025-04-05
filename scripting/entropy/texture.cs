using System;
using System.Runtime.CompilerServices;

namespace Entropy
{
    class Texture
    {
        // The method that will be called from native code
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern IntPtr Texture_Create(string path);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void Texture_Destroy(IntPtr ptr);

        public Texture(string path)
        {
            textureId = Texture_Create(path);
        }

        ~Texture()
        {
            Texture_Destroy(textureId);
        }

        public int textureId;
    }
}
