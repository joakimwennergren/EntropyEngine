using System;
using System.Runtime.CompilerServices;

namespace Entropy
{
    class Texture
    {
        // The method that will be called from native code
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern int Texture_Create(string path);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void Texture_Destroy(int ptr);

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
