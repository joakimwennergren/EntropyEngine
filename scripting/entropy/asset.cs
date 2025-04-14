using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace Entropy
{
    public enum AssetType {
      Texture,
      TextureAtlas,
      Unknown,
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct AssetHandle {
        public IntPtr asset;
        public int index;
        public IntPtr path; // prevent automatic marshaling
        public bool isAsync;
        public AssetType type;
    }
}
