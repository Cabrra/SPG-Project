using UnityEngine;
using System.Collections;
using System.Collections.Generic;


public class Enemy_SideScroll : MonoBehaviour
{
	public 	float					health 			= 100;	
	public 	float 					moveSpeed 		= 5;
	public 	float 					visualRange		= 20;
	public 	float 					attackCooldown 	= 1.0f;
	public 	float 					gravity 		= 20;
	
	private	Vector3 				moveDirection;
	private	float 					nextAttack = 0;
	private	CharacterController 	controller;
	private	GameObject 				target;
	private	Vector3 				destination;
	private	int					facing = 1;	
	
	
	//--------------------Start-----------------------		
	void Start () 
	{	
		// Get a reference to the Character Controller Component
		controller = gameObject.GetComponent<CharacterController>();

		// Find object named Player		
		target = GameObject.FindGameObjectWithTag("Player");
		
		// Set random destination in a 5 unit radius
		destination = transform.position + new Vector3(Random.Range(-5,5), 0, 0);		
	}
	
	
	void Update () 
	{
		//--------------------Check death-----------------------
		if(health < 1)
			Destroy(gameObject);
	
		//--------------------Check aggro-----------------------			
		// Find Player
		if(target == null)
			target = GameObject.FindGameObjectWithTag("Player");
		
		// Checks direct line of sight for the target
		bool hasLOS = ROG.hasLOS(transform.position, visualRange, target);
		
		// if line of sight, set target's position as destination
		if(hasLOS)
			destination = target.transform.position;
			
		// Get distance to destination
		float distance = Vector3.Distance(transform.position, destination);

		// check distance
		if(distance < 2)
		{			
			if(hasLOS)
			{
				// attack
				if(Time.time > nextAttack)
					Attack();
			}
			else
			{
				// set new destination
				Vector3 newPosition = transform.position + new Vector3(Random.Range(-5,5), 0, 0);				
				
				if(ROG.hasLOS(transform.position, newPosition))
					destination = newPosition;
			}			
		}
		
		//--------------------Movement-----------------------					
		Vector3 newDirection = (destination - transform.position).normalized;
		moveDirection.x = newDirection.x * moveSpeed;
		moveDirection.z = newDirection.z * moveSpeed;
		
		//Gravity
		if(controller != null && controller.isGrounded)
			moveDirection.y = 0;
		else
			moveDirection.y -= gravity * Time.deltaTime;
				
		// Set facing (for sprite and shoot direction)
		if(moveDirection.x > 0)
			facing = 1;
		else if(moveDirection.x < 0)
			facing = -1;
		
		// If we have a child sprite, set it's facing (flip horizontal scaling)
		GameObject sprite = transform.Find("SideBillboard").gameObject;
		if(sprite != null)
			sprite.renderer.material.mainTextureScale = new Vector2(facing, 1);

		// Move
		if(controller == null)
			transform.Translate(moveDirection * Time.deltaTime, Space.World);
		else
			controller.Move(moveDirection * Time.deltaTime);
	}
	

	//--------------------Attack-----------------------			
	public void Attack()
	{
		target.SendMessage("ModifyHealth", -10, SendMessageOptions.DontRequireReceiver);
		
		nextAttack = Time.time + attackCooldown;
	}

	
	//--------------------Modify Health-----------------------
	public void ModifyHealth(float amount)
	{
		health += amount;	
	}		
}