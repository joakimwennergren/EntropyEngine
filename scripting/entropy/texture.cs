using System;
using System.Runtime.InteropServices;

class Texture
{

    // Declare the external C++ functions
    [DllImport("libentropy", CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr MyClass_create();

    [DllImport("libentropy", CallingConvention = CallingConvention.Cdecl)]
    public static extern void MyClass_sayHello(IntPtr obj);

    [DllImport("libentropy", CallingConvention = CallingConvention.Cdecl)]
    public static extern void MyClass_destroy(IntPtr obj);


    public Texture(string path)
    {
        // Call the internal method to create the texture
        //Internal_Create(path);
        MyClass_create();
    }
    
    ~Texture()
    {
        // Call the internal method to destroy the texture
        //Internal_Destroy();
    }
}