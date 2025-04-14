using Entropy;
using Entropy.ECS;
using System;

public class EntityScript : EntropyScript
{
     private void CreateSprite(TextureAtlas atlas, string sprite,  Dimension dim, Position pos, Rotation rot)
     {
       var e = new Entity();
       e.AddComponent(new Sprite(
         atlas,
         sprite,
         dim,
         pos,
         rot
       ));
     }

     public override void Start()
     {
          var atlas = new TextureAtlas(new[] { "stensopp.png", "svart_trumpetsvamp.png", "toppmurkla.png", "tegelsopp.png" /* "sprod_vaxskivling.png" */});
          //var atlas2 = new TextureAtlas(new[] { "toppig_giftspindelskivling.png",  "tegelsopp.png", "sprod_vaxskivling.png"});
          CreateSprite(atlas, "stensopp.png", new Dimension(100.0f, 100.0f), new Position(200.0f, 200.0f), new Rotation(180.0f));
          //CreateSprite(atlas, "svart_trumpetsvamp.png", new Dimension(100.0f, 100.0f), new Position(300.0f, 300.0f), new Rotation(0.0f));
          //CreateSprite(atlas, "toppmurkla.png", new Dimension(25.0f, 50.0f), new Position(400.0f, 400.0f), new Rotation(0.0f));
          //CreateSprite(atlas2, "toppig_giftspindelskivling.png", new Dimension(200.0f, 200.0f), new Position(600.0f, 500.0f), new Rotation(0.0f));
     }
}