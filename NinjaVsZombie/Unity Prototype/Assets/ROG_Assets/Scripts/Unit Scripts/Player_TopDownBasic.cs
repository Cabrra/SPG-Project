using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class Player_TopDownBasic : MonoBehaviour 
{
	public 	float 				health 		= 100;
	public 	float 				moveSpeed 	= 5;
	public 	float 				jumpSpeed 	= 12;
	public 	float 				gravity 	= 20;
	public 	GameObject 			projectile; // set in the editor
	public	float				shotsPerSecond = 1;
	public 	int 				score 		= 0;
	
	private Vector3 			moveDirection = Vector3.zero; // movement direction	
	private CharacterController	controller; // a special collider optimized for character movement
	private float				nextShootTime = 0;
	
	//-------------------- Start -----------------------
	void Start ()
	{
		// get reference to the character controller
		controller = this.gameObject.GetComponent<CharacterController>();
	}
	
	
	//-------------------- Update -----------------------
	void Update () 
	{		
		// Uses the Input manager to set move direction (See: Edit menu > Project Settings > Input)
		moveDirection.x = Input.GetAxis("Horizontal") * moveSpeed;
		moveDirection.z = Input.GetAxis("Vertical") * moveSpeed;	

		transform.rotation = ROG.LookAtMouse(gameObject, true);	
		
		// Jump & Gravity (only valid if a CharacterController is used)
		if(controller != null)
		{
			if(controller.isGrounded)
			{
				// Reset the Y axis of moveDirection while grounded
				moveDirection.y = 0;
				
				// Jump
				if(Input.GetKey(KeyCode.Space))
					moveDirection.y = jumpSpeed;
			}
			else
				// Gravity
				moveDirection.y -= gravity * Time.deltaTime;
		}
		
		// Move
		if(controller == null)
			transform.Translate(moveDirection * Time.deltaTime, Space.World);
		else
			controller.Move(moveDirection * Time.deltaTime);
		
		if(Input.GetMouseButton(0) && Time.time > nextShootTime)
			Shoot();
	}
	
	
	//-------------------- Shoot -----------------------	
	void Shoot()
	{
		if(projectile)
			Instantiate(projectile, transform.position + transform.forward * 1, transform.rotation);
		
		nextShootTime = Time.time + 1.0f/shotsPerSecond;
	}
	
	
	//-------------------- Modify Health -----------------------	
	public void ModifyHealth(float amount)
	{
		health += amount;
		
		new FloatingText(transform.position, amount.ToString(), Color.red);
	}

	public void ModifyScore(int amount)
	{
		score += amount;
	}
}