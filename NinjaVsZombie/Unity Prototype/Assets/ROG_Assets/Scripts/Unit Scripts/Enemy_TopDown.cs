using UnityEngine;
using System.Collections;
using System.Collections.Generic;


public class Enemy_TopDown : MonoBehaviour
{
	//--------------------Public---------------------
	public	float					health 			= 100;
	public	float					moveSpeed 		= 5;
	public	float					visualRange		= 20;
	public	float					attackCooldown 	= 1.0f;
	public	float					meleeRange		= 2.0f;
	public	float					gravity 		= 20;
	public	int 					point			= 10;
	
	//--------------------Assets----------------------
	public 	AudioClip 				attackSound;
	public 	AudioClip 				takeDamageSound;
	public	GameObject				takeDamageEffect;
	public 	AudioClip 				deathSound;
	public	GameObject				deathEffect;
	
	//--------------------Private---------------------
	private float					healthMax		= 100;
	private float					nextAttack 		= 0;
	private float					nextWander		= 10.0f;
	private float					wanderTimeout 	= 10.0f;
	
	private Vector3					moveDirection;
	private Vector3					destination;
	private CharacterController		controller;
	private GameObject				target;

		
	//--------------------Start-----------------------	
	
	void Start () 
	{	
		// 
		healthMax = health;
		
		// Get a reference to the Character Controller Component
		controller = gameObject.GetComponent<CharacterController>();

		// Find object named Player
		target = GameObject.FindGameObjectWithTag("Player");
		
		// Set random destination
		SetNewDestination();
	}
	
	
	void Update () 
	{
		//----------- Check Target / Set Destination -----------
		HandleTargeting();

		//----------- Move -----------
		MoveAndRotate();	
	}
	
	
//	void OnGUI()
//	{
//		ROG.DrawHealthBarAtWorldPosition(health/healthMax, transform.position, (int)(Screen.width * 0.1f), (int)(Screen.height * 0.02f), -20, Color.red, 0.75f);
//	}
	
	
	void HandleTargeting()
	{		
		// Assume we don't have line of sight to our target
		bool hasTargetLOS = false;
		
		// If target is null, attempt to find Player
		if(target == null)	
			target = GameObject.FindGameObjectWithTag("Player");
		// If target is not null, check if we have line of sight to it
		else
			hasTargetLOS = ROG.hasLOS(transform.position, visualRange, target);
		
		// if has line of sight, set the target position as our destination and check attack conditions
		// else continue wandering or choose a new wander destination
		if(hasTargetLOS)
		{
			// Set target position as our destination
			destination = target.transform.position;
			
			// Get distance to target
			float distanceToTarget = Vector3.Distance(transform.position, target.transform.position);
			
			// Attack if conditions are met
			if(distanceToTarget < meleeRange && Time.time > nextAttack)
				Attack();
		}
		else		
		{
			// Get distance to destination
			float distanceToDestination = Vector3.Distance(transform.position, destination);
	
			// Set a new destination if we have reached our destination, or our wander timer has timed out
			if(distanceToDestination < 1.0f || Time.time > nextWander)
				SetNewDestination();		
		}
		
		// Optional debug option, draws line to current destination
		// This can only be seen in editor windows, not in the game camera
		Debug.DrawLine(transform.position, destination);
	}
	
		
	//--------------------Movement-----------------------	
	void MoveAndRotate()
	{		
		// Get direction to our destination
		Vector3 newDirection = (destination - transform.position).normalized;
		
		// Set our move direction to the new direction and include gravity (if we have a controller)
		moveDirection.x = newDirection.x * moveSpeed;
		moveDirection.z = newDirection.z * moveSpeed;
		
		//Gravity
		if(controller != null && controller.isGrounded)
			moveDirection.y = 0;
		else
			moveDirection.y -= gravity * Time.deltaTime;

		// Rotate the object towards to face the direction it is moving
		Vector3 lookDirection = moveDirection;
		lookDirection.y = 0;
		if(lookDirection.magnitude > 0)
			transform.rotation = Quaternion.LookRotation(lookDirection);

		// Move
		if(controller == null)
			transform.Translate(moveDirection * Time.deltaTime, Space.World);
		else
			controller.Move(moveDirection * Time.deltaTime);
	}
		
	// Sets a new random destination that is within our line of sight
	void SetNewDestination()
	{
		float radius = 10.0f;
		
		do
		{
			destination = transform.position + Random.insideUnitSphere * radius;
			destination.y = transform.position.y;
			radius -= Time.deltaTime * 2;			
		}
		while(!ROG.hasLOS(transform.position, destination) && radius > 1);
		
		nextWander = Time.time + wanderTimeout;	
	}
	
	
	//--------------------Attack-----------------------			
	public void Attack()
	{
		target.SendMessage("ModifyHealth", -10, SendMessageOptions.DontRequireReceiver);
		
		nextAttack = Time.time + attackCooldown;
	}
	

	//--------------------Take Damage/Heal-----------------------	
	public void ModifyHealth(float amount)
	{	
		health = Mathf.Min(health + amount, healthMax);	
				
		// Optional - Draw floating text (red for damage, green for heal)
		if(amount < 0)
			new FloatingText(transform.position, (amount).ToString(), Color.red);
		else
			new FloatingText(transform.position, (amount).ToString(), Color.green);
				
		//--------------------Check death-----------------------
		if(health < 1)
		{
			if(deathSound)
				ROG.PlaySound(deathSound);
			
			if(deathEffect)
				Instantiate(deathEffect, transform.position, transform.rotation);

			target.SendMessage("ModifyScore", 10, SendMessageOptions.DontRequireReceiver);
			Destroy(gameObject);
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