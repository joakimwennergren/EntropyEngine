using System;
using System.Runtime.CompilerServices;

namespace Entropy
{
    public class TextureAtlas
    {
        public AssetHandle asset_handle;

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern AssetHandle TextureAtlas_Create(string[] paths);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern AssetHandle TextureAtlas_Destroy(AssetHandle handle);

        public TextureAtlas(string[] paths)
        {
            asset_handle = TextureAtlas_Create(paths);
        }

        ~TextureAtlas()
        {
            //TextureAtlas_Destroy(asset_handle);
        }
    }
}
