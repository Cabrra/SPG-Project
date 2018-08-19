using UnityEngine;
using System.Collections;

public class MaterialTiling : MonoBehaviour 
{
	public float tilingX = 1;
	public float tilingY = 1;
	public bool matchObjectScale = false;
	public enum TilingMatch {NONE, OBJ_SCALE_X, OBJ_SCALE_Y, OBJ_SCALE_Z};
	public TilingMatch MaterialTilingX = TilingMatch.NONE;
	public TilingMatch MaterialTilingY = TilingMatch.NONE;
	public float xScaleMultiplier = 1;
	public float yScaleMultiplier = 1;
	
	void Start ()
	{
		Vector2 tiling = renderer.material.mainTextureOffset;
		
		if(matchObjectScale)
		{						
			if(MaterialTilingX == TilingMatch.OBJ_SCALE_X)
				tiling.x = transform.localScale.x;
			else if(MaterialTilingX == TilingMatch.OBJ_SCALE_Y)
				tiling.x = transform.localScale.y;
			else if(MaterialTilingX == TilingMatch.OBJ_SCALE_Z)
				tiling.x = transform.localScale.z;
			
			if(MaterialTilingY == TilingMatch.OBJ_SCALE_X)
				tiling.y = transform.localScale.x;
			else if(MaterialTilingY == TilingMatch.OBJ_SCALE_Y)
				tiling.y = transform.localScale.y;
			else if(MaterialTilingY == TilingMatch.OBJ_SCALE_Z)
				tiling.y = transform.localScale.z;
			
			tiling.x *= xScaleMultiplier;
			tiling.y *= yScaleMultiplier;
		}
		else
		{
			tiling.x = tilingX;
			tiling.y = tilingY;
		}
		
		renderer.material.mainTextureScale = tiling;
	}
}
