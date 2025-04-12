using Entropy;
using Entropy.ECS;
using System;

public class EntityScript : EntropyScript
{
     private void CreateSprite(string[] paths, Dimension dim, Position pos)
     {
       var sprite = new Entity();
       sprite.AddComponent(new Sprite(
         paths,
         dim,
         pos
       ));
     }

     public override void Start()
     {
          Console.WriteLine("Hello from EntityScript!");
          CreateSprite(new[] { "svart_trumpetsvamp.png", "toppmurkla.png", "tegelsopp.png" }, new Dimension(100.0f, 100.0f), new Position(100.0f, 100.0f));
     }
}