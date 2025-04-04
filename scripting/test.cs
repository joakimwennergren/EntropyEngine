using Entropy;
using System;

public class EntityScript : EntropyScript {
    public override void Start() {
         Texture texture = new Texture("testhahah.png");
         Console.WriteLine("Hello World2!");
    }

    public override void Update() {
         Console.WriteLine("Updating...");
    }
}