using UnityEngine;
using System.Collections;

public class ClickToMove : MonoBehaviour 
{
	public float moveSpeed = 10;
	Vector3 destination;
	CharacterController controller;
	
	void Start () 
	{
		destination = transform.position;
		controller = gameObject.GetComponent<CharacterController>();
	}
	
	
	void Update () 
	{
		// Set the destination to where (in the world) the mouse was clicked.
		// This requires colliders to determine the click point, it won't work
		// in an empty world.
		
		if(Input.GetMouseButtonDown(0))
		{
			Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
			RaycastHit hit;
			if(Physics.Raycast(ray, out hit, 1000))
			{
				destination = hit.point;			
			}
		}		
		
		// Get the distance to our destination
		float distance = Vector3.Distance(transform.position, destination);
				
		// Create a new Vector using linear interpolation to the destination
		Vector3 newPos = Vector3.Lerp(transform.position, destination, Time.deltaTime/distance * moveSpeed);
		Vector3 moveTo = (newPos - transform.position);
		// Make the object face the destination
		if(moveTo.normalized.magnitude != 0)
			transform.rotation = Quaternion.LookRotation(moveTo.normalized);

		// Move (Uses the CharacterController if available, otherwise uses a regular Translate)
		if(controller != null)
			controller.Move(moveTo);
		else
			transform.Translate(moveTo, Space.World);
	}
}
