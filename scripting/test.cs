using Entropy;
using Entropy.ECS;
using System;

public class EntityScript : EntropyScript
{
     private Entity sprite_;
     private Entity sprite2_;

     public override void Start()
     {
          Console.WriteLine("Hello from EntityScript!");

          sprite_ = new Entity();
          sprite_.AddComponent(new Sprite(
            "test.png",
            new Dimension(10.0f, 10.0f),
            new Position(700.0f, 700.0f)
          ));

          sprite2_ = new Entity();
          sprite2_.AddComponent(new Sprite(
            "test.png",
            new Dimension(100.0f, 100.0f),
            new Position(300.0f, 300.0f)
          ));

     }
}