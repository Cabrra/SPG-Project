using UnityEngine;
using System.Collections;

public class Billboard : MonoBehaviour 
{
	public Transform lookTarget;
	public Vector2 spriteFacing;
	public bool lockRotation = true;
	
	void Start() 
	{
		//spriteFacing = new Vector2(1,1);
		
		if(!lookTarget)
			lookTarget = Camera.main.transform;
	}
	
	void LateUpdate () 
	{		
		if(lookTarget == null)
		{
			print("Billboard target is null");
			return;
		}
		
		if(spriteFacing.magnitude != 0)
			renderer.material.mainTextureScale = spriteFacing;
				
		if(lockRotation)
		{
			transform.rotation = lookTarget.rotation;
			transform.Rotate(-90,0,0);			
		}
		else
		{
			transform.LookAt(lookTarget);
			transform.Rotate(90,0,0);
		}
	}
	
	public void SetFacing(Vector2 facing)
	{
		spriteFacing = facing;		
	}
}                                 



//Vector3 v = Camera.main.transform.position - transform.position;
//v.y = v.z = 0.0f;
//transform.LookAt(Camera.main.transform.position - v); 