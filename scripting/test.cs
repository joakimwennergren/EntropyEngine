using Entropy;
using Entropy.ECS;
using System;

public class EntityScript : EntropyScript
{
     public override void Start()
     {
          var entity = new Entity();
          var tex = new Texture("test.png");
          entity.AddComponent(new TwoDQuad());
          entity.AddComponent(new Position(400.0f, 400.0f, 1.0f));
          entity.AddComponent(new Dimension(100.0f, 100.0f, 0.0f));
          entity.AddComponent(new Texture(tex.textureId));

          Console.WriteLine("Hello from EntityScript!");
     }

     public override void Update()
     {
          Console.WriteLine("Updating...");
     }
}