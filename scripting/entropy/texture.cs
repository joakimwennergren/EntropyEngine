using System;
using System.Runtime.CompilerServices;

namespace Entropy
{
    class Texture
    {

        private IntPtr nativePtr;

        // The method that will be called from native code
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern IntPtr Internal_Create(string path);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void Internal_Destroy(IntPtr ptr);

        public Texture(string path)
        {
            // Call the internal method to create the texture
            nativePtr = Internal_Create(path);
            //MyClass_create();
        }

        ~Texture()
        {
            // Call the internal method to destroy the texture
            Internal_Destroy(nativePtr);
        }
    }
}
