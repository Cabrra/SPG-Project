using UnityEngine;
using System.Collections;

public class Player_SideScroll : MonoBehaviour
{
	public	float 					moveSpeed 	= 5;
	public	float 					jumpSpeed 	= 12;
	public	float 					gravity 	= 20;
	public	GameObject				projectile; // set in editor
	
	private	Vector3 				moveDirection = Vector3.zero;
	private	Vector2					facing;
	private	CharacterController 	controller;
	
	
	void Start () 
	{
		// Get a reference to the character controller
		controller = this.gameObject.GetComponent<CharacterController>();
		
		// Facing (2D) of the player (for side scroll use)
		facing = new Vector2(1,1);		
	}
	
	
	void Update ()
	{		
		// Get Input (See: "Edit menu > Project Settings > Input" for mapping)
		moveDirection.x = Input.GetAxis("Horizontal") * moveSpeed;
		moveDirection.z = Input.GetAxis("Vertical") * moveSpeed;
		
		// Set facing (for sprite and shoot direction)
		if(moveDirection.x > 0)
			facing.x = 1;
		else if(moveDirection.x < 0)
			facing.x = -1;
		
		// Set the facing of our child sprite (if we have one)
		GameObject sprite = transform.Find("SideBillboard").gameObject;
		if(sprite != null)
			sprite.renderer.material.mainTextureScale = facing;
		
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
		
		// Shoot
		if(Input.GetMouseButtonDown(0))
			Shoot();
	}
	
	
	void Shoot()
	{
		if(projectile)
		{
			Vector3 forward = new Vector3(facing.x, 0, 0);
			Instantiate(projectile, transform.position + forward * 0.6f, Quaternion.LookRotation(forward));						
		}
	}	
}
