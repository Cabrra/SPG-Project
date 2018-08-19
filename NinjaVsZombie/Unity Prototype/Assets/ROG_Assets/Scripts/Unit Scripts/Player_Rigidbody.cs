using UnityEngine;
using System.Collections;
using System.Collections.Generic;

[RequireComponent(typeof(Rigidbody))]

public class Player_Rigidbody : MonoBehaviour
{
	public 	float 		health		= 100;
	public	float 		moveSpeed	= 5;
	public 	GameObject 	projectile; // set in the editor
	
	void Update () 
	{
		Vector3 moveDirection = Vector3.zero;
		moveDirection.x = Input.GetAxis("Horizontal") * moveSpeed;
		moveDirection.z = Input.GetAxis("Vertical") * moveSpeed;
		
		rigidbody.MovePosition(transform.position + moveDirection * Time.deltaTime);
	
		transform.rotation = ROG.LookAtMouse(gameObject, true);
		
		if(Input.GetMouseButtonDown(0))
			Shoot();
	}
	
	
	//-------------------- Shoot -----------------------	
	void Shoot()
	{
		if(projectile)
			Instantiate(projectile, transform.position + transform.forward * 1, transform.rotation);
	}
	
	
	//-------------------- Modify Health -----------------------	
	public void ModifyHealth(float amount)
	{
		health += amount;
	}
	
	
	void OnCollisionEnter(Collision collision)
	{
		//print("Player collided with:" + collision.gameObject.name);	
	}
	
	void OnTriggerEnter(Collider other)
	{
		//print("Player hit trigger:" + other.gameObject.name);	
	}
}
