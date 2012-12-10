using UnityEngine;
using System.Collections;

public enum SpriteState
{
    Static,
    MovingLeft,
    MovingRight,
    NoAnimation
}

public class AwesomeSpriteSheetAnimatorCS : MonoBehaviour
{
    //vars 
    private int colcount = 3;
    private int rowcount = 4;
    private int rownum = 0;
    private int colnum = 0;
    private int totalcells = 3;
    private int fps = 10;
    private Vector2 offset;

    // states
    public int colStatic, colMoving;
    public int rowStatic, rowMoving;
    public int totalStatic, totalMoving;
    public int FPS_Static, FPS_Moving;

    private Renderer rend;

    private int i;
    public SpriteState State;

    //public Material Static, MovingLeft, MovingRight;

    public Material[] MaterialsForAnimation;
    private int index;

    private GameObject oldObj, newObj;

    void Start()
    {
        State = SpriteState.Static;
        index = 0;
    }
    void Update()
    {
        switch (State)
        {
            case SpriteState.Static: // 0
                if (index != 0)
                {
                    Destroy(gameObject.renderer.material);
                    transform.renderer.material = MaterialsForAnimation[0];
                    index = 0;
                }

                colcount = colStatic;
                rowcount = rowStatic;
                totalcells = totalStatic;
                fps = FPS_Static;
                spriteanimation(colcount, rowcount, rownum, colnum, totalcells, fps);
                break;

                case SpriteState.MovingLeft: // 1
                if (index != 1)
                {
                    Destroy(gameObject.renderer.material);
                    transform.renderer.material = MaterialsForAnimation[1];
                    index = 1;
                }

                colcount = colMoving;
                rowcount = rowMoving;
                totalcells = totalMoving;
                fps = FPS_Moving;
                spriteanimation(colcount, rowcount, rownum, colnum, totalcells, fps);
                break;

                case SpriteState.MovingRight: // 2
                if (index != 2)
                {
                    Destroy(gameObject.renderer.material);
                    transform.renderer.material = MaterialsForAnimation[2];
                    index = 2;
                }

                colcount = colMoving;
                rowcount = rowMoving;
                totalcells = totalMoving;
                fps = FPS_Moving;
                spriteanimation(colcount, rowcount, rownum, colnum, totalcells, fps);
                break;

                case SpriteState.NoAnimation:
                break;
        }
    }

    private void spriteanimation(int colCount, int rowCount, int rowNumber, int colNumber, int totalCells, int fps)
    {
        // Calculate index
        int index = (int)(Time.time * (float)fps);
        // Repeat when exhausting all cells
        index = index % totalCells;

        // Size of every cell
        Vector2 size = new Vector2(1.0f / colCount, 1.0f / rowCount);

        // split into horizontal and vertical index
        int uIndex = index % colCount;
        int vIndex = index / colCount;

        // build offset
        // v coordinate is the bottom of the image in opengl so we need to invert.
        offset = new Vector2((uIndex + colNumber) * size.x, (1.0f - size.y) - (vIndex + rowNumber) * size.y);

        renderer.material.SetTextureOffset("_MainTex", offset);
        renderer.material.SetTextureScale("_MainTex", size);
    }
}