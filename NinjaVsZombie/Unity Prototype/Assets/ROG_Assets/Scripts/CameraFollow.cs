using UnityEngine;
using System.Collections;

public enum CameraPerspectives {SIDE, ANGLE, TOP};//, TOP_A, TOP_B, TOP_C};

public class CameraFollow : MonoBehaviour 
{
	public float 				followDistance = 15;
	public float 				viewAngle = 45;
	public float 				followSpeedDamping = 0.01f;
	public GameObject	 		target;
	public CameraPerspectives 	perspective = CameraPerspectives.TOP;
	
	void Start()
	{
		// The gameObject to follow (Default is object tagged as "Player")
		if(target == null)
			target = GameObject.FindGameObjectWithTag("Player");		
	}
	
	// Late Update happens after each Update is called (good for camera operations)	
	void LateUpdate()
	{				
		if(target == null)
			target = GameObject.FindGameObjectWithTag("Player");
		if(target == null)
			return;
		
		Vector3 	newPosition = Vector3.zero;
		Quaternion 	newRotation = Quaternion.identity;
		
		// Set the camera position and angle based on the Perspective enum		
		
		if(perspective == CameraPerspectives.TOP)
		{
			// Camera follow from Top-Down view
			newPosition = target.transform.position + new Vector3(0, followDistance, 0);
			newRotation = Quaternion.Euler(90,0,0);
		}
		else if(perspective == CameraPerspectives.SIDE)
		{
			// Camera follow from Side-Scroller view
			newPosition = target.transform.position + new Vector3(0, 0, -followDistance);
			newRotation = Quaternion.Euler(0,0,0);
		}
		else if(perspective == CameraPerspectives.ANGLE)
		{
			// Camera follow from Angled/Side-Scroller
			newPosition = target.transform.position + new Vector3(0, followDistance/1.5f, -followDistance/1.5f);
			newRotation = Quaternion.Euler(viewAngle,0,0);
		}
		
		
		transform.position = Vector3.Lerp(transform.position, newPosition, Time.deltaTime/followSpeedDamping);
		transform.rotation = newRotation;
	}
}
