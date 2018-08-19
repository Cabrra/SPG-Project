using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class Player_TopDown : MonoBehaviour 
{
	public 		float 					health 				= 100;
	public 		float 					healthMax			= 100;
	public 		float 					moveSpeed			= 5;
	public 		float 					jumpSpeed 			= 12;
	public 		float 					gravity 			= 20;
	
	public		GameObject				projectileObj;		// define prefab obj to shoot	(set in editor
	public		float					attackCoooldown		= 0.5f; // cooldown in seconds
	private		float					nextAttackTime		= 0.0f;	// next attack (game time)
	
	public		Transform				sprite;				// sprite/billboard (optional)
	public		Texture2D[]				spriteTextures;		// sprite textures to use		(set in editor)

	public		AudioClip				takeDamageSound;	// Optional hit feedback sound 	(set in editor)
	public		GameObject				takeDamageEffect;	// Optional hit feedback effect (set in editor)
	
	private		CharacterController		controller;			// The character controller
	private		Vector3 				moveDirection		= Vector3.zero;

	public 		enum 					MoveStyles			{AUTOFACING, ROTATIONAL, MOUSEAIM};
	public		MoveStyles				moveStyle			= MoveStyles.MOUSEAIM;
	public		GUIStyle				playerStyle;
	
	
	public virtual void Start () 
	{
		// Get a reference to the character controller
		controller = this.gameObject.GetComponent<CharacterController>();

		// Find the child named sprite (null if not found)
		sprite = transform.Find("Sprite");
	}
	

	//----------- Update -----------
	public virtual void Update () 
	{		
		//----------- Set Input/Movement -----------
		SetMoveDirection();
		
		//----------- Gravity & Jump -----------
		GravityAndJump();

		//----------- Movement -----------
		Move();
			
		//----------- Shoot -----------
		if(Input.GetMouseButton(0))
			Shoot();
	}

	
	//----------- GUI -----------
	public virtual void OnGUI()
	{
		ROG.DrawHealthBar(health/healthMax, 0, 0, Screen.width/3, Screen.height/20, playerStyle);	
	}
	
	
	private void SetMoveDirection()
	{
		// See: "Edit menu > Project Settings > Input" for Axis mapping & options
		
		// Set and normalize input vector
		Vector3 inputVector = Vector3.zero;
		inputVector.x = Input.GetAxis("Horizontal");
		inputVector.z = Input.GetAxis("Vertical");
		inputVector.Normalize();
		
		// Set vector for movement (moveDirection)
		moveDirection.x = inputVector.x * moveSpeed;
		moveDirection.z = inputVector.z * moveSpeed;
		
		// Rotational move style uses forward facing to determine moveDirection
		if(moveStyle == MoveStyles.ROTATIONAL)
		{				
			moveDirection.x = transform.forward.x * moveSpeed * inputVector.z;
			moveDirection.z = transform.forward.z * moveSpeed * inputVector.z;
		}
		
		// Set Rotation	(Based on moveStyle Enum)
		if(moveStyle == MoveStyles.AUTOFACING)
		{	
			// Auto-facing - Player faces moveDirection)		
			Vector3 lookDirection = moveDirection;
			lookDirection.y = 0;
			if(lookDirection.magnitude >= 1.0f)
				transform.rotation = Quaternion.LookRotation(lookDirection);
		}			
		else if(moveStyle == MoveStyles.ROTATIONAL)
		{	
			// Manual Rotation - Player faces forward, horizontal input rotates
			transform.Rotate(Vector3.up, inputVector.x * 360 * Time.deltaTime);
		}				
		else if(moveStyle == MoveStyles.MOUSEAIM)
		{	
			// Mouse Aim - Set rotation to look at the mouse (world position)
			transform.rotation = ROG.LookAtMouse(gameObject, true);	
		}	
	}
	
	
	//----------- Gravity & Jump -----------
	private void GravityAndJump()
	{		
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
	}
	
	
	//----------- Orient (Varying styles based on the Perspectives Enum) -----------
	private void SetRotation()
	{			
		
	}
	
	
	//----------- Perform the move (Based on moveDirection) -----------
	public virtual void Move()
	{		
		if(controller == null)
			transform.Translate(moveDirection * Time.deltaTime, Space.World);
		else
			controller.Move(moveDirection * Time.deltaTime);
		
		if(sprite)
			sprite.renderer.material.mainTexture = spriteTextures[0];
	}
	
	
	//----------- Shoot -----------
	public virtual void Shoot()
	{
		if(projectileObj == null)
			return;
		
		if(Input.GetMouseButton(0))
		{			
			if(Time.time > nextAttackTime)
			{
				// Create a bullet facing the new direction
				Quaternion shootDirection = Quaternion.LookRotation(transform.forward);
				Vector3 shootPosition = transform.position + transform.forward * 1.0f;
				
				//GameObject bulletInstance = (GameObject)Instantiate(projectileObj, shootPosition, shootDirection);
				Instantiate(projectileObj, shootPosition, shootDirection);				
	
				nextAttackTime = Time.time + attackCoooldown;
			}
			
			if(sprite)
				sprite.renderer.material.mainTexture = spriteTextures[1];
		}	
	}

	
	//----------- Take Damage -----------
	public virtual void ModifyHealth(float amount)
	{	
		health = Mathf.Min(health + amount, healthMax);
	
		if(amount < 0)
			new FloatingText(transform.position, (amount).ToString(), Color.red);
		else
			new FloatingText(transform.position, (amount).ToString(), Color.green);
				
		if(health < 1)
		{
			// Destroy this object (mark for deletion)
			Destroy(gameObject);
			
			// Restart Level
			Application.LoadLevel(Application.loadedLevel);
		}
		
		// Take damage feedback effects
		if(amount < 0)
		{
			if(takeDamageEffect)
				Instantiate(takeDamageEffect, transform.position, transform.rotation);
			
			if(takeDamageSound)
				ROG.PlaySound(takeDamageSound);
		}				
	}
}
