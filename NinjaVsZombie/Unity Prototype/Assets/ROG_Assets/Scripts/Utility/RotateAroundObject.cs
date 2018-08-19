using UnityEngine;
using System.Collections;

public class RotateAroundObject : MonoBehaviour
{
	public Transform target;
	public float radius = 5;
	public float degreesPerSecond = 90;
	
	void Start()
	{
		if(target)
			transform.position = target.transform.position + transform.forward * radius;
		
		transform.parent = target.transform;
	}
	
	void Update() 
	{
		if(target)
		{
			transform.RotateAround(target.position, Vector3.up, degreesPerSecond * Time.deltaTime);
		}
	}
}
