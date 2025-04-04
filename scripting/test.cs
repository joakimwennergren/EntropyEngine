using Entropy;
using System;

public class EntityScript : EntropyScript {
    public override void Start() {
         var texture = new Texture("test.png");
         Console.WriteLine("Hello World2!");
    }

    public override void Update() {
         Console.WriteLine("Updating...");
    }
}