using Entropy;
using Entropy.ECS;
using System;

public class EntityScript : EntropyScript
{
     private Entity sprite_;
     private Entity sprite2_;

     private void CreateSprite(string path, Dimension dim, Position pos)
     {
       var sprite = new Entity();
       sprite.AddComponent(new Sprite(
         path,
         dim,
         pos
       ));
     }

     public override void Start()
     {
          Console.WriteLine("Hello from EntityScript!");
          CreateSprite("svart_trumpetsvamp.png", new Dimension(100.0f, 100.0f), new Position(100.0f, 100.0f));
          CreateSprite("toppmurkla.png", new Dimension(25.0f, 50.0f), new Position(400.0f, 400.0f));
          CreateSprite("tegelsopp.png", new Dimension(25.0f, 50.0f), new Position(500.0f, 200.0f));
     }
}